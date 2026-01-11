#include <stdio.h>
#include <assert.h>
#include "jv_unicode.h"
#include "jv_utf8_tables.h"

// jvp_utf8_backtrack returns the beginning of the last codepoint in the
// string, assuming that start is the last byte in the string.
// If the last codepoint is incomplete, returns the number of missing bytes via
// *missing_bytes.  If there are no leading bytes or an invalid byte is
// encountered, NULL is returned and *missing_bytes is not altered.
const char* jvp_utf8_backtrack(const char* start, const char* min, int* missing_bytes) {
  fprintf(stderr, "enter jvp_utf8_backtrack 1\n");
  assert(min <= start);
  // fprintf(stderr, "exit jvp_utf8_backtrack 1\n");

  if (min == start) {
    fprintf(stderr, "enter jvp_utf8_backtrack 2\n");
    return min;
    // fprintf(stderr, "exit jvp_utf8_backtrack 2\n");
  }

  fprintf(stderr, "enter jvp_utf8_backtrack 3\n");
  int length = 0;
  int seen = 1;
  // fprintf(stderr, "exit jvp_utf8_backtrack 3\n");

  while (start >= min &&
         (length = utf8_coding_length[(unsigned char)*start]) == UTF8_CONTINUATION_BYTE) {
    fprintf(stderr, "enter jvp_utf8_backtrack 4\n");
    start--;
    seen++;
    // fprintf(stderr, "exit jvp_utf8_backtrack 4\n");
  }

  fprintf(stderr, "enter jvp_utf8_backtrack 5\n");
  if (length == 0 || length == UTF8_CONTINUATION_BYTE || length - seen < 0) {
    fprintf(stderr, "enter jvp_utf8_backtrack 6\n");
    return NULL;
    // fprintf(stderr, "exit jvp_utf8_backtrack 6\n");
  }
  // fprintf(stderr, "exit jvp_utf8_backtrack 5\n");

  fprintf(stderr, "enter jvp_utf8_backtrack 7\n");
  if (missing_bytes) {
    *missing_bytes = length - seen;
  }
  return start;
  // fprintf(stderr, "exit jvp_utf8_backtrack 7\n");
}

const char* jvp_utf8_next(const char* in, const char* end, int* codepoint_ret) {
  fprintf(stderr, "enter jvp_utf8_next 1\n");
  assert(in <= end);
  // fprintf(stderr, "exit jvp_utf8_next 1\n");

  if (in == end) {
    fprintf(stderr, "enter jvp_utf8_next 2\n");
    return 0;
    // fprintf(stderr, "exit jvp_utf8_next 2\n");
  }

  fprintf(stderr, "enter jvp_utf8_next 3\n");
  int codepoint = -1;
  unsigned char first = (unsigned char)in[0];
  int length = utf8_coding_length[first];
  // fprintf(stderr, "exit jvp_utf8_next 3\n");

  if ((first & 0x80) == 0) {
    fprintf(stderr, "enter jvp_utf8_next 4\n");
    /* Fast-path for ASCII */
    codepoint = first;
    length = 1;
    // fprintf(stderr, "exit jvp_utf8_next 4\n");

  } else if (length == 0 || length == UTF8_CONTINUATION_BYTE) {
    fprintf(stderr, "enter jvp_utf8_next 5\n");
    /* Invalid single byte */
    length = 1;
    // fprintf(stderr, "exit jvp_utf8_next 5\n");

  } else if (in + length > end) {
    fprintf(stderr, "enter jvp_utf8_next 6\n");
    /* Truncated UTF-8 sequence */
    length = end - in;
    // fprintf(stderr, "exit jvp_utf8_next 6\n");

  } else {
    fprintf(stderr, "enter jvp_utf8_next 7\n");
    codepoint = ((unsigned)in[0]) & utf8_coding_bits[first];
    // fprintf(stderr, "exit jvp_utf8_next 7\n");

    for (int i = 1; i < length; i++) {
      fprintf(stderr, "enter jvp_utf8_next 8\n");
      unsigned ch = (unsigned char)in[i];
      // fprintf(stderr, "exit jvp_utf8_next 8\n");

      if (utf8_coding_length[ch] != UTF8_CONTINUATION_BYTE) {
        fprintf(stderr, "enter jvp_utf8_next 9\n");
        /* Invalid continuation */
        codepoint = -1;
        length = i;
        break;
        // fprintf(stderr, "exit jvp_utf8_next 9\n");
      }

      fprintf(stderr, "enter jvp_utf8_next 10\n");
      codepoint = (codepoint << 6) | (ch & 0x3f);
      // fprintf(stderr, "exit jvp_utf8_next 10\n");
    }

    fprintf(stderr, "enter jvp_utf8_next 11\n");
    if (codepoint < utf8_first_codepoint[length]) {
      fprintf(stderr, "enter jvp_utf8_next 12\n");
      /* Overlong sequence */
      codepoint = -1;
      // fprintf(stderr, "exit jvp_utf8_next 12\n");
    }
    // fprintf(stderr, "exit jvp_utf8_next 11\n");

    fprintf(stderr, "enter jvp_utf8_next 13\n");
    if (0xD800 <= codepoint && codepoint <= 0xDFFF) {
      fprintf(stderr, "enter jvp_utf8_next 14\n");
      /* Surrogate */
      codepoint = -1;
      // fprintf(stderr, "exit jvp_utf8_next 14\n");
    }
    // fprintf(stderr, "exit jvp_utf8_next 13\n");

    fprintf(stderr, "enter jvp_utf8_next 15\n");
    if (codepoint > 0x10FFFF) {
      fprintf(stderr, "enter jvp_utf8_next 16\n");
      /* Out of range */
      codepoint = -1;
      // fprintf(stderr, "exit jvp_utf8_next 16\n");
    }
    // fprintf(stderr, "exit jvp_utf8_next 15\n");
  }

  fprintf(stderr, "enter jvp_utf8_next 17\n");
  assert(length > 0);
  *codepoint_ret = codepoint;
  return in + length;
  // fprintf(stderr, "exit jvp_utf8_next 17\n");
}

int jvp_utf8_is_valid(const char* in, const char* end) {
  fprintf(stderr, "enter jvp_utf8_is_valid 1\n");
  int codepoint;
  // fprintf(stderr, "exit jvp_utf8_is_valid 1\n");

  while ((in = jvp_utf8_next(in, end, &codepoint))) {
    fprintf(stderr, "enter jvp_utf8_is_valid 2\n");
    if (codepoint == -1) return 0;
    // fprintf(stderr, "exit jvp_utf8_is_valid 2\n");
  }

  fprintf(stderr, "enter jvp_utf8_is_valid 3\n");
  return 1;
  // fprintf(stderr, "exit jvp_utf8_is_valid 3\n");
}

/* Assumes startchar is the first byte of a valid character sequence */
int jvp_utf8_decode_length(char startchar) {
  fprintf(stderr, "enter jvp_utf8_decode_length 1\n");
  if ((startchar & 0x80) == 0) return 1;
  else if ((startchar & 0xE0) == 0xC0) return 2;
  else if ((startchar & 0xF0) == 0xE0) return 3;
  else return 4;
  // fprintf(stderr, "exit jvp_utf8_decode_length 1\n");
}

int jvp_utf8_encode_length(int codepoint) {
  fprintf(stderr, "enter jvp_utf8_encode_length 1\n");
  if (codepoint <= 0x7F) return 1;
  else if (codepoint <= 0x7FF) return 2;
  else if (codepoint <= 0xFFFF) return 3;
  else return 4;
  // fprintf(stderr, "exit jvp_utf8_encode_length 1\n");
}

int jvp_utf8_encode(int codepoint, char* out) {
  fprintf(stderr, "enter jvp_utf8_encode 1\n");
  assert(codepoint >= 0 && codepoint <= 0x10FFFF);

  char* start = out;
  // fprintf(stderr, "exit jvp_utf8_encode 1\n");

  if (codepoint <= 0x7F) {
    fprintf(stderr, "enter jvp_utf8_encode 2\n");
    *out++ = codepoint;
    // fprintf(stderr, "exit jvp_utf8_encode 2\n");

  } else if (codepoint <= 0x7FF) {
    fprintf(stderr, "enter jvp_utf8_encode 3\n");
    *out++ = 0xC0 + ((codepoint & 0x7C0) >> 6);
    *out++ = 0x80 + (codepoint & 0x03F);
    // fprintf(stderr, "exit jvp_utf8_encode 3\n");

  } else if (codepoint <= 0xFFFF) {
    fprintf(stderr, "enter jvp_utf8_encode 4\n");
    *out++ = 0xE0 + ((codepoint & 0xF000) >> 12);
    *out++ = 0x80 + ((codepoint & 0x0FC0) >> 6);
    *out++ = 0x80 + (codepoint & 0x003F);
    // fprintf(stderr, "exit jvp_utf8_encode 4\n");

  } else {
    fprintf(stderr, "enter jvp_utf8_encode 5\n");
    *out++ = 0xF0 + ((codepoint & 0x1C0000) >> 18);
    *out++ = 0x80 + ((codepoint & 0x03F000) >> 12);
    *out++ = 0x80 + ((codepoint & 0x000FC0) >> 6);
    *out++ = 0x80 + (codepoint & 0x00003F);
    // fprintf(stderr, "exit jvp_utf8_encode 5\n");
  }

  fprintf(stderr, "enter jvp_utf8_encode 6\n");
  assert(out - start == jvp_utf8_encode_length(codepoint));
  return out - start;
  // fprintf(stderr, "exit jvp_utf8_encode 6\n");
}

// characters with White_Space property in:
// https://www.unicode.org/Public/UCD/latest/ucd/PropList.txt
int jvp_codepoint_is_whitespace(int c) {
  fprintf(stderr, "enter jvp_codepoint_is_whitespace 1\n");
  return
    (c >= 0x0009 && c <= 0x000D) ||
    c == 0x0020 ||
    c == 0x0085 ||
    c == 0x00A0 ||
    c == 0x1680 ||
    (c >= 0x2000 && c <= 0x200A) ||
    c == 0x2028 ||
    c == 0x2029 ||
    c == 0x202F ||
    c == 0x205F ||
    c == 0x3000;
  // fprintf(stderr, "exit jvp_codepoint_is_whitespace 1\n");
}

// Total cost: 0.024522
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 138)]
// Total instrumented cost: 0.024522, input tokens: 35635, output tokens: 13546, cache read tokens: 0, cache write tokens: 0