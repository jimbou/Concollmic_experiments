import subprocess
import sys
import signal

def execute_program(timeout: int) -> tuple[str, int]:
    
    xml_input = "<root></root>\n"

    try:
        result = subprocess.run(
            ["./temp_executable"],     # xmlwf binary
            input=xml_input,           # sent to stdin
            text=True,
            capture_output=True,
            timeout=timeout,
        )

        return result.stderr, result.returncode

    except subprocess.TimeoutExpired as e:
        return e.stderr, -signal.SIGKILL

    except Exception as e:
        raise e
