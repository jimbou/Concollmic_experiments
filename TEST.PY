import litellm
import os
resp = litellm.completion(
    model="gpt-4.1-mini",
    api_base="https://api.302.ai/v1",
    api_key=os.environ["API_KEY_302"],
    custom_headers={"Authorization": f"302AI {os.environ['API_KEY_302']}"},
    messages=[{"role":"user", "content":"Say hello in one short sentence."}],
)
print(resp.choices[0].message.content)
