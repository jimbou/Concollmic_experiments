/* This file is included (from xmltok.c, 1-3 times depending on XML_MIN_SIZE)!
                            __  __            _
                         ___\ \/ /_ __   __ _| |_
                        / _ \\  /| '_ \ / _` | __|
                       |  __//  \| |_) | (_| | |_
                        \___/_/\_\ .__/ \__,_|\__|
                                 |_| XML parser

   Copyright (c) 1997-2000 Thai Open Source Software Center Ltd
   Copyright (c) 2000      Clark Cooper <coopercc@users.sourceforge.net>
   Copyright (c) 2002      Fred L. Drake, Jr. <fdrake@users.sourceforge.net>
   Copyright (c) 2002-2016 Karl Waclawek <karl@waclawek.net>
   Copyright (c) 2016-2022 Sebastian Pipping <sebastian@pipping.org>
   Copyright (c) 2017      Rhodri James <rhodri@wildebeest.org.uk>
   Copyright (c) 2018      Benjamin Peterson <benjamin@python.org>
   Copyright (c) 2018      Anton Maklakov <antmak.pub@gmail.com>
   Copyright (c) 2019      David Loffredo <loffredo@steptools.com>
   Copyright (c) 2020      Boris Kolpackov <boris@codesynthesis.com>
   Copyright (c) 2022      Martin Ettl <ettl.martin78@googlemail.com>
   Licensed under the MIT license:

   Permission is  hereby granted,  free of charge,  to any  person obtaining
   a  copy  of  this  software   and  associated  documentation  files  (the
   "Software"),  to  deal in  the  Software  without restriction,  including
   without  limitation the  rights  to use,  copy,  modify, merge,  publish,
   distribute, sublicense, and/or sell copies of the Software, and to permit
   persons  to whom  the Software  is  furnished to  do so,  subject to  the
   following conditions:

   The above copyright  notice and this permission notice  shall be included
   in all copies or substantial portions of the Software.

   THE  SOFTWARE  IS  PROVIDED  "AS  IS",  WITHOUT  WARRANTY  OF  ANY  KIND,
   EXPRESS  OR IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO  THE WARRANTIES  OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
   NO EVENT SHALL THE AUTHORS OR  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
   DAMAGES OR  OTHER LIABILITY, WHETHER  IN AN  ACTION OF CONTRACT,  TORT OR
   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
   USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef XML_TOK_IMPL_C

#  ifndef IS_INVALID_CHAR // i.e. for UTF-16 and XML_MIN_SIZE not defined
#    define IS_INVALID_CHAR(enc, ptr, n) (0)
#  endif

#  define INVALID_LEAD_CASE(n, ptr, nextTokPtr)                                \
  case BT_LEAD##n:                                                             \
    if (end - ptr < n)                                                         \
      return XML_TOK_PARTIAL_CHAR;                                             \
    if (IS_INVALID_CHAR(enc, ptr, n)) {                                        \
      *(nextTokPtr) = (ptr);                                                   \
      return XML_TOK_INVALID;                                                  \
    }                                                                          \
    ptr += n;                                                                  \
    break;

#  define INVALID_CASES(ptr, nextTokPtr)                                       \
    INVALID_LEAD_CASE(2, ptr, nextTokPtr)                                      \
    INVALID_LEAD_CASE(3, ptr, nextTokPtr)                                      \
    INVALID_LEAD_CASE(4, ptr, nextTokPtr)                                      \
  case BT_NONXML:                                                              \
  case BT_MALFORM:                                                             \
  case BT_TRAIL:                                                               \
    *(nextTokPtr) = (ptr);                                                     \
    return XML_TOK_INVALID;

#  define CHECK_NAME_CASE(n, enc, ptr, end, nextTokPtr)                        \
  case BT_LEAD##n:                                                             \
    if (end - ptr < n)                                                         \
      return XML_TOK_PARTIAL_CHAR;                                             \
    if (IS_INVALID_CHAR(enc, ptr, n) || ! IS_NAME_CHAR(enc, ptr, n)) {         \
      *nextTokPtr = ptr;                                                       \
      return XML_TOK_INVALID;                                                  \
    }                                                                          \
    ptr += n;                                                                  \
    break;

#  define CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)                          \
  case BT_NONASCII:                                                            \
    if (! IS_NAME_CHAR_MINBPC(enc, ptr)) {                                     \
      *nextTokPtr = ptr;                                                       \
      return XML_TOK_INVALID;                                                  \
    }                                                                          \
    /* fall through */                                                         \
  case BT_NMSTRT:                                                              \
  case BT_HEX:                                                                 \
  case BT_DIGIT:                                                               \
  case BT_NAME:                                                                \
  case BT_MINUS:                                                               \
    ptr += MINBPC(enc);                                                        \
    break;                                                                     \
    CHECK_NAME_CASE(2, enc, ptr, end, nextTokPtr)                              \
    CHECK_NAME_CASE(3, enc, ptr, end, nextTokPtr)                              \
    CHECK_NAME_CASE(4, enc, ptr, end, nextTokPtr)

#  define CHECK_NMSTRT_CASE(n, enc, ptr, end, nextTokPtr)                      \
  case BT_LEAD##n:                                                             \
    if ((end) - (ptr) < (n))                                                   \
      return XML_TOK_PARTIAL_CHAR;                                             \
    if (IS_INVALID_CHAR(enc, ptr, n) || ! IS_NMSTRT_CHAR(enc, ptr, n)) {       \
      *nextTokPtr = ptr;                                                       \
      return XML_TOK_INVALID;                                                  \
    }                                                                          \
    ptr += n;                                                                  \
    break;

#  define CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)                        \
  case BT_NONASCII:                                                            \
    if (! IS_NMSTRT_CHAR_MINBPC(enc, ptr)) {                                   \
      *nextTokPtr = ptr;                                                       \
      return XML_TOK_INVALID;                                                  \
    }                                                                          \
    /* fall through */                                                         \
  case BT_NMSTRT:                                                              \
  case BT_HEX:                                                                 \
    ptr += MINBPC(enc);                                                        \
    break;                                                                     \
    CHECK_NMSTRT_CASE(2, enc, ptr, end, nextTokPtr)                            \
    CHECK_NMSTRT_CASE(3, enc, ptr, end, nextTokPtr)                            \
    CHECK_NMSTRT_CASE(4, enc, ptr, end, nextTokPtr)

#  ifndef PREFIX
#    define PREFIX(ident) ident
#  endif

#  define HAS_CHARS(enc, ptr, end, count)                                      \
    ((end) - (ptr) >= ((count) * MINBPC(enc)))

#  define HAS_CHAR(enc, ptr, end) HAS_CHARS(enc, ptr, end, 1)

#  define REQUIRE_CHARS(enc, ptr, end, count)                                  \
    {                                                                          \
      if (! HAS_CHARS(enc, ptr, end, count)) {                                 \
        return XML_TOK_PARTIAL;                                                \
      }                                                                        \
    }

#  define REQUIRE_CHAR(enc, ptr, end) REQUIRE_CHARS(enc, ptr, end, 1)

/* ptr points to character following "<!-" */

static int PTRCALL
PREFIX(scanComment)(const ENCODING *enc, const char *ptr, const char *end,
                    const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 1\n");
  if (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 2\n");
    if (! CHAR_MATCHES(enc, ptr, ASCII_MINUS)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 3\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 3\n");
    }
    ptr += MINBPC(enc);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 2\n");
    
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 4\n");
    while (HAS_CHAR(enc, ptr, end)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 5\n");
      switch (BYTE_TYPE(enc, ptr)) {
        INVALID_CASES(ptr, nextTokPtr)
      case BT_MINUS:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 6\n");
        ptr += MINBPC(enc);
        REQUIRE_CHAR(enc, ptr, end);
        if (CHAR_MATCHES(enc, ptr, ASCII_MINUS)) {
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 7\n");
          ptr += MINBPC(enc);
          REQUIRE_CHAR(enc, ptr, end);
          if (! CHAR_MATCHES(enc, ptr, ASCII_GT)) {
            fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 8\n");
            *nextTokPtr = ptr;
            return XML_TOK_INVALID;
            // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 8\n");
          }
          *nextTokPtr = ptr + MINBPC(enc);
          return XML_TOK_COMMENT;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 7\n");
        }
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 6\n");
        break;
      default:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanComment) 9\n");
        ptr += MINBPC(enc);
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 9\n");
        break;
      }
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 5\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 4\n");
  }
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanComment) 1\n");
}

/* ptr points to character following "<!" */
static int PTRCALL
PREFIX(scanDecl)(const ENCODING *enc, const char *ptr, const char *end,
                 const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanDecl 1\n");
  REQUIRE_CHAR(enc, ptr, end);
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_MINUS:
    fprintf(stderr, "[xmltok_impl.c] enter scanDecl 2\n");
    return PREFIX(scanComment)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 2\n");
  case BT_LSQB:
    fprintf(stderr, "[xmltok_impl.c] enter scanDecl 3\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_COND_SECT_OPEN;
    // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 3\n");
  case BT_NMSTRT:
  case BT_HEX:
    fprintf(stderr, "[xmltok_impl.c] enter scanDecl 4\n");
    ptr += MINBPC(enc);
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 4\n");
  default:
    fprintf(stderr, "[xmltok_impl.c] enter scanDecl 5\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 5\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanDecl 6\n");
  while (HAS_CHAR(enc, ptr, end)) {
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_PERCNT:
      fprintf(stderr, "[xmltok_impl.c] enter scanDecl 7\n");
      REQUIRE_CHARS(enc, ptr, end, 2);
      /* don't allow <!ENTITY% foo "whatever"> */
      switch (BYTE_TYPE(enc, ptr + MINBPC(enc))) {
      case BT_S:
      case BT_CR:
      case BT_LF:
      case BT_PERCNT:
        fprintf(stderr, "[xmltok_impl.c] enter scanDecl 8\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 8\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 7\n");
      /* fall through */
    case BT_S:
    case BT_CR:
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter scanDecl 9\n");
      *nextTokPtr = ptr;
      return XML_TOK_DECL_OPEN;
      // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 9\n");
    case BT_NMSTRT:
    case BT_HEX:
      fprintf(stderr, "[xmltok_impl.c] enter scanDecl 10\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 10\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanDecl 11\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 11\n");
    }
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanDecl 12\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 12\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 6\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanDecl 1\n");
}

static int PTRCALL
PREFIX(checkPiTarget)(const ENCODING *enc, const char *ptr, const char *end,
                      int *tokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 1\n");
  int upper = 0;
  UNUSED_P(enc);
  *tokPtr = XML_TOK_PI;
  if (end - ptr != MINBPC(enc) * 3) {
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 2\n");
    return 1;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 2\n");
  }
  switch (BYTE_TO_ASCII(enc, ptr)) {
  case ASCII_x:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 3\n");
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 3\n");
  case ASCII_X:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 4\n");
    upper = 1;
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 4\n");
  default:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 5\n");
    return 1;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 5\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 6\n");
  ptr += MINBPC(enc);
  switch (BYTE_TO_ASCII(enc, ptr)) {
  case ASCII_m:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 7\n");
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 7\n");
  case ASCII_M:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 8\n");
    upper = 1;
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 8\n");
  default:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 9\n");
    return 1;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 9\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 10\n");
  ptr += MINBPC(enc);
  switch (BYTE_TO_ASCII(enc, ptr)) {
  case ASCII_l:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 11\n");
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 11\n");
  case ASCII_L:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 12\n");
    upper = 1;
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 12\n");
  default:
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 13\n");
    return 1;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 13\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 14\n");
  if (upper) {
    fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 15\n");
    return 0;
    // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 15\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter checkPiTarget 16\n");
  *tokPtr = XML_TOK_XML_DECL;
  return 1;
  // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 16\n");
  // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 14\n");
  // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 10\n");
  // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 6\n");
  // fprintf(stderr, "[xmltok_impl.c] exit checkPiTarget 1\n");
}

/* ptr points to character following "<?" */

static int PTRCALL
PREFIX(scanPi)(const ENCODING *enc, const char *ptr, const char *end,
               const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanPi 1\n");
  int tok;
  const char *target = ptr;
  REQUIRE_CHAR(enc, ptr, end);
  switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  default:
    fprintf(stderr, "[xmltok_impl.c] enter scanPi 2\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit scanPi 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanPi 3\n");
  while (HAS_CHAR(enc, ptr, end)) {
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_S:
    case BT_CR:
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter scanPi 4\n");
      if (! PREFIX(checkPiTarget)(enc, target, ptr, &tok)) {
        fprintf(stderr, "[xmltok_impl.c] enter scanPi 5\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanPi 5\n");
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanPi 6\n");
      ptr += MINBPC(enc);
      while (HAS_CHAR(enc, ptr, end)) {
        switch (BYTE_TYPE(enc, ptr)) {
          INVALID_CASES(ptr, nextTokPtr)
        case BT_QUEST:
          fprintf(stderr, "[xmltok_impl.c] enter scanPi 7\n");
          ptr += MINBPC(enc);
          REQUIRE_CHAR(enc, ptr, end);
          if (CHAR_MATCHES(enc, ptr, ASCII_GT)) {
            fprintf(stderr, "[xmltok_impl.c] enter scanPi 8\n");
            *nextTokPtr = ptr + MINBPC(enc);
            return tok;
            // fprintf(stderr, "[xmltok_impl.c] exit scanPi 8\n");
          }
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit scanPi 7\n");
        default:
          fprintf(stderr, "[xmltok_impl.c] enter scanPi 9\n");
          ptr += MINBPC(enc);
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit scanPi 9\n");
        }
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanPi 10\n");
      return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok_impl.c] exit scanPi 10\n");
      // fprintf(stderr, "[xmltok_impl.c] exit scanPi 6\n");
      // fprintf(stderr, "[xmltok_impl.c] exit scanPi 4\n");
    case BT_QUEST:
      fprintf(stderr, "[xmltok_impl.c] enter scanPi 11\n");
      if (! PREFIX(checkPiTarget)(enc, target, ptr, &tok)) {
        fprintf(stderr, "[xmltok_impl.c] enter scanPi 12\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanPi 12\n");
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanPi 13\n");
      ptr += MINBPC(enc);
      REQUIRE_CHAR(enc, ptr, end);
      if (CHAR_MATCHES(enc, ptr, ASCII_GT)) {
        fprintf(stderr, "[xmltok_impl.c] enter scanPi 14\n");
        *nextTokPtr = ptr + MINBPC(enc);
        return tok;
        // fprintf(stderr, "[xmltok_impl.c] exit scanPi 14\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit scanPi 13\n");
      /* fall through */
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanPi 15\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanPi 15\n");
      // fprintf(stderr, "[xmltok_impl.c] exit scanPi 11\n");
    }
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanPi 16\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanPi 16\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanPi 3\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanPi 1\n");
}

static int PTRCALL
PREFIX(scanCdataSection)(const ENCODING *enc, const char *ptr, const char *end,
                         const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanCdataSection 1\n");
  static const char CDATA_LSQB[]
      = {ASCII_C, ASCII_D, ASCII_A, ASCII_T, ASCII_A, ASCII_LSQB};
  int i;
  UNUSED_P(enc);
  /* CDATA[ */
  REQUIRE_CHARS(enc, ptr, end, 6);
  for (i = 0; i < 6; i++, ptr += MINBPC(enc)) {
    if (! CHAR_MATCHES(enc, ptr, CDATA_LSQB[i])) {
      fprintf(stderr, "[xmltok_impl.c] enter scanCdataSection 2\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanCdataSection 2\n");
    }
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanCdataSection 3\n");
  *nextTokPtr = ptr;
  return XML_TOK_CDATA_SECT_OPEN;
  // fprintf(stderr, "[xmltok_impl.c] exit scanCdataSection 3\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanCdataSection 1\n");
}
static int PTRCALL
PREFIX(cdataSectionTok)(const ENCODING *enc, const char *ptr, const char *end,
                        const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 1\n");
  if (ptr >= end)
    return XML_TOK_NONE;
  // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 1\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 2\n");
  if (MINBPC(enc) > 1) {
    fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 3\n");
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 4\n");
      n &= ~(MINBPC(enc) - 1);
      if (n == 0)
        return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 4\n");
      
      fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 5\n");
      end = ptr + n;
      // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 5\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 3\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 2\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 6\n");
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_RSQB:
    fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 7\n");
    ptr += MINBPC(enc);
    REQUIRE_CHAR(enc, ptr, end);
    if (! CHAR_MATCHES(enc, ptr, ASCII_RSQB))
      break;
    ptr += MINBPC(enc);
    REQUIRE_CHAR(enc, ptr, end);
    if (! CHAR_MATCHES(enc, ptr, ASCII_GT)) {
      fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 8\n");
      ptr -= MINBPC(enc);
      // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 8\n");
      break;
    }
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_CDATA_SECT_CLOSE;
    // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 7\n");
  case BT_CR:
    fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 9\n");
    ptr += MINBPC(enc);
    REQUIRE_CHAR(enc, ptr, end);
    if (BYTE_TYPE(enc, ptr) == BT_LF)
      ptr += MINBPC(enc);
    *nextTokPtr = ptr;
    return XML_TOK_DATA_NEWLINE;
    // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 9\n");
  case BT_LF:
    fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 10\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_DATA_NEWLINE;
    // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 10\n");
    INVALID_CASES(ptr, nextTokPtr)
  default:
    fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 11\n");
    ptr += MINBPC(enc);
    // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 11\n");
    break;
  }
  // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 6\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 12\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 13\n");
    switch (BYTE_TYPE(enc, ptr)) {
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n:                                                             \
    if (end - ptr < n || IS_INVALID_CHAR(enc, ptr, n)) {                       \
      fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 14\n");                           \
      *nextTokPtr = ptr;                                                       \
      return XML_TOK_DATA_CHARS;                                               \
      /* fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 14\n"); */                      \
    }                                                                          \
    fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 15\n");                             \
    ptr += n;                                                                  \
    /* fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 15\n"); */                        \
    break;
      LEAD_CASE(2)
      LEAD_CASE(3)
      LEAD_CASE(4)
#  undef LEAD_CASE
    case BT_NONXML:
    case BT_MALFORM:
    case BT_TRAIL:
    case BT_CR:
    case BT_LF:
    case BT_RSQB:
      fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 16\n");
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 16\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 17\n");
      ptr += MINBPC(enc);
      // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 17\n");
      break;
    }
    // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 13\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 12\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter cdataSectionTok 18\n");
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
  // fprintf(stderr, "[xmltok_impl.c] exit cdataSectionTok 18\n");
}

/* ptr points to character following "</" */

static int PTRCALL
PREFIX(scanEndTag)(const ENCODING *enc, const char *ptr, const char *end,
                   const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 1\n");
  REQUIRE_CHAR(enc, ptr, end);
  switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  default:
    fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 2\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 2\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 1\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 3\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 4\n");
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_S:
    case BT_CR:
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 5\n");
      for (ptr += MINBPC(enc); HAS_CHAR(enc, ptr, end); ptr += MINBPC(enc)) {
        fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 6\n");
        switch (BYTE_TYPE(enc, ptr)) {
        case BT_S:
        case BT_CR:
        case BT_LF:
          fprintf(stderr, "\n");
          // fprintf(stderr, "\n");
          break;
        case BT_GT:
          fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 8\n");
          *nextTokPtr = ptr + MINBPC(enc);
          return XML_TOK_END_TAG;
          // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 8\n");
        default:
          fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 9\n");
          *nextTokPtr = ptr;
          return XML_TOK_INVALID;
          // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 9\n");
        }
        // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 6\n");
      }
      return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 5\n");
#  ifdef XML_NS
    case BT_COLON:
      fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 10\n");
      /* no need to check qname syntax here,
         since end-tag must match exactly */
      ptr += MINBPC(enc);
      // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 10\n");
      break;
#  endif
    case BT_GT:
      fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 11\n");
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_END_TAG;
      // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 11\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 12\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 12\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 4\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 3\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter scanEndTag 13\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanEndTag 13\n");
}

/* ptr points to character following "&#X" */

static int PTRCALL
PREFIX(scanHexCharRef)(const ENCODING *enc, const char *ptr, const char *end,
                       const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 1\n");
  if (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_DIGIT:
    case BT_HEX:
      fprintf(stderr, "\n");
      // fprintf(stderr, "\n");
      break;
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 4\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 4\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 2\n");
    
    fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 5\n");
    for (ptr += MINBPC(enc); HAS_CHAR(enc, ptr, end); ptr += MINBPC(enc)) {
      fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 6\n");
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_DIGIT:
      case BT_HEX:
        fprintf(stderr, "\n");
        // fprintf(stderr, "\n");
        break;
      case BT_SEMI:
        fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 8\n");
        *nextTokPtr = ptr + MINBPC(enc);
        return XML_TOK_CHAR_REF;
        // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 8\n");
      default:
        fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 9\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 9\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 6\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 5\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 1\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter scanHexCharRef 10\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanHexCharRef 10\n");
}

/* ptr points to character following "&#" */

static int PTRCALL
PREFIX(scanCharRef)(const ENCODING *enc, const char *ptr, const char *end,
                    const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 1\n");
  if (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 2\n");
    if (CHAR_MATCHES(enc, ptr, ASCII_x))
      return PREFIX(scanHexCharRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 2\n");
    
    fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 3\n");
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_DIGIT:
      fprintf(stderr, "\n");
      // fprintf(stderr, "\n");
      break;
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 5\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 5\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 3\n");
    
    fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 6\n");
    for (ptr += MINBPC(enc); HAS_CHAR(enc, ptr, end); ptr += MINBPC(enc)) {
      fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 7\n");
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_DIGIT:
        fprintf(stderr, "\n");
        // fprintf(stderr, "\n");
        break;
      case BT_SEMI:
        fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 9\n");
        *nextTokPtr = ptr + MINBPC(enc);
        return XML_TOK_CHAR_REF;
        // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 9\n");
      default:
        fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 10\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 10\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 7\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 6\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 1\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter scanCharRef 11\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanCharRef 11\n");
}

/* ptr points to character following "&" */
static int PTRCALL
PREFIX(scanRef)(const ENCODING *enc, const char *ptr, const char *end,
                const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanRef 1\n");
  REQUIRE_CHAR(enc, ptr, end);
  // fprintf(stderr, "[xmltok_impl.c] exit scanRef 1\n");
  switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  case BT_NUM:
    fprintf(stderr, "[xmltok_impl.c] enter scanRef 2\n");
    return PREFIX(scanCharRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit scanRef 2\n");
  default:
    fprintf(stderr, "[xmltok_impl.c] enter scanRef 3\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit scanRef 3\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanRef 4\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanRef 5\n");
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_SEMI:
      fprintf(stderr, "[xmltok_impl.c] enter scanRef 6\n");
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_ENTITY_REF;
      // fprintf(stderr, "[xmltok_impl.c] exit scanRef 6\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanRef 7\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanRef 7\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanRef 5\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanRef 8\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanRef 8\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanRef 4\n");
}

/* ptr points to character following first character of attribute name */

static int PTRCALL
PREFIX(scanAtts)(const ENCODING *enc, const char *ptr, const char *end,
                 const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanAtts 1\n");
#  ifdef XML_NS
  int hadColon = 0;
#  endif
  // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 1\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanAtts 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
#  ifdef XML_NS
    case BT_COLON:
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 3\n");
      if (hadColon) {
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 4\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 4\n");
      }
      hadColon = 1;
      ptr += MINBPC(enc);
      REQUIRE_CHAR(enc, ptr, end);
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 3\n");
      switch (BYTE_TYPE(enc, ptr)) {
        CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
      default:
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 5\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 5\n");
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 6\n");
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 6\n");
#  endif
    case BT_S:
    case BT_CR:
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 7\n");
      for (;;) {
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 8\n");
        int t;

        ptr += MINBPC(enc);
        REQUIRE_CHAR(enc, ptr, end);
        t = BYTE_TYPE(enc, ptr);
        if (t == BT_EQUALS)
          break;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 8\n");
        switch (t) {
        case BT_S:
        case BT_LF:
        case BT_CR:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 9\n");
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 9\n");
        default:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 10\n");
          *nextTokPtr = ptr;
          return XML_TOK_INVALID;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 10\n");
        }
        fprintf(stderr, "\n");
        // fprintf(stderr, "\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 7\n");
      /* fall through */
    case BT_EQUALS: {
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 12\n");
      int open;
#  ifdef XML_NS
      hadColon = 0;
#  endif
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 12\n");
      for (;;) {
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 13\n");
        ptr += MINBPC(enc);
        REQUIRE_CHAR(enc, ptr, end);
        open = BYTE_TYPE(enc, ptr);
        if (open == BT_QUOT || open == BT_APOS)
          break;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 13\n");
        switch (open) {
        case BT_S:
        case BT_LF:
        case BT_CR:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 14\n");
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 14\n");
        default:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 15\n");
          *nextTokPtr = ptr;
          return XML_TOK_INVALID;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 15\n");
        }
        fprintf(stderr, "\n");
        // fprintf(stderr, "\n");
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 17\n");
      ptr += MINBPC(enc);
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 17\n");
      /* in attribute value */
      for (;;) {
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 18\n");
        int t;
        REQUIRE_CHAR(enc, ptr, end);
        t = BYTE_TYPE(enc, ptr);
        if (t == open)
          break;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 18\n");
        switch (t) {
          INVALID_CASES(ptr, nextTokPtr)
        case BT_AMP: {
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 19\n");
          int tok = PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, &ptr);
          if (tok <= 0) {
            fprintf(stderr, "[xmltok_impl.c] enter scanAtts 20\n");
            if (tok == XML_TOK_INVALID)
              *nextTokPtr = ptr;
            return tok;
            // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 20\n");
          }
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 19\n");
        }
        case BT_LT:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 21\n");
          *nextTokPtr = ptr;
          return XML_TOK_INVALID;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 21\n");
        default:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 22\n");
          ptr += MINBPC(enc);
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 22\n");
        }
        fprintf(stderr, "\n");
        // fprintf(stderr, "\n");
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 24\n");
      ptr += MINBPC(enc);
      REQUIRE_CHAR(enc, ptr, end);
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 24\n");
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_S:
      case BT_CR:
      case BT_LF:
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 25\n");
        break;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 25\n");
      case BT_SOL:
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 26\n");
        goto sol;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 26\n");
      case BT_GT:
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 27\n");
        goto gt;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 27\n");
      default:
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 28\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 28\n");
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 29\n");
      /* ptr points to closing quote */
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 29\n");
      for (;;) {
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 30\n");
        ptr += MINBPC(enc);
        REQUIRE_CHAR(enc, ptr, end);
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 30\n");
        switch (BYTE_TYPE(enc, ptr)) {
          CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
        case BT_S:
        case BT_CR:
        case BT_LF:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 31\n");
          continue;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 31\n");
        case BT_GT:
        gt:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 32\n");
          *nextTokPtr = ptr + MINBPC(enc);
          return XML_TOK_START_TAG_WITH_ATTS;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 32\n");
        case BT_SOL:
        sol:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 33\n");
          ptr += MINBPC(enc);
          REQUIRE_CHAR(enc, ptr, end);
          if (! CHAR_MATCHES(enc, ptr, ASCII_GT)) {
            fprintf(stderr, "[xmltok_impl.c] enter scanAtts 34\n");
            *nextTokPtr = ptr;
            return XML_TOK_INVALID;
            // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 34\n");
          }
          *nextTokPtr = ptr + MINBPC(enc);
          return XML_TOK_EMPTY_ELEMENT_WITH_ATTS;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 33\n");
        default:
          fprintf(stderr, "[xmltok_impl.c] enter scanAtts 35\n");
          *nextTokPtr = ptr;
          return XML_TOK_INVALID;
          // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 35\n");
        }
        fprintf(stderr, "[xmltok_impl.c] enter scanAtts 36\n");
        break;
        // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 36\n");
      }
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 37\n");
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 37\n");
    }
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanAtts 38\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 38\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanAtts 39\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanAtts 39\n");
}

/* ptr points to character following "<" */
static int PTRCALL
PREFIX(scanLt)(const ENCODING *enc, const char *ptr, const char *end,
               const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 1\n");
#  ifdef XML_NS
  int hadColon;
#  endif
  REQUIRE_CHAR(enc, ptr, end);
  switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  case BT_EXCL:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 2\n");
    ptr += MINBPC(enc);
    REQUIRE_CHAR(enc, ptr, end);
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_MINUS:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 3\n");
      return PREFIX(scanComment)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 3\n");
    case BT_LSQB:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 4\n");
      return PREFIX(scanCdataSection)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 4\n");
    }
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 5\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 5\n");
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 2\n");
  case BT_QUEST:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 6\n");
    return PREFIX(scanPi)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 6\n");
  case BT_SOL:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 7\n");
    return PREFIX(scanEndTag)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 7\n");
  default:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 8\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 8\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 9\n");
#  ifdef XML_NS
  hadColon = 0;
#  endif
  /* we have a start-tag */
  while (HAS_CHAR(enc, ptr, end)) {
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
#  ifdef XML_NS
    case BT_COLON:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 10\n");
      if (hadColon) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 11\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 11\n");
      }
      hadColon = 1;
      ptr += MINBPC(enc);
      REQUIRE_CHAR(enc, ptr, end);
      switch (BYTE_TYPE(enc, ptr)) {
        CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
      default:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 12\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 12\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 10\n");
#  endif
    case BT_S:
    case BT_CR:
    case BT_LF: {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 13\n");
      ptr += MINBPC(enc);
      while (HAS_CHAR(enc, ptr, end)) {
        switch (BYTE_TYPE(enc, ptr)) {
          CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
        case BT_GT:
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 14\n");
          goto gt;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 14\n");
        case BT_SOL:
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 15\n");
          goto sol;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 15\n");
        case BT_S:
        case BT_CR:
        case BT_LF:
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 16\n");
          ptr += MINBPC(enc);
          continue;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 16\n");
        default:
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 17\n");
          *nextTokPtr = ptr;
          return XML_TOK_INVALID;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 17\n");
        }
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 18\n");
        return PREFIX(scanAtts)(enc, ptr, end, nextTokPtr);
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 18\n");
      }
      return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 13\n");
    }
    case BT_GT:
    gt:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 19\n");
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_START_TAG_NO_ATTS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 19\n");
    case BT_SOL:
    sol:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 20\n");
      ptr += MINBPC(enc);
      REQUIRE_CHAR(enc, ptr, end);
      if (! CHAR_MATCHES(enc, ptr, ASCII_GT)) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 21\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 21\n");
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_EMPTY_ELEMENT_NO_ATTS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 20\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(scanLt) 22\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 22\n");
    }
  }
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 9\n");
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(scanLt) 1\n");
}

static int PTRCALL
PREFIX(contentTok)(const ENCODING *enc, const char *ptr, const char *end,
                   const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 1\n");
  if (ptr >= end)
    return XML_TOK_NONE;
  if (MINBPC(enc) > 1) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 2\n");
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 3\n");
      n &= ~(MINBPC(enc) - 1);
      if (n == 0)
        return XML_TOK_PARTIAL;
      end = ptr + n;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 3\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 2\n");
  }
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_LT:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 4\n");
    return PREFIX(scanLt)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 4\n");
  case BT_AMP:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 5\n");
    return PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 5\n");
  case BT_CR:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 6\n");
    ptr += MINBPC(enc);
    if (! HAS_CHAR(enc, ptr, end))
      return XML_TOK_TRAILING_CR;
    if (BYTE_TYPE(enc, ptr) == BT_LF)
      ptr += MINBPC(enc);
    *nextTokPtr = ptr;
    return XML_TOK_DATA_NEWLINE;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 6\n");
  case BT_LF:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 7\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_DATA_NEWLINE;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 7\n");
  case BT_RSQB:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 8\n");
    ptr += MINBPC(enc);
    if (! HAS_CHAR(enc, ptr, end))
      return XML_TOK_TRAILING_RSQB;
    if (! CHAR_MATCHES(enc, ptr, ASCII_RSQB))
      break;
    ptr += MINBPC(enc);
    if (! HAS_CHAR(enc, ptr, end))
      return XML_TOK_TRAILING_RSQB;
    if (! CHAR_MATCHES(enc, ptr, ASCII_GT)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 9\n");
      ptr -= MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 9\n");
    }
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 8\n");
    INVALID_CASES(ptr, nextTokPtr)
  default:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 10\n");
    ptr += MINBPC(enc);
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 10\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 11\n");
  while (HAS_CHAR(enc, ptr, end)) {
    switch (BYTE_TYPE(enc, ptr)) {
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n:                                                             \
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 12_" #n "\n");                   \
    if (end - ptr < n || IS_INVALID_CHAR(enc, ptr, n)) {                       \
      *nextTokPtr = ptr;                                                       \
      return XML_TOK_DATA_CHARS;                                               \
    }                                                                          \
    ptr += n;                                                                  \
    break;                                                                     \
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 12_" #n "\n");
      LEAD_CASE(2)
      LEAD_CASE(3)
      LEAD_CASE(4)
#  undef LEAD_CASE
    case BT_RSQB:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 13\n");
      if (HAS_CHARS(enc, ptr, end, 2)) {
        if (! CHAR_MATCHES(enc, ptr + MINBPC(enc), ASCII_RSQB)) {
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 14\n");
          ptr += MINBPC(enc);
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 14\n");
        }
        if (HAS_CHARS(enc, ptr, end, 3)) {
          if (! CHAR_MATCHES(enc, ptr + 2 * MINBPC(enc), ASCII_GT)) {
            fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 15\n");
            ptr += MINBPC(enc);
            break;
            // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 15\n");
          }
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 16\n");
          *nextTokPtr = ptr + 2 * MINBPC(enc);
          return XML_TOK_INVALID;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 16\n");
        }
      }
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 13\n");
      /* fall through */
    case BT_AMP:
    case BT_LT:
    case BT_NONXML:
    case BT_MALFORM:
    case BT_TRAIL:
    case BT_CR:
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 17\n");
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 17\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 18\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 18\n");
    }
  }
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 11\n");
  
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(contentTok) 19\n");
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 19\n");
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(contentTok) 1\n");
}

/* ptr points to character following "%" */
static int PTRCALL
PREFIX(scanPercent)(const ENCODING *enc, const char *ptr, const char *end,
                    const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanPercent 1\n");
  REQUIRE_CHAR(enc, ptr, end);
  switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  case BT_S:
  case BT_LF:
  case BT_CR:
  case BT_PERCNT:
    fprintf(stderr, "[xmltok_impl.c] enter scanPercent 2\n");
    *nextTokPtr = ptr;
    return XML_TOK_PERCENT;
    // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 2\n");
  default:
    fprintf(stderr, "[xmltok_impl.c] enter scanPercent 3\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 3\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanPercent 4\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanPercent 5\n");
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_SEMI:
      fprintf(stderr, "[xmltok_impl.c] enter scanPercent 6\n");
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_PARAM_ENTITY_REF;
      // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 6\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanPercent 7\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 7\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 5\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanPercent 8\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 8\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 4\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanPercent 1\n");
}

static int PTRCALL
PREFIX(scanPoundName)(const ENCODING *enc, const char *ptr, const char *end,
                      const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanPoundName 1\n");
  REQUIRE_CHAR(enc, ptr, end);
  switch (BYTE_TYPE(enc, ptr)) {
    CHECK_NMSTRT_CASES(enc, ptr, end, nextTokPtr)
  default:
    fprintf(stderr, "[xmltok_impl.c] enter scanPoundName 2\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit scanPoundName 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanPoundName 3\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanPoundName 4\n");
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_CR:
    case BT_LF:
    case BT_S:
    case BT_RPAR:
    case BT_GT:
    case BT_PERCNT:
    case BT_VERBAR:
      fprintf(stderr, "[xmltok_impl.c] enter scanPoundName 5\n");
      *nextTokPtr = ptr;
      return XML_TOK_POUND_NAME;
      // fprintf(stderr, "[xmltok_impl.c] exit scanPoundName 5\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanPoundName 6\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit scanPoundName 6\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanPoundName 4\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanPoundName 7\n");
  return -XML_TOK_POUND_NAME;
  // fprintf(stderr, "[xmltok_impl.c] exit scanPoundName 7\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanPoundName 3\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanPoundName 1\n");
}

static int PTRCALL
PREFIX(scanLit)(int open, const ENCODING *enc, const char *ptr, const char *end,
                const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter scanLit 1\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter scanLit 2\n");
    int t = BYTE_TYPE(enc, ptr);
    switch (t) {
      INVALID_CASES(ptr, nextTokPtr)
    case BT_QUOT:
    case BT_APOS:
      fprintf(stderr, "[xmltok_impl.c] enter scanLit 3\n");
      ptr += MINBPC(enc);
      if (t != open) {
        fprintf(stderr, "[xmltok_impl.c] enter scanLit 4\n");
        break;
        // fprintf(stderr, "[xmltok_impl.c] exit scanLit 4\n");
      }
      if (! HAS_CHAR(enc, ptr, end)) {
        fprintf(stderr, "[xmltok_impl.c] enter scanLit 5\n");
        return -XML_TOK_LITERAL;
        // fprintf(stderr, "[xmltok_impl.c] exit scanLit 5\n");
      }
      *nextTokPtr = ptr;
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_S:
      case BT_CR:
      case BT_LF:
      case BT_GT:
      case BT_PERCNT:
      case BT_LSQB:
        fprintf(stderr, "[xmltok_impl.c] enter scanLit 6\n");
        return XML_TOK_LITERAL;
        // fprintf(stderr, "[xmltok_impl.c] exit scanLit 6\n");
      default:
        fprintf(stderr, "[xmltok_impl.c] enter scanLit 7\n");
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit scanLit 7\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit scanLit 3\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter scanLit 8\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit scanLit 8\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit scanLit 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter scanLit 9\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit scanLit 9\n");
  // fprintf(stderr, "[xmltok_impl.c] exit scanLit 1\n");
}
static int PTRCALL
PREFIX(prologTok)(const ENCODING *enc, const char *ptr, const char *end,
                  const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 1\n");
  int tok;
  if (ptr >= end)
    return XML_TOK_NONE;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 1\n");
  
  if (MINBPC(enc) > 1) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 2\n");
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 3\n");
      n &= ~(MINBPC(enc) - 1);
      if (n == 0)
        return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 3\n");
      end = ptr + n;
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 2\n");
  }
  
  switch (BYTE_TYPE(enc, ptr)) {
  case BT_QUOT:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 4\n");
    return PREFIX(scanLit)(BT_QUOT, enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 4\n");
  case BT_APOS:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 5\n");
    return PREFIX(scanLit)(BT_APOS, enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 5\n");
  case BT_LT: {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 6\n");
    ptr += MINBPC(enc);
    REQUIRE_CHAR(enc, ptr, end);
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_EXCL:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 7\n");
      return PREFIX(scanDecl)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 7\n");
    case BT_QUEST:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 8\n");
      return PREFIX(scanPi)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 8\n");
    case BT_NMSTRT:
    case BT_HEX:
    case BT_NONASCII:
    case BT_LEAD2:
    case BT_LEAD3:
    case BT_LEAD4:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 9\n");
      *nextTokPtr = ptr - MINBPC(enc);
      return XML_TOK_INSTANCE_START;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 9\n");
    }
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 10\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 10\n");
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 6\n");
  }
  case BT_CR:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 11\n");
    if (ptr + MINBPC(enc) == end) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 12\n");
      *nextTokPtr = end;
      /* indicate that this might be part of a CR/LF pair */
      return -XML_TOK_PROLOG_S;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 12\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 11\n");
    /* fall through */
  case BT_S:
  case BT_LF:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 13\n");
    for (;;) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 14\n");
      ptr += MINBPC(enc);
      if (! HAS_CHAR(enc, ptr, end))
        break;
      switch (BYTE_TYPE(enc, ptr)) {
      case BT_S:
      case BT_LF:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 15\n");
        break;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 15\n");
      case BT_CR:
        /* don't split CR/LF pair */
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 16\n");
        if (ptr + MINBPC(enc) != end)
          break;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 16\n");
        /* fall through */
      default:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 17\n");
        *nextTokPtr = ptr;
        return XML_TOK_PROLOG_S;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 17\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 14\n");
    }
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 18\n");
    *nextTokPtr = ptr;
    return XML_TOK_PROLOG_S;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 18\n");
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 13\n");
  case BT_PERCNT:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 19\n");
    return PREFIX(scanPercent)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 19\n");
  case BT_COMMA:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 20\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_COMMA;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 20\n");
  case BT_LSQB:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 21\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_OPEN_BRACKET;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 21\n");
  case BT_RSQB:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 22\n");
    ptr += MINBPC(enc);
    if (! HAS_CHAR(enc, ptr, end))
      return -XML_TOK_CLOSE_BRACKET;
    if (CHAR_MATCHES(enc, ptr, ASCII_RSQB)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 23\n");
      REQUIRE_CHARS(enc, ptr, end, 2);
      if (CHAR_MATCHES(enc, ptr + MINBPC(enc), ASCII_GT)) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 24\n");
        *nextTokPtr = ptr + 2 * MINBPC(enc);
        return XML_TOK_COND_SECT_CLOSE;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 24\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 23\n");
    }
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 25\n");
    *nextTokPtr = ptr;
    return XML_TOK_CLOSE_BRACKET;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 25\n");
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 22\n");
  case BT_LPAR:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 26\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_OPEN_PAREN;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 26\n");
  case BT_RPAR:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 27\n");
    ptr += MINBPC(enc);
    if (! HAS_CHAR(enc, ptr, end))
      return -XML_TOK_CLOSE_PAREN;
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_AST:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 28\n");
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_CLOSE_PAREN_ASTERISK;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 28\n");
    case BT_QUEST:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 29\n");
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_CLOSE_PAREN_QUESTION;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 29\n");
    case BT_PLUS:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 30\n");
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_CLOSE_PAREN_PLUS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 30\n");
    case BT_CR:
    case BT_LF:
    case BT_S:
    case BT_GT:
    case BT_COMMA:
    case BT_VERBAR:
    case BT_RPAR:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 31\n");
      *nextTokPtr = ptr;
      return XML_TOK_CLOSE_PAREN;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 31\n");
    }
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 32\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 32\n");
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 27\n");
  case BT_VERBAR:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 33\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_OR;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 33\n");
  case BT_GT:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 34\n");
    *nextTokPtr = ptr + MINBPC(enc);
    return XML_TOK_DECL_CLOSE;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 34\n");
  case BT_NUM:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 35\n");
    return PREFIX(scanPoundName)(enc, ptr + MINBPC(enc), end, nextTokPtr);
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 35\n");
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n:                                                             \
    fprintf(stderr, "enter PREFIX(prologTok) " #n "0\n"); \
    if (end - ptr < n)                                                         \
      return XML_TOK_PARTIAL_CHAR;                                             \
    if (IS_INVALID_CHAR(enc, ptr, n)) {                                        \
      *nextTokPtr = ptr;                                                       \
      return XML_TOK_INVALID;                                                  \
    }                                                                          \
    if (IS_NMSTRT_CHAR(enc, ptr, n)) {                                         \
      ptr += n;                                                                \
      tok = XML_TOK_NAME;                                                      \
      break;                                                                   \
    }                                                                          \
    if (IS_NAME_CHAR(enc, ptr, n)) {                                           \
      ptr += n;                                                                \
      tok = XML_TOK_NMTOKEN;                                                   \
      break;                                                                   \
    }                                                                          \
    *nextTokPtr = ptr;                                                         \
    return XML_TOK_INVALID;                                                    \
    // fprintf(stderr, "exit PREFIX(prologTok) " #n "0\n");
    LEAD_CASE(2)
    LEAD_CASE(3)
    LEAD_CASE(4)
#  undef LEAD_CASE
  case BT_NMSTRT:
  case BT_HEX:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 36\n");
    tok = XML_TOK_NAME;
    ptr += MINBPC(enc);
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 36\n");
  case BT_DIGIT:
  case BT_NAME:
  case BT_MINUS:
#  ifdef XML_NS
  case BT_COLON:
#  endif
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 37\n");
    tok = XML_TOK_NMTOKEN;
    ptr += MINBPC(enc);
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 37\n");
  case BT_NONASCII:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 38\n");
    if (IS_NMSTRT_CHAR_MINBPC(enc, ptr)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 39\n");
      ptr += MINBPC(enc);
      tok = XML_TOK_NAME;
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 39\n");
    }
    if (IS_NAME_CHAR_MINBPC(enc, ptr)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 40\n");
      ptr += MINBPC(enc);
      tok = XML_TOK_NMTOKEN;
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 40\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 38\n");
    /* fall through */
  default:
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 41\n");
    *nextTokPtr = ptr;
    return XML_TOK_INVALID;
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 41\n");
  }
  
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 42\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 43\n");
    switch (BYTE_TYPE(enc, ptr)) {
      CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
    case BT_GT:
    case BT_RPAR:
    case BT_COMMA:
    case BT_VERBAR:
    case BT_LSQB:
    case BT_PERCNT:
    case BT_S:
    case BT_CR:
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 44\n");
      *nextTokPtr = ptr;
      return tok;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 44\n");
#  ifdef XML_NS
    case BT_COLON:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 45\n");
      ptr += MINBPC(enc);
      switch (tok) {
      case XML_TOK_NAME:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 46\n");
        REQUIRE_CHAR(enc, ptr, end);
        tok = XML_TOK_PREFIXED_NAME;
        switch (BYTE_TYPE(enc, ptr)) {
          CHECK_NAME_CASES(enc, ptr, end, nextTokPtr)
        default:
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 47\n");
          tok = XML_TOK_NMTOKEN;
          break;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 47\n");
        }
        break;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 46\n");
      case XML_TOK_PREFIXED_NAME:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 48\n");
        tok = XML_TOK_NMTOKEN;
        break;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 48\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 45\n");
#  endif
    case BT_PLUS:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 49\n");
      if (tok == XML_TOK_NMTOKEN) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 50\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 50\n");
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_NAME_PLUS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 49\n");
    case BT_AST:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 51\n");
      if (tok == XML_TOK_NMTOKEN) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 52\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 52\n");
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_NAME_ASTERISK;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 51\n");
    case BT_QUEST:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 53\n");
      if (tok == XML_TOK_NMTOKEN) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 54\n");
        *nextTokPtr = ptr;
        return XML_TOK_INVALID;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 54\n");
      }
      *nextTokPtr = ptr + MINBPC(enc);
      return XML_TOK_NAME_QUESTION;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 53\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 55\n");
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 55\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 43\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(prologTok) 56\n");
  return -tok;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 56\n");
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(prologTok) 42\n");
}

static int PTRCALL
PREFIX(attributeValueTok)(const ENCODING *enc, const char *ptr, const char *end,
                          const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 1\n");
  const char *start;
  if (ptr >= end)
    return XML_TOK_NONE;
  else if (! HAS_CHAR(enc, ptr, end)) {
    /* This line cannot be executed.  The incoming data has already
     * been tokenized once, so incomplete characters like this have
     * already been eliminated from the input.  Retaining the paranoia
     * check is still valuable, however.
     */
    return XML_TOK_PARTIAL; /* LCOV_EXCL_LINE */
  }
  start = ptr;
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n:                                                             \
    fprintf(stderr, "enter PREFIX(attributeValueTok) " #n "0\n"); \
    ptr += n; /* NOTE: The encoding has already been validated. */             \
    break; \
    // fprintf(stderr, "exit PREFIX(attributeValueTok) " #n "0\n");
      LEAD_CASE(2)
      LEAD_CASE(3)
      LEAD_CASE(4)
#  undef LEAD_CASE
    case BT_AMP:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 3\n");
      if (ptr == start)
        return PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 3\n");
    case BT_LT:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 4\n");
      /* this is for inside entity references */
      *nextTokPtr = ptr;
      return XML_TOK_INVALID;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 4\n");
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 5\n");
      if (ptr == start) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 6\n");
        *nextTokPtr = ptr + MINBPC(enc);
        return XML_TOK_DATA_NEWLINE;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 6\n");
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 5\n");
    case BT_CR:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 7\n");
      if (ptr == start) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 8\n");
        ptr += MINBPC(enc);
        if (! HAS_CHAR(enc, ptr, end))
          return XML_TOK_TRAILING_CR;
        if (BYTE_TYPE(enc, ptr) == BT_LF)
          ptr += MINBPC(enc);
        *nextTokPtr = ptr;
        return XML_TOK_DATA_NEWLINE;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 8\n");
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 7\n");
    case BT_S:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 9\n");
      if (ptr == start) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 10\n");
        *nextTokPtr = ptr + MINBPC(enc);
        return XML_TOK_ATTRIBUTE_VALUE_S;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 10\n");
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 9\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 11\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 11\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(attributeValueTok) 12\n");
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 12\n");
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(attributeValueTok) 1\n");
}

static int PTRCALL
PREFIX(entityValueTok)(const ENCODING *enc, const char *ptr, const char *end,
                       const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 1\n");
  const char *start;
  if (ptr >= end)
    return XML_TOK_NONE;
  else if (! HAS_CHAR(enc, ptr, end)) {
    /* This line cannot be executed.  The incoming data has already
     * been tokenized once, so incomplete characters like this have
     * already been eliminated from the input.  Retaining the paranoia
     * check is still valuable, however.
     */
    return XML_TOK_PARTIAL; /* LCOV_EXCL_LINE */
  }
  start = ptr;
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n:                                                             \
    fprintf(stderr, "enter PREFIX(entityValueTok) " #n "0\n"); \
    ptr += n; /* NOTE: The encoding has already been validated. */             \
    break; \
    // fprintf(stderr, "exit PREFIX(entityValueTok) " #n "0\n");
      LEAD_CASE(2)
      LEAD_CASE(3)
      LEAD_CASE(4)
#  undef LEAD_CASE
    case BT_AMP:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 3\n");
      if (ptr == start)
        return PREFIX(scanRef)(enc, ptr + MINBPC(enc), end, nextTokPtr);
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 3\n");
    case BT_PERCNT:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 4\n");
      if (ptr == start) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 5\n");
        int tok = PREFIX(scanPercent)(enc, ptr + MINBPC(enc), end, nextTokPtr);
        return (tok == XML_TOK_PERCENT) ? XML_TOK_INVALID : tok;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 5\n");
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 4\n");
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 6\n");
      if (ptr == start) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 7\n");
        *nextTokPtr = ptr + MINBPC(enc);
        return XML_TOK_DATA_NEWLINE;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 7\n");
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 6\n");
    case BT_CR:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 8\n");
      if (ptr == start) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 9\n");
        ptr += MINBPC(enc);
        if (! HAS_CHAR(enc, ptr, end))
          return XML_TOK_TRAILING_CR;
        if (BYTE_TYPE(enc, ptr) == BT_LF)
          ptr += MINBPC(enc);
        *nextTokPtr = ptr;
        return XML_TOK_DATA_NEWLINE;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 9\n");
      }
      *nextTokPtr = ptr;
      return XML_TOK_DATA_CHARS;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 8\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 10\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 10\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(entityValueTok) 11\n");
  *nextTokPtr = ptr;
  return XML_TOK_DATA_CHARS;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 11\n");
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(entityValueTok) 1\n");
}

#  ifdef XML_DTD
static int PTRCALL
PREFIX(ignoreSectionTok)(const ENCODING *enc, const char *ptr, const char *end,
                         const char **nextTokPtr) {
  fprintf(stderr, "\n");
  int level = 0;
  if (MINBPC(enc) > 1) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 2\n");
    size_t n = end - ptr;
    if (n & (MINBPC(enc) - 1)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 3\n");
      n &= ~(MINBPC(enc) - 1);
      end = ptr + n;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 3\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 2\n");
  }
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 4\n");
    switch (BYTE_TYPE(enc, ptr)) {
      INVALID_CASES(ptr, nextTokPtr)
    case BT_LT:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 5\n");
      ptr += MINBPC(enc);
      REQUIRE_CHAR(enc, ptr, end);
      if (CHAR_MATCHES(enc, ptr, ASCII_EXCL)) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 6\n");
        ptr += MINBPC(enc);
        REQUIRE_CHAR(enc, ptr, end);
        if (CHAR_MATCHES(enc, ptr, ASCII_LSQB)) {
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 7\n");
          ++level;
          ptr += MINBPC(enc);
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 7\n");
        }
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 6\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 5\n");
    case BT_RSQB:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 8\n");
      ptr += MINBPC(enc);
      REQUIRE_CHAR(enc, ptr, end);
      if (CHAR_MATCHES(enc, ptr, ASCII_RSQB)) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 9\n");
        ptr += MINBPC(enc);
        REQUIRE_CHAR(enc, ptr, end);
        if (CHAR_MATCHES(enc, ptr, ASCII_GT)) {
          fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 10\n");
          ptr += MINBPC(enc);
          if (level == 0) {
            fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 11\n");
            *nextTokPtr = ptr;
            return XML_TOK_IGNORE_SECT;
            // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 11\n");
          }
          --level;
          // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 10\n");
        }
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 9\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 8\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 12\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 12\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 4\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(ignoreSectionTok) 13\n");
  return XML_TOK_PARTIAL;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(ignoreSectionTok) 13\n");
}

#  endif /* XML_DTD */

static int PTRCALL
PREFIX(isPublicId)(const ENCODING *enc, const char *ptr, const char *end,
                   const char **badPtr) {
  fprintf(stderr, "\n");
  ptr += MINBPC(enc);
  end -= MINBPC(enc);
  for (; HAS_CHAR(enc, ptr, end); ptr += MINBPC(enc)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(isPublicId) 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_DIGIT:
    case BT_HEX:
    case BT_MINUS:
    case BT_APOS:
    case BT_LPAR:
    case BT_RPAR:
    case BT_PLUS:
    case BT_COMMA:
    case BT_SOL:
    case BT_EQUALS:
    case BT_QUEST:
    case BT_CR:
    case BT_LF:
    case BT_SEMI:
    case BT_EXCL:
    case BT_AST:
    case BT_PERCNT:
    case BT_NUM:
#  ifdef XML_NS
    case BT_COLON:
#  endif
      break;
    case BT_S:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(isPublicId) 3\n");
      if (CHAR_MATCHES(enc, ptr, ASCII_TAB)) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(isPublicId) 4\n");
        *badPtr = ptr;
        return 0;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(isPublicId) 4\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(isPublicId) 3\n");
    case BT_NAME:
    case BT_NMSTRT:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(isPublicId) 5\n");
      if (! (BYTE_TO_ASCII(enc, ptr) & ~0x7f))
        break;
      /* fall through */
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(isPublicId) 5\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(isPublicId) 6\n");
      switch (BYTE_TO_ASCII(enc, ptr)) {
      case 0x24: /* $ */
      case 0x40: /* @ */
        break;
      default:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(isPublicId) 7\n");
        *badPtr = ptr;
        return 0;
        // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(isPublicId) 7\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(isPublicId) 6\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(isPublicId) 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(isPublicId) 8\n");
  return 1;
  // fprintf(stderr, "[xmltok_impl.c] exit PREFIX(isPublicId) 8\n");
}

/* This must only be called for a well-formed start-tag or empty
   element tag.  Returns the number of attributes.  Pointers to the
   first attsMax attributes are stored in atts.
*/
static int PTRCALL
PREFIX(getAtts)(const ENCODING *enc, const char *ptr, int attsMax,
                ATTRIBUTE *atts) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 1\n");
  enum { other, inName, inValue } state = inName;
  int nAtts = 0;
  int open = 0; /* defined when state == inValue;
                   initialization just to shut up compilers */

  for (ptr += MINBPC(enc);; ptr += MINBPC(enc)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
#  define START_NAME                                                           \
    if (state == other) {                                                      \
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 3\n");                            \
      if (nAtts < attsMax) {                                                   \
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 4\n");                          \
        atts[nAtts].name = ptr;                                                \
        atts[nAtts].normalized = 1;                                            \
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 4\n"); */                     \
      }                                                                        \
      state = inName;                                                          \
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 3\n"); */                       \
    }
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n: /* NOTE: The encoding has already been validated. */        \
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 5\n");                              \
    START_NAME ptr += (n - MINBPC(enc));                                       \
    break;                                                                     \
    /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 5\n"); */
      LEAD_CASE(2)
      LEAD_CASE(3)
      LEAD_CASE(4)
#  undef LEAD_CASE
    case BT_NONASCII:
    case BT_NMSTRT:
    case BT_HEX:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 6\n");
      START_NAME
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 6\n"); */
#  undef START_NAME
    case BT_QUOT:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 7\n");
      if (state != inValue) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 8\n");
        if (nAtts < attsMax)
          atts[nAtts].valuePtr = ptr + MINBPC(enc);
        state = inValue;
        open = BT_QUOT;
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 8\n"); */
      } else if (open == BT_QUOT) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 9\n");
        state = other;
        if (nAtts < attsMax)
          atts[nAtts].valueEnd = ptr;
        nAtts++;
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 9\n"); */
      }
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 7\n"); */
    case BT_APOS:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 10\n");
      if (state != inValue) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 11\n");
        if (nAtts < attsMax)
          atts[nAtts].valuePtr = ptr + MINBPC(enc);
        state = inValue;
        open = BT_APOS;
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 11\n"); */
      } else if (open == BT_APOS) {
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 12\n");
        state = other;
        if (nAtts < attsMax)
          atts[nAtts].valueEnd = ptr;
        nAtts++;
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 12\n"); */
      }
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 10\n"); */
    case BT_AMP:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 13\n");
      if (nAtts < attsMax)
        atts[nAtts].normalized = 0;
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 13\n"); */
    case BT_S:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 14\n");
      if (state == inName)
        state = other;
      else if (state == inValue && nAtts < attsMax && atts[nAtts].normalized
               && (ptr == atts[nAtts].valuePtr
                   || BYTE_TO_ASCII(enc, ptr) != ASCII_SPACE
                   || BYTE_TO_ASCII(enc, ptr + MINBPC(enc)) == ASCII_SPACE
                   || BYTE_TYPE(enc, ptr + MINBPC(enc)) == open))
        atts[nAtts].normalized = 0;
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 14\n"); */
    case BT_CR:
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 15\n");
      /* This case ensures that the first attribute name is counted
         Apart from that we could just change state on the quote. */
      if (state == inName)
        state = other;
      else if (state == inValue && nAtts < attsMax)
        atts[nAtts].normalized = 0;
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 15\n"); */
    case BT_GT:
    case BT_SOL:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 16\n");
      if (state != inValue)
        return nAtts;
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 16\n"); */
    default:
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(getAtts) 17\n");
      break;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 17\n"); */
    }
    /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 2\n"); */
  }
  /* not reached */
  /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(getAtts) 1\n"); */
}

static int PTRFASTCALL
PREFIX(charRefNumber)(const ENCODING *enc, const char *ptr) {
  fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 1\n");
  int result = 0;
  /* skip &# */
  UNUSED_P(enc);
  ptr += 2 * MINBPC(enc);
  if (CHAR_MATCHES(enc, ptr, ASCII_x)) {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 2\n");
    for (ptr += MINBPC(enc); ! CHAR_MATCHES(enc, ptr, ASCII_SEMI);
         ptr += MINBPC(enc)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 3\n");
      int c = BYTE_TO_ASCII(enc, ptr);
      switch (c) {
      case ASCII_0:
      case ASCII_1:
      case ASCII_2:
      case ASCII_3:
      case ASCII_4:
      case ASCII_5:
      case ASCII_6:
      case ASCII_7:
      case ASCII_8:
      case ASCII_9:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 4\n");
        result <<= 4;
        result |= (c - ASCII_0);
        break;
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 4\n"); */
      case ASCII_A:
      case ASCII_B:
      case ASCII_C:
      case ASCII_D:
      case ASCII_E:
      case ASCII_F:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 5\n");
        result <<= 4;
        result += 10 + (c - ASCII_A);
        break;
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 5\n"); */
      case ASCII_a:
      case ASCII_b:
      case ASCII_c:
      case ASCII_d:
      case ASCII_e:
      case ASCII_f:
        fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 6\n");
        result <<= 4;
        result += 10 + (c - ASCII_a);
        break;
        /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 6\n"); */
      }
      if (result >= 0x110000)
        return -1;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 3\n"); */
    }
    /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 2\n"); */
  } else {
    fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 7\n");
    for (; ! CHAR_MATCHES(enc, ptr, ASCII_SEMI); ptr += MINBPC(enc)) {
      fprintf(stderr, "[xmltok_impl.c] enter PREFIX(charRefNumber) 8\n");
      int c = BYTE_TO_ASCII(enc, ptr);
      result *= 10;
      result += (c - ASCII_0);
      if (result >= 0x110000)
        return -1;
      /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 8\n"); */
    }
    /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 7\n"); */
  }
  return checkCharRefNumber(result);
  /* fprintf(stderr, "[xmltok_impl.c] exit PREFIX(charRefNumber) 1\n"); */
}
static int PTRCALL
PREFIX(predefinedEntityName)(const ENCODING *enc, const char *ptr,
                             const char *end) {
  fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 1\n");
  UNUSED_P(enc);
  switch ((end - ptr) / MINBPC(enc)) {
  case 2:
    fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 2\n");
    if (CHAR_MATCHES(enc, ptr + MINBPC(enc), ASCII_t)) {
      fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 3\n");
      switch (BYTE_TO_ASCII(enc, ptr)) {
      case ASCII_l:
        fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 4\n");
        return ASCII_LT;
        // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 4\n");
      case ASCII_g:
        fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 5\n");
        return ASCII_GT;
        // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 5\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 3\n");
    }
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 2\n");
  case 3:
    fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 6\n");
    if (CHAR_MATCHES(enc, ptr, ASCII_a)) {
      fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 7\n");
      ptr += MINBPC(enc);
      if (CHAR_MATCHES(enc, ptr, ASCII_m)) {
        fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 8\n");
        ptr += MINBPC(enc);
        if (CHAR_MATCHES(enc, ptr, ASCII_p)) {
          fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 9\n");
          return ASCII_AMP;
          // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 9\n");
        }
        // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 8\n");
      }
      // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 7\n");
    }
    break;
    // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 6\n");
  case 4:
    fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 10\n");
    switch (BYTE_TO_ASCII(enc, ptr)) {
    case ASCII_q:
      fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 11\n");
      ptr += MINBPC(enc);
      if (CHAR_MATCHES(enc, ptr, ASCII_u)) {
        fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 12\n");
        ptr += MINBPC(enc);
        if (CHAR_MATCHES(enc, ptr, ASCII_o)) {
          fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 13\n");
          ptr += MINBPC(enc);
          if (CHAR_MATCHES(enc, ptr, ASCII_t)) {
            fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 14\n");
            return ASCII_QUOT;
            // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 14\n");
          }
          // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 13\n");
        }
        // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 12\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 11\n");
    case ASCII_a:
      fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 15\n");
      ptr += MINBPC(enc);
      if (CHAR_MATCHES(enc, ptr, ASCII_p)) {
        fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 16\n");
        ptr += MINBPC(enc);
        if (CHAR_MATCHES(enc, ptr, ASCII_o)) {
          fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 17\n");
          ptr += MINBPC(enc);
          if (CHAR_MATCHES(enc, ptr, ASCII_s)) {
            fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 18\n");
            return ASCII_APOS;
            // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 18\n");
          }
          // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 17\n");
        }
        // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 16\n");
      }
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 15\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 10\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter predefinedEntityName 19\n");
  return 0;
  // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 19\n");
  // fprintf(stderr, "[xmltok_impl.c] exit predefinedEntityName 1\n");
}

static int PTRCALL
PREFIX(nameMatchesAscii)(const ENCODING *enc, const char *ptr1,
                         const char *end1, const char *ptr2) {
  fprintf(stderr, "[xmltok_impl.c] enter nameMatchesAscii 1\n");
  UNUSED_P(enc);
  for (; *ptr2; ptr1 += MINBPC(enc), ptr2++) {
    fprintf(stderr, "[xmltok_impl.c] enter nameMatchesAscii 2\n");
    if (end1 - ptr1 < MINBPC(enc)) {
      fprintf(stderr, "[xmltok_impl.c] enter nameMatchesAscii 3\n");
      /* This line cannot be executed.  The incoming data has already
       * been tokenized once, so incomplete characters like this have
       * already been eliminated from the input.  Retaining the
       * paranoia check is still valuable, however.
       */
      return 0; /* LCOV_EXCL_LINE */
      // fprintf(stderr, "[xmltok_impl.c] exit nameMatchesAscii 3\n");
    }
    if (! CHAR_MATCHES(enc, ptr1, *ptr2)) {
      fprintf(stderr, "[xmltok_impl.c] enter nameMatchesAscii 4\n");
      return 0;
      // fprintf(stderr, "[xmltok_impl.c] exit nameMatchesAscii 4\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit nameMatchesAscii 2\n");
  }
  fprintf(stderr, "[xmltok_impl.c] enter nameMatchesAscii 5\n");
  return ptr1 == end1;
  // fprintf(stderr, "[xmltok_impl.c] exit nameMatchesAscii 5\n");
  // fprintf(stderr, "[xmltok_impl.c] exit nameMatchesAscii 1\n");
}

static int PTRFASTCALL
PREFIX(nameLength)(const ENCODING *enc, const char *ptr) {
  fprintf(stderr, "[xmltok_impl.c] enter nameLength 1\n");
  const char *start = ptr;
  for (;;) {
    fprintf(stderr, "[xmltok_impl.c] enter nameLength 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n:                                                             \
    fprintf(stderr, "[xmltok_impl.c] enter nameLength 3_" #n "\n");                            \
    ptr += n; /* NOTE: The encoding has already been validated. */             \
    break;                                                                     \
    // fprintf(stderr, "[xmltok_impl.c] exit nameLength 3_" #n "\n");
      LEAD_CASE(2)
      LEAD_CASE(3)
      LEAD_CASE(4)
#  undef LEAD_CASE
    case BT_NONASCII:
    case BT_NMSTRT:
#  ifdef XML_NS
    case BT_COLON:
#  endif
    case BT_HEX:
    case BT_DIGIT:
    case BT_NAME:
    case BT_MINUS:
      fprintf(stderr, "[xmltok_impl.c] enter nameLength 4\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit nameLength 4\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter nameLength 5\n");
      return (int)(ptr - start);
      // fprintf(stderr, "[xmltok_impl.c] exit nameLength 5\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit nameLength 2\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit nameLength 1\n");
}

static const char *PTRFASTCALL
PREFIX(skipS)(const ENCODING *enc, const char *ptr) {
  fprintf(stderr, "[xmltok_impl.c] enter skipS 1\n");
  for (;;) {
    fprintf(stderr, "[xmltok_impl.c] enter skipS 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
    case BT_LF:
    case BT_CR:
    case BT_S:
      fprintf(stderr, "[xmltok_impl.c] enter skipS 3\n");
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit skipS 3\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter skipS 4\n");
      return ptr;
      // fprintf(stderr, "[xmltok_impl.c] exit skipS 4\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit skipS 2\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit skipS 1\n");
}

static void PTRCALL
PREFIX(updatePosition)(const ENCODING *enc, const char *ptr, const char *end,
                       POSITION *pos) {
  fprintf(stderr, "[xmltok_impl.c] enter updatePosition 1\n");
  while (HAS_CHAR(enc, ptr, end)) {
    fprintf(stderr, "[xmltok_impl.c] enter updatePosition 2\n");
    switch (BYTE_TYPE(enc, ptr)) {
#  define LEAD_CASE(n)                                                         \
  case BT_LEAD##n:                                                             \
    fprintf(stderr, "[xmltok_impl.c] enter updatePosition 3_" #n "\n");                        \
    ptr += n; /* NOTE: The encoding has already been validated. */             \
    pos->columnNumber++;                                                       \
    break;                                                                     \
    // fprintf(stderr, "[xmltok_impl.c] exit updatePosition 3_" #n "\n");
      LEAD_CASE(2)
      LEAD_CASE(3)
      LEAD_CASE(4)
#  undef LEAD_CASE
    case BT_LF:
      fprintf(stderr, "[xmltok_impl.c] enter updatePosition 4\n");
      pos->columnNumber = 0;
      pos->lineNumber++;
      ptr += MINBPC(enc);
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit updatePosition 4\n");
    case BT_CR:
      fprintf(stderr, "[xmltok_impl.c] enter updatePosition 5\n");
      pos->lineNumber++;
      ptr += MINBPC(enc);
      if (HAS_CHAR(enc, ptr, end) && BYTE_TYPE(enc, ptr) == BT_LF) {
        fprintf(stderr, "[xmltok_impl.c] enter updatePosition 6\n");
        ptr += MINBPC(enc);
        // fprintf(stderr, "[xmltok_impl.c] exit updatePosition 6\n");
      }
      pos->columnNumber = 0;
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit updatePosition 5\n");
    default:
      fprintf(stderr, "[xmltok_impl.c] enter updatePosition 7\n");
      ptr += MINBPC(enc);
      pos->columnNumber++;
      break;
      // fprintf(stderr, "[xmltok_impl.c] exit updatePosition 7\n");
    }
    // fprintf(stderr, "[xmltok_impl.c] exit updatePosition 2\n");
  }
  // fprintf(stderr, "[xmltok_impl.c] exit updatePosition 1\n");
}

#  undef DO_LEAD_CASE
#  undef MULTIBYTE_CASES
#  undef INVALID_CASES
#  undef CHECK_NAME_CASE
#  undef CHECK_NAME_CASES
#  undef CHECK_NMSTRT_CASE
#  undef CHECK_NMSTRT_CASES

#endif /* XML_TOK_IMPL_C */
// Total cost: 1.340661
// Total split cost: 0.119233, input tokens: 63822, output tokens: 1169, cache read tokens: 63810, cache write tokens: 22005, split chunks: [(0, 180), (180, 353), (353, 542), (542, 723), (723, 921), (921, 1015), (1015, 1395), (1395, 1507), (1507, 1658), (1658, 1819)]
// Total instrumented cost: 1.221428, input tokens: 50106, output tokens: 63207, cache read tokens: 50055, cache write tokens: 68841
