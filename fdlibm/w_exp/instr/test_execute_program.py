
import signal
import subprocess
import os

def execute_program(timeout: int) -> tuple[str, int]:
    try:
        # Set up environment with LD_PRELOAD
        env = os.environ.copy()
        env["LD_PRELOAD"] = "./libcustommath.so"
        
        result = subprocess.run(
            ["./temp_executable", "1.0"],
            capture_output=True,
            encoding="utf-8",
            errors="replace",
            shell=True,
            timeout=timeout,
            env=env,
        )
        # Return stderr and the return code
        return result.stderr, result.returncode
    except subprocess.TimeoutExpired as e:
        # Timeout occurred, also ensure to return stderr captured before timeout and return code -signal.SIGKILL
        return e.stderr, -signal.SIGKILL
    except Exception as e:
        # ensure to raise the error if run failed
        raise e

# Test the function
stderr, return_code = execute_program(5)
print(f"STDERR: {stderr}")
print(f"Return code: {return_code}")
