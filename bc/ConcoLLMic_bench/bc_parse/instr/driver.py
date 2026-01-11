import subprocess
import signal

def execute_program(timeout: int) -> tuple[str, int]:
    exe = "./temp_executable"

    try:
        result = subprocess.run(
            [str(exe)],              # no -e
            input="1+1\n",           # pass expression via stdin
            text=True,
            capture_output=True,
            timeout=timeout,
        )
        return result.stderr, result.returncode

    except subprocess.TimeoutExpired as e:
        return e.stderr, -signal.SIGKILL

    except Exception:
        raise


#main   
if __name__ == "__main__":
    stderr, returncode = execute_program(timeout=5)
    print(f"Return code: {returncode}")
    print(f"Stderr: {stderr}")