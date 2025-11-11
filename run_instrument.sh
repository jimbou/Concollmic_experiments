#!/usr/bin/env bash
set -euo pipefail

# =====================================================
# Multi-file ACE instrumentation runner with JSON report
# =====================================================

# Output summary JSON file
SUMMARY_JSON="instrumentation_summary.json"
TMP_JSON=$(mktemp)

# Initialize empty JSON array
echo "[]" > "$TMP_JSON"

# Helper to append JSON object
append_result() {
  local file="$1"
  local status="$2"
  local log="$3"
  jq --arg f "$file" --arg s "$status" --arg l "$log" \
    '. += [{"file":$f,"status":$s,"log":$l}]' "$TMP_JSON" > "$TMP_JSON.tmp" && mv "$TMP_JSON.tmp" "$TMP_JSON"
}

# Wrapper to execute one instrumentation command
run_instrument() {
  local src="$1"
  local out="$2"
  local name
  #in the name get the last 2 dirs of the path
    name=$(basename "$(dirname "$src")")/$(basename "$src")

  echo "=============================="
  echo "Instrumenting: $name"
  echo "=============================="

  local log_file="logs/${name}_instrument.log"
  mkdir -p logs

#PRINT THE PYTHON COMMAND BEING RUN
    echo "Running: python3 ACE.py instrument --src_dir \"$src\" --out_dir \"$out\" --instr_languages c &> \"$log_file\""
    #create the log first
    mkdir -p "$(dirname "$log_file")"
    : > "$log_file"

     {
    echo "=== Instrumentation started: $(date) ==="
    echo "Source dir: $src"
    echo "Output dir: $out"
    echo
  } > "$log_file"
  if python3 ACE.py instrument \
      --src_dir "$src" \
      --out_dir "$out" \
      --instr_languages c &> "$log_file"; then
    echo "✅ SUCCESS: $name"
    append_result "$name" "success" "$log_file"
  else
    echo "❌ FAILED: $name (see $log_file)"
    append_result "$name" "failed" "$log_file"
  fi
}

# ========================
# Run all instrumentations
# ========================

# run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/7n+1_lo_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/7n+1_lo_l1_klee/instr"
# run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/5n+1_lo_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/5n+1_lo_l1_klee/instr"


run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/pid_csv_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/pid_csv_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_ln_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_ln_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/sha_cf_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/sha_cf_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/file_posix_csv_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/file_posix_csv_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/sin_ef_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/sin_ef_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/file_posix_cp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/file_posix_cp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/rand_ef_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/rand_ef_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/mthread_pp_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/mthread_pp_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stackoutofbound_sm_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stackoutofbound_sm_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/heap_bo_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/heap_bo_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/float3_fp_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/float3_fp_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/heapoutofbound_sm_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/heapoutofbound_sm_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/printint_int_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/printint_int_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/aes_cf_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/aes_cf_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stack_cp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stack_cp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/paraloop_lo_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/paraloop_lo_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/arrayjmp_sj_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/arrayjmp_sj_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/float5_fp_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/float5_fp_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/file_cp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/file_cp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/printfloat_ef_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/printfloat_ef_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/addint_to_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/addint_to_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/socket_cp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/socket_cp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/float4_fp_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/float4_fp_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/forkpipe_pp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/forkpipe_pp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/atof_ef_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/atof_ef_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/echo_cp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/echo_cp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/syscall_csv_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/syscall_csv_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/pointers_sj_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/pointers_sj_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/echofile_cp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/echofile_cp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/file_csv_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/file_csv_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/realloc_sm_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/realloc_sm_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/df2cf_cp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/df2cf_cp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/atoi_ef_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/atoi_ef_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/malloc_sm_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/malloc_sm_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/multiplyint_to_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/multiplyint_to_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/float1_fp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/float1_fp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/float2_fp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/float2_fp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/stacknocrash_bo_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/stacknocrash_bo_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/ping_csv_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/ping_csv_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/forkshm_pp_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/forkshm_pp_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/jmp_sj_l1_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/jmp_sj_l1_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/pow_ef_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/pow_ef_l2_klee/instr"
run_instrument "/home/jim/ConcoLLMic/logic_bombs/copies/ln_ef_l2_klee/src" "/home/jim/ConcoLLMic/logic_bombs/copies/ln_ef_l2_klee/instr"

# =======================
# Final summary
# =======================
mv "$TMP_JSON" "$SUMMARY_JSON"
echo
echo "📜 Instrumentation summary written to $SUMMARY_JSON"
jq . "$SUMMARY_JSON"


# # Auto-generated instrument commands.
# # Run this script to invoke ACE instrumentation for each file.

# # For file: pid_csv_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/pid_csv_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/pid_csv_klee/instr \
#     --instr_languages c

# # For file: stackarray_sm_ln_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_ln_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_ln_klee/instr \
#     --instr_languages c

# # For file: sha_cf_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/sha_cf_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/sha_cf_klee/instr \
#     --instr_languages c

# # For file: 2thread_pp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l1_klee/instr \
#     --instr_languages c

# # For file: file_posix_csv_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_posix_csv_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_posix_csv_klee/instr \
#     --instr_languages c

# # For file: sin_ef_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/sin_ef_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/sin_ef_l2_klee/instr \
#     --instr_languages c

# # For file: file_posix_cp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_posix_cp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_posix_cp_l1_klee/instr \
#     --instr_languages c

# # For file: rand_ef_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/rand_ef_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/rand_ef_l2_klee/instr \
#     --instr_languages c

# # For file: stack_bo_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l2_klee/instr \
#     --instr_languages c

# # For file: mthread_pp_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/mthread_pp_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/mthread_pp_l2_klee/instr \
#     --instr_languages c

# # For file: stackoutofbound_sm_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackoutofbound_sm_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackoutofbound_sm_l2_klee/instr \
#     --instr_languages c

# # For file: heap_bo_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/heap_bo_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/heap_bo_l1_klee/instr \
#     --instr_languages c

# # For file: float3_fp_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/float3_fp_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/float3_fp_l2_klee/instr \
#     --instr_languages c

# # For file: heapoutofbound_sm_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/heapoutofbound_sm_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/heapoutofbound_sm_l2_klee/instr \
#     --instr_languages c

# # For file: 5n+1_lo_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/5n+1_lo_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/5n+1_lo_l1_klee/instr \
#     --instr_languages c

# # For file: printint_int_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/printint_int_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/printint_int_l1_klee/instr \
#     --instr_languages c

# # For file: aes_cf_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/aes_cf_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/aes_cf_klee/instr \
#     --instr_languages c

# # For file: stack_cp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stack_cp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stack_cp_l1_klee/instr \
#     --instr_languages c

# # For file: paraloop_lo_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/paraloop_lo_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/paraloop_lo_l2_klee/instr \
#     --instr_languages c

# # For file: arrayjmp_sj_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/arrayjmp_sj_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/arrayjmp_sj_l2_klee/instr \
#     --instr_languages c

# # For file: float5_fp_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/float5_fp_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/float5_fp_l2_klee/instr \
#     --instr_languages c

# # For file: file_cp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_cp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_cp_l1_klee/instr \
#     --instr_languages c

# # For file: printfloat_ef_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/printfloat_ef_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/printfloat_ef_l1_klee/instr \
#     --instr_languages c

# # For file: addint_to_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/addint_to_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/addint_to_l1_klee/instr \
#     --instr_languages c

# # For file: socket_cp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/socket_cp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/socket_cp_l1_klee/instr \
#     --instr_languages c

# # For file: float4_fp_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/float4_fp_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/float4_fp_l2_klee/instr \
#     --instr_languages c

# # For file: forkpipe_pp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/forkpipe_pp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/forkpipe_pp_l1_klee/instr \
#     --instr_languages c

# # For file: atof_ef_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/atof_ef_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/atof_ef_l2_klee/instr \
#     --instr_languages c

# # For file: echo_cp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/echo_cp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/echo_cp_l1_klee/instr \
#     --instr_languages c

# # For file: stackarray_sm_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l2_klee/instr \
#     --instr_languages c

# # For file: syscall_csv_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/syscall_csv_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/syscall_csv_klee/instr \
#     --instr_languages c

# # For file: 2thread_pp_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/2thread_pp_l2_klee/instr \
#     --instr_languages c

# # For file: stackarray_sm_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stackarray_sm_l1_klee/instr \
#     --instr_languages c

# # For file: pointers_sj_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/pointers_sj_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/pointers_sj_l1_klee/instr \
#     --instr_languages c

# # For file: 7n+1_lo_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/7n+1_lo_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/7n+1_lo_l1_klee/instr \
#     --instr_languages c

# # For file: echofile_cp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/echofile_cp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/echofile_cp_l1_klee/instr \
#     --instr_languages c

# # For file: file_csv_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_csv_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/file_csv_klee/instr \
#     --instr_languages c

# # For file: realloc_sm_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/realloc_sm_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/realloc_sm_l1_klee/instr \
#     --instr_languages c

# # For file: df2cf_cp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/df2cf_cp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/df2cf_cp_l1_klee/instr \
#     --instr_languages c

# # For file: atoi_ef_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/atoi_ef_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/atoi_ef_l2_klee/instr \
#     --instr_languages c

# # For file: malloc_sm_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/malloc_sm_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/malloc_sm_l1_klee/instr \
#     --instr_languages c

# # For file: multiplyint_to_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/multiplyint_to_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/multiplyint_to_l1_klee/instr \
#     --instr_languages c

# # For file: float1_fp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/float1_fp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/float1_fp_l1_klee/instr \
#     --instr_languages c

# # For file: float2_fp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/float2_fp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/float2_fp_l1_klee/instr \
#     --instr_languages c

# # For file: collaz_lo_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l2_klee/instr \
#     --instr_languages c

# # For file: stack_bo_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stack_bo_l1_klee/instr \
#     --instr_languages c

# # For file: stacknocrash_bo_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/stacknocrash_bo_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/stacknocrash_bo_l1_klee/instr \
#     --instr_languages c

# # For file: collaz_lo_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/collaz_lo_l1_klee/instr \
#     --instr_languages c

# # For file: ping_csv_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/ping_csv_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/ping_csv_klee/instr \
#     --instr_languages c

# # For file: forkshm_pp_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/forkshm_pp_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/forkshm_pp_l1_klee/instr \
#     --instr_languages c

# # For file: jmp_sj_l1_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/jmp_sj_l1_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/jmp_sj_l1_klee/instr \
#     --instr_languages c

# # For file: pow_ef_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/pow_ef_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/pow_ef_l2_klee/instr \
#     --instr_languages c

# # For file: ln_ef_l2_klee.c
# python3 ACE.py instrument \
#     --src_dir /home/jim/ConcoLLMic/logic_bombs/copies/ln_ef_l2_klee/src \
#     --out_dir /home/jim/ConcoLLMic/logic_bombs/copies/ln_ef_l2_klee/instr \
#     --instr_languages c
