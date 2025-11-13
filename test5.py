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
    model="claude-3-7-sonnet-latest",
    messages=[{"role": "user", "content": "Add 3 and 5"}],
    tools=tools,
    max_tokens=200,
    api_key=key,
    base_url="https://api.openai-proxy.org/anthropic",
    custom_llm_provider="anthropic",
)

print(resp)
