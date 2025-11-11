import os
import time
import re
import json
import requests
import litellm
from litellm import completion_cost
from app.model.gpt import OpenaiModel
from app.model.common import Usage, ModelNoResponseError
from app.log import log_and_cprint



# --- Configuration ---
API_KEY_302 = os.environ.get("API_KEY_302")
if not API_KEY_302:
    raise ValueError("❌ Missing API_KEY_302 environment variable")

AI302_BASE_URL = "https://api.302.ai/v1/chat/completions"  # ✅ endpoint must include path
AI302_HEADERS = {
    "Content-Type": "application/json",
    "Authorization": f"Bearer {API_KEY_302}",               # ✅ confirmed working
}


class AI302Base(OpenaiModel):
    """
    302.AI wrapper aligned with the Claude wrapper semantics.
    Uses the same interface but performs manual HTTP POSTs to 302.AI.
    """

    _instances = {}

    def __new__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super().__new__(cls)
            cls._instances[cls]._initialized = False
        return cls._instances[cls]

    def __init__(
        self,
        name: str,
        max_output_token: int,
        cost_per_input: float,
        cost_per_output: float,
        parallel_tool_call: bool = False,
    ):
        if getattr(self, "_initialized", False):
            return
        super().__init__(name, cost_per_input, cost_per_output, parallel_tool_call)
        self.max_output_token = max_output_token
        self._initialized = True

    # ----------------------------------------------------------------------
    # Required interface
    # ----------------------------------------------------------------------
    def setup(self) -> None:
        self.check_api_key()

    def check_api_key(self) -> str:
        if not API_KEY_302:
            raise ValueError("Please set API_KEY_302")
        return API_KEY_302

    # ----------------------------------------------------------------------
    # Utilities
    # ----------------------------------------------------------------------
    def _normalize_c_code_block(self, content: str) -> str:
        if not content:
            return ""
        s = content.strip()
        if "\\n" in s and "\n" not in s:
            try:
                s = bytes(s, "utf-8").decode("unicode_escape").strip()
            except Exception:
                s = s.replace("\\n", "\n").replace("\\t", "\t").strip()
        for pat in [
            r"^```(?:\s*c\s*)\n([\s\S]*?)\n?```$",
            r"^'''(?:\s*c\s*)\n([\s\S]*?)\n?'''$",
        ]:
            m = re.search(pat, s, flags=re.IGNORECASE)
            if m:
                return m.group(1).strip()
        return s

    def _flatten_messages(self, messages: list[dict]) -> list[dict]:
        """Convert Anthropic-style [{'type':'text','text':...}] to OpenAI-style string content."""
        flattened = []
        for m in messages:
            role = m.get("role")
            content = m.get("content")

            # Case 1: Already plain string
            if isinstance(content, str):
                flattened.append({"role": role, "content": content})
                continue

            # Case 2: Anthropic-style list of {'type':'text','text':...}
            if isinstance(content, list):
                text_parts = []
                for c in content:
                    if isinstance(c, dict) and "text" in c:
                        text_parts.append(c["text"])
                    elif isinstance(c, str):
                        text_parts.append(c)
                flattened.append({"role": role, "content": "\n".join(text_parts)})
                continue

            # Case 3: Unknown format (fallback)
            flattened.append({"role": role, "content": str(content)})

        return flattened

    # ----------------------------------------------------------------------
    # Core model call
    # ----------------------------------------------------------------------
    def _perform_call(
    self,
    messages: list[dict],
    top_p: float = 1.0,
    tools: list[dict] | None = None,
    response_format: str = "text",
    temperature: float | None = None,
    **kwargs,
):
        """Perform POST to 302.AI with automatic flattening and schema cleaning."""
        if temperature is None:
            from app import model as common
            temperature = getattr(common, "MODEL_TEMP", 0.5)

        # -- Step 1: ensure plain-string content
        flattened = self._flatten_messages(messages)

        # -- Step 2: inject JSON-mode instruction if needed
        if response_format == "json_object" and flattened:
            last = flattened[-1]
            if isinstance(last.get("content"), str):
                last["content"] += (
                    "\nYour response must be a valid JSON object (start with '{', end with '}')."
                )
            flattened[-1] = last

        # -- Step 3: build payload
        payload = {
            "model": self.name,
            "messages": flattened,
            "temperature": float(temperature or 0.5),
            "top_p": float(top_p or 1.0),
            "max_tokens": int(self.max_output_token or 1024),
            "stream": False,
        }

        # Optional OpenAI-compatible fields
        if tools:
            payload["tools"] = tools
        tool_choice = kwargs.get("tool_choice")
        if tool_choice:
            payload["tool_choice"] = tool_choice
        start_time = time.time()    
        # -- Step 4: send
        if "tool_choice" in payload:
            del payload["tool_choice"]

        print("=== DEBUG PAYLOAD ===")
        print(json.dumps(payload, indent=2, ensure_ascii=False))
        for m in payload["messages"]:
            assert isinstance(m["content"], str), f"Invalid message format: {m}"

        try:
            r = requests.post(AI302_BASE_URL, headers=AI302_HEADERS, json=payload, timeout=120)
        except Exception as e:
            raise ModelNoResponseError(f"HTTP request to 302.AI failed: {e}")

        if r.status_code != 200:
            # Print payload for debugging once
            import pprint; pprint.pp(payload)
            raise ModelNoResponseError(
                f"302.AI returned HTTP {r.status_code}: {r.text}"
            )

        data = r.json()
        if "error" in data:
            raise ModelNoResponseError(f"302.AI error: {data['error']}")

        msg = data["choices"][0]["message"].get("content", "")
        tool_calls = data["choices"][0]["message"].get("tool_calls")

        usage = data.get("usage", {})
        latency = time.time() - start_time
        cost = completion_cost(model=self.name, completion_response=data)

        return (
            msg.strip(),
            tool_calls,
            Usage(
                model=self.name,
                input_tokens=usage.get("prompt_tokens", 0),
                output_tokens=usage.get("completion_tokens", 0),
                cost=cost,
                latency=latency,
                call_cnt=1,
            ),
        )





# Concrete classes unchanged, just ensure names are raw (no prefixes)
class GPT4_1(AI302Base):
    def __init__(self):
        super().__init__("gpt-4.1", 8192, 0.000003, 0.000012, parallel_tool_call=False)
        self.note = "OpenAI GPT-4.1 via 302.AI endpoint."

class GPT4_1Mini(AI302Base):
    def __init__(self):
        super().__init__("gpt-4.1-mini", 8192, 0.000002, 0.000006, parallel_tool_call=False)
        self.note = "OpenAI GPT-4.1-mini via 302.AI endpoint."
class GPT4oMini(AI302Base):
    def __init__(self):
        super().__init__("gpt-4o-mini", 8192, 0.000002, 0.000006, parallel_tool_call=False)
        self.note = "OpenAI GPT-4o-mini via 302.AI endpoint."

class GPT4oNormal(AI302Base):
    def __init__(self):
        super().__init__("gpt-4o-2024-08-06", 8192, 0.000002, 0.000006, parallel_tool_call=False)
        self.note = "OpenAI GPT-4o via 302.AI endpoint."        

class DeepSeekR1Aliyun(AI302Base):
    def __init__(self):
        super().__init__("deepseek-r1-aliyun", 8192, 0.000001, 0.000004, parallel_tool_call=False)
        self.note = "DeepSeek-R1 Aliyun variant (302.AI)."

class DeepSeekV3Aliyun(AI302Base):
    def __init__(self):
        super().__init__("deepseek-v3-aliyun", 8192, 0.000001, 0.000004, parallel_tool_call=False)
        self.note = "DeepSeek-V3 Aliyun variant (302.AI)."

class DeepSeekR1Huoshan(AI302Base):
    def __init__(self):
        super().__init__("deepseek-r1-huoshan", 8192, 0.000001, 0.000004, parallel_tool_call=False)
        self.note = "DeepSeek-R1 Huoshan variant (302.AI)."

class DeepSeekV3Huoshan(AI302Base):
    def __init__(self):
        super().__init__("deepseek-v3-huoshan", 8192, 0.000001, 0.000004, parallel_tool_call=False)
        self.note = "DeepSeek-V3 Huoshan variant (302.AI)."
