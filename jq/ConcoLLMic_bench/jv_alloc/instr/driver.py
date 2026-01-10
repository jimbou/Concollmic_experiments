import subprocess
import signal
from pathlib import Path

def execute_program(timeout: int) -> tuple[str, int]:
    script_dir = Path(__file__).resolve().parent
    exe = script_dir / "temp_executable"   # jq binary

    # Minimal valid JSON to force parser path
    json_input = "{}"

    try:
        result = subprocess.run(
            [str(exe), "."],        # FILTER only
            input=json_input,       # JSON provided explicitly
            text=True,
            capture_output=True,
            timeout=timeout,
        )
        return result.stderr, result.returncode

    except subprocess.TimeoutExpired as e:
        return e.stderr or "", -signal.SIGKILL

#main
if __name__ == "__main__":
    timeout_seconds = 5
    stderr_output, return_code = execute_program(timeout_seconds)
    print("Return Code:", return_code)
    print("Stderr Output:")
    print(stderr_output)