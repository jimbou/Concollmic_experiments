
import os
import sys
import time
from typing import Literal

import litellm
from litellm import completion_cost
from litellm.exceptions import (
    BadRequestError as LiteLLMBadRequestError,
    ContentPolicyViolationError as LiteLLMContentPolicyViolationError,
)
from litellm.utils import Choices, Message, ModelResponse

from app.log import log_and_cprint, log_and_print
from app.model.common import (
    Model,
    ModelNoResponseError,
    Usage,
)
import app.model.common as common
# ============================================================
# DeepSeek manual cost override (LiteLLM does NOT know these)
# ============================================================


# ============================================================
# Unified CloseAI Model
# ============================================================
class CloseAIModel(Model):
    """
    Automatic CloseAI wrapper:
    - Claude models → Anthropic native endpoint (/anthropic/v1/messages)
    - Other models → OpenAI-compatible endpoint (/v1/chat/completions)
    """

    _instances = {}

    def __new__(cls):
        if cls not in cls._instances:
            inst = super().__new__(cls)
            inst._initialized = False
            cls._instances[cls] = inst
        return cls._instances[cls]

    def __init__(
        self,
        name: str,
        cost_per_input: float,
        cost_per_output: float,
        max_output_token: int = 4096,
        parallel_tool_call: bool = False,
    ):
        if self._initialized:
            return

        super().__init__(name, cost_per_input, cost_per_output, parallel_tool_call)
        self.max_output_token = max_output_token
        self._initialized = True

    # -------------------------------------
    # Required by Model base class
    # -------------------------------------
    def setup(self):
        key = os.getenv("CLOSE_API_KEY")
        if not key:
            print("Please set CLOSE_API_KEY")
            sys.exit(1)
        return key
    
    def check_api_key(self):
        key = os.getenv("CLOSE_API_KEY")
        if not key:
            print("Please set CLOSE_API_KEY")
            sys.exit(1)
        return key

    # ============================================================
    # Auto-detect correct endpoint based on model name
    # ============================================================
    def _select_endpoint(self):
        if self.name.startswith("claude-"):
            return ("https://api.openai-proxy.org/anthropic", "anthropic")

        if self.name.startswith("deepseek"):
            # DeepSeek uses OpenAI-compatible endpoint but needs its own provider
            return ("https://api.openai-proxy.org/v1", "deepseek")

        # Default OpenAI-compatible models
        base_url = os.getenv("OPENAI_BASE_URL", "https://api.openai-proxy.org/v1")
        return (base_url, "openai")

    # -------------------------------------
    # Main call
    # -------------------------------------
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

        # GPT-5 special rules
        if self.name.startswith("gpt-5"):
            temperature = 1
            top_p = None

            # 🔥 Replace max_tokens → max_completion_tokens
            kwargs["max_completion_tokens"] = self.max_output_token
            kwargs.pop("max_tokens", None)
        else:
            # All other models use normal max_tokens
            kwargs["max_tokens"] = self.max_output_token


        api_key = self.setup()
        base_url, provider = self._select_endpoint()

        # JSON output enforcement
        if response_format == "json_object":
            messages[-1]["content"] += (
                "\nYour response must be a valid JSON object starting with '{' and ending with '}'."
            )

        try:
            start = time.time()

            # ----------- Execute API call -----------------
            response = litellm.completion(
                model=self.name,
                messages=messages,
                temperature=temperature,
                top_p=top_p,
                api_key=api_key,
                base_url=base_url,
                stream=False,
                tools=tools,
                custom_llm_provider=provider,
                **kwargs,
            )
            # ------------------------------------------------

            latency = time.time() - start
            cost = completion_cost(model=self.name, completion_response=response)

            if not isinstance(response, ModelResponse):
                raise ModelNoResponseError(f"Bad response: {response}")

            if not response.choices:
                raise ModelNoResponseError(f"No choices from {self.name}")

            usage = response.usage
            prompt_tok = int(usage.prompt_tokens)
            completion_tok = int(usage.completion_tokens)
            cache_write = int(usage.get("cache_creation_input_tokens", 0))
            cache_read = int(usage.get("cache_read_input_tokens", 0))

            msg: Message = response.choices[0].message
            content = msg.content or ""
            tool_calls = getattr(msg, "tool_calls", None)

            # Log
            log_and_cprint(
                f"Model ({self.name}) [{provider}] usage: "
                f"in={prompt_tok}, out={completion_tok}, "
                f"cache_r={cache_read}, cache_w={cache_write}, "
                f"cost={cost:.6f}$, latency={latency:.3f}s",
                style="yellow",
            )

            return (
                content,
                tool_calls,
                Usage(
                    model=self.name,
                    input_tokens=prompt_tok,
                    output_tokens=completion_tok,
                    cache_read_tokens=cache_read,
                    cache_write_tokens=cache_write,
                    cost=cost,
                    latency=latency,
                    call_cnt=1,
                ),
            )

        except LiteLLMContentPolicyViolationError:
            log_and_print("CloseAI policy violation.")
            raise

        except LiteLLMBadRequestError as e:
            if e.code == "context_length_exceeded":
                log_and_print("Context exceeded.")
            raise

# ============================================================
# CloseAI Model Definitions (clean, readable subclass layout)
# ============================================================

class GPT4o(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="gpt-4o",
            cost_per_input=0.000005,
            cost_per_output=0.000015,
            max_output_token=8192,
        )


class GPT4oMini(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="gpt-4o-mini",
            cost_per_input=0.0000005,
            cost_per_output=0.0000015,
            max_output_token=8192,
        )


class GPT5(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="gpt-5",
            cost_per_input=0.000020,
            cost_per_output=0.000060,
            max_output_token=8192,
        )


class Claude37SonnetLatest(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="claude-3-7-sonnet-latest",
            cost_per_input=0.000015,
            cost_per_output=0.000030,
            max_output_token=8192,
        )


class ClaudeSonnet40(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="claude-sonnet-4-0",
            cost_per_input=0.000018,
            cost_per_output=0.000036,
            max_output_token=8192,
        )


class Gemini25Pro(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="gemini-2.5-pro",
            cost_per_input=0.000003,
            cost_per_output=0.000009,
            max_output_token=8192,
        )

class Gemini25Flash(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="gemini-2.5-flash",
            cost_per_input=0.000002,
            cost_per_output=0.000006,
            max_output_token=8192,
        )
# ============================================================
# DeepSeek models (OpenAI-compatible endpoint)
# ============================================================

class DeepSeekChat(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="deepseek-chat",
            cost_per_input=0.000000825,
            cost_per_output=0.00000255,
            max_output_token=8192,
        )

class DeepSeekReasoner(CloseAIModel):
    def __init__(self):
        super().__init__(
            name="deepseek-reasoner",
            cost_per_input=0.0000010,
            cost_per_output=0.0000030,
            max_output_token=8192,
        )
