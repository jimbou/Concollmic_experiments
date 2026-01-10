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
 * Common code for the lexers.
 *
 */

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <lex.h>
#include <vm.h>
#include <bc.h>

void
bc_lex_invalidChar(BcLex* l, char c)
{
	fprintf(stderr, "[lex.c] enter bc_lex_invalidChar 1\n");
	l->t = BC_LEX_INVALID;
	bc_lex_verr(l, BC_ERR_PARSE_CHAR, c);
	// fprintf(stderr, "[lex.c] exit bc_lex_invalidChar 1\n");
}

void
bc_lex_lineComment(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_lineComment 1\n");
	l->t = BC_LEX_WHITESPACE;
	while (l->i < l->len && l->buf[l->i] != '\n')
	{
		fprintf(stderr, "[lex.c] enter bc_lex_lineComment 2\n");
		l->i += 1;
		// fprintf(stderr, "[lex.c] exit bc_lex_lineComment 2\n");
	}
	// fprintf(stderr, "[lex.c] exit bc_lex_lineComment 1\n");
}

void
bc_lex_comment(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_comment 1\n");
	size_t i, nlines = 0;
	const char* buf;
	bool end = false, got_more;
	char c;

	l->i += 1;
	l->t = BC_LEX_WHITESPACE;
	// fprintf(stderr, "[lex.c] exit bc_lex_comment 1\n");

	// This loop is complex because it might need to request more data from
	// stdin if the comment is not ended. This loop is taken until the comment
	// is finished or we have EOF.
	fprintf(stderr, "[lex.c] enter bc_lex_comment 2\n");
	do
	{
		fprintf(stderr, "[lex.c] enter bc_lex_comment 3\n");
		buf = l->buf;
		got_more = false;

		// If we are in stdin mode, the buffer must be the one used for stdin.
#if !BC_ENABLE_OSSFUZZ
		fprintf(stderr, "[lex.c] enter bc_lex_comment 4\n");
		assert(vm->mode != BC_MODE_STDIN || buf == vm->buffer.v);
		// fprintf(stderr, "[lex.c] exit bc_lex_comment 4\n");
#endif // !BC_ENABLE_OSSFUZZ

		// Find the end of the comment.
		fprintf(stderr, "[lex.c] enter bc_lex_comment 5\n");
		for (i = l->i; !end; i += !end)
		{
			fprintf(stderr, "[lex.c] enter bc_lex_comment 6\n");
			// While we don't have an asterisk, eat, but increment nlines.
			for (; (c = buf[i]) && c != '*'; ++i)
			{
				fprintf(stderr, "[lex.c] enter bc_lex_comment 7\n");
				nlines += (c == '\n');
				// fprintf(stderr, "[lex.c] exit bc_lex_comment 7\n");
			}

			// If this is true, we need to request more data.
			if (BC_ERR(!c || buf[i + 1] == '\0'))
			{
				fprintf(stderr, "[lex.c] enter bc_lex_comment 8\n");
#if !BC_ENABLE_OSSFUZZ
				// Read more, if possible.
				if (!vm->eof && l->mode != BC_MODE_FILE)
				{
					fprintf(stderr, "[lex.c] enter bc_lex_comment 9\n");
					got_more = bc_lex_readLine(l);
					// fprintf(stderr, "[lex.c] exit bc_lex_comment 9\n");
				}
#endif // !BC_ENABLE_OSSFUZZ

				break;
				// fprintf(stderr, "[lex.c] exit bc_lex_comment 8\n");
			}

			// If this turns true, we found the end. Yay!
			fprintf(stderr, "[lex.c] enter bc_lex_comment 10\n");
			end = (buf[i + 1] == '/');
			// fprintf(stderr, "[lex.c] exit bc_lex_comment 10\n");
		}
		// fprintf(stderr, "[lex.c] exit bc_lex_comment 6\n");
		// fprintf(stderr, "[lex.c] exit bc_lex_comment 5\n");
		// fprintf(stderr, "[lex.c] exit bc_lex_comment 3\n");
	}
	while (got_more && !end);
	// fprintf(stderr, "[lex.c] exit bc_lex_comment 2\n");

	// If we didn't find the end, barf.
	fprintf(stderr, "\n");
	if (!end)
	{
		fprintf(stderr, "[lex.c] enter bc_lex_comment 12\n");
		l->i = i;
		bc_lex_err(l, BC_ERR_PARSE_COMMENT);
		// fprintf(stderr, "[lex.c] exit bc_lex_comment 12\n");
	}

	fprintf(stderr, "[lex.c] enter bc_lex_comment 13\n");
	l->i = i + 2;
	l->line += nlines;
	// fprintf(stderr, "[lex.c] exit bc_lex_comment 13\n");
}

void
bc_lex_whitespace(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_whitespace 1\n");
	char c;

	l->t = BC_LEX_WHITESPACE;
	// fprintf(stderr, "[lex.c] exit bc_lex_whitespace 1\n");

	// Eat. We don't eat newlines because they can be special.
	fprintf(stderr, "[lex.c] enter bc_lex_whitespace 2\n");
	for (c = l->buf[l->i]; c != '\n' && isspace(c); c = l->buf[++l->i])
	{
		fprintf(stderr, "[lex.c] enter bc_lex_whitespace 3\n");
		continue;
		// fprintf(stderr, "[lex.c] exit bc_lex_whitespace 3\n");
	}
	// fprintf(stderr, "[lex.c] exit bc_lex_whitespace 2\n");
}

void
bc_lex_commonTokens(BcLex* l, char c)
{
	fprintf(stderr, "[lex.c] enter bc_lex_commonTokens 1\n");
	if (!c) l->t = BC_LEX_EOF;
	else if (c == '\n') l->t = BC_LEX_NLINE;
	else bc_lex_whitespace(l);
	// fprintf(stderr, "[lex.c] exit bc_lex_commonTokens 1\n");
}

/**
 * Parses a number.
 * @param l         The lexer.
 * @param start     The start character.
 * @param int_only  Whether this function should only look for an integer. This
 *                  is used to implement the exponent of scientific notation.
 */
static size_t
bc_lex_num(BcLex* l, char start, bool int_only)
{
	fprintf(stderr, "[lex.c] enter bc_lex_num 1\n");
	const char* buf = l->buf + l->i;
	size_t i;
	char c;
	bool last_pt, pt = (start == '.');
	// fprintf(stderr, "[lex.c] exit bc_lex_num 1\n");

	// This loop looks complex. It is not. It is asking if the character is not
	// a nul byte and it if it a valid num character based on what we have found
	// thus far, or whether it is a backslash followed by a newline. I can do
	// i+1 on the buffer because the buffer must have a nul byte.
	for (i = 0; (c = buf[i]) && (BC_LEX_NUM_CHAR(c, pt, int_only) ||
	                             (c == '\\' && buf[i + 1] == '\n'));
	     ++i)
	{
		fprintf(stderr, "[lex.c] enter bc_lex_num 2\n");
		// I don't need to test that the next character is a newline because
		// the loop condition above ensures that.
		if (c == '\\')
		{
			fprintf(stderr, "[lex.c] enter bc_lex_num 3\n");
			i += 2;

			// Make sure to eat whitespace at the beginning of the line.
			while (isspace(buf[i]) && buf[i] != '\n')
			{
				fprintf(stderr, "[lex.c] enter bc_lex_num 4\n");
				i += 1;
				// fprintf(stderr, "[lex.c] exit bc_lex_num 4\n");
			}

			c = buf[i];

			// If the next character is not a number character, bail.
			if (!BC_LEX_NUM_CHAR(c, pt, int_only)) break;
			// fprintf(stderr, "[lex.c] exit bc_lex_num 3\n");
		}

		fprintf(stderr, "[lex.c] enter bc_lex_num 5\n");
		// Did we find the radix point?
		last_pt = (c == '.');

		// If we did, and we already have one, then break because it's not part
		// of this number.
		if (pt && last_pt) break;

		// Set whether we have found a radix point.
		pt = pt || last_pt;

		bc_vec_push(&l->str, &c);
		// fprintf(stderr, "[lex.c] exit bc_lex_num 5\n");
		// fprintf(stderr, "[lex.c] exit bc_lex_num 2\n");
	}

	fprintf(stderr, "[lex.c] enter bc_lex_num 6\n");
	return i;
	// fprintf(stderr, "[lex.c] exit bc_lex_num 6\n");
}

void
bc_lex_number(BcLex* l, char start)
{
	fprintf(stderr, "[lex.c] enter bc_lex_number 1\n");
	l->t = BC_LEX_NUMBER;

	// Make sure the string is clear.
	bc_vec_popAll(&l->str);
	bc_vec_push(&l->str, &start);

	// Parse the number.
	l->i += bc_lex_num(l, start, false);
	// fprintf(stderr, "[lex.c] exit bc_lex_number 1\n");

#if BC_ENABLE_EXTRA_MATH
	{
		fprintf(stderr, "[lex.c] enter bc_lex_number 2\n");
		char c = l->buf[l->i];

		// Do we have a number in scientific notation?
		if (c == 'e')
		{
			fprintf(stderr, "[lex.c] enter bc_lex_number 3\n");
#if BC_ENABLED
			// Barf for POSIX.
			if (BC_IS_POSIX) bc_lex_err(l, BC_ERR_POSIX_EXP_NUM);
#endif // BC_ENABLED

			// Push the e.
			bc_vec_push(&l->str, &c);
			l->i += 1;
			c = l->buf[l->i];

			// Check for negative specifically because bc_lex_num() does not.
			if (c == BC_LEX_NEG_CHAR)
			{
				fprintf(stderr, "[lex.c] enter bc_lex_number 4\n");
				bc_vec_push(&l->str, &c);
				l->i += 1;
				c = l->buf[l->i];
				// fprintf(stderr, "[lex.c] exit bc_lex_number 4\n");
			}

			fprintf(stderr, "[lex.c] enter bc_lex_number 5\n");
			// We must have a number character, so barf if not.
			if (BC_ERR(!BC_LEX_NUM_CHAR(c, false, true)))
			{
				fprintf(stderr, "[lex.c] enter bc_lex_number 6\n");
				bc_lex_verr(l, BC_ERR_PARSE_CHAR, c);
				// fprintf(stderr, "[lex.c] exit bc_lex_number 6\n");
			}

			// Parse the exponent.
			l->i += bc_lex_num(l, 0, true);
			// fprintf(stderr, "[lex.c] exit bc_lex_number 5\n");
			// fprintf(stderr, "[lex.c] exit bc_lex_number 3\n");
		}
		// fprintf(stderr, "[lex.c] exit bc_lex_number 2\n");
	}
#endif // BC_ENABLE_EXTRA_MATH

	fprintf(stderr, "[lex.c] enter bc_lex_number 7\n");
	bc_vec_pushByte(&l->str, '\0');
	// fprintf(stderr, "[lex.c] exit bc_lex_number 7\n");
}

void
bc_lex_name(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_name 1\n");
	size_t i = 0;
	const char* buf = l->buf + l->i - 1;
	char c = buf[i];

	l->t = BC_LEX_NAME;
	// fprintf(stderr, "[lex.c] exit bc_lex_name 1\n");

	// Should be obvious. It's looking for valid characters.
	while ((c >= 'a' && c <= 'z') || isdigit(c) || c == '_')
	{
		fprintf(stderr, "[lex.c] enter bc_lex_name 2\n");
		c = buf[++i];
		// fprintf(stderr, "[lex.c] exit bc_lex_name 2\n");
	}

	fprintf(stderr, "[lex.c] enter bc_lex_name 3\n");
	// Set the string to the identifier.
	bc_vec_string(&l->str, i, buf);

	// Increment the index. We minus 1 because it has already been incremented.
	l->i += i - 1;
	// fprintf(stderr, "[lex.c] exit bc_lex_name 3\n");
}

void
bc_lex_init(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_init 1\n");
	BC_SIG_ASSERT_LOCKED;
	assert(l != NULL);
	bc_vec_init(&l->str, sizeof(char), BC_DTOR_NONE);
	// fprintf(stderr, "[lex.c] exit bc_lex_init 1\n");
}

void
bc_lex_free(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_free 1\n");
	BC_SIG_ASSERT_LOCKED;
	assert(l != NULL);
	bc_vec_free(&l->str);
	// fprintf(stderr, "[lex.c] exit bc_lex_free 1\n");
}

void
bc_lex_file(BcLex* l, const char* file)
{
	fprintf(stderr, "[lex.c] enter bc_lex_file 1\n");
	assert(l != NULL && file != NULL);
	l->line = 1;
	vm->file = file;
	// fprintf(stderr, "[lex.c] exit bc_lex_file 1\n");
}

void
bc_lex_next(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_next 1\n");
	BC_SIG_ASSERT_LOCKED;

	assert(l != NULL);

	l->last = l->t;

	// If this wasn't here, the line number would be off.
	l->line += (l->i != 0 && l->buf[l->i - 1] == '\n');
	// fprintf(stderr, "[lex.c] exit bc_lex_next 1\n");

	// If the last token was EOF, someone called this one too many times.
	if (BC_ERR(l->last == BC_LEX_EOF))
	{
		fprintf(stderr, "[lex.c] enter bc_lex_next 2\n");
		bc_lex_err(l, BC_ERR_PARSE_EOF);
		// fprintf(stderr, "[lex.c] exit bc_lex_next 2\n");
	}

	fprintf(stderr, "[lex.c] enter bc_lex_next 3\n");
	l->t = BC_LEX_EOF;
	// fprintf(stderr, "[lex.c] exit bc_lex_next 3\n");

	// We are done if this is true.
	if (l->i == l->len)
	{
		fprintf(stderr, "[lex.c] enter bc_lex_next 4\n");
		return;
		// fprintf(stderr, "[lex.c] exit bc_lex_next 4\n");
	}

	fprintf(stderr, "[lex.c] enter bc_lex_next 5\n");
	// Loop until failure or we don't have whitespace. This
	// is so the parser doesn't get inundated with whitespace.
	do
	{
		fprintf(stderr, "[lex.c] enter bc_lex_next 6\n");
		vm->next(l);
		// fprintf(stderr, "[lex.c] exit bc_lex_next 6\n");
	}
	while (l->t == BC_LEX_WHITESPACE);
	// fprintf(stderr, "[lex.c] exit bc_lex_next 5\n");
}

/**
 * Updates the buffer and len so that they are not invalidated when the stdin
 * buffer grows.
 * @param l     The lexer.
 * @param text  The text.
 * @param len   The length of the text.
 */
static void
bc_lex_fixText(BcLex* l, const char* text, size_t len)
{
	fprintf(stderr, "[lex.c] enter bc_lex_fixText 1\n");
	l->buf = text;
	l->len = len;
	// fprintf(stderr, "[lex.c] exit bc_lex_fixText 1\n");
}
bool
bc_lex_readLine(BcLex* l)
{
	fprintf(stderr, "[lex.c] enter bc_lex_readLine 1\n");
	bool good;

	// These are reversed because they should be already locked, but
	// bc_vm_readLine() needs them to be unlocked.
	BC_SIG_UNLOCK;
	// fprintf(stderr, "[lex.c] exit bc_lex_readLine 1\n");

	// Make sure we read from the appropriate place.
	switch (l->mode)
	{
		case BC_MODE_EXPRS:
		{
			fprintf(stderr, "[lex.c] enter bc_lex_readLine 2\n");
			good = bc_vm_readBuf(false);
			break;
			// fprintf(stderr, "[lex.c] exit bc_lex_readLine 2\n");
		}

		case BC_MODE_FILE:
		{
			fprintf(stderr, "[lex.c] enter bc_lex_readLine 3\n");
			good = false;
			break;
			// fprintf(stderr, "[lex.c] exit bc_lex_readLine 3\n");
		}

#if !BC_ENABLE_OSSFUZZ

		case BC_MODE_STDIN:
		{
			fprintf(stderr, "[lex.c] enter bc_lex_readLine 4\n");
			good = bc_vm_readLine(false);
			break;
			// fprintf(stderr, "[lex.c] exit bc_lex_readLine 4\n");
		}

#endif // !BC_ENABLE_OSSFUZZ

#ifdef __GNUC__
#ifndef __clang__
		default:
		{
			fprintf(stderr, "[lex.c] enter bc_lex_readLine 5\n");
			// We should never get here.
			abort();
			// fprintf(stderr, "[lex.c] exit bc_lex_readLine 5\n");
		}
#endif // __clang__
#endif // __GNUC__
	}
	fprintf(stderr, "[lex.c] enter bc_lex_readLine 6\n");
	BC_SIG_LOCK;

	bc_lex_fixText(l, vm->buffer.v, vm->buffer.len - 1);

	return good;
	// fprintf(stderr, "[lex.c] exit bc_lex_readLine 6\n");
}

void
bc_lex_text(BcLex* l, const char* text, BcMode mode)
{
	fprintf(stderr, "[lex.c] enter bc_lex_text 1\n");
	BC_SIG_ASSERT_LOCKED;

	assert(l != NULL && text != NULL);

	bc_lex_fixText(l, text, strlen(text));
	l->i = 0;
	l->t = l->last = BC_LEX_INVALID;
	l->mode = mode;

	bc_lex_next(l);
	// fprintf(stderr, "[lex.c] exit bc_lex_text 1\n");
}
// Total cost: 0.027314
// Total split cost: 0.002614, input tokens: 8428, output tokens: 308, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 154), (154, 345), (345, 411)]
// Total instrumented cost: 0.024699, input tokens: 28648, output tokens: 15422, cache read tokens: 0, cache write tokens: 0
