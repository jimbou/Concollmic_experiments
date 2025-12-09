#!/usr/bin/env bash
set -euo pipefail

BASE_DIR="/home/jim/ConcoLLMic/fdlibm"

echo "🐍 Generating driver.py files from *_driver.sh ..."
echo

find "$BASE_DIR" -type d -name instr | while read -r instr_dir; do
  sh_driver=$(ls "$instr_dir"/*_driver.sh 2>/dev/null || true)

  if [[ -z "$sh_driver" ]]; then
    echo "⚠️  No *_driver.sh found in: $instr_dir"
    continue
  fi

  echo "✅ Processing: $sh_driver"

  # Extract the last non-comment command (the actual execution line)
  CMD=$(grep -v '^\s*#' "$sh_driver" | tail -n 1 | sed 's/^[[:space:]]*//')

  if [[ -z "$CMD" ]]; then
    echo "❌ Could not extract command from $sh_driver"
    continue
  fi

  PY_DRIVER="$instr_dir/driver.py"

  cat > "$PY_DRIVER" <<EOF
def execute_program(timeout: int) -> tuple[str, int]:
    import signal
    import subprocess

    try:
        result = subprocess.run(
            ["$CMD"],
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
EOF

  echo "📝 Created: $PY_DRIVER"
done

echo
echo "✅ All driver.py files generated successfully."
