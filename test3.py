import os
from openai import OpenAI

# Make sure you've exported:
#   export CLOSE_API_KEY="your_key_here"
#   export OPENAI_BASE_URL="https://api.openai-proxy.org/v1"
# or set them manually here:
api_key = os.getenv("CLOSE_API_KEY")
base_url = os.getenv("OPENAI_BASE_URL", "https://api.openai-proxy.org/v1")

if not api_key:
    raise EnvironmentError("❌ CLOSE_API_KEY is not set in your environment!")

# Instantiate client
client = OpenAI(api_key=api_key, base_url=base_url)

# === Simple text completion test ===
print("➡️ Sending test message to CloseAI...")
response = client.chat.completions.create(
    model="gpt-4o-mini",  # or whichever CloseAI model you have access to
    messages=[
        {"role": "system", "content": "You are a concise assistant."},
        {"role": "user", "content": "Say hello and tell me the current year."},
    ],
)

print("\n✅ Response:")
print(response.choices[0].message.content)
