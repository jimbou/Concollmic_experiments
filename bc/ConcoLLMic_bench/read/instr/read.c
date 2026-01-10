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
 * Code to handle special I/O for bc.
 *
 */
#include <stdio.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32

#include <read.h>
#include <history.h>
#include <program.h>
#include <vm.h>

/**
 * A portability file open function. This is copied to gen/strgen.c. Make sure
 * to update that if this changes.
 * @param path  The path to the file to open.
 * @param mode  The mode to open in.
 */
static int
bc_read_open(const char* path, int mode)
{
	fprintf(stderr, "[read.c] enter bc_read_open 1\n");
	int fd;
	// fprintf(stderr, "[read.c] exit bc_read_open 1\n");

#ifndef _WIN32
	fprintf(stderr, "[read.c] enter bc_read_open 2\n");
	fd = open(path, mode);
	// fprintf(stderr, "[read.c] exit bc_read_open 2\n");
#else // _WIN32
	fprintf(stderr, "[read.c] enter bc_read_open 3\n");
	fd = -1;
	open(&fd, path, mode);
	// fprintf(stderr, "[read.c] exit bc_read_open 3\n");
#endif

	fprintf(stderr, "[read.c] enter bc_read_open 4\n");
	return fd;
	// fprintf(stderr, "[read.c] exit bc_read_open 4\n");
}

/**
 * Returns true if the buffer data is non-text.
 * @param buf   The buffer to test.
 * @param size  The size of the buffer.
 */
static bool
bc_read_binary(const char* buf, size_t size)
{
	fprintf(stderr, "[read.c] enter bc_read_binary 1\n");
	size_t i;
	// fprintf(stderr, "[read.c] exit bc_read_binary 1\n");

	for (i = 0; i < size; ++i)
	{
		if (BC_ERR(BC_READ_BIN_CHAR(buf[i])))
		{
			fprintf(stderr, "[read.c] enter bc_read_binary 2\n");
			return true;
			// fprintf(stderr, "[read.c] exit bc_read_binary 2\n");
		}
	}

	fprintf(stderr, "[read.c] enter bc_read_binary 3\n");
	return false;
	// fprintf(stderr, "[read.c] exit bc_read_binary 3\n");
}

bool
bc_read_buf(BcVec* vec, char* buf, size_t* buf_len)
{
	fprintf(stderr, "[read.c] enter bc_read_buf 1\n");
	char* nl;
	// fprintf(stderr, "[read.c] exit bc_read_buf 1\n");

	// If nothing there, return.
	if (!*buf_len)
	{
		fprintf(stderr, "[read.c] enter bc_read_buf 2\n");
		return false;
		// fprintf(stderr, "[read.c] exit bc_read_buf 2\n");
	}

	// Find the newline.
	fprintf(stderr, "[read.c] enter bc_read_buf 3\n");
	nl = strchr(buf, '\n');
	// fprintf(stderr, "[read.c] exit bc_read_buf 3\n");

	// If a newline exists...
	if (nl != NULL)
	{
		fprintf(stderr, "[read.c] enter bc_read_buf 4\n");
		// Get the size of the data up to, and including, the newline.
		size_t nllen = (size_t) ((nl + 1) - buf);

		nllen = *buf_len >= nllen ? nllen : *buf_len;

		// Move data into the vector, and move the rest of the data in the
		// buffer up.
		bc_vec_npush(vec, nllen, buf);
		*buf_len -= nllen;
		// NOLINTNEXTLINE
		memmove(buf, nl + 1, *buf_len + 1);

		return true;
		// fprintf(stderr, "[read.c] exit bc_read_buf 4\n");
	}

	// Just put the data into the vector.
	fprintf(stderr, "[read.c] enter bc_read_buf 5\n");
	bc_vec_npush(vec, *buf_len, buf);
	*buf_len = 0;
	// fprintf(stderr, "[read.c] exit bc_read_buf 5\n");

	fprintf(stderr, "[read.c] enter bc_read_buf 6\n");
	return false;
	// fprintf(stderr, "[read.c] exit bc_read_buf 6\n");
}
BcStatus
bc_read_chars(BcVec* vec, const char* prompt)
{
	fprintf(stderr, "[read.c] enter bc_read_chars 1\n");
	bool done = false;

	assert(vec != NULL && vec->size == sizeof(char));

	BC_SIG_ASSERT_NOT_LOCKED;

	// Clear the vector.
	bc_vec_popAll(vec);
	// fprintf(stderr, "[read.c] exit bc_read_chars 1\n");

	// Handle the prompt, if desired.
	if (BC_PROMPT)
	{
		fprintf(stderr, "[read.c] enter bc_read_chars 2\n");
		bc_file_puts(&vm->fout, bc_flush_none, prompt);
		bc_file_flush(&vm->fout, bc_flush_none);
		// fprintf(stderr, "[read.c] exit bc_read_chars 2\n");
	}

	// Try reading from the buffer, and if successful, just return.
	if (bc_read_buf(vec, vm->buf, &vm->buf_len))
	{
		fprintf(stderr, "[read.c] enter bc_read_chars 3\n");
		bc_vec_pushByte(vec, '\0');
		return BC_STATUS_SUCCESS;
		// fprintf(stderr, "[read.c] exit bc_read_chars 3\n");
	}

	// Loop until we have something.
	while (!done)
	{
		fprintf(stderr, "[read.c] enter bc_read_chars 4\n");
		ssize_t r;

		BC_SIG_LOCK;

		// Read data from stdin.
		r = read(STDIN_FILENO, vm->buf + vm->buf_len,
		         BC_VM_STDIN_BUF_SIZE - vm->buf_len);
		// fprintf(stderr, "[read.c] exit bc_read_chars 4\n");

		// If there was an error...
		if (BC_UNLIKELY(r < 0))
		{
			// If interupted...
			if (errno == EINTR)
			{
				fprintf(stderr, "[read.c] enter bc_read_chars 5\n");
				// Jump out if we are supposed to quit, which certain signals
				// will require.
				if (vm->status == (sig_atomic_t) BC_STATUS_QUIT) BC_JMP;

				assert(vm->sig != 0);

				// Clear the signal and status.
				vm->sig = 0;
				vm->status = (sig_atomic_t) BC_STATUS_SUCCESS;

				// Print the ready message and prompt again.
				bc_file_puts(&vm->fout, bc_flush_none, bc_program_ready_msg);
				if (BC_PROMPT)
				{
					bc_file_puts(&vm->fout, bc_flush_none, prompt);
				}
				bc_file_flush(&vm->fout, bc_flush_none);

				BC_SIG_UNLOCK;

				continue;
				// fprintf(stderr, "[read.c] exit bc_read_chars 5\n");
			}

			fprintf(stderr, "[read.c] enter bc_read_chars 6\n");
			BC_SIG_UNLOCK;

			// If we get here, it's bad. Barf.
			bc_vm_fatalError(BC_ERR_FATAL_IO_ERR);
			// fprintf(stderr, "[read.c] exit bc_read_chars 6\n");
		}

		fprintf(stderr, "[read.c] enter bc_read_chars 7\n");
		BC_SIG_UNLOCK;
		// fprintf(stderr, "[read.c] exit bc_read_chars 7\n");

		// If we read nothing, make sure to terminate the string and return EOF.
		if (r == 0)
		{
			fprintf(stderr, "[read.c] enter bc_read_chars 8\n");
			bc_vec_pushByte(vec, '\0');
			return BC_STATUS_EOF;
			// fprintf(stderr, "[read.c] exit bc_read_chars 8\n");
		}

		fprintf(stderr, "[read.c] enter bc_read_chars 9\n");
		BC_SIG_LOCK;

		// Add to the buffer.
		vm->buf_len += (size_t) r;
		vm->buf[vm->buf_len] = '\0';

		// Read from the buffer.
		done = bc_read_buf(vec, vm->buf, &vm->buf_len);

		BC_SIG_UNLOCK;
		// fprintf(stderr, "[read.c] exit bc_read_chars 9\n");
	}

	fprintf(stderr, "[read.c] enter bc_read_chars 10\n");
	// Terminate the string.
	bc_vec_pushByte(vec, '\0');

	return BC_STATUS_SUCCESS;
	// fprintf(stderr, "[read.c] exit bc_read_chars 10\n");
}

BcStatus
bc_read_line(BcVec* vec, const char* prompt)
{
	fprintf(stderr, "[read.c] enter bc_read_line 1\n");
	BcStatus s;
	// fprintf(stderr, "[read.c] exit bc_read_line 1\n");

#if BC_ENABLE_HISTORY
	// Get a line from either history or manual reading.
	if (BC_TTY && !vm->history.badTerm)
	{
		fprintf(stderr, "[read.c] enter bc_read_line 2\n");
		s = bc_history_line(&vm->history, vec, prompt);
		// fprintf(stderr, "[read.c] exit bc_read_line 2\n");
	}
	else
	{
		fprintf(stderr, "[read.c] enter bc_read_line 3\n");
		s = bc_read_chars(vec, prompt);
		// fprintf(stderr, "[read.c] exit bc_read_line 3\n");
	}
#else // BC_ENABLE_HISTORY
	fprintf(stderr, "[read.c] enter bc_read_line 4\n");
	s = bc_read_chars(vec, prompt);
	// fprintf(stderr, "[read.c] exit bc_read_line 4\n");
#endif // BC_ENABLE_HISTORY

	if (BC_ERR(bc_read_binary(vec->v, vec->len - 1)))
	{
		fprintf(stderr, "[read.c] enter bc_read_line 5\n");
		bc_verr(BC_ERR_FATAL_BIN_FILE, bc_program_stdin_name);
		// fprintf(stderr, "[read.c] exit bc_read_line 5\n");
	}

	fprintf(stderr, "[read.c] enter bc_read_line 6\n");
	return s;
	// fprintf(stderr, "[read.c] exit bc_read_line 6\n");
}

char*
bc_read_file(const char* path)
{
	fprintf(stderr, "[read.c] enter bc_read_file 1\n");
	BcErr e = BC_ERR_FATAL_IO_ERR;
	size_t size, to_read;
	struct stat pstat;
	int fd;
	char* buf;
	char* buf2;

	// This has been copied to gen/strgen.c. Make sure to change that if this
	// changes.

	BC_SIG_ASSERT_LOCKED;

	assert(path != NULL);

#if BC_DEBUG
	// Need this to quiet MSan.
	// NOLINTNEXTLINE
	memset(&pstat, 0, sizeof(struct stat));
#endif // BC_DEBUG

	fd = bc_read_open(path, O_RDONLY);
	// fprintf(stderr, "[read.c] exit bc_read_file 1\n");

	// If we can't read a file, we just barf.
	if (BC_ERR(fd < 0))
	{
		fprintf(stderr, "[read.c] enter bc_read_file 2\n");
		bc_verr(BC_ERR_FATAL_FILE_ERR, path);
		// fprintf(stderr, "[read.c] exit bc_read_file 2\n");
	}

	// The reason we call fstat is to eliminate TOCTOU race conditions. This
	// way, we have an open file, so it's not going anywhere.
	if (BC_ERR(fstat(fd, &pstat) == -1))
	{
		fprintf(stderr, "[read.c] enter bc_read_file 3\n");
		goto malloc_err;
		// fprintf(stderr, "[read.c] exit bc_read_file 3\n");
	}

	// Make sure it's not a directory.
	if (BC_ERR(S_ISDIR(pstat.st_mode)))
	{
		fprintf(stderr, "[read.c] enter bc_read_file 4\n");
		e = BC_ERR_FATAL_PATH_DIR;
		goto malloc_err;
		// fprintf(stderr, "[read.c] exit bc_read_file 4\n");
	}

	fprintf(stderr, "[read.c] enter bc_read_file 5\n");
	// Get the size of the file and allocate that much.
	size = (size_t) pstat.st_size;
	buf = bc_vm_malloc(size + 1);
	buf2 = buf;
	to_read = size;
	// fprintf(stderr, "[read.c] exit bc_read_file 5\n");

	do
	{
		fprintf(stderr, "[read.c] enter bc_read_file 6\n");
		// Read the file. We just bail if a signal interrupts. This is so that
		// users can interrupt the reading of big files if they want.
		ssize_t r = read(fd, buf2, to_read);
		// fprintf(stderr, "[read.c] exit bc_read_file 6\n");
		if (BC_ERR(r < 0))
		{
			fprintf(stderr, "[read.c] enter bc_read_file 7\n");
			goto read_err;
			// fprintf(stderr, "[read.c] exit bc_read_file 7\n");
		}
		fprintf(stderr, "[read.c] enter bc_read_file 8\n");
		to_read -= (size_t) r;
		buf2 += (size_t) r;
		// fprintf(stderr, "[read.c] exit bc_read_file 8\n");
	}
	while (to_read);

	fprintf(stderr, "[read.c] enter bc_read_file 9\n");
	// Got to have a nul byte.
	buf[size] = '\0';
	// fprintf(stderr, "[read.c] exit bc_read_file 9\n");

	if (BC_ERR(bc_read_binary(buf, size)))
	{
		fprintf(stderr, "[read.c] enter bc_read_file 10\n");
		e = BC_ERR_FATAL_BIN_FILE;
		goto read_err;
		// fprintf(stderr, "[read.c] exit bc_read_file 10\n");
	}

	fprintf(stderr, "[read.c] enter bc_read_file 11\n");
	close(fd);

	return buf;
	// fprintf(stderr, "[read.c] exit bc_read_file 11\n");

read_err:
	fprintf(stderr, "[read.c] enter bc_read_file 12\n");
	free(buf);
	// fprintf(stderr, "[read.c] exit bc_read_file 12\n");
malloc_err:
	fprintf(stderr, "[read.c] enter bc_read_file 13\n");
	close(fd);
	bc_verr(e, path);
	return NULL;
	// fprintf(stderr, "[read.c] exit bc_read_file 13\n");
}
// Total cost: 7.330745
// Total split cost: 0.018074, input tokens: 6283, output tokens: 1022, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 130), (130, 329)]
// Total instrumented cost: 7.312672, input tokens: 403747, output tokens: 718570, cache read tokens: 0, cache write tokens: 0
