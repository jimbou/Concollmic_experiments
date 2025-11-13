import os
import traceback

from app.model.closeai import (
    GPT4o,
    GPT4oMini,
    GPT5,
    Claude37SonnetLatest,
    ClaudeSonnet40,
    Gemini25Pro,
    Gemini25Flash,
    DeepSeekChat,
    DeepSeekReasoner
)

# Ensure env vars
# os.environ.setdefault("CLOSE_API_KEY", "")
os.environ.setdefault("OPENAI_BASE_URL", "https://api.openai-proxy.org/v1")

MODELS = [
    # GPT4o(),
    # GPT4oMini(),
    # GPT5(),
    # Claude37SonnetLatest(),
    # ClaudeSonnet40(),
    # Gemini25Pro(),
    # Gemini25Flash(),
    DeepSeekChat(),
    DeepSeekReasoner(),
]

# Simple messages exactly like your instrumenter uses
MESSAGES = [
    {"role": "system", "content": "You are a test model."},
    {"role": "user", "content": "Reply with the word OK."},
]

def test_model(model):
    print("=" * 60)
    print(f"TESTING MODEL: {model.name}")
    try:
        content, tools, usage = model.call(MESSAGES)
        print(f"SUCCESS for {model.name}: {content}")
    except Exception as e:
        print(f"ERROR for {model.name}: {type(e).__name__}: {e}")
        traceback.print_exc()
    print("=" * 60)
    print()

if __name__ == "__main__":
    print("\n### CloseAI Model Test Runner ###\n")
    for m in MODELS:
        test_model(m)

