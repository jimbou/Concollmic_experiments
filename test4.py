import litellm
import os

litellm._turn_on_debug()

api_key = os.getenv("CLOSE_API_KEY")

response = litellm.completion(
    model="claude-3-7-sonnet-latest",
    messages=[{"role": "user", "content": "Say OK"}],
    api_key=api_key,
    base_url="https://api.openai-proxy.org/anthropic",
    custom_llm_provider="anthropic",     # IMPORTANT!!!
    max_tokens=20
)

print("RESPONSE:", response)
