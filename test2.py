import os, json, requests

API_KEY_302 = os.getenv("API_KEY_302")
assert API_KEY_302, "❌ Please export API_KEY_302 first"

BASE = "https://api.302.ai/v1/chat/completions"
headers = {"Content-Type": "application/json", "Authorization": f"Bearer {API_KEY_302}"}

# -- Normal text mode
payload_text = {
    "model": "gpt-4o-2024-08-06",
    "messages": [{"role": "user", "content": "Tell me a short joke about compilers."}],
    "temperature": 0.5,
}

print("\n→ Normal chat mode:")
resp = requests.post(BASE, headers=headers, json=payload_text)
print(resp.status_code)
print(json.dumps(resp.json(), indent=2, ensure_ascii=False))

# -- Tool calling mode
tools = [
    {
        "type": "function",
        "function": {
            "name": "get_current_weather",
            "description": "Get the current weather in a given location",
            "parameters": {
                "type": "object",
                "properties": {
                    "location": {"type": "string"},
                    "unit": {"type": "string", "enum": ["celsius", "fahrenheit"]},
                },
                "required": ["location"],
            },
        },
    }
]

payload_tools = {
    "model": "gpt-4o-2024-08-06",
    "messages": [{"role": "user", "content": "What's the weather in Beijing?"}],
    "tools": tools,
    "tool_choice": "auto",
    "temperature": 0.5,
}

print("\n→ Tool calling mode:")
resp = requests.post(BASE, headers=headers, json=payload_tools)
print(resp.status_code)
print(json.dumps(resp.json(), indent=2, ensure_ascii=False))
