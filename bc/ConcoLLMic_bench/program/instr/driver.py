import subprocess
import signal
from pathlib import Path

def execute_program(timeout: int) -> tuple[str, int]:
    script_dir = Path(__file__).resolve().parent
    exe = script_dir / "temp_executable"

    try:
        result = subprocess.run(
            [str(exe), "-e", "1+1"],
            text=True,
            capture_output=True,
            timeout=timeout,
        )
        return result.stderr, result.returncode

    except subprocess.TimeoutExpired as e:
        return e.stderr, -signal.SIGKILL

    except Exception:
        raise


