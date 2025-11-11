"""
CloseAI model interface (OpenAI-compatible proxy via LiteLLM).

Usage:
    export CLOSE_API_KEY="your_key_here"
    export OPENAI_BASE_URL="https://api.openai-proxy.org/v1"
"""

import os
import sys
import time
from typing import Literal

import litellm
from litellm import completion_cost
from litellm.exceptions import BadRequestError as LiteLLMBadRequestError
from litellm.exceptions import (
    ContentPolicyViolationError as LiteLLMContentPolicyViolationError,
)
from litellm.utils import Choices, Message, ModelResponse

from app.log import log_and_cprint, log_and_print
from app.model import common
from app.model.common import (
    Model,
    ModelNoResponseError,
    Usage,
)


class CloseAIModel(Model):
    """
    Base class for creating Singleton instances of CloseAI models.
    Uses the OpenAI-compatible proxy endpoint at https://api.openai-proxy.org/v1
    """

    _instances = {}

    def __new__(cls):
        if cls not in cls._instances:
            cls._instances[cls] = super().__new__(cls)
            cls._instances[cls]._initialized = False
        return cls._instances[cls]

    def __init__(
        self,
        name: str,
        cost_per_input: float,
        cost_per_output: float,
        max_output_token: int = 8192,
        parallel_tool_call: bool = True,
    ):
        if self._initialized:
            return
        super().__init__(name, cost_per_input, cost_per_output, parallel_tool_call)
        self.max_output_token = max_output_token
        self._initialized = True

    def setup(self) -> None:
        self.check_api_key()

    def check_api_key(self) -> str:
        key_name = "CLOSE_API_KEY"
        key = os.getenv(key_name)
        if not key:
            print(f"Please set the {key_name} env var")
            sys.exit(1)
        return key

    def extract_resp_content(self, chat_message: Message) -> str:
        content = chat_message.content
        return content or ""

    def _perform_call(
        self,
        messages: list[dict],
        top_p=1,
        tools=None,
        response_format: Literal["text", "json_object"] = "text",
        temperature: float | None = None,
        **kwargs,
    ):
        if temperature is None:
            temperature = common.MODEL_TEMP

        api_key = self.check_api_key()
        base_url = os.getenv("OPENAI_BASE_URL", "https://api.openai-proxy.org/v1")

        try:
            if response_format == "json_object":
                last_content = messages[-1]["content"]
                messages[-1]["content"] = (
                    last_content
                    + "\nYour response should start with { and end with }. "
                      "DO NOT write anything else other than the JSON."
                )

            start_time = time.time()

            response = litellm.completion(
                model=self.name,
                messages=messages,
                temperature=temperature,
                max_tokens=self.max_output_token,
                top_p=top_p,
                stream=False,
                tools=tools,
                # tool_choice="auto",  # ✅ FIX: replaces 'any' which is now invalid
                api_key=api_key,
                base_url=base_url,
                **kwargs,
            )


            latency = time.time() - start_time
            cost = completion_cost(model=self.name, completion_response=response)

            assert isinstance(response, ModelResponse)
            if not response.choices or len(response.choices) == 0:
                raise ModelNoResponseError(
                    f"Model {self.name} returned no choices. Response: {response}"
                )

            resp_usage = response.usage
            assert resp_usage is not None

            input_tokens = int(resp_usage.prompt_tokens)
            output_tokens = int(resp_usage.completion_tokens)
            cache_creation_tokens = int(resp_usage.get("cache_creation_input_tokens", 0))
            cache_read_tokens = int(resp_usage.get("cache_read_input_tokens", 0))

            first_choice = response.choices[0]
            assert isinstance(first_choice, Choices)
            resp_msg: Message = first_choice.message

            content = self.extract_resp_content(resp_msg)
            tool_calls = getattr(resp_msg, "tool_calls", None)

            log_and_cprint(
                f"Model ({self.name}) API usage: "
                f"{{input={input_tokens}, output={output_tokens}, cache_read={cache_read_tokens}, "
                f"cache_write={cache_creation_tokens}}}, cost={cost:.6f} USD, latency={latency:.3f}s",
                style="yellow",
            )

            return (
                content,
                tool_calls,
                Usage(
                    model=self.name,
                    input_tokens=input_tokens,
                    output_tokens=output_tokens,
                    cache_read_tokens=cache_read_tokens,
                    cache_write_tokens=cache_creation_tokens,
                    cost=cost,
                    latency=latency,
                    call_cnt=1,
                ),
            )

        except LiteLLMContentPolicyViolationError:
            log_and_print("CloseAI content policy violation.")
            raise

        except LiteLLMBadRequestError as e:
            if e.code == "context_length_exceeded":
                log_and_print("Context length exceeded")
            raise e


# === Concrete model classes ===

class GPT4o(CloseAIModel):
    def __init__(self):
        super().__init__(
            "gpt-4o",  # model ID exposed by CloseAI/OpenAI proxy
            0.00001,
            0.00003,
            max_output_token=8192,
        )
        self.note = "CloseAI GPT-4o model (OpenAI proxy)"


class GPT4oMini(CloseAIModel):
    def __init__(self):
        super().__init__(
            "gpt-4o-mini",
            0.000005,
            0.000015,
            max_output_token=8192,
        )
        self.note = "CloseAI GPT-4o-mini model (fast + cost-efficient)"
