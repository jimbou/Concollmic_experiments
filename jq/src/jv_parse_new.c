#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "jv.h"
#include "jv_dtoa.h"
#include "jv_unicode.h"
#include "jv_alloc.h"
#include "jv_dtoa.h"

typedef const char* presult;

#ifndef MAX_PARSING_DEPTH
#define MAX_PARSING_DEPTH (10000)
#endif

#define TRY(x) do {presult msg__ = (x); if (msg__) return msg__; } while(0)
#ifdef __GNUC__
#define pfunc __attribute__((warn_unused_result)) presult
#else
#define pfunc presult
#endif

enum last_seen {
  JV_LAST_NONE = 0,
  JV_LAST_OPEN_ARRAY = '[',
  JV_LAST_OPEN_OBJECT = '{',
  JV_LAST_COLON = ':',
  JV_LAST_COMMA = ',',
  JV_LAST_VALUE = 'V',
};

struct jv_parser {
  const char* curr_buf;
  int curr_buf_length;
  int curr_buf_pos;
  int curr_buf_is_partial;
  int eof;
  unsigned bom_strip_position;

  int flags;

  jv* stack;                   // parser
  int stackpos;                // parser
  int stacklen;                // both (optimization; it's really pathlen for streaming)
  jv path;                     // streamer
  enum last_seen last_seen;    // streamer
  jv output;                   // streamer
  jv next;                     // both

  char* tokenbuf;
  int tokenpos;
  int tokenlen;

  int line, column;

  struct dtoa_context dtoa;

  enum {
    JV_PARSER_NORMAL,
    JV_PARSER_STRING,
    JV_PARSER_STRING_ESCAPE,
    JV_PARSER_WAITING_FOR_RS // parse error, waiting for RS
  } st;
  unsigned int last_ch_was_ws:1;
};


static void parser_init(struct jv_parser* p, int flags) {
  fprintf(stderr, "[jv_parse.c] enter parser_init 1\n");
  p->flags = flags;
  if ((p->flags & JV_PARSE_STREAMING)) {
    p->path = jv_array();
  } else {
    p->path = jv_invalid();
    p->flags &= ~(JV_PARSE_STREAM_ERRORS);
  }
  p->stack = 0;
  p->stacklen = p->stackpos = 0;
  p->last_seen = JV_LAST_NONE;
  p->output = jv_invalid();
  p->next = jv_invalid();
  p->tokenbuf = 0;
  p->tokenlen = p->tokenpos = 0;
  if ((p->flags & JV_PARSE_SEQ))
    p->st = JV_PARSER_WAITING_FOR_RS;
  else
    p->st = JV_PARSER_NORMAL;
  p->eof = 0;
  p->curr_buf = 0;
  p->curr_buf_length = p->curr_buf_pos = p->curr_buf_is_partial = 0;
  p->bom_strip_position = 0;
  p->last_ch_was_ws = 0;
  p->line = 1;
  p->column = 0;
  jvp_dtoa_context_init(&p->dtoa);
  // fprintf(stderr, "[jv_parse.c] exit parser_init 1\n");
}

static void parser_reset(struct jv_parser* p) {
  fprintf(stderr, "[jv_parse.c] enter parser_reset 1\n");
  if ((p->flags & JV_PARSE_STREAMING)) {
    jv_free(p->path);
    p->path = jv_array();
    p->stacklen = 0;
  }
  p->last_seen = JV_LAST_NONE;
  jv_free(p->output);
  p->output = jv_invalid();
  jv_free(p->next);
  p->next = jv_invalid();
  for (int i=0; i<p->stackpos; i++)
    jv_free(p->stack[i]);
  p->stackpos = 0;
  p->tokenpos = 0;
  p->st = JV_PARSER_NORMAL;
  // fprintf(stderr, "[jv_parse.c] exit parser_reset 1\n");
}

static void parser_free(struct jv_parser* p) {
  fprintf(stderr, "[jv_parse.c] enter parser_free 1\n");
  parser_reset(p);
  jv_free(p->path);
  jv_free(p->output);
  jv_mem_free(p->stack);
  jv_mem_free(p->tokenbuf);
  jvp_dtoa_context_free(&p->dtoa);
  // fprintf(stderr, "[jv_parse.c] exit parser_free 1\n");
}

static pfunc value(struct jv_parser* p, jv val) {
  fprintf(stderr, "\n");
  if ((p->flags & JV_PARSE_STREAMING)) {
    fprintf(stderr, "[jv_parse.c] enter value 2\n");
    if (jv_is_valid(p->next) || p->last_seen == JV_LAST_VALUE) {
      fprintf(stderr, "[jv_parse.c] enter value 3\n");
      jv_free(val);
      return "Expected separator between values";
      // fprintf(stderr, "[jv_parse.c] exit value 3\n");
    }
    fprintf(stderr, "[jv_parse.c] enter value 4\n");
    if (p->stacklen > 0)
      p->last_seen = JV_LAST_VALUE;
    else
      p->last_seen = JV_LAST_NONE;
    // fprintf(stderr, "[jv_parse.c] exit value 4\n");
    // fprintf(stderr, "[jv_parse.c] exit value 2\n");
  } else {
    fprintf(stderr, "[jv_parse.c] enter value 5\n");
    if (jv_is_valid(p->next)) {
      fprintf(stderr, "[jv_parse.c] enter value 6\n");
      jv_free(val);
      return "Expected separator between values";
      // fprintf(stderr, "[jv_parse.c] exit value 6\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit value 5\n");
  }
  fprintf(stderr, "[jv_parse.c] enter value 7\n");
  jv_free(p->next);
  p->next = val;
  return 0;
  // fprintf(stderr, "[jv_parse.c] exit value 7\n");
}

static void push(struct jv_parser* p, jv v) {
  fprintf(stderr, "\n");
  assert(p->stackpos <= p->stacklen);
  if (p->stackpos == p->stacklen) {
    fprintf(stderr, "[jv_parse.c] enter push 2\n");
    p->stacklen = p->stacklen * 2 + 10;
    p->stack = jv_mem_realloc(p->stack, p->stacklen * sizeof(jv));
    // fprintf(stderr, "[jv_parse.c] exit push 2\n");
  }
  fprintf(stderr, "[jv_parse.c] enter push 3\n");
  assert(p->stackpos < p->stacklen);
  p->stack[p->stackpos++] = v;
  // fprintf(stderr, "[jv_parse.c] exit push 3\n");
}
static pfunc parse_token(struct jv_parser* p, char ch) {
  fprintf(stderr, "\n");
  switch (ch) {
  case '[':
    fprintf(stderr, "[jv_parse.c] enter parse_token 2\n");
    if (p->stackpos >= MAX_PARSING_DEPTH) return "Exceeds depth limit for parsing";
    if (jv_is_valid(p->next)) return "Expected separator between values";
    push(p, jv_array());
    break;
    // fprintf(stderr, "[jv_parse.c] exit parse_token 2\n");

  case '{':
    fprintf(stderr, "[jv_parse.c] enter parse_token 3\n");
    if (p->stackpos >= MAX_PARSING_DEPTH) return "Exceeds depth limit for parsing";
    if (jv_is_valid(p->next)) return "Expected separator between values";
    push(p, jv_object());
    break;
    // fprintf(stderr, "[jv_parse.c] exit parse_token 3\n");

  case ':':
    fprintf(stderr, "[jv_parse.c] enter parse_token 4\n");
    if (!jv_is_valid(p->next))
      return "Expected string key before ':'";
    if (p->stackpos == 0 || jv_get_kind(p->stack[p->stackpos-1]) != JV_KIND_OBJECT)
      return "':' not as part of an object";
    if (jv_get_kind(p->next) != JV_KIND_STRING)
      return "Object keys must be strings";
    push(p, p->next);
    p->next = jv_invalid();
    break;
    // fprintf(stderr, "[jv_parse.c] exit parse_token 4\n");

  case ',':
    fprintf(stderr, "[jv_parse.c] enter parse_token 5\n");
    if (!jv_is_valid(p->next))
      return "Expected value before ','";
    if (p->stackpos == 0)
      return "',' not as part of an object or array";
    if (jv_get_kind(p->stack[p->stackpos-1]) == JV_KIND_ARRAY) {
      fprintf(stderr, "[jv_parse.c] enter parse_token 6\n");
      p->stack[p->stackpos-1] = jv_array_append(p->stack[p->stackpos-1], p->next);
      p->next = jv_invalid();
      // fprintf(stderr, "[jv_parse.c] exit parse_token 6\n");
    } else if (jv_get_kind(p->stack[p->stackpos-1]) == JV_KIND_STRING) {
      fprintf(stderr, "[jv_parse.c] enter parse_token 7\n");
      assert(p->stackpos > 1 && jv_get_kind(p->stack[p->stackpos-2]) == JV_KIND_OBJECT);
      p->stack[p->stackpos-2] = jv_object_set(p->stack[p->stackpos-2],
                                              p->stack[p->stackpos-1], p->next);
      p->stackpos--;
      p->next = jv_invalid();
      // fprintf(stderr, "[jv_parse.c] exit parse_token 7\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter parse_token 8\n");
      // this case hits on input like {"a", "b"}
      return "Objects must consist of key:value pairs";
      // fprintf(stderr, "[jv_parse.c] exit parse_token 8\n");
    }
    break;
    // fprintf(stderr, "[jv_parse.c] exit parse_token 5\n");

  case ']':
    fprintf(stderr, "[jv_parse.c] enter parse_token 9\n");
    if (p->stackpos == 0 || jv_get_kind(p->stack[p->stackpos-1]) != JV_KIND_ARRAY)
      return "Unmatched ']'";
    if (jv_is_valid(p->next)) {
      fprintf(stderr, "[jv_parse.c] enter parse_token 10\n");
      p->stack[p->stackpos-1] = jv_array_append(p->stack[p->stackpos-1], p->next);
      p->next = jv_invalid();
      // fprintf(stderr, "[jv_parse.c] exit parse_token 10\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter parse_token 11\n");
      if (jv_array_length(jv_copy(p->stack[p->stackpos-1])) != 0) {
        fprintf(stderr, "[jv_parse.c] enter parse_token 12\n");
        // this case hits on input like [1,2,3,]
        return "Expected another array element";
        // fprintf(stderr, "[jv_parse.c] exit parse_token 12\n");
      }
      // fprintf(stderr, "[jv_parse.c] exit parse_token 11\n");
    }
    jv_free(p->next);
    p->next = p->stack[--p->stackpos];
    break;
    // fprintf(stderr, "[jv_parse.c] exit parse_token 9\n");

  case '}':
    fprintf(stderr, "[jv_parse.c] enter parse_token 13\n");
    if (p->stackpos == 0)
      return "Unmatched '}'";
    if (jv_is_valid(p->next)) {
      fprintf(stderr, "[jv_parse.c] enter parse_token 14\n");
      if (jv_get_kind(p->stack[p->stackpos-1]) != JV_KIND_STRING)
        return "Objects must consist of key:value pairs";
      assert(p->stackpos > 1 && jv_get_kind(p->stack[p->stackpos-2]) == JV_KIND_OBJECT);
      p->stack[p->stackpos-2] = jv_object_set(p->stack[p->stackpos-2],
                                              p->stack[p->stackpos-1], p->next);
      p->stackpos--;
      p->next = jv_invalid();
      // fprintf(stderr, "[jv_parse.c] exit parse_token 14\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter parse_token 15\n");
      if (jv_get_kind(p->stack[p->stackpos-1]) != JV_KIND_OBJECT)
        return "Unmatched '}'";
      if (jv_object_length(jv_copy(p->stack[p->stackpos-1])) != 0)
        return "Expected another key-value pair";
      // fprintf(stderr, "[jv_parse.c] exit parse_token 15\n");
    }
    jv_free(p->next);
    p->next = p->stack[--p->stackpos];
    break;
    // fprintf(stderr, "[jv_parse.c] exit parse_token 13\n");
  }
  fprintf(stderr, "[jv_parse.c] enter parse_token 16\n");
  return 0;
  // fprintf(stderr, "[jv_parse.c] exit parse_token 16\n");
}
static pfunc stream_token(struct jv_parser* p, char ch) {
  jv_kind k;
  jv last;

  switch (ch) {
  case '[':
    fprintf(stderr, "[jv_parse.c] enter stream_token 1\n");
    if (jv_is_valid(p->next))
      return "Expected a separator between values";
    if (p->last_seen == JV_LAST_OPEN_OBJECT)
      // Looks like {["foo"]}
      return "Expected string key after '{', not '['";
    if (p->last_seen == JV_LAST_COMMA) {
      last = jv_array_get(jv_copy(p->path), p->stacklen - 1);
      k = jv_get_kind(last);
      jv_free(last);
      if (k != JV_KIND_NUMBER)
        // Looks like {"x":"y",["foo"]}
        return "Expected string key after ',' in object, not '['";
    }
    p->path = jv_array_append(p->path, jv_number(0)); // push
    p->last_seen = JV_LAST_OPEN_ARRAY;
    p->stacklen++;
    break;
    // fprintf(stderr, "[jv_parse.c] exit stream_token 1\n");

  case '{':
    fprintf(stderr, "[jv_parse.c] enter stream_token 2\n");
    if (p->last_seen == JV_LAST_VALUE)
      return "Expected a separator between values";
    if (p->last_seen == JV_LAST_OPEN_OBJECT)
      // Looks like {{"foo":"bar"}}
      return "Expected string key after '{', not '{'";
    if (p->last_seen == JV_LAST_COMMA) {
      last = jv_array_get(jv_copy(p->path), p->stacklen - 1);
      k = jv_get_kind(last);
      jv_free(last);
      if (k != JV_KIND_NUMBER)
        // Looks like {"x":"y",{"foo":"bar"}}
        return "Expected string key after ',' in object, not '{'";
    }
    // Push object key: null, since we don't know it yet
    p->path = jv_array_append(p->path, jv_null()); // push
    p->last_seen = JV_LAST_OPEN_OBJECT;
    p->stacklen++;
    break;
    // fprintf(stderr, "[jv_parse.c] exit stream_token 2\n");

  case ':':
    fprintf(stderr, "[jv_parse.c] enter stream_token 3\n");
    last = jv_invalid();
    if (p->stacklen == 0 || jv_get_kind(last = jv_array_get(jv_copy(p->path), p->stacklen - 1)) == JV_KIND_NUMBER) {
      jv_free(last);
      return "':' not as part of an object";
    }
    jv_free(last);
    if (!jv_is_valid(p->next) || p->last_seen == JV_LAST_NONE)
      return "Expected string key before ':'";
    if (jv_get_kind(p->next) != JV_KIND_STRING)
      return "Object keys must be strings";
    if (p->last_seen != JV_LAST_VALUE)
      return "':' should follow a key";
    p->last_seen = JV_LAST_COLON;
    p->path = jv_array_set(p->path, p->stacklen - 1, p->next);
    p->next = jv_invalid();
    break;
    // fprintf(stderr, "[jv_parse.c] exit stream_token 3\n");

  case ',':
    fprintf(stderr, "[jv_parse.c] enter stream_token 4\n");
    if (p->last_seen != JV_LAST_VALUE)
      return "Expected value before ','";
    if (p->stacklen == 0)
      return "',' not as part of an object or array";
    last = jv_array_get(jv_copy(p->path), p->stacklen - 1);
    k = jv_get_kind(last);
    if (k == JV_KIND_NUMBER) {
      fprintf(stderr, "[jv_parse.c] enter stream_token 5\n");
      int idx = jv_number_value(last);

      if (jv_is_valid(p->next)) {
        p->output = JV_ARRAY(jv_copy(p->path), p->next);
        p->next = jv_invalid();
      }
      p->path = jv_array_set(p->path, p->stacklen - 1, jv_number(idx + 1));
      p->last_seen = JV_LAST_COMMA;
      // fprintf(stderr, "[jv_parse.c] exit stream_token 5\n");
    } else if (k == JV_KIND_STRING) {
      fprintf(stderr, "[jv_parse.c] enter stream_token 6\n");
      if (jv_is_valid(p->next)) {
        p->output = JV_ARRAY(jv_copy(p->path), p->next);
        p->next = jv_invalid();
      }
      p->path = jv_array_set(p->path, p->stacklen - 1, jv_null()); // ready for another key:value pair
      p->last_seen = JV_LAST_COMMA;
      // fprintf(stderr, "[jv_parse.c] exit stream_token 6\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter stream_token 7\n");
      assert(k == JV_KIND_NULL);
      // this case hits on input like {,}
      // make sure to handle input like {"a", "b"} and {"a":, ...}
      jv_free(last);
      return "Objects must consist of key:value pairs";
      // fprintf(stderr, "[jv_parse.c] exit stream_token 7\n");
    }
    jv_free(last);
    break;
    // fprintf(stderr, "[jv_parse.c] exit stream_token 4\n");

  case ']':
    fprintf(stderr, "[jv_parse.c] enter stream_token 8\n");
    if (p->stacklen == 0)
      return "Unmatched ']' at the top-level";
    if (p->last_seen == JV_LAST_COMMA)
      return "Expected another array element";
    if (p->last_seen == JV_LAST_OPEN_ARRAY)
      assert(!jv_is_valid(p->next));

    last = jv_array_get(jv_copy(p->path), p->stacklen - 1);
    k = jv_get_kind(last);
    jv_free(last);

    if (k != JV_KIND_NUMBER)
      return "Unmatched ']' in the middle of an object";
    if (jv_is_valid(p->next)) {
      fprintf(stderr, "[jv_parse.c] enter stream_token 9\n");
      p->output = JV_ARRAY(jv_copy(p->path), p->next, jv_true());
      p->next = jv_invalid();
      // fprintf(stderr, "[jv_parse.c] exit stream_token 9\n");
    } else if (p->last_seen != JV_LAST_OPEN_ARRAY) {
      fprintf(stderr, "[jv_parse.c] enter stream_token 10\n");
      p->output = JV_ARRAY(jv_copy(p->path));
      // fprintf(stderr, "[jv_parse.c] exit stream_token 10\n");
    }

    p->path = jv_array_slice(p->path, 0, --(p->stacklen)); // pop
    //assert(!jv_is_valid(p->next));
    jv_free(p->next);
    p->next = jv_invalid();

    if (p->last_seen == JV_LAST_OPEN_ARRAY) {
      fprintf(stderr, "[jv_parse.c] enter stream_token 11\n");
      p->output = JV_ARRAY(jv_copy(p->path), jv_array()); // Empty arrays are leaves
      // fprintf(stderr, "[jv_parse.c] exit stream_token 11\n");
    }

    if (p->stacklen == 0)
      p->last_seen = JV_LAST_NONE;
    else
      p->last_seen = JV_LAST_VALUE;
    break;
    // fprintf(stderr, "[jv_parse.c] exit stream_token 8\n");

  case '}':
    fprintf(stderr, "[jv_parse.c] enter stream_token 12\n");
    if (p->stacklen == 0)
      return "Unmatched '}' at the top-level";
    if (p->last_seen == JV_LAST_COMMA)
      return "Expected another key:value pair";
    if (p->last_seen == JV_LAST_OPEN_OBJECT)
      assert(!jv_is_valid(p->next));

    last = jv_array_get(jv_copy(p->path), p->stacklen - 1);
    k = jv_get_kind(last);
    jv_free(last);
    if (k == JV_KIND_NUMBER)
      return "Unmatched '}' in the middle of an array";

    if (jv_is_valid(p->next)) {
      fprintf(stderr, "[jv_parse.c] enter stream_token 13\n");
      if (k != JV_KIND_STRING)
        return "Objects must consist of key:value pairs";
      p->output = JV_ARRAY(jv_copy(p->path), p->next, jv_true());
      p->next = jv_invalid();
      // fprintf(stderr, "[jv_parse.c] exit stream_token 13\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter stream_token 14\n");
      // Perhaps {"a":[]}
      if (p->last_seen == JV_LAST_COLON)
        // Looks like {"a":}
        return "Missing value in key:value pair";
      if (p->last_seen == JV_LAST_COMMA)
        // Looks like {"a":0,}
        return "Expected another key-value pair";
      if (p->last_seen == JV_LAST_OPEN_ARRAY)
        return "Unmatched '}' in the middle of an array";
      if (p->last_seen != JV_LAST_VALUE && p->last_seen != JV_LAST_OPEN_OBJECT)
        return "Unmatched '}'";
      if (p->last_seen != JV_LAST_OPEN_OBJECT) {
        fprintf(stderr, "[jv_parse.c] enter stream_token 15\n");
        p->output = JV_ARRAY(jv_copy(p->path));
        // fprintf(stderr, "[jv_parse.c] exit stream_token 15\n");
      }
      // fprintf(stderr, "[jv_parse.c] exit stream_token 14\n");
    }
    p->path = jv_array_slice(p->path, 0, --(p->stacklen)); // pop
    jv_free(p->next);
    p->next = jv_invalid();

    if (p->last_seen == JV_LAST_OPEN_OBJECT) {
      fprintf(stderr, "[jv_parse.c] enter stream_token 16\n");
      p->output = JV_ARRAY(jv_copy(p->path), jv_object()); // Empty arrays are leaves
      // fprintf(stderr, "[jv_parse.c] exit stream_token 16\n");
    }

    if (p->stacklen == 0)
      p->last_seen = JV_LAST_NONE;
    else
      p->last_seen = JV_LAST_VALUE;
    break;
    // fprintf(stderr, "[jv_parse.c] exit stream_token 12\n");
  }
  fprintf(stderr, "[jv_parse.c] enter stream_token 17\n");
  return 0;
  // fprintf(stderr, "[jv_parse.c] exit stream_token 17\n");
}

static void tokenadd(struct jv_parser* p, char c) {
  fprintf(stderr, "[jv_parse.c] enter tokenadd 1\n");
  assert(p->tokenpos <= p->tokenlen);
  if (p->tokenpos >= (p->tokenlen - 1)) {
    fprintf(stderr, "[jv_parse.c] enter tokenadd 2\n");
    p->tokenlen = p->tokenlen*2 + 256;
    p->tokenbuf = jv_mem_realloc(p->tokenbuf, p->tokenlen);
    // fprintf(stderr, "[jv_parse.c] exit tokenadd 2\n");
  }
  fprintf(stderr, "[jv_parse.c] enter tokenadd 3\n");
  assert(p->tokenpos < p->tokenlen);
  p->tokenbuf[p->tokenpos++] = c;
  // fprintf(stderr, "[jv_parse.c] exit tokenadd 3\n");
  // fprintf(stderr, "[jv_parse.c] exit tokenadd 1\n");
}
static int unhex4(char* hex) {
  int r = 0;
  for (int i=0; i<4; i++) {
    char c = *hex++;
    int n;
    if ('0' <= c && c <= '9') n = c - '0';
    else if ('a' <= c && c <= 'f') n = c - 'a' + 10;
    else if ('A' <= c && c <= 'F') n = c - 'A' + 10;
    else return -1;
    r <<= 4;
    r |= n;
  }
  return r;
}

static pfunc found_string(struct jv_parser* p) {
  char* in = p->tokenbuf;
  char* out = p->tokenbuf;
  char* end = p->tokenbuf + p->tokenpos;

  while (in < end) {
    char c = *in++;
    if (c == '\\') {
      if (in >= end)
        return "Expected escape character at end of string";
      c = *in++;
      switch (c) {
      case '\\':
      case '"':
      case '/': *out++ = c;    break;
      case 'b': *out++ = '\b'; break;
      case 'f': *out++ = '\f'; break;
      case 't': *out++ = '\t'; break;
      case 'n': *out++ = '\n'; break;
      case 'r': *out++ = '\r'; break;

      case 'u':
        /* ahh, the complicated case */
        if (in + 4 > end)
          return "Invalid \\uXXXX escape";
        int hexvalue = unhex4(in);
        if (hexvalue < 0)
          return "Invalid characters in \\uXXXX escape";
        unsigned long codepoint = (unsigned long)hexvalue;
        in += 4;
        if (0xD800 <= codepoint && codepoint <= 0xDBFF) {
          /* who thought UTF-16 surrogate pairs were a good idea? */
          if (in + 6 > end || in[0] != '\\' || in[1] != 'u')
            return "Invalid \\uXXXX\\uXXXX surrogate pair escape";
          unsigned long surrogate = unhex4(in+2);
          if (!(0xDC00 <= surrogate && surrogate <= 0xDFFF))
            return "Invalid \\uXXXX\\uXXXX surrogate pair escape";
          in += 6;
          codepoint = 0x10000 + (((codepoint - 0xD800) << 10)
                                 |(surrogate - 0xDC00));
        }
        if (codepoint > 0x10FFFF)
          codepoint = 0xFFFD; // U+FFFD REPLACEMENT CHARACTER
        out += jvp_utf8_encode(codepoint, out);
        break;

      default:
        return "Invalid escape";
      }
    } else {
      if (!(c & ~0x1F))
        return "Invalid string: control characters from U+0000 through U+001F must be escaped";
      *out++ = c;
    }
  }
  TRY(value(p, jv_string_sized(p->tokenbuf, out - p->tokenbuf)));
  p->tokenpos = 0;
  return 0;
}

static pfunc check_literal(struct jv_parser* p) {
  if (p->tokenpos == 0) return 0;

  const char* pattern = 0;
  int plen;
  jv v;
  switch (p->tokenbuf[0]) {
  case 't': pattern = "true"; plen = 4; v = jv_true(); break;
  case 'f': pattern = "false"; plen = 5; v = jv_false(); break;
  case '\'':
    return "Invalid string literal; expected \", but got '";
  case 'n':
    // if it starts with 'n', it could be a literal "nan"
    if (p->tokenpos > 1 && p->tokenbuf[1] == 'u') {
      pattern = "null"; plen = 4; v = jv_null();
    }
  }
  if (pattern) {
    if (p->tokenpos != plen) return "Invalid literal";
    for (int i=0; i<plen; i++)
      if (p->tokenbuf[i] != pattern[i])
        return "Invalid literal";
    TRY(value(p, v));
  } else {
    // FIXME: better parser
    p->tokenbuf[p->tokenpos] = 0;
#ifdef USE_DECNUM
    jv number = jv_number_with_literal(p->tokenbuf);
    if (jv_get_kind(number) == JV_KIND_INVALID) {
      return "Invalid numeric literal";
    }
    TRY(value(p, number));
#else
    char *end = 0;
    double d = jvp_strtod(&p->dtoa, p->tokenbuf, &end);
    if (end == 0 || *end != 0) {
      return "Invalid numeric literal";
    }
    TRY(value(p, jv_number(d)));
#endif
  }
  p->tokenpos = 0;
  return 0;
}

typedef enum {
  LITERAL,
  WHITESPACE,
  STRUCTURE,
  QUOTE,
  INVALID
} chclass;

static chclass classify(char c) {
  switch (c) {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    return WHITESPACE;
  case '"':
    return QUOTE;
  case '[':
  case ',':
  case ']':
  case '{':
  case ':':
  case '}':
    return STRUCTURE;
  default:
    return LITERAL;
  }
}


static const presult OK = "output produced";

static int parse_check_done(struct jv_parser* p, jv* out) {
  if (p->stackpos == 0 && jv_is_valid(p->next)) {
    *out = p->next;
    p->next = jv_invalid();
    return 1;
  } else {
    return 0;
  }
}

static int stream_check_done(struct jv_parser* p, jv* out) {
  if (p->stacklen == 0 && jv_is_valid(p->next)) {
    *out = JV_ARRAY(jv_copy(p->path),p->next);
    p->next = jv_invalid();
    return 1;
  } else if (jv_is_valid(p->output)) {
    if (jv_array_length(jv_copy(p->output)) > 2) {
      // At end of an array or object, necessitating one more output by
      // which to indicate this
      *out = jv_array_slice(jv_copy(p->output), 0, 2);
      p->output = jv_array_slice(p->output, 0, 1);      // arrange one more output
    } else {
      // No further processing needed
      *out = p->output;
      p->output = jv_invalid();
    }
    return 1;
  } else {
    return 0;
  }
}

static int seq_check_truncation(struct jv_parser* p) {
  return (!p->last_ch_was_ws && (p->stackpos > 0 || p->tokenpos > 0 || jv_get_kind(p->next) == JV_KIND_NUMBER));
}

static int stream_seq_check_truncation(struct jv_parser* p) {
  jv_kind k = jv_get_kind(p->next);
  return (p->stacklen > 0 || k == JV_KIND_NUMBER || k == JV_KIND_TRUE || k == JV_KIND_FALSE || k == JV_KIND_NULL);
}

static int parse_is_top_num(struct jv_parser* p) {
  return (p->stackpos == 0 && jv_get_kind(p->next) == JV_KIND_NUMBER);
}

static int stream_is_top_num(struct jv_parser* p) {
  fprintf(stderr, "[jv_parse.c] enter stream_is_top_num 1\n");
  return (p->stacklen == 0 && jv_get_kind(p->next) == JV_KIND_NUMBER);
  // fprintf(stderr, "[jv_parse.c] exit stream_is_top_num 1\n");
}

#define check_done(p, o) \
   (((p)->flags & JV_PARSE_STREAMING) ? stream_check_done((p), (o)) : parse_check_done((p), (o)))

#define token(p, ch) \
   (((p)->flags & JV_PARSE_STREAMING) ? stream_token((p), (ch)) : parse_token((p), (ch)))

#define check_truncation(p) \
   (((p)->flags & JV_PARSE_STREAMING) ? stream_seq_check_truncation((p)) : seq_check_truncation((p)))

#define is_top_num(p) \
   (((p)->flags & JV_PARSE_STREAMING) ? stream_is_top_num((p)) : parse_is_top_num((p)))

static pfunc scan(struct jv_parser* p, char ch, jv* out) {
  fprintf(stderr, "\n");
  p->column++;
  if (ch == '\n') {
    fprintf(stderr, "[jv_parse.c] enter scan 2\n");
    p->line++;
    p->column = 0;
    // fprintf(stderr, "[jv_parse.c] exit scan 2\n");
  }
  if ((p->flags & JV_PARSE_SEQ)
      && ch == '\036' /* ASCII RS; see draft-ietf-json-sequence-07 */) {
    fprintf(stderr, "[jv_parse.c] enter scan 3\n");
    if (check_truncation(p)) {
      fprintf(stderr, "[jv_parse.c] enter scan 4\n");
      if (check_literal(p) == 0 && is_top_num(p))
        return "Potentially truncated top-level numeric value";
      return "Truncated value";
      // fprintf(stderr, "[jv_parse.c] exit scan 4\n");
    }
    TRY(check_literal(p));
    if (p->st == JV_PARSER_NORMAL && check_done(p, out))
      return OK;
    // shouldn't happen?
    assert(!jv_is_valid(*out));
    parser_reset(p);
    jv_free(*out);
    *out = jv_invalid();
    return OK;
    // fprintf(stderr, "[jv_parse.c] exit scan 3\n");
  }
  fprintf(stderr, "[jv_parse.c] enter scan 5\n");
  presult answer = 0;
  p->last_ch_was_ws = 0;
  if (p->st == JV_PARSER_NORMAL) {
    fprintf(stderr, "[jv_parse.c] enter scan 6\n");
    chclass cls = classify(ch);
    if (cls == WHITESPACE)
      p->last_ch_was_ws = 1;
    if (cls != LITERAL) {
      TRY(check_literal(p));
      if (check_done(p, out)) answer = OK;
    }
    switch (cls) {
    case LITERAL:
      fprintf(stderr, "[jv_parse.c] enter scan 7\n");
      tokenadd(p, ch);
      break;
      // fprintf(stderr, "[jv_parse.c] exit scan 7\n");
    case WHITESPACE:
      fprintf(stderr, "[jv_parse.c] enter scan 8\n");
      break;
      // fprintf(stderr, "[jv_parse.c] exit scan 8\n");
    case QUOTE:
      fprintf(stderr, "[jv_parse.c] enter scan 9\n");
      p->st = JV_PARSER_STRING;
      break;
      // fprintf(stderr, "[jv_parse.c] exit scan 9\n");
    case STRUCTURE:
      fprintf(stderr, "[jv_parse.c] enter scan 10\n");
      TRY(token(p, ch));
      break;
      // fprintf(stderr, "[jv_parse.c] exit scan 10\n");
    case INVALID:
      fprintf(stderr, "[jv_parse.c] enter scan 11\n");
      return "Invalid character";
      // fprintf(stderr, "[jv_parse.c] exit scan 11\n");
    }
    if (check_done(p, out)) answer = OK;
    // fprintf(stderr, "[jv_parse.c] exit scan 6\n");
  } else {
    fprintf(stderr, "[jv_parse.c] enter scan 12\n");
    if (ch == '"' && p->st == JV_PARSER_STRING) {
      fprintf(stderr, "[jv_parse.c] enter scan 13\n");
      TRY(found_string(p));
      p->st = JV_PARSER_NORMAL;
      if (check_done(p, out)) answer = OK;
      // fprintf(stderr, "[jv_parse.c] exit scan 13\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter scan 14\n");
      tokenadd(p, ch);
      if (ch == '\\' && p->st == JV_PARSER_STRING) {
        fprintf(stderr, "[jv_parse.c] enter scan 15\n");
        p->st = JV_PARSER_STRING_ESCAPE;
        // fprintf(stderr, "[jv_parse.c] exit scan 15\n");
      } else {
        fprintf(stderr, "[jv_parse.c] enter scan 16\n");
        p->st = JV_PARSER_STRING;
        // fprintf(stderr, "[jv_parse.c] exit scan 16\n");
      }
      // fprintf(stderr, "[jv_parse.c] exit scan 14\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit scan 12\n");
  }
  return answer;
  // fprintf(stderr, "[jv_parse.c] exit scan 5\n");
}

struct jv_parser* jv_parser_new(int flags) {
  fprintf(stderr, "[jv_parse.c] enter jv_parser_new 1\n");
  struct jv_parser* p = jv_mem_alloc(sizeof(struct jv_parser));
  parser_init(p, flags);
  p->flags = flags;
  return p;
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_new 1\n");
}

void jv_parser_free(struct jv_parser* p) {
  fprintf(stderr, "[jv_parse.c] enter jv_parser_free 1\n");
  parser_free(p);
  jv_mem_free(p);
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_free 1\n");
}

static const unsigned char UTF8_BOM[] = {0xEF,0xBB,0xBF};

int jv_parser_remaining(struct jv_parser* p) {
  fprintf(stderr, "[jv_parse.c] enter jv_parser_remaining 1\n");
  if (p->curr_buf == 0)
    return 0;
  return (p->curr_buf_length - p->curr_buf_pos);
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_remaining 1\n");
}

void jv_parser_set_buf(struct jv_parser* p, const char* buf, int length, int is_partial) {
  fprintf(stderr, "[jv_parse.c] enter jv_parser_set_buf 1\n");
  assert((p->curr_buf == 0 || p->curr_buf_pos == p->curr_buf_length)
         && "previous buffer not exhausted");
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_set_buf 1\n");
  
  while (length > 0 && p->bom_strip_position < sizeof(UTF8_BOM)) {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_set_buf 2\n");
    if ((unsigned char)*buf == UTF8_BOM[p->bom_strip_position]) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_set_buf 3\n");
      // matched a BOM character
      buf++;
      length--;
      p->bom_strip_position++;
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_set_buf 3\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_set_buf 4\n");
      if (p->bom_strip_position == 0) {
        fprintf(stderr, "[jv_parse.c] enter jv_parser_set_buf 5\n");
        // no BOM in this document
        p->bom_strip_position = sizeof(UTF8_BOM);
        // fprintf(stderr, "[jv_parse.c] exit jv_parser_set_buf 5\n");
      } else {
        fprintf(stderr, "[jv_parse.c] enter jv_parser_set_buf 6\n");
        // malformed BOM (prefix present, rest missing)
        p->bom_strip_position = 0xff;
        // fprintf(stderr, "[jv_parse.c] exit jv_parser_set_buf 6\n");
      }
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_set_buf 4\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_set_buf 2\n");
  }
  
  fprintf(stderr, "[jv_parse.c] enter jv_parser_set_buf 7\n");
  p->curr_buf = buf;
  p->curr_buf_length = length;
  p->curr_buf_pos = 0;
  p->curr_buf_is_partial = is_partial;
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_set_buf 7\n");
}

static jv make_error(struct jv_parser*, const char *, ...) JV_PRINTF_LIKE(2, 3);

static jv make_error(struct jv_parser* p, const char *fmt, ...) {
  fprintf(stderr, "[jv_parse.c] enter make_error 1\n");
  va_list ap;
  va_start(ap, fmt);
  jv e = jv_string_vfmt(fmt, ap);
  va_end(ap);
  if ((p->flags & JV_PARSE_STREAM_ERRORS))
    return JV_ARRAY(e, jv_copy(p->path));
  return jv_invalid_with_msg(e);
  // fprintf(stderr, "[jv_parse.c] exit make_error 1\n");
}
jv jv_parser_next(struct jv_parser* p) {
  fprintf(stderr, "[jv_parse.c] enter jv_parser_next 1\n");
  if (p->eof)
  {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 2\n");
    return jv_invalid();
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 2\n");
  }
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 1\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parser_next 3\n");
  if (!p->curr_buf)
  {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 4\n");
    return jv_invalid(); // Need a buffer
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 4\n");
  }
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 3\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parser_next 5\n");
  if (p->bom_strip_position == 0xff) {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 6\n");
    if (!(p->flags & JV_PARSE_SEQ))
    {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 7\n");
      return jv_invalid_with_msg(jv_string("Malformed BOM"));
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 7\n");
    }
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 8\n");
    p->st =JV_PARSER_WAITING_FOR_RS;
    parser_reset(p);
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 8\n");
  }
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 6\n");
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 5\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parser_next 9\n");
  jv value = jv_invalid();
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 9\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parser_next 10\n");
  if ((p->flags & JV_PARSE_STREAMING) && stream_check_done(p, &value))
  {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 11\n");
    return value;
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 11\n");
  }
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 10\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parser_next 12\n");
  char ch;
  presult msg = 0;
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 12\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parser_next 13\n");
  while (!msg && p->curr_buf_pos < p->curr_buf_length) {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 14\n");
    ch = p->curr_buf[p->curr_buf_pos++];
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 14\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 15\n");
    if (p->st == JV_PARSER_WAITING_FOR_RS) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 16\n");
      if (ch == '\n') {
        fprintf(stderr, "[jv_parse.c] enter jv_parser_next 17\n");
        p->line++;
        p->column = 0;
        // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 17\n");
      } else {
        fprintf(stderr, "[jv_parse.c] enter jv_parser_next 18\n");
        p->column++;
        // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 18\n");
      }
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 19\n");
      if (ch == '\036')
      {
        fprintf(stderr, "[jv_parse.c] enter jv_parser_next 20\n");
        p->st = JV_PARSER_NORMAL;
        // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 20\n");
      }
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 19\n");
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 21\n");
      continue; // need to resync, wait for RS
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 21\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 16\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 22\n");
    msg = scan(p, ch, &value);
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 22\n");
  }
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 15\n");
  // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 13\n");
  fprintf(stderr, "\n");
  if (msg == OK) {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 24\n");
    return value;
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 24\n");
  } else if (msg) {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 25\n");
    jv_free(value);
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 25\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 26\n");
    if (ch != '\036' && (p->flags & JV_PARSE_SEQ)) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 27\n");
      // Skip to the next RS
      p->st = JV_PARSER_WAITING_FOR_RS;
      value = make_error(p, "%s at line %d, column %d (need RS to resync)", msg, p->line, p->column);
      parser_reset(p);
      return value;
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 27\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 26\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 28\n");
    value = make_error(p, "%s at line %d, column %d", msg, p->line, p->column);
    parser_reset(p);
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 28\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 29\n");
    if (!(p->flags & JV_PARSE_SEQ)) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 30\n");
      // We're not parsing a JSON text sequence; throw this buffer away.
      // XXX We should fail permanently here.
      p->curr_buf = 0;
      p->curr_buf_pos = 0;
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 30\n");
    } // Else ch must be RS; don't clear buf so we can start parsing again after this ch
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 29\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 31\n");
    return value;
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 31\n");
  } else if (p->curr_buf_is_partial) {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 32\n");
    assert(p->curr_buf_pos == p->curr_buf_length);
    // need another buffer
    return jv_invalid();
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 32\n");
  } else {
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 33\n");
    // at EOF
    p->eof = 1;
    assert(p->curr_buf_pos == p->curr_buf_length);
    jv_free(value);
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 33\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 34\n");
    if (p->st == JV_PARSER_WAITING_FOR_RS)
    {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 35\n");
      return make_error(p, "Unfinished abandoned text at EOF at line %d, column %d", p->line, p->column);
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 35\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 34\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 36\n");
    if (p->st != JV_PARSER_NORMAL) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 37\n");
      value = make_error(p, "Unfinished string at EOF at line %d, column %d", p->line, p->column);
      parser_reset(p);
      p->st = JV_PARSER_WAITING_FOR_RS;
      return value;
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 37\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 36\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 38\n");
    if ((msg = check_literal(p))) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 39\n");
      value = make_error(p, "%s at EOF at line %d, column %d", msg, p->line, p->column);
      parser_reset(p);
      p->st = JV_PARSER_WAITING_FOR_RS;
      return value;
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 39\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 38\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 40\n");
    if (((p->flags & JV_PARSE_STREAMING) && p->stacklen != 0) ||
        (!(p->flags & JV_PARSE_STREAMING) && p->stackpos != 0)) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 41\n");
      value = make_error(p, "Unfinished JSON term at EOF at line %d, column %d", p->line, p->column);
      parser_reset(p);
      p->st = JV_PARSER_WAITING_FOR_RS;
      return value;
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 41\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 40\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 42\n");
    // p->next is either invalid (nothing here, but no syntax error)
    // or valid (this is the value). either way it's the thing to return
    if ((p->flags & JV_PARSE_STREAMING) && jv_is_valid(p->next)) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 43\n");
      value = JV_ARRAY(jv_copy(p->path), p->next); // except in streaming mode we've got to make it [path,value]
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 43\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 44\n");
      value = p->next;
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 44\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 42\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 45\n");
    p->next = jv_invalid();
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 45\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 46\n");
    if ((p->flags & JV_PARSE_SEQ) && !p->last_ch_was_ws && jv_get_kind(value) == JV_KIND_NUMBER) {
      fprintf(stderr, "[jv_parse.c] enter jv_parser_next 47\n");
      jv_free(value);
      return make_error(p, "Potentially truncated top-level numeric value at EOF at line %d, column %d", p->line, p->column);
      // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 47\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 46\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parser_next 48\n");
    return value;
    // fprintf(stderr, "[jv_parse.c] exit jv_parser_next 48\n");
  }
}

jv jv_parse_sized_custom_flags(const char* string, int length, int flags) {
  fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 1\n");
  struct jv_parser parser;
  parser_init(&parser, flags);
  jv_parser_set_buf(&parser, string, length, 0);
  jv value = jv_parser_next(&parser);
  // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 1\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 2\n");
  if (jv_is_valid(value)) {
    fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 3\n");
    jv next = jv_parser_next(&parser);
    // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 3\n");
    fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 4\n");
    if (jv_is_valid(next)) {
      fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 5\n");
      // multiple JSON values, we only wanted one
      jv_free(value);
      jv_free(next);
      value = jv_invalid_with_msg(jv_string("Unexpected extra JSON values"));
      // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 5\n");
    } else if (jv_invalid_has_msg(jv_copy(next))) {
      fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 6\n");
      // parser error after the first JSON value
      jv_free(value);
      value = next;
      // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 6\n");
    } else {
      fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 7\n");
      // a single valid JSON value
      jv_free(next);
      // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 7\n");
    }
    // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 4\n");
  } else if (jv_invalid_has_msg(jv_copy(value))) {
    fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 8\n");
    // parse error, we'll return it
    // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 8\n");
  } else {
    fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 9\n");
    // no value at all
    jv_free(value);
    value = jv_invalid_with_msg(jv_string("Expected JSON value"));
    // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 9\n");
  }
  // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 2\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 10\n");
  parser_free(&parser);
  // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 10\n");

  fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 11\n");
  if (!jv_is_valid(value) && jv_invalid_has_msg(jv_copy(value))) {
    fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 12\n");
    jv msg = jv_invalid_get_msg(value);
    value = jv_invalid_with_msg(jv_string_fmt("%s (while parsing '%s')",
                                              jv_string_value(msg),
                                              string));
    jv_free(msg);
    // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 12\n");
  }
  // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 11\n");
  fprintf(stderr, "[jv_parse.c] enter jv_parse_sized_custom_flags 13\n");
  return value;
  // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized_custom_flags 13\n");
}

jv jv_parse_sized(const char* string, int length) {
  fprintf(stderr, "[jv_parse.c] enter jv_parse_sized 1\n");
  return jv_parse_sized_custom_flags(string, length, 0);
  // fprintf(stderr, "[jv_parse.c] exit jv_parse_sized 1\n");
}

jv jv_parse(const char* string) {
  fprintf(stderr, "[jv_parse.c] enter jv_parse 1\n");
  return jv_parse_sized(string, strlen(string));
  // fprintf(stderr, "[jv_parse.c] exit jv_parse 1\n");
}

jv jv_parse_custom_flags(const char* string, int flags) {
  fprintf(stderr, "[jv_parse.c] enter jv_parse_custom_flags 1\n");
  return jv_parse_sized_custom_flags(string, strlen(string), flags);
  // fprintf(stderr, "[jv_parse.c] exit jv_parse_custom_flags 1\n");
}
// Total cost: 0.068996
// Total split cost: 0.006332, input tokens: 20968, output tokens: 610, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 155), (155, 240), (240, 430), (430, 627), (627, 766), (766, 913)]
// Total instrumented cost: 0.062664, input tokens: 83462, output tokens: 36481, cache read tokens: 0, cache write tokens: 0
