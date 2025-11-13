import os
import litellm

litellm._turn_on_debug()

key = os.getenv("CLOSE_API_KEY")

tools = [
    {
        "type": "function",
        "function": {
            "name": "add_numbers",
            "description": "Add two numbers",
            "parameters": {
                "type": "object",
                "properties": {
                    "a": {"type": "number"},
                    "b": {"type": "number"},
                },
                "required": ["a", "b"],
            },
        },
    }
]

resp = litellm.completion(
    model="deepseek-chat",
    messages=[{"role": "user", "content": "Add 4 and 9"}],
    tools=tools,
    api_key=key,
    base_url="https://api.openai-proxy.org/v1",  # MUST be v1
    custom_llm_provider="deepseek",               # CRITICAL
)

print(resp)
