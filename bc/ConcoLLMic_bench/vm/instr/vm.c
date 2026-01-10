/*
 * *****************************************************************************
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018-2025 Gavin D. Howard and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * *****************************************************************************
 *
 * Code common to all of bc and dc.
 *
 */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>

#include <signal.h>

#include <setjmp.h>

#ifndef _WIN32

#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#else // _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <io.h>

#endif // _WIN32

#include <status.h>
#include <vector.h>
#include <args.h>
#include <vm.h>
#include <read.h>
#include <bc.h>
#if BC_ENABLE_LIBRARY
#include <library.h>
#endif // BC_ENABLE_LIBRARY
#if BC_ENABLE_OSSFUZZ
#include <ossfuzz.h>
#endif // BC_ENABLE_OSSFUZZ

#if !BC_ENABLE_LIBRARY

// The actual globals.
char output_bufs[BC_VM_BUF_SIZE];
BcVm vm_data;
BcVm* vm = &vm_data;

#endif // !BC_ENABLE_LIBRARY

#if BC_DEBUG_CODE
BC_NORETURN void
bc_vm_jmp(const char* f)
{
#else // BC_DEBUG_CODE
BC_NORETURN void
bc_vm_jmp(void)
{
#endif

	fprintf(stderr, "[vm.c] enter bc_vm_jmp 1\n");
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	assert(BC_SIG_EXC(vm));

	BC_SIG_MAYLOCK;
	// fprintf(stderr, "[vm.c] exit bc_vm_jmp 1\n");

#if BC_DEBUG_CODE
	fprintf(stderr, "[vm.c] enter bc_vm_jmp 2\n");
	bc_file_puts(&vm->ferr, bc_flush_none, "Longjmp: ");
	bc_file_puts(&vm->ferr, bc_flush_none, f);
	bc_file_putchar(&vm->ferr, bc_flush_none, '\n');
	bc_file_flush(&vm->ferr, bc_flush_none);
	// fprintf(stderr, "[vm.c] exit bc_vm_jmp 2\n");
#endif // BC_DEBUG_CODE

#if BC_DEBUG
	fprintf(stderr, "[vm.c] enter bc_vm_jmp 3\n");
	assert(vm->jmp_bufs.len - (size_t) vm->sig_pop);
	// fprintf(stderr, "[vm.c] exit bc_vm_jmp 3\n");
#endif // BC_DEBUG

	fprintf(stderr, "[vm.c] enter bc_vm_jmp 4\n");
	if (vm->jmp_bufs.len == 0) abort();
	// fprintf(stderr, "[vm.c] exit bc_vm_jmp 4\n");

	fprintf(stderr, "[vm.c] enter bc_vm_jmp 5\n");
	if (vm->sig_pop) bc_vec_pop(&vm->jmp_bufs);
	else vm->sig_pop = 1;
	// fprintf(stderr, "[vm.c] exit bc_vm_jmp 5\n");

	fprintf(stderr, "[vm.c] enter bc_vm_jmp 6\n");
	siglongjmp(*((sigjmp_buf*) bc_vec_top(&vm->jmp_bufs)), 1);
	// fprintf(stderr, "[vm.c] exit bc_vm_jmp 6\n");
}

#if !BC_ENABLE_LIBRARY

/**
 * Handles signals. This is the signal handler.
 * @param sig  The signal to handle.
 */
static void
bc_vm_sig(int sig)
{
	fprintf(stderr, "[vm.c] enter bc_vm_sig 1\n");
	// There is already a signal in flight if this is true.
	if (vm->status == (sig_atomic_t) BC_STATUS_QUIT || vm->sig != 0)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_sig 2\n");
		if (!BC_I || sig != SIGINT) vm->status = BC_STATUS_QUIT;
		return;
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 2\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_sig 1\n");

	fprintf(stderr, "[vm.c] enter bc_vm_sig 3\n");
	// We always want to set this because a stack trace can be printed if we do.
	vm->sig = sig;
	// fprintf(stderr, "[vm.c] exit bc_vm_sig 3\n");

	fprintf(stderr, "[vm.c] enter bc_vm_sig 4\n");
	// Only reset under these conditions; otherwise, quit.
	if (sig == SIGINT && BC_SIGINT && BC_I)
	// fprintf(stderr, "[vm.c] exit bc_vm_sig 4\n");
	{
		fprintf(stderr, "[vm.c] enter bc_vm_sig 5\n");
		int err = errno;
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 5\n");

#if BC_ENABLE_EDITLINE
		fprintf(stderr, "[vm.c] enter bc_vm_sig 6\n");
		// Editline needs this, for some unknown reason.
		if (write(STDOUT_FILENO, "^C", 2) != (ssize_t) 2)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_sig 7\n");
			vm->status = BC_STATUS_ERROR_FATAL;
			// fprintf(stderr, "[vm.c] exit bc_vm_sig 7\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 6\n");
#endif // BC_ENABLE_EDITLINE

		fprintf(stderr, "[vm.c] enter bc_vm_sig 8\n");
		// Write the message.
		if (write(STDOUT_FILENO, vm->sigmsg, vm->siglen) !=
		    (ssize_t) vm->siglen)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_sig 9\n");
			vm->status = BC_STATUS_ERROR_FATAL;
			// fprintf(stderr, "[vm.c] exit bc_vm_sig 9\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 8\n");

		fprintf(stderr, "[vm.c] enter bc_vm_sig 10\n");
		errno = err;
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 10\n");
	}
	else
	{
		fprintf(stderr, "\n");
#if BC_ENABLE_EDITLINE
		fprintf(stderr, "[vm.c] enter bc_vm_sig 12\n");
		if (write(STDOUT_FILENO, "^C", 2) != (ssize_t) 2)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_sig 13\n");
			vm->status = BC_STATUS_ERROR_FATAL;
			return;
			// fprintf(stderr, "[vm.c] exit bc_vm_sig 13\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 12\n");
#endif // BC_ENABLE_EDITLINE

		fprintf(stderr, "[vm.c] enter bc_vm_sig 14\n");
		vm->status = BC_STATUS_QUIT;
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 14\n");
	}

#if BC_ENABLE_LINE_LIB
	fprintf(stderr, "[vm.c] enter bc_vm_sig 15\n");
	// Readline and Editline need this to actually handle sigints correctly.
	if (sig == SIGINT && bc_history_inlinelib)
	// fprintf(stderr, "[vm.c] exit bc_vm_sig 15\n");
	{
		fprintf(stderr, "[vm.c] enter bc_vm_sig 16\n");
		bc_history_inlinelib = 0;
		siglongjmp(bc_history_jmpbuf, 1);
		// fprintf(stderr, "[vm.c] exit bc_vm_sig 16\n");
	}
#endif // BC_ENABLE_LINE_LIB

	fprintf(stderr, "[vm.c] enter bc_vm_sig 17\n");
	assert(vm->jmp_bufs.len);
	// fprintf(stderr, "[vm.c] exit bc_vm_sig 17\n");

	fprintf(stderr, "[vm.c] enter bc_vm_sig 18\n");
	// Only jump if signals are not locked. The jump will happen by whoever
	// unlocks signals.
	if (!vm->sig_lock) BC_JMP;
	// fprintf(stderr, "[vm.c] exit bc_vm_sig 18\n");
}
/**
 * Sets up signal handling.
 */
static void
bc_vm_sigaction(void)
{
  fprintf(stderr, "[vm.c] enter bc_vm_sigaction 1\n");
#ifndef _WIN32

	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_NODEFER;

	// This mess is to silence a warning on Clang with regards to glibc's
	// sigaction handler, which activates the warning here.
#if BC_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif // BC_CLANG
	sa.sa_handler = bc_vm_sig;
#if BC_CLANG
#pragma clang diagnostic pop
#endif // BC_CLANG

	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

#if BC_ENABLE_HISTORY
	if (BC_TTY) sigaction(SIGHUP, &sa, NULL);
#endif // BC_ENABLE_HISTORY

#else // _WIN32

	signal(SIGTERM, bc_vm_sig);
	signal(SIGINT, bc_vm_sig);

#endif // _WIN32
  // fprintf(stderr, "[vm.c] exit bc_vm_sigaction 1\n");
}

void
bc_vm_info(const char* const help)
{
  fprintf(stderr, "[vm.c] enter bc_vm_info 1\n");
	BC_SIG_ASSERT_LOCKED;

	// Print the banner.
	bc_file_printf(&vm->fout, "%s %s\n%s", vm->name, BC_VERSION, bc_copyright);

	// Print the help.
	if (help != NULL)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_info 2\n");
		bc_file_putchar(&vm->fout, bc_flush_none, '\n');

#if BC_ENABLED
		if (BC_IS_BC)
		{
      fprintf(stderr, "[vm.c] enter bc_vm_info 3\n");
			const char* const banner = BC_DEFAULT_BANNER ? "to" : "to not";
			const char* const sigint = BC_DEFAULT_SIGINT_RESET ? "to reset" :
			                                                     "to exit";
			const char* const tty = BC_DEFAULT_TTY_MODE ? "enabled" :
			                                              "disabled";
			const char* const prompt = BC_DEFAULT_PROMPT ? "enabled" :
			                                               "disabled";
			const char* const expr = BC_DEFAULT_EXPR_EXIT ? "to exit" :
			                                                "to not exit";
			const char* const clamp = BC_DEFAULT_DIGIT_CLAMP ? "to clamp" :
			                                                   "to not clamp";

			bc_file_printf(&vm->fout, help, vm->name, vm->name, BC_VERSION,
			               BC_BUILD_TYPE, banner, sigint, tty, prompt, expr,
			               clamp);
      // fprintf(stderr, "[vm.c] exit bc_vm_info 3\n");
		}
#endif // BC_ENABLED

#if DC_ENABLED
		if (BC_IS_DC)
		{
      fprintf(stderr, "[vm.c] enter bc_vm_info 4\n");
			const char* const sigint = DC_DEFAULT_SIGINT_RESET ? "to reset" :
			                                                     "to exit";
			const char* const tty = DC_DEFAULT_TTY_MODE ? "enabled" :
			                                              "disabled";
			const char* const prompt = DC_DEFAULT_PROMPT ? "enabled" :
			                                               "disabled";
			const char* const expr = DC_DEFAULT_EXPR_EXIT ? "to exit" :
			                                                "to not exit";
			const char* const clamp = DC_DEFAULT_DIGIT_CLAMP ? "to clamp" :
			                                                   "to not clamp";

			bc_file_printf(&vm->fout, help, vm->name, vm->name, BC_VERSION,
			               BC_BUILD_TYPE, sigint, tty, prompt, expr, clamp);
      // fprintf(stderr, "[vm.c] exit bc_vm_info 4\n");
		}
#endif // DC_ENABLED
    // fprintf(stderr, "[vm.c] exit bc_vm_info 2\n");
	}

	// Flush.
	bc_file_flush(&vm->fout, bc_flush_none);
  // fprintf(stderr, "[vm.c] exit bc_vm_info 1\n");
}
#endif // !BC_ENABLE_LIBRARY

#if !BC_ENABLE_LIBRARY && !BC_ENABLE_MEMCHECK
BC_NORETURN
#endif // !BC_ENABLE_LIBRARY && !BC_ENABLE_MEMCHECK
void
bc_vm_fatalError(BcErr e)
{
  fprintf(stderr, "[vm.c] enter bc_vm_fatalError 1\n");
	bc_err(e);
#if !BC_ENABLE_LIBRARY && !BC_ENABLE_MEMCHECK
	BC_UNREACHABLE
#if !BC_CLANG
	abort();
#endif // !BC_CLANG
#endif // !BC_ENABLE_LIBRARY && !BC_ENABLE_MEMCHECK
  // fprintf(stderr, "[vm.c] exit bc_vm_fatalError 1\n");
}

#if BC_ENABLE_LIBRARY
BC_NORETURN void
bc_vm_handleError(BcErr e)
{
  fprintf(stderr, "[vm.c] enter bc_vm_handleError 1\n");
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	assert(e < BC_ERR_NELEMS);
	assert(!vm->sig_pop);

	BC_SIG_LOCK;

	// If we have a normal error...
	if (e <= BC_ERR_MATH_DIVIDE_BY_ZERO)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_handleError 2\n");
		// Set the error.
		vm->err = (BclError) (e - BC_ERR_MATH_NEGATIVE +
		                      BCL_ERROR_MATH_NEGATIVE);
    // fprintf(stderr, "[vm.c] exit bc_vm_handleError 2\n");
	}
	// Abort if we should.
	else if (vm->abrt) 
  {
    fprintf(stderr, "[vm.c] enter bc_vm_handleError 3\n");
    abort();
    // fprintf(stderr, "[vm.c] exit bc_vm_handleError 3\n");
  }
	else if (e == BC_ERR_FATAL_ALLOC_ERR) 
  {
    fprintf(stderr, "[vm.c] enter bc_vm_handleError 4\n");
    vm->err = BCL_ERROR_FATAL_ALLOC_ERR;
    // fprintf(stderr, "[vm.c] exit bc_vm_handleError 4\n");
  }
	else 
  {
    fprintf(stderr, "[vm.c] enter bc_vm_handleError 5\n");
    vm->err = BCL_ERROR_FATAL_UNKNOWN_ERR;
    // fprintf(stderr, "[vm.c] exit bc_vm_handleError 5\n");
  }

	BC_JMP;
  // fprintf(stderr, "[vm.c] exit bc_vm_handleError 1\n");
}
#else // BC_ENABLE_LIBRARY
#if BC_DEBUG
void
bc_vm_handleError(BcErr e, const char* file, int fline, size_t line, ...)
#else // BC_DEBUG
void
bc_vm_handleError(BcErr e, size_t line, ...)
#endif // BC_DEBUG
{
	fprintf(stderr, "[vm.c] enter bc_vm_handleError 4938\n");
	BcStatus s;
	BcStatus fout_s;
	va_list args;
	uchar id = bc_err_ids[e];
	const char* err_type = vm->err_ids[id];
	sig_atomic_t lock;

	assert(e < BC_ERR_NELEMS);
	assert(!vm->sig_pop);
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 4938\n");

#if BC_ENABLED
	// Figure out if the POSIX error should be an error, a warning, or nothing.
	if (!BC_S && e >= BC_ERR_POSIX_START)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 4958\n");
		if (BC_W)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_handleError 1759\n");
			// Make sure to not return an error.
			id = UCHAR_MAX;
			err_type = vm->err_ids[BC_ERR_IDX_WARN];
			// fprintf(stderr, "[vm.c] exit bc_vm_handleError 1759\n");
		}
		else
		{
			fprintf(stderr, "[vm.c] enter bc_vm_handleError 4820\n");
			return;
			// fprintf(stderr, "[vm.c] exit bc_vm_handleError 4820\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 4958\n");
	}
#endif // BC_ENABLED

	fprintf(stderr, "[vm.c] enter bc_vm_handleError 4756\n");
	BC_SIG_TRYLOCK(lock);

	// Make sure all of stdout is written first.
	fout_s = bc_file_flushErr(&vm->fout, bc_flush_err);

	// XXX: Keep the status for later.

	// Print the error message.
	va_start(args, line);
	bc_file_putchar(&vm->ferr, bc_flush_none, '\n');
	bc_file_puts(&vm->ferr, bc_flush_none, err_type);
	bc_file_putchar(&vm->ferr, bc_flush_none, ' ');
	bc_file_vprintf(&vm->ferr, vm->err_msgs[e], args);
	va_end(args);
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 4756\n");

	// Print the extra information if we have it.
	if (BC_NO_ERR(vm->file != NULL))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 6\n");
		// This is the condition for parsing vs runtime.
		// If line is not 0, it is parsing.
		if (line)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_handleError 7\n");
			bc_file_puts(&vm->ferr, bc_flush_none, "\n    ");
			bc_file_puts(&vm->ferr, bc_flush_none, vm->file);
			bc_file_printf(&vm->ferr, ":%zu\n", line);
			// fprintf(stderr, "[vm.c] exit bc_vm_handleError 7\n");
		}
		else
		{
			fprintf(stderr, "[vm.c] enter bc_vm_handleError 8\n");
			// Print a stack trace.
			bc_file_putchar(&vm->ferr, bc_flush_none, '\n');
			bc_program_printStackTrace(&vm->prog);
			// fprintf(stderr, "[vm.c] exit bc_vm_handleError 8\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 6\n");
	}
	else
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 9\n");
		bc_file_putchar(&vm->ferr, bc_flush_none, '\n');
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 9\n");
	}

#if BC_DEBUG
	fprintf(stderr, "[vm.c] enter bc_vm_handleError 10\n");
	bc_file_printf(&vm->ferr, "\n    %s:%d\n", file, fline);
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 10\n");
#endif // BC_DEBUG

	fprintf(stderr, "[vm.c] enter bc_vm_handleError 11\n");
	bc_file_puts(&vm->ferr, bc_flush_none, "\n");
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 11\n");

	// If flushing to stdout failed, try to print *that* error, as long as that
	// was not the error already.
	if (fout_s == BC_STATUS_ERROR_FATAL && e != BC_ERR_FATAL_IO_ERR)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 12\n");
		bc_file_putchar(&vm->ferr, bc_flush_none, '\n');
		bc_file_puts(&vm->ferr, bc_flush_none,
		             vm->err_ids[bc_err_ids[BC_ERR_FATAL_IO_ERR]]);
		bc_file_putchar(&vm->ferr, bc_flush_none, ' ');
		bc_file_puts(&vm->ferr, bc_flush_none,
		             vm->err_msgs[BC_ERR_FATAL_IO_ERR]);
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 12\n");
	}

	fprintf(stderr, "[vm.c] enter bc_vm_handleError 13\n");
	s = bc_file_flushErr(&vm->ferr, bc_flush_err);
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 13\n");

#if !BC_ENABLE_MEMCHECK

	fprintf(stderr, "[vm.c] enter bc_vm_handleError 14\n");
	// Because this function is called by a BC_NORETURN function when fatal
	// errors happen, we need to make sure to exit on fatal errors. This will
	// be faster anyway. This function *cannot jump when a fatal error occurs!*
	if (BC_ERR(id == BC_ERR_IDX_FATAL || fout_s == BC_STATUS_ERROR_FATAL ||
	           s == BC_STATUS_ERROR_FATAL))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 15\n");
		exit((int) BC_STATUS_ERROR_FATAL);
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 15\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 14\n");

#else // !BC_ENABLE_MEMCHECK
	fprintf(stderr, "[vm.c] enter bc_vm_handleError 16\n");
	if (BC_ERR(fout_s == BC_STATUS_ERROR_FATAL))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 17\n");
		vm->status = (sig_atomic_t) fout_s;
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 17\n");
	}
	else if (BC_ERR(s == BC_STATUS_ERROR_FATAL))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 18\n");
		vm->status = (sig_atomic_t) s;
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 18\n");
	}
	else
	{
		fprintf(stderr, "\n");
		// fprintf(stderr, "\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 16\n");
#endif // !BC_ENABLE_MEMCHECK

	fprintf(stderr, "[vm.c] enter bc_vm_handleError 20\n");
	{
		vm->status = (sig_atomic_t) (uchar) (id + 1);
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 20\n");

	// Only jump if there is an error.
	if (BC_ERR(vm->status))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_handleError 21\n");
		BC_JMP;
		// fprintf(stderr, "[vm.c] exit bc_vm_handleError 21\n");
	}

	fprintf(stderr, "[vm.c] enter bc_vm_handleError 22\n");
	BC_SIG_TRYUNLOCK(lock);
	// fprintf(stderr, "[vm.c] exit bc_vm_handleError 22\n");
}

char*
bc_vm_getenv(const char* var)
{
	fprintf(stderr, "[vm.c] enter bc_vm_getenv 1\n");
	char* ret;
	// fprintf(stderr, "[vm.c] exit bc_vm_getenv 1\n");

#ifndef _WIN32
	fprintf(stderr, "[vm.c] enter bc_vm_getenv 2\n");
	ret = getenv(var);
	// fprintf(stderr, "[vm.c] exit bc_vm_getenv 2\n");
#else // _WIN32
	fprintf(stderr, "[vm.c] enter bc_vm_getenv 3\n");
	_dupenv_s(&ret, NULL, var);
	// fprintf(stderr, "[vm.c] exit bc_vm_getenv 3\n");
#endif // _WIN32

	fprintf(stderr, "[vm.c] enter bc_vm_getenv 4\n");
	return ret;
	// fprintf(stderr, "[vm.c] exit bc_vm_getenv 4\n");
}

void
bc_vm_getenvFree(char* val)
{
	fprintf(stderr, "[vm.c] enter bc_vm_getenvFree 1\n");
	BC_UNUSED(val);
	// fprintf(stderr, "[vm.c] exit bc_vm_getenvFree 1\n");
#ifdef _WIN32
	fprintf(stderr, "[vm.c] enter bc_vm_getenvFree 2\n");
	free(val);
	// fprintf(stderr, "[vm.c] exit bc_vm_getenvFree 2\n");
#endif // _WIN32
}

/**
 * Sets a flag from an environment variable and the default.
 * @param var   The environment variable.
 * @param def   The default.
 * @param flag  The flag to set.
 */
static void
bc_vm_setenvFlag(const char* const var, int def, uint16_t flag)
{
	fprintf(stderr, "[vm.c] enter bc_vm_setenvFlag 1\n");
	// Get the value.
	char* val = bc_vm_getenv(var);

	// If there is no value...
	if (val == NULL)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_setenvFlag 2\n");
		// Set the default.
		if (def) vm->flags |= flag;
		else vm->flags &= ~(flag);
		// fprintf(stderr, "[vm.c] exit bc_vm_setenvFlag 2\n");
	}
	// Parse the value.
	else if (strtoul(val, NULL, 0)) vm->flags |= flag;
	else vm->flags &= ~(flag);

	bc_vm_getenvFree(val);
	// fprintf(stderr, "[vm.c] exit bc_vm_setenvFlag 1\n");
}

/**
 * Parses the arguments in {B,D]C_ENV_ARGS.
 * @param env_args_name  The environment variable to use.
 * @param scale          A pointer to return the scale that the arguments set,
 *                       if any.
 * @param ibase          A pointer to return the ibase that the arguments set,
 *                       if any.
 * @param obase          A pointer to return the obase that the arguments set,
 *                       if any.
 */
static void
bc_vm_envArgs(const char* const env_args_name, BcBigDig* scale, BcBigDig* ibase,
              BcBigDig* obase)
{
	char *env_args = bc_vm_getenv(env_args_name), *buf, *start;
	char instr = '\0';

	BC_SIG_ASSERT_LOCKED;

	if (env_args == NULL) return;

	// Windows already allocates, so we don't need to.
#ifndef _WIN32
	start = buf = vm->env_args_buffer = bc_vm_strdup(env_args);
#else // _WIN32
	start = buf = vm->env_args_buffer = env_args;
#endif // _WIN32

	assert(buf != NULL);

	// Create two buffers for parsing. These need to stay throughout the entire
	// execution of bc, unfortunately, because of filenames that might be in
	// there.
	bc_vec_init(&vm->env_args, sizeof(char*), BC_DTOR_NONE);
	bc_vec_push(&vm->env_args, &env_args_name);

	// While we haven't reached the end of the args...
	while (*buf)
	{
		// If we don't have whitespace...
		if (!isspace(*buf))
		{
			// If we have the start of a string...
			if (*buf == '"' || *buf == '\'')
			{
				// Set stuff appropriately.
				instr = *buf;
				buf += 1;

				// Check for the empty string.
				if (*buf == instr)
				{
					instr = '\0';
					buf += 1;
					continue;
				}
			}

			// Push the pointer to the args buffer.
			bc_vec_push(&vm->env_args, &buf);

			// Parse the string.
			while (*buf &&
			       ((!instr && !isspace(*buf)) || (instr && *buf != instr)))
			{
				buf += 1;
			}

			// If we did find the end of the string...
			if (*buf)
			{
				if (instr) instr = '\0';

				// Reset stuff.
				*buf = '\0';
				buf += 1;
				start = buf;
			}
			else if (instr) bc_error(BC_ERR_FATAL_OPTION, 0, start);
		}
		// If we have whitespace, eat it.
		else buf += 1;
	}

	// Make sure to push a NULL pointer at the end.
	buf = NULL;
	bc_vec_push(&vm->env_args, &buf);

	// Parse the arguments.
	bc_args((int) vm->env_args.len - 1, bc_vec_item(&vm->env_args, 0), false,
	        scale, ibase, obase);
}

/**
 * Gets the {B,D}C_LINE_LENGTH.
 * @param var  The environment variable to pull it from.
 * @return     The line length.
 */
static size_t
bc_vm_envLen(const char* var)
{
	char* lenv = bc_vm_getenv(var);
	size_t i, len = BC_NUM_PRINT_WIDTH;
	int num;

	// Return the default with none.
	if (lenv == NULL) return len;

	len = strlen(lenv);

	// Figure out if it's a number.
	for (num = 1, i = 0; num && i < len; ++i)
	{
		num = isdigit(lenv[i]);
	}

	// If it is a number...
	if (num)
	{
		// Parse it and clamp it if needed.
		len = (size_t) strtol(lenv, NULL, 10);
		if (len != 0)
		{
			len -= 1;
			if (len < 2 || len >= UINT16_MAX) len = BC_NUM_PRINT_WIDTH;
		}
	}
	// Set the default.
	else len = BC_NUM_PRINT_WIDTH;

	bc_vm_getenvFree(lenv);

	return len;
}
#endif // BC_ENABLE_LIBRARY

void
bc_vm_shutdown(void)
{
	BC_SIG_ASSERT_LOCKED;

#if BC_ENABLE_NLS
	if (vm->catalog != BC_VM_INVALID_CATALOG) catclose(vm->catalog);
#endif // BC_ENABLE_NLS

#if !BC_ENABLE_LIBRARY
#if BC_ENABLE_HISTORY
	// This must always run to ensure that the terminal is back to normal, i.e.,
	// has raw mode disabled. But we should only do it if we did not have a bad
	// terminal because history was not initialized if it is a bad terminal.
	if (BC_TTY && !vm->history.badTerm) bc_history_free(&vm->history);
#endif // BC_ENABLE_HISTORY
#endif // !BC_ENABLE_LIBRARY

#if BC_DEBUG || BC_ENABLE_MEMCHECK
#if !BC_ENABLE_LIBRARY
	bc_vec_free(&vm->env_args);
	free(vm->env_args_buffer);
	bc_vec_free(&vm->files);
	bc_vec_free(&vm->exprs);

	if (BC_PARSE_IS_INITED(&vm->read_prs, &vm->prog))
	{
		bc_vec_free(&vm->read_buf);
		bc_parse_free(&vm->read_prs);
	}

	bc_parse_free(&vm->prs);
	bc_program_free(&vm->prog);

	bc_slabvec_free(&vm->slabs);
#endif // !BC_ENABLE_LIBRARY

	bc_vm_freeTemps();
#endif // BC_DEBUG || BC_ENABLE_MEMCHECK

#if !BC_ENABLE_LIBRARY
	// We always want to flush.
	bc_file_free(&vm->fout);
	bc_file_free(&vm->ferr);
#endif // !BC_ENABLE_LIBRARY
}

void
bc_vm_addTemp(BcDig* num)
{
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	BC_SIG_ASSERT_LOCKED;

	// If we don't have room, just free.
	if (vm->temps_len == BC_VM_MAX_TEMPS) free(num);
	else
	{
		// Add to the buffer and length.
		vm->temps_buf[vm->temps_len] = num;
		vm->temps_len += 1;
	}
}

BcDig*
bc_vm_takeTemp(void)
{
	fprintf(stderr, "[vm.c] enter bc_vm_takeTemp 1\n");
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	BC_SIG_ASSERT_LOCKED;

	if (!vm->temps_len) return NULL;
	// fprintf(stderr, "[vm.c] exit bc_vm_takeTemp 1\n");

	fprintf(stderr, "[vm.c] enter bc_vm_takeTemp 2\n");
	vm->temps_len -= 1;

	return vm->temps_buf[vm->temps_len];
	// fprintf(stderr, "[vm.c] exit bc_vm_takeTemp 2\n");
}

BcDig*
bc_vm_getTemp(void)
{
	fprintf(stderr, "[vm.c] enter bc_vm_getTemp 1\n");
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	BC_SIG_ASSERT_LOCKED;

	if (!vm->temps_len) return NULL;
	// fprintf(stderr, "[vm.c] exit bc_vm_getTemp 1\n");

	fprintf(stderr, "[vm.c] enter bc_vm_getTemp 2\n");
	return vm->temps_buf[vm->temps_len - 1];
	// fprintf(stderr, "[vm.c] exit bc_vm_getTemp 2\n");
}

void
bc_vm_freeTemps(void)
{
	fprintf(stderr, "[vm.c] enter bc_vm_freeTemps 1\n");
	size_t i;
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	BC_SIG_ASSERT_LOCKED;

	if (!vm->temps_len) return;
	// fprintf(stderr, "[vm.c] exit bc_vm_freeTemps 1\n");

	fprintf(stderr, "[vm.c] enter bc_vm_freeTemps 2\n");
	// Free them all...
	for (i = 0; i < vm->temps_len; ++i)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_freeTemps 3\n");
		free(vm->temps_buf[i]);
		// fprintf(stderr, "[vm.c] exit bc_vm_freeTemps 3\n");
	}

	vm->temps_len = 0;
	// fprintf(stderr, "[vm.c] exit bc_vm_freeTemps 2\n");
}

#if !BC_ENABLE_LIBRARY

size_t
bc_vm_numDigits(size_t val)
{
	fprintf(stderr, "[vm.c] enter bc_vm_numDigits 1\n");
	size_t digits = 0;

	do
	{
		fprintf(stderr, "[vm.c] enter bc_vm_numDigits 2\n");
		digits += 1;
		val /= 10;
		// fprintf(stderr, "[vm.c] exit bc_vm_numDigits 2\n");
	}
	while (val != 0);

	return digits;
	// fprintf(stderr, "[vm.c] exit bc_vm_numDigits 1\n");
}

#endif // !BC_ENABLE_LIBRARY

inline size_t
bc_vm_arraySize(size_t n, size_t size)
{
	fprintf(stderr, "[vm.c] enter bc_vm_arraySize 1\n");
	size_t res = n * size;

	if (BC_ERR(BC_VM_MUL_OVERFLOW(n, size, res)))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_arraySize 2\n");
		bc_vm_fatalError(BC_ERR_FATAL_ALLOC_ERR);
		// fprintf(stderr, "[vm.c] exit bc_vm_arraySize 2\n");
	}

	return res;
	// fprintf(stderr, "[vm.c] exit bc_vm_arraySize 1\n");
}

inline size_t
bc_vm_growSize(size_t a, size_t b)
{
	fprintf(stderr, "[vm.c] enter bc_vm_growSize 1\n");
	size_t res = a + b;

	if (BC_ERR(res >= SIZE_MAX || res < a))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_growSize 2\n");
		bc_vm_fatalError(BC_ERR_FATAL_ALLOC_ERR);
		// fprintf(stderr, "[vm.c] exit bc_vm_growSize 2\n");
	}

	return res;
	// fprintf(stderr, "[vm.c] exit bc_vm_growSize 1\n");
}

void*
bc_vm_malloc(size_t n)
{
	fprintf(stderr, "[vm.c] enter bc_vm_malloc 1\n");
	void* ptr;

	BC_SIG_ASSERT_LOCKED;

	ptr = malloc(n);

	if (BC_ERR(ptr == NULL))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_malloc 2\n");
		bc_vm_freeTemps();

		ptr = malloc(n);

		if (BC_ERR(ptr == NULL)) bc_vm_fatalError(BC_ERR_FATAL_ALLOC_ERR);
		// fprintf(stderr, "[vm.c] exit bc_vm_malloc 2\n");
	}

	return ptr;
	// fprintf(stderr, "[vm.c] exit bc_vm_malloc 1\n");
}

void*
bc_vm_realloc(void* ptr, size_t n)
{
	fprintf(stderr, "[vm.c] enter bc_vm_realloc 1\n");
	void* temp;

	BC_SIG_ASSERT_LOCKED;

	temp = realloc(ptr, n);

	if (BC_ERR(temp == NULL))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_realloc 2\n");
		bc_vm_freeTemps();

		temp = realloc(ptr, n);

		if (BC_ERR(temp == NULL)) bc_vm_fatalError(BC_ERR_FATAL_ALLOC_ERR);
		// fprintf(stderr, "[vm.c] exit bc_vm_realloc 2\n");
	}

	return temp;
	// fprintf(stderr, "[vm.c] exit bc_vm_realloc 1\n");
}

char*
bc_vm_strdup(const char* str)
{
	fprintf(stderr, "[vm.c] enter bc_vm_strdup 1\n");
	char* s;

	BC_SIG_ASSERT_LOCKED;

	s = strdup(str);

	if (BC_ERR(s == NULL))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_strdup 2\n");
		bc_vm_freeTemps();

		s = strdup(str);

		if (BC_ERR(s == NULL)) bc_vm_fatalError(BC_ERR_FATAL_ALLOC_ERR);
		// fprintf(stderr, "[vm.c] exit bc_vm_strdup 2\n");
	}

	return s;
	// fprintf(stderr, "[vm.c] exit bc_vm_strdup 1\n");
}

#if !BC_ENABLE_LIBRARY
void
bc_vm_printf(const char* fmt, ...)
{
	fprintf(stderr, "[vm.c] enter bc_vm_printf 1\n");
	va_list args;
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#else // BC_ENABLE_LIBRARY
	sig_atomic_t lock;
#endif // BC_ENABLE_LIBRARY

	BC_SIG_TRYLOCK(lock);

	va_start(args, fmt);
	bc_file_vprintf(&vm->fout, fmt, args);
	va_end(args);

	vm->nchars = 0;

	BC_SIG_TRYUNLOCK(lock);
	// fprintf(stderr, "[vm.c] exit bc_vm_printf 1\n");
}
#endif // !BC_ENABLE_LIBRARY

void
bc_vm_putchar(int c, BcFlushType type)
{
	fprintf(stderr, "[vm.c] enter bc_vm_putchar 1\n");
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
	bc_vec_pushByte(&vm->out, (uchar) c);
#else // BC_ENABLE_LIBRARY
	bc_file_putchar(&vm->fout, type, (uchar) c);
	vm->nchars = (c == '\n' ? 0 : vm->nchars + 1);
#endif // BC_ENABLE_LIBRARY
	// fprintf(stderr, "[vm.c] exit bc_vm_putchar 1\n");
}

#if !BC_ENABLE_LIBRARY

#ifdef __OpenBSD__
/**
 * Aborts with a message. This should never be called because I have carefully
 * made sure that the calls to pledge() and unveil() are correct, but it's here
 * just in case.
 * @param msg  The message to print.
 */
BC_NORETURN static void
bc_abortm(const char* msg)
{
	fprintf(stderr, "[vm.c] enter bc_abortm 1\n");
	bc_file_puts(&vm->ferr, bc_flush_none, msg);
	bc_file_puts(&vm->ferr, bc_flush_none, "; this is a bug");
	bc_file_flush(&vm->ferr, bc_flush_none);
	abort();
	// fprintf(stderr, "[vm.c] exit bc_abortm 1\n");
}

void
bc_pledge(const char* promises, const char* execpromises)
{
	fprintf(stderr, "[vm.c] enter bc_pledge 1\n");
	int r = pledge(promises, execpromises);
	if (r) {
		fprintf(stderr, "[vm.c] enter bc_pledge 2\n");
		bc_abortm("pledge() failed");
		// fprintf(stderr, "[vm.c] exit bc_pledge 2\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_pledge 1\n");
}

#if BC_ENABLE_EXTRA_MATH

/**
 * A convenience and portability function for OpenBSD's unveil().
 * @param path         The path.
 * @param permissions  The permissions for the path.
 */
static void
bc_unveil(const char* path, const char* permissions)
{
	fprintf(stderr, "[vm.c] enter bc_unveil 1\n");
	int r = unveil(path, permissions);
	if (r) {
		fprintf(stderr, "[vm.c] enter bc_unveil 2\n");
		bc_abortm("unveil() failed");
		// fprintf(stderr, "[vm.c] exit bc_unveil 2\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_unveil 1\n");
}

#endif // BC_ENABLE_EXTRA_MATH

#else // __OpenBSD__

void
bc_pledge(const char* promises, const char* execpromises)
{
	fprintf(stderr, "[vm.c] enter bc_pledge 2640\n");
	BC_UNUSED(promises);
	BC_UNUSED(execpromises);
	// fprintf(stderr, "[vm.c] exit bc_pledge 2640\n");
}

#if BC_ENABLE_EXTRA_MATH
static void
bc_unveil(const char* path, const char* permissions)
{
	fprintf(stderr, "[vm.c] enter bc_unveil 4155\n");
	BC_UNUSED(path);
	BC_UNUSED(permissions);
	// fprintf(stderr, "[vm.c] exit bc_unveil 4155\n");
}
#endif // BC_ENABLE_EXTRA_MATH

#endif // __OpenBSD__

/**
 * Cleans unneeded variables, arrays, functions, strings, and constants when
 * done executing a line of stdin. This is to prevent memory usage growing
 * without bound. This is an idea from busybox.
 */
static void
bc_vm_clean(void)
{
	fprintf(stderr, "[vm.c] enter bc_vm_clean 1\n");
	BcVec* fns = &vm->prog.fns;
	BcFunc* f = bc_vec_item(fns, BC_PROG_MAIN);
	BcInstPtr* ip = bc_vec_item(&vm->prog.stack, 0);
	bool good = ((vm->status && vm->status != BC_STATUS_QUIT) || vm->sig != 0);

	BC_SIG_ASSERT_LOCKED;

	// If all is good, go ahead and reset.
	if (good) {
		fprintf(stderr, "[vm.c] enter bc_vm_clean 2\n");
		bc_program_reset(&vm->prog);
		// fprintf(stderr, "[vm.c] exit bc_vm_clean 2\n");
	}

#if BC_ENABLED
	// bc has this extra condition. If it not satisfied, it is in the middle of
	// a parse.
	if (good && BC_IS_BC) {
		fprintf(stderr, "[vm.c] enter bc_vm_clean 3\n");
		good = !BC_PARSE_NO_EXEC(&vm->prs);
		// fprintf(stderr, "[vm.c] exit bc_vm_clean 3\n");
	}
#endif // BC_ENABLED

#if DC_ENABLED
	// For dc, it is safe only when all of the results on the results stack are
	// safe, which means that they are temporaries or other things that don't
	// need strings or constants.
	if (BC_IS_DC)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_clean 4\n");
		size_t i;

		good = true;

		for (i = 0; good && i < vm->prog.results.len; ++i)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_clean 5\n");
			BcResult* r = (BcResult*) bc_vec_item(&vm->prog.results, i);
			good = BC_VM_SAFE_RESULT(r);
			// fprintf(stderr, "[vm.c] exit bc_vm_clean 5\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_clean 4\n");
	}
#endif // DC_ENABLED

	// If this condition is true, we can get rid of strings,
	// constants, and code.
	if (good && vm->prog.stack.len == 1 && ip->idx == f->code.len)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_clean 6\n");
		// XXX: Nothing can be popped in dc. Deal with it.

#if BC_ENABLED
		if (BC_IS_BC)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_clean 7\n");
			// XXX: you cannot delete strings, functions, or constants in bc.
			// Deal with it.
			bc_vec_popAll(&f->labels);
			// fprintf(stderr, "[vm.c] exit bc_vm_clean 7\n");
		}
#endif // BC_ENABLED

		bc_vec_popAll(&f->code);

		ip->idx = 0;
		// fprintf(stderr, "[vm.c] exit bc_vm_clean 6\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_clean 1\n");
}

/**
 * Process a bunch of text.
 * @param text  The text to process.
 * @param mode  The mode to process in.
 */
static void
bc_vm_process(const char* text, BcMode mode)
{
	fprintf(stderr, "[vm.c] enter bc_vm_process 1\n");
	// Set up the parser.
	bc_parse_text(&vm->prs, text, mode);

	while (vm->prs.l.t != BC_LEX_EOF)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_process 2\n");
		// Parsing requires a signal lock. We also don't parse everything; we
		// want to execute as soon as possible for *everything*.
		BC_SIG_LOCK;
		vm->parse(&vm->prs);
		BC_SIG_UNLOCK;

		// Execute if possible.
		if (BC_IS_DC || !BC_PARSE_NO_EXEC(&vm->prs)) {
			fprintf(stderr, "[vm.c] enter bc_vm_process 3\n");
			bc_program_exec(&vm->prog);
			// fprintf(stderr, "[vm.c] exit bc_vm_process 3\n");
		}

		assert(BC_IS_DC || vm->prog.results.len == 0);

		// Flush in interactive mode.
		if (BC_I) {
			fprintf(stderr, "[vm.c] enter bc_vm_process 4\n");
			bc_file_flush(&vm->fout, bc_flush_save);
			// fprintf(stderr, "[vm.c] exit bc_vm_process 4\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_process 2\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_process 1\n");
}

#if BC_ENABLED

/**
 * Ends a series of if statements. This is to ensure that full parses happen
 * when a file finishes or stdin has no more data. Without this, bc thinks that
 * it cannot parse any further. But if we reach the end of a file or stdin has
 * no more data, we know we can add an empty else clause.
 */
static void
bc_vm_endif(void)
{
	fprintf(stderr, "[vm.c] enter bc_vm_endif 1\n");
	bc_parse_endif(&vm->prs);
	bc_program_exec(&vm->prog);
	// fprintf(stderr, "[vm.c] exit bc_vm_endif 1\n");
}

#endif // BC_ENABLED
/**
 * Processes a file.
 * @param file  The filename.
 */
static void
bc_vm_file(const char* file)
{
	fprintf(stderr, "[vm.c] enter bc_vm_file 1\n");
	char* data = NULL;
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	assert(!vm->sig_pop);

	vm->mode = BC_MODE_FILE;

	// Set up the lexer.
	bc_lex_file(&vm->prs.l, file);

	BC_SIG_LOCK;

	// Read the file.
	data = bc_read_file(file);

	assert(data != NULL);

	BC_SETJMP_LOCKED(vm, err);

	BC_SIG_UNLOCK;

	// Process it.
	bc_vm_process(data, BC_MODE_FILE);

#if BC_ENABLED
	// Make sure to end any open if statements.
	if (BC_IS_BC) bc_vm_endif();
#endif // BC_ENABLED

err:

	BC_SIG_MAYLOCK;

	// Cleanup.
	free(data);
	bc_vm_clean();

	// bc_program_reset(), called by bc_vm_clean(), resets the status.
	// We want it to clear the sig_pop variable in case it was set.
	if (vm->status == (sig_atomic_t) BC_STATUS_SUCCESS) BC_LONGJMP_STOP;

	BC_LONGJMP_CONT(vm);
	// fprintf(stderr, "[vm.c] exit bc_vm_file 1\n");
}

#if !BC_ENABLE_OSSFUZZ

bool
bc_vm_readLine(bool clear)
{
	fprintf(stderr, "[vm.c] enter bc_vm_readLine 1\n");
	BcStatus s;
	bool good;

	BC_SIG_ASSERT_NOT_LOCKED;

	// Clear the buffer if desired.
	if (clear) bc_vec_empty(&vm->buffer);

	// Empty the line buffer.
	bc_vec_empty(&vm->line_buf);

	if (vm->eof) return false;
	// fprintf(stderr, "[vm.c] exit bc_vm_readLine 1\n");

	do
	{
		fprintf(stderr, "[vm.c] enter bc_vm_readLine 2\n");
		// bc_read_line() must always return either BC_STATUS_SUCCESS or
		// BC_STATUS_EOF. Everything else, it and whatever it calls, must jump
		// out instead.
		s = bc_read_line(&vm->line_buf, ">>> ");
		vm->eof = (s == BC_STATUS_EOF);
		// fprintf(stderr, "[vm.c] exit bc_vm_readLine 2\n");
	}
	while (s == BC_STATUS_SUCCESS && !vm->eof && vm->line_buf.len < 1);

	fprintf(stderr, "[vm.c] enter bc_vm_readLine 3\n");
	good = (vm->line_buf.len > 1);

	// Concat if we found something.
	if (good) bc_vec_concat(&vm->buffer, vm->line_buf.v);

	return good;
	// fprintf(stderr, "[vm.c] exit bc_vm_readLine 3\n");
}

/**
 * Processes text from stdin.
 */
static void
bc_vm_stdin(void)
{
	fprintf(stderr, "[vm.c] enter bc_vm_stdin 1\n");
	bool clear;

#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	clear = true;
	vm->mode = BC_MODE_STDIN;

	// Set up the lexer.
	bc_lex_file(&vm->prs.l, bc_program_stdin_name);

	// These are global so that the lexers can access them, but they are
	// allocated and freed in this function because they should only be used for
	// stdin and expressions (they are used in bc_vm_exprs() as well). So they
	// are tied to this function, really. Well, this and bc_vm_readLine(). These
	// are the reasons that we have vm->is_stdin to tell the lexers if we are
	// reading from stdin. Well, both lexers care. And the reason they care is
	// so that if a comment or a string goes across multiple lines, the lexer
	// can request more data from stdin until the comment or string is ended.
	BC_SIG_LOCK;
	bc_vec_init(&vm->buffer, sizeof(uchar), BC_DTOR_NONE);
	bc_vec_init(&vm->line_buf, sizeof(uchar), BC_DTOR_NONE);
	BC_SETJMP_LOCKED(vm, err);
	BC_SIG_UNLOCK;

// This label exists because errors can cause jumps to end up at the err label
// below. If that happens, and the error should be cleared and execution
// continue, then we need to jump back.
restart:

	// While we still read data from stdin.
	while (bc_vm_readLine(clear))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_stdin 2\n");
		size_t len = vm->buffer.len - 1;
		const char* str = vm->buffer.v;

		// We don't want to clear the buffer when the line ends with a backslash
		// because a backslash newline is special in bc.
		clear = (len < 2 || str[len - 2] != '\\' || str[len - 1] != '\n');
		if (!clear) continue;
		// fprintf(stderr, "[vm.c] exit bc_vm_stdin 2\n");

		fprintf(stderr, "[vm.c] enter bc_vm_stdin 3\n");
		// Process the data.
		bc_vm_process(vm->buffer.v, BC_MODE_STDIN);

		if (vm->eof) break;
		else
		{
			fprintf(stderr, "[vm.c] enter bc_vm_stdin 4\n");
			BC_SIG_LOCK;
			bc_vm_clean();
			BC_SIG_UNLOCK;
			// fprintf(stderr, "[vm.c] exit bc_vm_stdin 4\n");
		}
		// fprintf(stderr, "[vm.c] exit bc_vm_stdin 3\n");
	}

#if BC_ENABLED
	// End the if statements.
	if (BC_IS_BC) bc_vm_endif();
#endif // BC_ENABLED

err:

	BC_SIG_MAYLOCK;

	// Cleanup.
	bc_vm_clean();

#if !BC_ENABLE_MEMCHECK
	assert(vm->status != BC_STATUS_ERROR_FATAL);

	vm->status = vm->status == BC_STATUS_QUIT || !BC_I ? vm->status :
	                                                     BC_STATUS_SUCCESS;
#else // !BC_ENABLE_MEMCHECK
	vm->status = vm->status == BC_STATUS_ERROR_FATAL ||
	                     vm->status == BC_STATUS_QUIT || !BC_I ?
	                 vm->status :
	                 BC_STATUS_SUCCESS;
#endif // !BC_ENABLE_MEMCHECK

	if (!vm->status && !vm->eof)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_stdin 5\n");
		bc_vec_empty(&vm->buffer);
		BC_LONGJMP_STOP;
		BC_SIG_UNLOCK;
		goto restart;
		// fprintf(stderr, "[vm.c] exit bc_vm_stdin 5\n");
	}

#if BC_DEBUG
	// Since these are tied to this function, free them here. We only free in
	// debug mode because stdin is always the last thing read.
	bc_vec_free(&vm->line_buf);
	bc_vec_free(&vm->buffer);
#endif // BC_DEBUG

	BC_LONGJMP_CONT(vm);
	// fprintf(stderr, "[vm.c] exit bc_vm_stdin 1\n");
}

#endif // BC_ENABLE_OSSFUZZ
bool
bc_vm_readBuf(bool clear)
{
  fprintf(stderr, "\n");
	size_t len = vm->exprs.len - 1;
	bool more;

	BC_SIG_ASSERT_NOT_LOCKED;

	// Clear the buffer if desired.
	if (clear) {
    fprintf(stderr, "[vm.c] enter bc_vm_readBuf 2\n");
    bc_vec_empty(&vm->buffer);
    // fprintf(stderr, "[vm.c] exit bc_vm_readBuf 2\n");
  }

	// We want to pop the nul byte off because that's what bc_read_buf()
	// expects.
  fprintf(stderr, "[vm.c] enter bc_vm_readBuf 3\n");
	bc_vec_pop(&vm->buffer);

	// Read one line of expressions.
	more = bc_read_buf(&vm->buffer, vm->exprs.v, &len);
	bc_vec_pushByte(&vm->buffer, '\0');

	return more;
  // fprintf(stderr, "[vm.c] exit bc_vm_readBuf 3\n");
}

static void
bc_vm_exprs(void)
{
  fprintf(stderr, "[vm.c] enter bc_vm_exprs 1\n");
	bool clear;

#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	clear = true;
	vm->mode = BC_MODE_EXPRS;

	// Prepare the lexer.
	bc_lex_file(&vm->prs.l, bc_program_exprs_name);

	// We initialize this so that the lexer can access it in the case that it
	// needs more data for expressions, such as for a multiline string or
	// comment. See the comment on the allocation of vm->buffer above in
	// bc_vm_stdin() for more information.
	BC_SIG_LOCK;
	bc_vec_init(&vm->buffer, sizeof(uchar), BC_DTOR_NONE);
	BC_SETJMP_LOCKED(vm, err);
	BC_SIG_UNLOCK;
  // fprintf(stderr, "[vm.c] exit bc_vm_exprs 1\n");

	while (bc_vm_readBuf(clear))
	{
    fprintf(stderr, "[vm.c] enter bc_vm_exprs 2\n");
		size_t len = vm->buffer.len - 1;
		const char* str = vm->buffer.v;

		// We don't want to clear the buffer when the line ends with a backslash
		// because a backslash newline is special in bc.
		clear = (len < 2 || str[len - 2] != '\\' || str[len - 1] != '\n');
		if (!clear) {
      fprintf(stderr, "[vm.c] enter bc_vm_exprs 3\n");
      continue;
      // fprintf(stderr, "[vm.c] exit bc_vm_exprs 3\n");
    }

		// Process the data.
    fprintf(stderr, "[vm.c] enter bc_vm_exprs 4\n");
		bc_vm_process(vm->buffer.v, BC_MODE_EXPRS);
    // fprintf(stderr, "[vm.c] exit bc_vm_exprs 4\n");
    // fprintf(stderr, "[vm.c] exit bc_vm_exprs 2\n");
	}

	// If we were not supposed to clear, then we should process everything. This
	// makes sure that errors get reported.
	if (!clear) {
    fprintf(stderr, "[vm.c] enter bc_vm_exprs 5\n");
    bc_vm_process(vm->buffer.v, BC_MODE_EXPRS);
    // fprintf(stderr, "[vm.c] exit bc_vm_exprs 5\n");
  }

err:

	BC_SIG_MAYLOCK;

	// Cleanup.
  fprintf(stderr, "[vm.c] enter bc_vm_exprs 6\n");
	bc_vm_clean();

	// bc_program_reset(), called by bc_vm_clean(), resets the status.
	// We want it to clear the sig_pop variable in case it was set.
	if (vm->status == (sig_atomic_t) BC_STATUS_SUCCESS) {
    fprintf(stderr, "[vm.c] enter bc_vm_exprs 7\n");
    BC_LONGJMP_STOP;
    // fprintf(stderr, "[vm.c] exit bc_vm_exprs 7\n");
  }

	// Since this is tied to this function, free it here. We always free it here
	// because bc_vm_stdin() may or may not use it later.
	bc_vec_free(&vm->buffer);

	BC_LONGJMP_CONT(vm);
  // fprintf(stderr, "[vm.c] exit bc_vm_exprs 6\n");
}

#if BC_ENABLED

/**
 * Loads a math library.
 * @param name  The name of the library.
 * @param text  The text of the source code.
 */
static void
bc_vm_load(const char* name, const char* text)
{
  fprintf(stderr, "[vm.c] enter bc_vm_load 1\n");
	bc_lex_file(&vm->prs.l, name);
	bc_parse_text(&vm->prs, text, BC_MODE_FILE);

	BC_SIG_LOCK;

	while (vm->prs.l.t != BC_LEX_EOF)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_load 2\n");
		vm->parse(&vm->prs);
    // fprintf(stderr, "[vm.c] exit bc_vm_load 2\n");
	}

	BC_SIG_UNLOCK;
  // fprintf(stderr, "[vm.c] exit bc_vm_load 1\n");
}

#endif // BC_ENABLED

/**
 * Loads the default error messages.
 */
static void
bc_vm_defaultMsgs(void)
{
  fprintf(stderr, "[vm.c] enter bc_vm_defaultMsgs 1\n");
	size_t i;

	// Load the error categories.
	for (i = 0; i < BC_ERR_IDX_NELEMS + BC_ENABLED; ++i)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_defaultMsgs 2\n");
		vm->err_ids[i] = bc_errs[i];
    // fprintf(stderr, "[vm.c] exit bc_vm_defaultMsgs 2\n");
	}

	// Load the error messages.
	for (i = 0; i < BC_ERR_NELEMS; ++i)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_defaultMsgs 3\n");
		vm->err_msgs[i] = bc_err_msgs[i];
    // fprintf(stderr, "[vm.c] exit bc_vm_defaultMsgs 3\n");
	}
  // fprintf(stderr, "[vm.c] exit bc_vm_defaultMsgs 1\n");
}

/**
 * Loads the error messages for the locale. If NLS is disabled, this just loads
 * the default messages.
 */
static void
bc_vm_gettext(void)
{
  fprintf(stderr, "[vm.c] enter bc_vm_gettext 1\n");
#if BC_ENABLE_NLS
	uchar id = 0;
	int set, msg = 1;
	size_t i;

	// If no locale, load the defaults.
	if (vm->locale == NULL)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_gettext 2\n");
		vm->catalog = BC_VM_INVALID_CATALOG;
		bc_vm_defaultMsgs();
		return;
    // fprintf(stderr, "[vm.c] exit bc_vm_gettext 2\n");
  }

  fprintf(stderr, "[vm.c] enter bc_vm_gettext 3\n");
	vm->catalog = catopen(BC_MAINEXEC, NL_CAT_LOCALE);

	// If no catalog, load the defaults.
	if (vm->catalog == BC_VM_INVALID_CATALOG)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_gettext 4\n");
		bc_vm_defaultMsgs();
		return;
    // fprintf(stderr, "[vm.c] exit bc_vm_gettext 4\n");
  }

	// Load the error categories.
  fprintf(stderr, "[vm.c] enter bc_vm_gettext 5\n");
	for (set = 1; msg <= BC_ERR_IDX_NELEMS + BC_ENABLED; ++msg)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_gettext 6\n");
		vm->err_ids[msg - 1] = catgets(vm->catalog, set, msg, bc_errs[msg - 1]);
    // fprintf(stderr, "[vm.c] exit bc_vm_gettext 6\n");
	}

	i = 0;
	id = bc_err_ids[i];

	// Load the error messages. In order to understand this loop, you must know
	// the order of messages and categories in the enum and in the locale files.
  fprintf(stderr, "[vm.c] enter bc_vm_gettext 7\n");
	for (set = id + 2, msg = 1; i < BC_ERR_NELEMS; ++i, ++msg)
	{
    fprintf(stderr, "[vm.c] enter bc_vm_gettext 8\n");
		if (id != bc_err_ids[i])
		{
      fprintf(stderr, "[vm.c] enter bc_vm_gettext 9\n");
			msg = 1;
			id = bc_err_ids[i];
			set = id + 2;
      // fprintf(stderr, "[vm.c] exit bc_vm_gettext 9\n");
		}

    fprintf(stderr, "[vm.c] enter bc_vm_gettext 10\n");
		vm->err_msgs[i] = catgets(vm->catalog, set, msg, bc_err_msgs[i]);
    // fprintf(stderr, "[vm.c] exit bc_vm_gettext 10\n");
    // fprintf(stderr, "[vm.c] exit bc_vm_gettext 8\n");
	}
  // fprintf(stderr, "[vm.c] exit bc_vm_gettext 7\n");
  // fprintf(stderr, "[vm.c] exit bc_vm_gettext 5\n");
  // fprintf(stderr, "[vm.c] exit bc_vm_gettext 3\n");
#else // BC_ENABLE_NLS
  fprintf(stderr, "[vm.c] enter bc_vm_gettext 11\n");
	bc_vm_defaultMsgs();
  // fprintf(stderr, "[vm.c] exit bc_vm_gettext 11\n");
#endif // BC_ENABLE_NLS
  // fprintf(stderr, "[vm.c] exit bc_vm_gettext 1\n");
}
/**
 * Starts execution. Really, this is a function of historical accident; it could
 * probably be combined with bc_vm_boot(), but I don't care enough. Really, this
 * function starts when execution of bc or dc source code starts.
 */
static void
bc_vm_exec(void)
{
	size_t i;
#if DC_ENABLED
	bool has_file = false;
#endif // DC_ENABLED

#if BC_ENABLED
	// Load the math libraries.
	if (BC_IS_BC && (vm->flags & BC_FLAG_L))
	{
		// Can't allow redefinitions in the builtin library.
		vm->no_redefine = true;

		bc_vm_load(bc_lib_name, bc_lib);

#if BC_ENABLE_EXTRA_MATH
		if (!BC_IS_POSIX) bc_vm_load(bc_lib2_name, bc_lib2);
#endif // BC_ENABLE_EXTRA_MATH

		// Make sure to clear this.
		vm->no_redefine = false;

		// Execute to ensure that all is hunky dory. Without this, scale can be
		// set improperly.
		bc_program_exec(&vm->prog);
	}
#endif // BC_ENABLED

	assert(!BC_ENABLE_OSSFUZZ || BC_EXPR_EXIT == 0);

	// If there are expressions to execute...
	if (vm->exprs.len)
	{
		// Process the expressions.
		bc_vm_exprs();

		// Sometimes, executing expressions means we need to quit.
		if (vm->status != BC_STATUS_SUCCESS ||
		    (!vm->no_exprs && vm->exit_exprs && BC_EXPR_EXIT))
		{
			return;
		}
	}

	// Process files.
	for (i = 0; i < vm->files.len; ++i)
	{
		char* path = *((char**) bc_vec_item(&vm->files, i));
		if (!strcmp(path, "")) continue;
#if DC_ENABLED
		has_file = true;
#endif // DC_ENABLED
		bc_vm_file(path);

		if (vm->status != BC_STATUS_SUCCESS) return;
	}

#if BC_ENABLE_EXTRA_MATH
	// These are needed for the pseudo-random number generator.
	bc_unveil("/dev/urandom", "r");
	bc_unveil("/dev/random", "r");
	bc_unveil(NULL, NULL);
#endif // BC_ENABLE_EXTRA_MATH

#if BC_ENABLE_HISTORY

	// We need to keep tty if history is enabled, and we need to keep rpath for
	// the times when we read from /dev/urandom.
	if (BC_TTY && !vm->history.badTerm) bc_pledge(bc_pledge_end_history, NULL);
	else
#endif // BC_ENABLE_HISTORY
	{
		bc_pledge(bc_pledge_end, NULL);
	}

#if BC_ENABLE_AFL
	// This is the thing that makes fuzzing with AFL++ so fast. If you move this
	// back, you won't cause any problems, but fuzzing will slow down. If you
	// move this forward, you won't fuzz anything because you will be skipping
	// the reading from stdin.
	__AFL_INIT();
#endif // BC_ENABLE_AFL

#if BC_ENABLE_OSSFUZZ

	if (BC_VM_RUN_STDIN(has_file))
	{
		// XXX: Yes, this is a hack to run the fuzzer for OSS-Fuzz, but it
		// works.
		bc_vm_load("<stdin>", (const char*) bc_fuzzer_data);
	}

#else // BC_ENABLE_OSSFUZZ

	// Execute from stdin. bc always does.
	if (BC_VM_RUN_STDIN(has_file)) bc_vm_stdin();

#endif // BC_ENABLE_OSSFUZZ
}

BcStatus
bc_vm_boot(int argc, const char* argv[])
{
	fprintf(stderr, "[vm.c] enter bc_vm_boot 1\n");
	int ttyin, ttyout, ttyerr;
	bool tty;
	const char* const env_len = BC_VM_LINE_LENGTH_STR;
	const char* const env_args = BC_VM_ENV_ARGS_STR;
	const char* const env_exit = BC_VM_EXPR_EXIT_STR;
	const char* const env_clamp = BC_VM_DIGIT_CLAMP_STR;
	int env_exit_def = BC_VM_EXPR_EXIT_DEF;
	int env_clamp_def = BC_VM_DIGIT_CLAMP_DEF;
	BcBigDig scale = BC_NUM_BIGDIG_MAX;
	BcBigDig env_scale = BC_NUM_BIGDIG_MAX;
	BcBigDig ibase = BC_NUM_BIGDIG_MAX;
	BcBigDig env_ibase = BC_NUM_BIGDIG_MAX;
	BcBigDig obase = BC_NUM_BIGDIG_MAX;
	BcBigDig env_obase = BC_NUM_BIGDIG_MAX;

	// We need to know which of stdin, stdout, and stderr are tty's.
	ttyin = isatty(STDIN_FILENO);
	ttyout = isatty(STDOUT_FILENO);
	ttyerr = isatty(STDERR_FILENO);
	tty = (ttyin != 0 && ttyout != 0 && ttyerr != 0);

	vm->flags |= ttyin ? BC_FLAG_TTYIN : 0;
	vm->flags |= tty ? BC_FLAG_TTY : 0;
	vm->flags |= ttyin && ttyout ? BC_FLAG_I : 0;
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 1\n");

	// Set up signals.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 2\n");
	bc_vm_sigaction();
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 2\n");

	// Initialize some vm stuff. This is separate to make things easier for the
	// library.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 3\n");
	bc_vm_init();
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 3\n");

	// Explicitly set this in case NULL isn't all zeroes.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 4\n");
	vm->file = NULL;
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 4\n");

	// Set the error messages.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 5\n");
	bc_vm_gettext();
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 5\n");

#if BC_ENABLE_LINE_LIB

	// Initialize the output file buffers.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 6\n");
	bc_file_init(&vm->ferr, stderr, true);
	bc_file_init(&vm->fout, stdout, false);

	// Set the input buffer.
	vm->buf = output_bufs;
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 6\n");

#else // BC_ENABLE_LINE_LIB

	// Initialize the output file buffers. They each take portions of the global
	// buffer. stdout gets more because it will probably have more data.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 7\n");
	bc_file_init(&vm->ferr, STDERR_FILENO, output_bufs + BC_VM_STDOUT_BUF_SIZE,
	             BC_VM_STDERR_BUF_SIZE, true);
	bc_file_init(&vm->fout, STDOUT_FILENO, output_bufs, BC_VM_STDOUT_BUF_SIZE,
	             false);

	// Set the input buffer to the rest of the global buffer.
	vm->buf = output_bufs + BC_VM_STDOUT_BUF_SIZE + BC_VM_STDERR_BUF_SIZE;
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 7\n");
#endif // BC_ENABLE_LINE_LIB

	// Set the line length by environment variable.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 8\n");
	vm->line_len = (uint16_t) bc_vm_envLen(env_len);
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 8\n");

	fprintf(stderr, "[vm.c] enter bc_vm_boot 9\n");
	bc_vm_setenvFlag(env_exit, env_exit_def, BC_FLAG_EXPR_EXIT);
	bc_vm_setenvFlag(env_clamp, env_clamp_def, BC_FLAG_DIGIT_CLAMP);
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 9\n");

	// Clear the files and expressions vectors, just in case. This marks them as
	// *not* allocated.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 10\n");
	bc_vec_clear(&vm->files);
	bc_vec_clear(&vm->exprs);
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 10\n");

#if !BC_ENABLE_LIBRARY

	// Initialize the slab vector.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 11\n");
	bc_slabvec_init(&vm->slabs);
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 11\n");

#endif // !BC_ENABLE_LIBRARY

	// Initialize the program and main parser. These have to be in this order
	// because the program has to be initialized first, since a pointer to it is
	// passed to the parser.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 12\n");
	bc_program_init(&vm->prog);
	bc_parse_init(&vm->prs, &vm->prog, BC_PROG_MAIN);
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 12\n");

	// Set defaults.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 13\n");
	vm->flags |= BC_TTY ? BC_FLAG_P | BC_FLAG_R : 0;
	vm->flags |= BC_I ? BC_FLAG_Q : 0;
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 13\n");

#if BC_ENABLED
	fprintf(stderr, "[vm.c] enter bc_vm_boot 14\n");
	if (BC_IS_BC)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_boot 15\n");
		// bc checks this environment variable to see if it should run in
		// standard mode.
		char* var = bc_vm_getenv("POSIXLY_CORRECT");

		vm->flags |= BC_FLAG_S * (var != NULL);
		bc_vm_getenvFree(var);

		// Set whether we print the banner or not.
		if (BC_I) bc_vm_setenvFlag("BC_BANNER", BC_DEFAULT_BANNER, BC_FLAG_Q);
		// fprintf(stderr, "[vm.c] exit bc_vm_boot 15\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 14\n");
#endif // BC_ENABLED

	// Are we in TTY mode?
	fprintf(stderr, "[vm.c] enter bc_vm_boot 16\n");
	if (BC_TTY)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_boot 17\n");
		const char* const env_tty = BC_VM_TTY_MODE_STR;
		int env_tty_def = BC_VM_TTY_MODE_DEF;
		const char* const env_prompt = BC_VM_PROMPT_STR;
		int env_prompt_def = BC_VM_PROMPT_DEF;

		// Set flags for TTY mode and prompt.
		bc_vm_setenvFlag(env_tty, env_tty_def, BC_FLAG_TTY);
		bc_vm_setenvFlag(env_prompt, tty ? env_prompt_def : 0, BC_FLAG_P);

#if BC_ENABLE_HISTORY
		// If TTY mode is used, activate history.
		if (BC_TTY) bc_history_init(&vm->history);
#endif // BC_ENABLE_HISTORY
		// fprintf(stderr, "[vm.c] exit bc_vm_boot 17\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 16\n");

	// Process environment and command-line arguments.
	fprintf(stderr, "[vm.c] enter bc_vm_boot 18\n");
	bc_vm_envArgs(env_args, &env_scale, &env_ibase, &env_obase);
	bc_args(argc, argv, true, &scale, &ibase, &obase);
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 18\n");

	// This section is here because we don't want the math library to stomp on
	// the user's given value for scale. And we don't want ibase affecting how
	// the scale is interpreted. Also, it's sectioned off just for this comment.
	{
		fprintf(stderr, "[vm.c] enter bc_vm_boot 19\n");
		BC_SIG_UNLOCK;

		scale = scale == BC_NUM_BIGDIG_MAX ? env_scale : scale;
#if BC_ENABLED
		// Assign the library value only if it is used and no value was set.
		scale = scale == BC_NUM_BIGDIG_MAX && BC_L ? 20 : scale;
#endif // BC_ENABLED
		obase = obase == BC_NUM_BIGDIG_MAX ? env_obase : obase;
		ibase = ibase == BC_NUM_BIGDIG_MAX ? env_ibase : ibase;

		if (scale != BC_NUM_BIGDIG_MAX)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_boot 20\n");
			bc_program_assignBuiltin(&vm->prog, true, false, scale);
			// fprintf(stderr, "[vm.c] exit bc_vm_boot 20\n");
		}

		if (obase != BC_NUM_BIGDIG_MAX)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_boot 21\n");
			bc_program_assignBuiltin(&vm->prog, false, true, obase);
			// fprintf(stderr, "[vm.c] exit bc_vm_boot 21\n");
		}

		// This is last to avoid it affecting the value of the others.
		if (ibase != BC_NUM_BIGDIG_MAX)
		{
			fprintf(stderr, "[vm.c] enter bc_vm_boot 22\n");
			bc_program_assignBuiltin(&vm->prog, false, false, ibase);
			// fprintf(stderr, "[vm.c] exit bc_vm_boot 22\n");
		}

		BC_SIG_LOCK;
		// fprintf(stderr, "[vm.c] exit bc_vm_boot 19\n");
	}

	// If we are in interactive mode...
	fprintf(stderr, "[vm.c] enter bc_vm_boot 23\n");
	if (BC_I)
	{
		fprintf(stderr, "[vm.c] enter bc_vm_boot 24\n");
		const char* const env_sigint = BC_VM_SIGINT_RESET_STR;
		int env_sigint_def = BC_VM_SIGINT_RESET_DEF;

		// Set whether we reset on SIGINT or not.
		bc_vm_setenvFlag(env_sigint, env_sigint_def, BC_FLAG_SIGINT);
		// fprintf(stderr, "[vm.c] exit bc_vm_boot 24\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 23\n");

#if BC_ENABLED
	fprintf(stderr, "[vm.c] enter bc_vm_boot 25\n");
	// Disable global stacks in POSIX mode.
	if (BC_IS_POSIX) vm->flags &= ~(BC_FLAG_G);

	// Print the banner if allowed. We have to be in bc, in interactive mode,
	// and not be quieted by command-line option or environment variable.
	if (BC_IS_BC && BC_I && (vm->flags & BC_FLAG_Q))
	{
		fprintf(stderr, "[vm.c] enter bc_vm_boot 26\n");
		bc_vm_info(NULL);
		bc_file_putchar(&vm->fout, bc_flush_none, '\n');
		bc_file_flush(&vm->fout, bc_flush_none);
		// fprintf(stderr, "[vm.c] exit bc_vm_boot 26\n");
	}
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 25\n");
#endif // BC_ENABLED

	fprintf(stderr, "[vm.c] enter bc_vm_boot 27\n");
	BC_SIG_UNLOCK;

	// Start executing.
	bc_vm_exec();

	BC_SIG_LOCK;

	// Exit.
	return (BcStatus) vm->status;
	// fprintf(stderr, "[vm.c] exit bc_vm_boot 27\n");
}
#endif // !BC_ENABLE_LIBRARY
void
bc_vm_init(void)
{
  fprintf(stderr, "[vm.c] enter bc_vm_init 1\n");
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY

	BC_SIG_ASSERT_LOCKED;

#if !BC_ENABLE_LIBRARY
	// Set up the constant zero.
	bc_num_setup(&vm->zero, vm->zero_num, BC_VM_ONE_CAP);
#endif // !BC_ENABLE_LIBRARY

	// Set up more constant BcNum's.
	bc_num_setup(&vm->one, vm->one_num, BC_VM_ONE_CAP);
	bc_num_one(&vm->one);

	// Set up more constant BcNum's.
	// NOLINTNEXTLINE
	memcpy(vm->max_num, bc_num_bigdigMax,
	       bc_num_bigdigMax_size * sizeof(BcDig));
	// NOLINTNEXTLINE
	memcpy(vm->max2_num, bc_num_bigdigMax2,
	       bc_num_bigdigMax2_size * sizeof(BcDig));
	bc_num_setup(&vm->max, vm->max_num, BC_NUM_BIGDIG_LOG10);
	bc_num_setup(&vm->max2, vm->max2_num, BC_NUM_BIGDIG_LOG10);
	vm->max.len = bc_num_bigdigMax_size;
	vm->max2.len = bc_num_bigdigMax2_size;

	// Set up the maxes for the globals.
	vm->maxes[BC_PROG_GLOBALS_IBASE] = BC_NUM_MAX_POSIX_IBASE;
	vm->maxes[BC_PROG_GLOBALS_OBASE] = BC_MAX_OBASE;
	vm->maxes[BC_PROG_GLOBALS_SCALE] = BC_MAX_SCALE;

#if BC_ENABLE_EXTRA_MATH
	vm->maxes[BC_PROG_MAX_RAND] = ((BcRand) 0) - 1;
#endif // BC_ENABLE_EXTRA_MATH

#if BC_ENABLED
#if !BC_ENABLE_LIBRARY
	// bc has a higher max ibase when it's not in POSIX mode.
	if (BC_IS_BC && !BC_IS_POSIX)
#endif // !BC_ENABLE_LIBRARY
	{
	  fprintf(stderr, "[vm.c] enter bc_vm_init 2\n");
		vm->maxes[BC_PROG_GLOBALS_IBASE] = BC_NUM_MAX_IBASE;
		// fprintf(stderr, "[vm.c] exit bc_vm_init 2\n");
	}
#endif // BC_ENABLED
	// fprintf(stderr, "[vm.c] exit bc_vm_init 1\n");
}

#if BC_ENABLE_LIBRARY
void
bc_vm_atexit(void)
{
  fprintf(stderr, "[vm.c] enter bc_vm_atexit 1\n");
#if BC_DEBUG
#if BC_ENABLE_LIBRARY
	BcVm* vm = bcl_getspecific();
#endif // BC_ENABLE_LIBRARY
#endif // BC_DEBUG

	bc_vm_shutdown();

#if BC_DEBUG
	bc_vec_free(&vm->jmp_bufs);
#endif // BC_DEBUG
	// fprintf(stderr, "[vm.c] exit bc_vm_atexit 1\n");
}
#else // BC_ENABLE_LIBRARY
BcStatus
bc_vm_atexit(BcStatus status)
{
  fprintf(stderr, "[vm.c] enter bc_vm_atexit 2\n");
	// Set the status correctly.
	BcStatus s = BC_STATUS_IS_ERROR(status) ? status : BC_STATUS_SUCCESS;

	bc_vm_shutdown();

#if BC_DEBUG
	bc_vec_free(&vm->jmp_bufs);
#endif // BC_DEBUG

	return s;
	// fprintf(stderr, "[vm.c] exit bc_vm_atexit 2\n");
}
#endif // BC_ENABLE_LIBRARY
// Total cost: 0.107393
// Total split cost: 0.009419, input tokens: 31690, output tokens: 784, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 187), (187, 333), (333, 510), (510, 702), (702, 900), (900, 1067), (1067, 1260), (1260, 1444), (1444, 1551), (1551, 1748), (1748, 1830)]
// Total instrumented cost: 0.097975, input tokens: 148544, output tokens: 52607, cache read tokens: 0, cache write tokens: 0
