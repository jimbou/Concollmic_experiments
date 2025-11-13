# #!/usr/bin/env python3
# """
# Harness that always executes a binary with one fixed string argument.
# Compatible with ConcoLLMic's expected signature: execute_program(timeout: int).
# """

# import subprocess
# import signal
# from typing import Tuple

# def execute_program(timeout: int) -> Tuple[str, int]:
#     """
#     Execute the instrumented program with one string argument.
#     Returns (stderr_text, return_code).
#     """
#     # ✅ Always run this binary (adjust path if needed)
#     exe_path = "./temp_executable"

#     # ✅ Always pass one string argument (change if your program needs specific format)
#     arg = "AAAA"

#     try:
#         result = subprocess.run(
#             [exe_path, arg],
#             capture_output=True,
#             text=True,
#             timeout=timeout,
#         )
#         return result.stderr or "", result.returncode
#     except subprocess.TimeoutExpired as e:
#         stderr_text = e.stderr if isinstance(e.stderr, str) else ""
#         return stderr_text, -getattr(signal, "SIGKILL", 9)
#     except Exception as e:
#         return f"Harness error: {type(e).__name__}: {e}", -1


def execute_program(timeout: int) -> tuple[str, int]:
    import signal
    import subprocess

    try:
        result = subprocess.run(
            ["./temp_executable AAAA"],
            capture_output=True,
            encoding="utf-8",
            errors="replace",
            shell=True,
            timeout=timeout,
        )
        # Return stderr and the return code
        return result.stderr, result.returncode
    except subprocess.TimeoutExpired as e:
        # Timeout occurred, also ensure to return stderr captured before timeout and return code -signal.SIGKILL
        return e.stderr, -signal.SIGKILL
    except Exception as e:
        # ensure to raise the error if run failed
        raise e
