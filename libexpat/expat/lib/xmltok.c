/*
                            __  __            _
                         ___\ \/ /_ __   __ _| |_
                        / _ \\  /| '_ \ / _` | __|
                       |  __//  \| |_) | (_| | |_
                        \___/_/\_\ .__/ \__,_|\__|
                                 |_| XML parser

   Copyright (c) 1997-2000 Thai Open Source Software Center Ltd
   Copyright (c) 2000      Clark Cooper <coopercc@users.sourceforge.net>
   Copyright (c) 2001-2003 Fred L. Drake, Jr. <fdrake@users.sourceforge.net>
   Copyright (c) 2002      Greg Stein <gstein@users.sourceforge.net>
   Copyright (c) 2002-2016 Karl Waclawek <karl@waclawek.net>
   Copyright (c) 2005-2009 Steven Solie <steven@solie.ca>
   Copyright (c) 2016-2024 Sebastian Pipping <sebastian@pipping.org>
   Copyright (c) 2016      Pascal Cuoq <cuoq@trust-in-soft.com>
   Copyright (c) 2016      Don Lewis <truckman@apache.org>
   Copyright (c) 2017      Rhodri James <rhodri@wildebeest.org.uk>
   Copyright (c) 2017      Alexander Bluhm <alexander.bluhm@gmx.net>
   Copyright (c) 2017      Benbuck Nason <bnason@netflix.com>
   Copyright (c) 2017      José Gutiérrez de la Concha <jose@zeroc.com>
   Copyright (c) 2019      David Loffredo <loffredo@steptools.com>
   Copyright (c) 2021      Donghee Na <donghee.na@python.org>
   Copyright (c) 2022      Martin Ettl <ettl.martin78@googlemail.com>
   Copyright (c) 2022      Sean McBride <sean@rogue-research.com>
   Copyright (c) 2023      Hanno Böck <hanno@gentoo.org>
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

#include "expat_config.h"

#include <stddef.h>
#include <string.h> /* memcpy */
#include <stdbool.h>
#include <stdio.h>

#ifdef _WIN32
#  include "winconfig.h"
#endif

#include "internal.h"
#include "xmltok.h"
#include "nametab.h"

#ifdef XML_DTD
#  define IGNORE_SECTION_TOK_VTABLE , PREFIX(ignoreSectionTok)
#else
#  define IGNORE_SECTION_TOK_VTABLE /* as nothing */
#endif

#define VTABLE1                                                                \
  {PREFIX(prologTok), PREFIX(contentTok),                                      \
   PREFIX(cdataSectionTok) IGNORE_SECTION_TOK_VTABLE},                         \
      {PREFIX(attributeValueTok), PREFIX(entityValueTok)},                     \
      PREFIX(nameMatchesAscii), PREFIX(nameLength), PREFIX(skipS),             \
      PREFIX(getAtts), PREFIX(charRefNumber), PREFIX(predefinedEntityName),    \
      PREFIX(updatePosition), PREFIX(isPublicId)

#define VTABLE VTABLE1, PREFIX(toUtf8), PREFIX(toUtf16)

#define UCS2_GET_NAMING(pages, hi, lo)                                         \
  (namingBitmap[(pages[hi] << 3) + ((lo) >> 5)] & (1u << ((lo) & 0x1F)))

/* A 2 byte UTF-8 representation splits the characters 11 bits between
   the bottom 5 and 6 bits of the bytes.  We need 8 bits to index into
   pages, 3 bits to add to that index and 5 bits to generate the mask.
*/
#define UTF8_GET_NAMING2(pages, byte)                                          \
  (namingBitmap[((pages)[(((byte)[0]) >> 2) & 7] << 3)                         \
                + ((((byte)[0]) & 3) << 1) + ((((byte)[1]) >> 5) & 1)]         \
   & (1u << (((byte)[1]) & 0x1F)))

/* A 3 byte UTF-8 representation splits the characters 16 bits between
   the bottom 4, 6 and 6 bits of the bytes.  We need 8 bits to index
   into pages, 3 bits to add to that index and 5 bits to generate the
   mask.
*/
#define UTF8_GET_NAMING3(pages, byte)                                          \
  (namingBitmap                                                                \
       [((pages)[((((byte)[0]) & 0xF) << 4) + ((((byte)[1]) >> 2) & 0xF)]      \
         << 3)                                                                 \
        + ((((byte)[1]) & 3) << 1) + ((((byte)[2]) >> 5) & 1)]                 \
   & (1u << (((byte)[2]) & 0x1F)))

/* Detection of invalid UTF-8 sequences is based on Table 3.1B
   of Unicode 3.2: https://www.unicode.org/unicode/reports/tr28/
   with the additional restriction of not allowing the Unicode
   code points 0xFFFF and 0xFFFE (sequences EF,BF,BF and EF,BF,BE).
   Implementation details:
     (A & 0x80) == 0     means A < 0x80
   and
     (A & 0xC0) == 0xC0  means A > 0xBF
*/

#define UTF8_INVALID2(p)                                                       \
  ((*p) < 0xC2 || ((p)[1] & 0x80) == 0 || ((p)[1] & 0xC0) == 0xC0)

#define UTF8_INVALID3(p)                                                       \
  (((p)[2] & 0x80) == 0                                                        \
   || ((*p) == 0xEF && (p)[1] == 0xBF ? (p)[2] > 0xBD                          \
                                      : ((p)[2] & 0xC0) == 0xC0)               \
   || ((*p) == 0xE0                                                            \
           ? (p)[1] < 0xA0 || ((p)[1] & 0xC0) == 0xC0                          \
           : ((p)[1] & 0x80) == 0                                              \
                 || ((*p) == 0xED ? (p)[1] > 0x9F : ((p)[1] & 0xC0) == 0xC0)))

#define UTF8_INVALID4(p)                                                       \
  (((p)[3] & 0x80) == 0 || ((p)[3] & 0xC0) == 0xC0 || ((p)[2] & 0x80) == 0     \
   || ((p)[2] & 0xC0) == 0xC0                                                  \
   || ((*p) == 0xF0                                                            \
           ? (p)[1] < 0x90 || ((p)[1] & 0xC0) == 0xC0                          \
           : ((p)[1] & 0x80) == 0                                              \
                 || ((*p) == 0xF4 ? (p)[1] > 0x8F : ((p)[1] & 0xC0) == 0xC0)))

static int PTRFASTCALL
isNever(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter isNever 1\n");
  UNUSED_P(enc);
  UNUSED_P(p);
  return 0;
  // fprintf(stderr, "[xmltok.c] exit isNever 1\n");
}

static int PTRFASTCALL
utf8_isName2(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter utf8_isName2 1\n");
  UNUSED_P(enc);
  return UTF8_GET_NAMING2(namePages, (const unsigned char *)p);
  // fprintf(stderr, "[xmltok.c] exit utf8_isName2 1\n");
}

static int PTRFASTCALL
utf8_isName3(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter utf8_isName3 1\n");
  UNUSED_P(enc);
  return UTF8_GET_NAMING3(namePages, (const unsigned char *)p);
  // fprintf(stderr, "[xmltok.c] exit utf8_isName3 1\n");
}

#define utf8_isName4 isNever

static int PTRFASTCALL
utf8_isNmstrt2(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter utf8_isNmstrt2 1\n");
  UNUSED_P(enc);
  return UTF8_GET_NAMING2(nmstrtPages, (const unsigned char *)p);
  // fprintf(stderr, "[xmltok.c] exit utf8_isNmstrt2 1\n");
}

static int PTRFASTCALL
utf8_isNmstrt3(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter utf8_isNmstrt3 1\n");
  UNUSED_P(enc);
  return UTF8_GET_NAMING3(nmstrtPages, (const unsigned char *)p);
  // fprintf(stderr, "[xmltok.c] exit utf8_isNmstrt3 1\n");
}

#define utf8_isNmstrt4 isNever

static int PTRFASTCALL
utf8_isInvalid2(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter utf8_isInvalid2 1\n");
  UNUSED_P(enc);
  return UTF8_INVALID2((const unsigned char *)p);
  // fprintf(stderr, "[xmltok.c] exit utf8_isInvalid2 1\n");
}

static int PTRFASTCALL
utf8_isInvalid3(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter utf8_isInvalid3 1\n");
  UNUSED_P(enc);
  return UTF8_INVALID3((const unsigned char *)p);
  // fprintf(stderr, "[xmltok.c] exit utf8_isInvalid3 1\n");
}

static int PTRFASTCALL
utf8_isInvalid4(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter utf8_isInvalid4 1\n");
  UNUSED_P(enc);
  return UTF8_INVALID4((const unsigned char *)p);
  // fprintf(stderr, "[xmltok.c] exit utf8_isInvalid4 1\n");
}

struct normal_encoding {
  ENCODING enc;
  unsigned char type[256];
#ifdef XML_MIN_SIZE
  int(PTRFASTCALL *byteType)(const ENCODING *, const char *);
  int(PTRFASTCALL *isNameMin)(const ENCODING *, const char *);
  int(PTRFASTCALL *isNmstrtMin)(const ENCODING *, const char *);
  int(PTRFASTCALL *byteToAscii)(const ENCODING *, const char *);
  int(PTRCALL *charMatches)(const ENCODING *, const char *, int);
#endif /* XML_MIN_SIZE */
  int(PTRFASTCALL *isName2)(const ENCODING *, const char *);
  int(PTRFASTCALL *isName3)(const ENCODING *, const char *);
  int(PTRFASTCALL *isName4)(const ENCODING *, const char *);
  int(PTRFASTCALL *isNmstrt2)(const ENCODING *, const char *);
  int(PTRFASTCALL *isNmstrt3)(const ENCODING *, const char *);
  int(PTRFASTCALL *isNmstrt4)(const ENCODING *, const char *);
  int(PTRFASTCALL *isInvalid2)(const ENCODING *, const char *);
  int(PTRFASTCALL *isInvalid3)(const ENCODING *, const char *);
  int(PTRFASTCALL *isInvalid4)(const ENCODING *, const char *);
};

#define AS_NORMAL_ENCODING(enc) ((const struct normal_encoding *)(enc))

#ifdef XML_MIN_SIZE

#  define STANDARD_VTABLE(E)                                                   \
    E##byteType, E##isNameMin, E##isNmstrtMin, E##byteToAscii, E##charMatches,

#else

#  define STANDARD_VTABLE(E) /* as nothing */

#endif

#define NORMAL_VTABLE(E)                                                       \
  E##isName2, E##isName3, E##isName4, E##isNmstrt2, E##isNmstrt3,              \
      E##isNmstrt4, E##isInvalid2, E##isInvalid3, E##isInvalid4

#define NULL_VTABLE                                                            \
  /* isName2 */ NULL, /* isName3 */ NULL, /* isName4 */ NULL,                  \
      /* isNmstrt2 */ NULL, /* isNmstrt3 */ NULL, /* isNmstrt4 */ NULL,        \
      /* isInvalid2 */ NULL, /* isInvalid3 */ NULL, /* isInvalid4 */ NULL

static int FASTCALL checkCharRefNumber(int result);

#include "xmltok_impl.h"
#include "ascii.h"

#ifdef XML_MIN_SIZE
#  define sb_isNameMin isNever
#  define sb_isNmstrtMin isNever
#endif

#ifdef XML_MIN_SIZE
#  define MINBPC(enc) ((enc)->minBytesPerChar)
#else
/* minimum bytes per character */
#  define MINBPC(enc) 1
#endif

#define SB_BYTE_TYPE(enc, p)                                                   \
  (((const struct normal_encoding *)(enc))->type[(unsigned char)*(p)])

#ifdef XML_MIN_SIZE
static int PTRFASTCALL
sb_byteType(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter sb_byteType 1\n");
  return SB_BYTE_TYPE(enc, p);
  // fprintf(stderr, "[xmltok.c] exit sb_byteType 1\n");
}
#  define BYTE_TYPE(enc, p) (AS_NORMAL_ENCODING(enc)->byteType(enc, p))
#else
#  define BYTE_TYPE(enc, p) SB_BYTE_TYPE(enc, p)
#endif

#ifdef XML_MIN_SIZE
#  define BYTE_TO_ASCII(enc, p) (AS_NORMAL_ENCODING(enc)->byteToAscii(enc, p))
static int PTRFASTCALL
sb_byteToAscii(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter sb_byteToAscii 1\n");
  UNUSED_P(enc);
  return *p;
  // fprintf(stderr, "[xmltok.c] exit sb_byteToAscii 1\n");
}
#else
#  define BYTE_TO_ASCII(enc, p) (*(p))
#endif

#define IS_NAME_CHAR(enc, p, n) (AS_NORMAL_ENCODING(enc)->isName##n(enc, p))
#define IS_NMSTRT_CHAR(enc, p, n) (AS_NORMAL_ENCODING(enc)->isNmstrt##n(enc, p))
#ifdef XML_MIN_SIZE
#  define IS_INVALID_CHAR(enc, p, n)                                           \
    (AS_NORMAL_ENCODING(enc)->isInvalid##n                                     \
     && AS_NORMAL_ENCODING(enc)->isInvalid##n(enc, p))
#else
#  define IS_INVALID_CHAR(enc, p, n)                                           \
    (AS_NORMAL_ENCODING(enc)->isInvalid##n(enc, p))
#endif

#ifdef XML_MIN_SIZE
#  define IS_NAME_CHAR_MINBPC(enc, p)                                          \
    (AS_NORMAL_ENCODING(enc)->isNameMin(enc, p))
#  define IS_NMSTRT_CHAR_MINBPC(enc, p)                                        \
    (AS_NORMAL_ENCODING(enc)->isNmstrtMin(enc, p))
#else
#  define IS_NAME_CHAR_MINBPC(enc, p) (0)
#  define IS_NMSTRT_CHAR_MINBPC(enc, p) (0)
#endif

#ifdef XML_MIN_SIZE
#  define CHAR_MATCHES(enc, p, c)                                              \
    (AS_NORMAL_ENCODING(enc)->charMatches(enc, p, c))
static int PTRCALL
sb_charMatches(const ENCODING *enc, const char *p, int c) {
  fprintf(stderr, "[xmltok.c] enter sb_charMatches 1\n");
  UNUSED_P(enc);
  return *p == c;
  // fprintf(stderr, "[xmltok.c] exit sb_charMatches 1\n");
}
#else
/* c is an ASCII character */
#  define CHAR_MATCHES(enc, p, c) (*(p) == (c))
#endif

#define PREFIX(ident) normal_##ident
#define XML_TOK_IMPL_C
#include "xmltok_impl.c"
#undef XML_TOK_IMPL_C

#undef MINBPC
#undef BYTE_TYPE
#undef BYTE_TO_ASCII
#undef CHAR_MATCHES
#undef IS_NAME_CHAR
#undef IS_NAME_CHAR_MINBPC
#undef IS_NMSTRT_CHAR
#undef IS_NMSTRT_CHAR_MINBPC
#undef IS_INVALID_CHAR

enum { /* UTF8_cvalN is value of masked first byte of N byte sequence */
       UTF8_cval1 = 0x00,
       UTF8_cval2 = 0xc0,
       UTF8_cval3 = 0xe0,
       UTF8_cval4 = 0xf0
};

void
_INTERNAL_trim_to_complete_utf8_characters(const char *from,
                                           const char **fromLimRef) {
  fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 1\n");
  const char *fromLim = *fromLimRef;
  size_t walked = 0;
  for (; fromLim > from; fromLim--, walked++) {
    fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 2\n");
    const unsigned char prev = (unsigned char)fromLim[-1];
    if ((prev & 0xf8u)
        == 0xf0u) { /* 4-byte character, lead by 0b11110xxx byte */
      fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 3\n");
      if (walked + 1 >= 4) {
        fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 4\n");
        fromLim += 4 - 1;
        break;
        // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 4\n");
      } else {
        fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 5\n");
        walked = 0;
        // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 5\n");
      }
      // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 3\n");
    } else if ((prev & 0xf0u)
               == 0xe0u) { /* 3-byte character, lead by 0b1110xxxx byte */
      fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 6\n");
      if (walked + 1 >= 3) {
        fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 7\n");
        fromLim += 3 - 1;
        break;
        // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 7\n");
      } else {
        fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 8\n");
        walked = 0;
        // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 8\n");
      }
      // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 6\n");
    } else if ((prev & 0xe0u)
               == 0xc0u) { /* 2-byte character, lead by 0b110xxxxx byte */
      fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 9\n");
      if (walked + 1 >= 2) {
        fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 10\n");
        fromLim += 2 - 1;
        break;
        // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 10\n");
      } else {
        fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 11\n");
        walked = 0;
        // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 11\n");
      }
      // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 9\n");
    } else if ((prev & 0x80u)
               == 0x00u) { /* 1-byte character, matching 0b0xxxxxxx */
      fprintf(stderr, "[xmltok.c] enter _INTERNAL_trim_to_complete_utf8_characters 12\n");
      break;
      // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 12\n");
    }
    // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 2\n");
  }
  *fromLimRef = fromLim;
  // fprintf(stderr, "[xmltok.c] exit _INTERNAL_trim_to_complete_utf8_characters 1\n");
}

static enum XML_Convert_Result PTRCALL
utf8_toUtf8(const ENCODING *enc, const char **fromP, const char *fromLim,
            char **toP, const char *toLim) {
  fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 1\n");
  bool input_incomplete = false;
  bool output_exhausted = false;

  /* Avoid copying partial characters (due to limited space). */
  const ptrdiff_t bytesAvailable = fromLim - *fromP;
  const ptrdiff_t bytesStorable = toLim - *toP;
  UNUSED_P(enc);
  if (bytesAvailable > bytesStorable) {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 2\n");
    fromLim = *fromP + bytesStorable;
    output_exhausted = true;
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 2\n");
  }

  /* Avoid copying partial characters (from incomplete input). */
  {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 3\n");
    const char *const fromLimBefore = fromLim;
    _INTERNAL_trim_to_complete_utf8_characters(*fromP, &fromLim);
    if (fromLim < fromLimBefore) {
      fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 4\n");
      input_incomplete = true;
      // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 4\n");
    }
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 3\n");
  }

  {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 5\n");
    const ptrdiff_t bytesToCopy = fromLim - *fromP;
    memcpy(*toP, *fromP, bytesToCopy);
    *fromP += bytesToCopy;
    *toP += bytesToCopy;
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 5\n");
  }

  if (output_exhausted) /* needs to go first */
  {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 6\n");
    return XML_CONVERT_OUTPUT_EXHAUSTED;
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 6\n");
  }
  else if (input_incomplete)
  {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 7\n");
    return XML_CONVERT_INPUT_INCOMPLETE;
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 7\n");
  }
  else
  {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf8 8\n");
    return XML_CONVERT_COMPLETED;
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 8\n");
  }
  // fprintf(stderr, "[xmltok.c] exit utf8_toUtf8 1\n");
}

static enum XML_Convert_Result PTRCALL
utf8_toUtf16(const ENCODING *enc, const char **fromP, const char *fromLim,
             unsigned short **toP, const unsigned short *toLim) {
  fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 1\n");
  enum XML_Convert_Result res = XML_CONVERT_COMPLETED;
  unsigned short *to = *toP;
  const char *from = *fromP;
  while (from < fromLim && to < toLim) {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 2\n");
    switch (SB_BYTE_TYPE(enc, from)) {
    case BT_LEAD2:
      fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 3\n");
      if (fromLim - from < 2) {
        fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 4\n");
        res = XML_CONVERT_INPUT_INCOMPLETE;
        goto after;
        // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 4\n");
      }
      *to++ = (unsigned short)(((from[0] & 0x1f) << 6) | (from[1] & 0x3f));
      from += 2;
      // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 3\n");
      break;
    case BT_LEAD3:
      fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 5\n");
      if (fromLim - from < 3) {
        fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 6\n");
        res = XML_CONVERT_INPUT_INCOMPLETE;
        goto after;
        // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 6\n");
      }
      *to++ = (unsigned short)(((from[0] & 0xf) << 12) | ((from[1] & 0x3f) << 6)
                               | (from[2] & 0x3f));
      from += 3;
      // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 5\n");
      break;
    case BT_LEAD4: {
      fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 7\n");
      unsigned long n;
      if (toLim - to < 2) {
        fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 8\n");
        res = XML_CONVERT_OUTPUT_EXHAUSTED;
        goto after;
        // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 8\n");
      }
      if (fromLim - from < 4) {
        fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 9\n");
        res = XML_CONVERT_INPUT_INCOMPLETE;
        goto after;
        // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 9\n");
      }
      n = ((from[0] & 0x7) << 18) | ((from[1] & 0x3f) << 12)
          | ((from[2] & 0x3f) << 6) | (from[3] & 0x3f);
      n -= 0x10000;
      to[0] = (unsigned short)((n >> 10) | 0xD800);
      to[1] = (unsigned short)((n & 0x3FF) | 0xDC00);
      to += 2;
      from += 4;
      // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 7\n");
    } break;
    default:
      fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 10\n");
      *to++ = *from++;
      // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 10\n");
      break;
    }
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 2\n");
  }
  if (from < fromLim)
  {
    fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 11\n");
    res = XML_CONVERT_OUTPUT_EXHAUSTED;
    // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 11\n");
  }
after:
  fprintf(stderr, "[xmltok.c] enter utf8_toUtf16 12\n");
  *fromP = from;
  *toP = to;
  return res;
  // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 12\n");
  // fprintf(stderr, "[xmltok.c] exit utf8_toUtf16 1\n");
}
static int PTRFASTCALL
unicode_byte_type(char hi, char lo) {
  fprintf(stderr, "\n");
  switch ((unsigned char)hi) {
  /* 0xD800-0xDBFF first 16-bit code unit or high surrogate (W1) */
  case 0xD8:
  case 0xD9:
  case 0xDA:
  case 0xDB:
    fprintf(stderr, "[xmltok.c] enter unicode_byte_type 2\n");
    return BT_LEAD4;
    // fprintf(stderr, "[xmltok.c] exit unicode_byte_type 2\n");
  /* 0xDC00-0xDFFF second 16-bit code unit or low surrogate (W2) */
  case 0xDC:
  case 0xDD:
  case 0xDE:
  case 0xDF:
    fprintf(stderr, "[xmltok.c] enter unicode_byte_type 3\n");
    return BT_TRAIL;
    // fprintf(stderr, "[xmltok.c] exit unicode_byte_type 3\n");
  case 0xFF:
    fprintf(stderr, "[xmltok.c] enter unicode_byte_type 4\n");
    switch ((unsigned char)lo) {
    case 0xFF: /* noncharacter-FFFF */
    case 0xFE: /* noncharacter-FFFE */
      fprintf(stderr, "[xmltok.c] enter unicode_byte_type 5\n");
      return BT_NONXML;
      // fprintf(stderr, "[xmltok.c] exit unicode_byte_type 5\n");
    }
    // fprintf(stderr, "[xmltok.c] exit unicode_byte_type 4\n");
    break;
  }
  fprintf(stderr, "[xmltok.c] enter unicode_byte_type 6\n");
  return BT_NONASCII;
  // fprintf(stderr, "[xmltok.c] exit unicode_byte_type 6\n");
}

#define DEFINE_UTF16_TO_UTF8(E)                                                \
  static enum XML_Convert_Result PTRCALL E##toUtf8(                            \
      const ENCODING *enc, const char **fromP, const char *fromLim,            \
      char **toP, const char *toLim) {                                         \
    fprintf(stderr, "enter " #E "toUtf8 1\n");                                 \
    const char *from = *fromP;                                                 \
    UNUSED_P(enc);                                                             \
    fromLim = from + (((fromLim - from) >> 1) << 1); /* shrink to even */      \
    for (; from < fromLim; from += 2) {                                        \
      fprintf(stderr, "enter " #E "toUtf8 2\n");                               \
      int plane;                                                               \
      unsigned char lo2;                                                       \
      unsigned char lo = GET_LO(from);                                         \
      unsigned char hi = GET_HI(from);                                         \
      switch (hi) {                                                            \
      case 0:                                                                  \
        fprintf(stderr, "enter " #E "toUtf8 3\n");                             \
        if (lo < 0x80) {                                                       \
          fprintf(stderr, "enter " #E "toUtf8 4\n");                           \
          if (*toP == toLim) {                                                 \
            fprintf(stderr, "enter " #E "toUtf8 5\n");                         \
            *fromP = from;                                                     \
            return XML_CONVERT_OUTPUT_EXHAUSTED;                               \
            /* fprintf(stderr, "exit " #E "toUtf8 5\n"); */                    \
          }                                                                    \
          *(*toP)++ = lo;                                                      \
          /* fprintf(stderr, "exit " #E "toUtf8 4\n"); */                      \
          break;                                                               \
        }                                                                      \
        /* fall through */                                                     \
        /* fprintf(stderr, "exit " #E "toUtf8 3\n"); */                        \
      case 0x1:                                                                \
      case 0x2:                                                                \
      case 0x3:                                                                \
      case 0x4:                                                                \
      case 0x5:                                                                \
      case 0x6:                                                                \
      case 0x7:                                                                \
        fprintf(stderr, "enter " #E "toUtf8 6\n");                             \
        if (toLim - *toP < 2) {                                                \
          fprintf(stderr, "enter " #E "toUtf8 7\n");                           \
          *fromP = from;                                                       \
          return XML_CONVERT_OUTPUT_EXHAUSTED;                                 \
          /* fprintf(stderr, "exit " #E "toUtf8 7\n"); */                      \
        }                                                                      \
        *(*toP)++ = ((lo >> 6) | (hi << 2) | UTF8_cval2);                      \
        *(*toP)++ = ((lo & 0x3f) | 0x80);                                      \
        /* fprintf(stderr, "exit " #E "toUtf8 6\n"); */                        \
        break;                                                                 \
      default:                                                                 \
        fprintf(stderr, "enter " #E "toUtf8 8\n");                             \
        if (toLim - *toP < 3) {                                                \
          fprintf(stderr, "enter " #E "toUtf8 9\n");                           \
          *fromP = from;                                                       \
          return XML_CONVERT_OUTPUT_EXHAUSTED;                                 \
          /* fprintf(stderr, "exit " #E "toUtf8 9\n"); */                      \
        }                                                                      \
        /* 16 bits divided 4, 6, 6 amongst 3 bytes */                          \
        *(*toP)++ = ((hi >> 4) | UTF8_cval3);                                  \
        *(*toP)++ = (((hi & 0xf) << 2) | (lo >> 6) | 0x80);                    \
        *(*toP)++ = ((lo & 0x3f) | 0x80);                                      \
        /* fprintf(stderr, "exit " #E "toUtf8 8\n"); */                        \
        break;                                                                 \
      case 0xD8:                                                               \
      case 0xD9:                                                               \
      case 0xDA:                                                               \
      case 0xDB:                                                               \
        fprintf(stderr, "enter " #E "toUtf8 10\n");                            \
        if (toLim - *toP < 4) {                                                \
          fprintf(stderr, "enter " #E "toUtf8 11\n");                          \
          *fromP = from;                                                       \
          return XML_CONVERT_OUTPUT_EXHAUSTED;                                 \
          /* fprintf(stderr, "exit " #E "toUtf8 11\n"); */                     \
        }                                                                      \
        if (fromLim - from < 4) {                                              \
          fprintf(stderr, "enter " #E "toUtf8 12\n");                          \
          *fromP = from;                                                       \
          return XML_CONVERT_INPUT_INCOMPLETE;                                 \
          /* fprintf(stderr, "exit " #E "toUtf8 12\n"); */                     \
        }                                                                      \
        plane = (((hi & 0x3) << 2) | ((lo >> 6) & 0x3)) + 1;                   \
        *(*toP)++ = (char)((plane >> 2) | UTF8_cval4);                         \
        *(*toP)++ = (((lo >> 2) & 0xF) | ((plane & 0x3) << 4) | 0x80);         \
        from += 2;                                                             \
        lo2 = GET_LO(from);                                                    \
        *(*toP)++ = (((lo & 0x3) << 4) | ((GET_HI(from) & 0x3) << 2)           \
                     | (lo2 >> 6) | 0x80);                                     \
        *(*toP)++ = ((lo2 & 0x3f) | 0x80);                                     \
        /* fprintf(stderr, "exit " #E "toUtf8 10\n"); */                       \
        break;                                                                 \
      }                                                                        \
      /* fprintf(stderr, "exit " #E "toUtf8 2\n"); */                          \
    }                                                                          \
    *fromP = from;                                                             \
    if (from < fromLim) {                                                      \
      fprintf(stderr, "enter " #E "toUtf8 13\n");                              \
      return XML_CONVERT_INPUT_INCOMPLETE;                                     \
      /* fprintf(stderr, "exit " #E "toUtf8 13\n"); */                         \
    }                                                                          \
    else {                                                                     \
      fprintf(stderr, "enter " #E "toUtf8 14\n");                              \
      return XML_CONVERT_COMPLETED;                                            \
      /* fprintf(stderr, "exit " #E "toUtf8 14\n"); */                         \
    }                                                                          \
    /* fprintf(stderr, "exit " #E "toUtf8 1\n"); */                            \
  }

#define DEFINE_UTF16_TO_UTF16(E)                                               \
  static enum XML_Convert_Result PTRCALL E##toUtf16(                           \
      const ENCODING *enc, const char **fromP, const char *fromLim,            \
      unsigned short **toP, const unsigned short *toLim) {                     \
    fprintf(stderr, "enter " #E "toUtf16 1\n");                                \
    enum XML_Convert_Result res = XML_CONVERT_COMPLETED;                       \
    UNUSED_P(enc);                                                             \
    fromLim = *fromP + (((fromLim - *fromP) >> 1) << 1); /* shrink to even */  \
    /* Avoid copying first half only of surrogate */                           \
    if (fromLim - *fromP > ((toLim - *toP) << 1)                               \
        && (GET_HI(fromLim - 2) & 0xF8) == 0xD8) {                             \
      fprintf(stderr, "enter " #E "toUtf16 2\n");                              \
      fromLim -= 2;                                                            \
      res = XML_CONVERT_INPUT_INCOMPLETE;                                      \
      /* fprintf(stderr, "exit " #E "toUtf16 2\n"); */                         \
    }                                                                          \
    for (; *fromP < fromLim && *toP < toLim; *fromP += 2) {                    \
      fprintf(stderr, "enter " #E "toUtf16 3\n");                              \
      *(*toP)++ = (GET_HI(*fromP) << 8) | GET_LO(*fromP);                      \
      /* fprintf(stderr, "exit " #E "toUtf16 3\n"); */                         \
    }                                                                          \
    if ((*toP == toLim) && (*fromP < fromLim)) {                               \
      fprintf(stderr, "enter " #E "toUtf16 4\n");                              \
      return XML_CONVERT_OUTPUT_EXHAUSTED;                                     \
      /* fprintf(stderr, "exit " #E "toUtf16 4\n"); */                         \
    }                                                                          \
    else {                                                                     \
      fprintf(stderr, "enter " #E "toUtf16 5\n");                              \
      return res;                                                              \
      /* fprintf(stderr, "exit " #E "toUtf16 5\n"); */                         \
    }                                                                          \
    /* fprintf(stderr, "exit " #E "toUtf16 1\n"); */                           \
  }

#define GET_LO(ptr) ((unsigned char)(ptr)[0])
#define GET_HI(ptr) ((unsigned char)(ptr)[1])

DEFINE_UTF16_TO_UTF8(little2_)
DEFINE_UTF16_TO_UTF16(little2_)

#undef GET_LO
#undef GET_HI

#define GET_LO(ptr) ((unsigned char)(ptr)[1])
#define GET_HI(ptr) ((unsigned char)(ptr)[0])

DEFINE_UTF16_TO_UTF8(big2_)
DEFINE_UTF16_TO_UTF16(big2_)

#undef GET_LO
#undef GET_HI

#define LITTLE2_BYTE_TYPE(enc, p)                                              \
  ((p)[1] == 0 ? SB_BYTE_TYPE(enc, p) : unicode_byte_type((p)[1], (p)[0]))
#define LITTLE2_BYTE_TO_ASCII(p) ((p)[1] == 0 ? (p)[0] : -1)
#define LITTLE2_CHAR_MATCHES(p, c) ((p)[1] == 0 && (p)[0] == (c))
#define LITTLE2_IS_NAME_CHAR_MINBPC(p)                                         \
  UCS2_GET_NAMING(namePages, (unsigned char)p[1], (unsigned char)p[0])
#define LITTLE2_IS_NMSTRT_CHAR_MINBPC(p)                                       \
  UCS2_GET_NAMING(nmstrtPages, (unsigned char)p[1], (unsigned char)p[0])

#ifdef XML_MIN_SIZE

static int PTRFASTCALL
little2_byteType(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter little2_byteType 1\n");
  return LITTLE2_BYTE_TYPE(enc, p);
  // fprintf(stderr, "[xmltok.c] exit little2_byteType 1\n");
}

static int PTRFASTCALL
little2_byteToAscii(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter little2_byteToAscii 1\n");
  UNUSED_P(enc);
  return LITTLE2_BYTE_TO_ASCII(p);
  // fprintf(stderr, "[xmltok.c] exit little2_byteToAscii 1\n");
}

static int PTRCALL
little2_charMatches(const ENCODING *enc, const char *p, int c) {
  fprintf(stderr, "[xmltok.c] enter little2_charMatches 1\n");
  UNUSED_P(enc);
  return LITTLE2_CHAR_MATCHES(p, c);
  // fprintf(stderr, "[xmltok.c] exit little2_charMatches 1\n");
}

static int PTRFASTCALL
little2_isNameMin(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter little2_isNameMin 1\n");
  UNUSED_P(enc);
  return LITTLE2_IS_NAME_CHAR_MINBPC(p);
  // fprintf(stderr, "[xmltok.c] exit little2_isNameMin 1\n");
}

static int PTRFASTCALL
little2_isNmstrtMin(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter little2_isNmstrtMin 1\n");
  UNUSED_P(enc);
  return LITTLE2_IS_NMSTRT_CHAR_MINBPC(p);
  // fprintf(stderr, "[xmltok.c] exit little2_isNmstrtMin 1\n");
}

#  undef VTABLE
#  define VTABLE VTABLE1, little2_toUtf8, little2_toUtf16

#else /* not XML_MIN_SIZE */

#  undef PREFIX
#  define PREFIX(ident) little2_##ident
#  define MINBPC(enc) 2
/* CHAR_MATCHES is guaranteed to have MINBPC bytes available. */
#  define BYTE_TYPE(enc, p) LITTLE2_BYTE_TYPE(enc, p)
#  define BYTE_TO_ASCII(enc, p) LITTLE2_BYTE_TO_ASCII(p)
#  define CHAR_MATCHES(enc, p, c) LITTLE2_CHAR_MATCHES(p, c)
#  define IS_NAME_CHAR(enc, p, n) 0
#  define IS_NAME_CHAR_MINBPC(enc, p) LITTLE2_IS_NAME_CHAR_MINBPC(p)
#  define IS_NMSTRT_CHAR(enc, p, n) (0)
#  define IS_NMSTRT_CHAR_MINBPC(enc, p) LITTLE2_IS_NMSTRT_CHAR_MINBPC(p)

#  define XML_TOK_IMPL_C
#  include "xmltok_impl.c"
#  undef XML_TOK_IMPL_C

#  undef MINBPC
#  undef BYTE_TYPE
#  undef BYTE_TO_ASCII
#  undef CHAR_MATCHES
#  undef IS_NAME_CHAR
#  undef IS_NAME_CHAR_MINBPC
#  undef IS_NMSTRT_CHAR
#  undef IS_NMSTRT_CHAR_MINBPC
#  undef IS_INVALID_CHAR

#endif /* not XML_MIN_SIZE */

#ifdef XML_NS

static const struct normal_encoding little2_encoding_ns
    = {{VTABLE, 2, 0,
#  if BYTEORDER == 1234
        1
#  else
        0
#  endif
       },
       {
#  include "asciitab.h"
#  include "latin1tab.h"
       },
       STANDARD_VTABLE(little2_) NULL_VTABLE};

#endif

static const struct normal_encoding little2_encoding
    = {{VTABLE, 2, 0,
#if BYTEORDER == 1234
        1
#else
        0
#endif
       },
       {
#define BT_COLON BT_NMSTRT
#include "asciitab.h"
#undef BT_COLON
#include "latin1tab.h"
       },
       STANDARD_VTABLE(little2_) NULL_VTABLE};

#if BYTEORDER != 4321

#  ifdef XML_NS

static const struct normal_encoding internal_little2_encoding_ns
    = {{VTABLE, 2, 0, 1},
       {
#    include "iasciitab.h"
#    include "latin1tab.h"
       },
       STANDARD_VTABLE(little2_) NULL_VTABLE};

#  endif

static const struct normal_encoding internal_little2_encoding
    = {{VTABLE, 2, 0, 1},
       {
#  define BT_COLON BT_NMSTRT
#  include "iasciitab.h"
#  undef BT_COLON
#  include "latin1tab.h"
       },
       STANDARD_VTABLE(little2_) NULL_VTABLE};

#endif

#define BIG2_BYTE_TYPE(enc, p)                                                 \
  ((p)[0] == 0 ? SB_BYTE_TYPE(enc, p + 1) : unicode_byte_type((p)[0], (p)[1]))
#define BIG2_BYTE_TO_ASCII(p) ((p)[0] == 0 ? (p)[1] : -1)
#define BIG2_CHAR_MATCHES(p, c) ((p)[0] == 0 && (p)[1] == (c))
#define BIG2_IS_NAME_CHAR_MINBPC(p)                                            \
  UCS2_GET_NAMING(namePages, (unsigned char)p[0], (unsigned char)p[1])
#define BIG2_IS_NMSTRT_CHAR_MINBPC(p)                                          \
  UCS2_GET_NAMING(nmstrtPages, (unsigned char)p[0], (unsigned char)p[1])

#ifdef XML_MIN_SIZE

static int PTRFASTCALL
big2_byteType(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter big2_byteType 1\n");
  return BIG2_BYTE_TYPE(enc, p);
  // fprintf(stderr, "[xmltok.c] exit big2_byteType 1\n");
}

static int PTRFASTCALL
big2_byteToAscii(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter big2_byteToAscii 1\n");
  UNUSED_P(enc);
  return BIG2_BYTE_TO_ASCII(p);
  // fprintf(stderr, "[xmltok.c] exit big2_byteToAscii 1\n");
}

static int PTRCALL
big2_charMatches(const ENCODING *enc, const char *p, int c) {
  fprintf(stderr, "[xmltok.c] enter big2_charMatches 1\n");
  UNUSED_P(enc);
  return BIG2_CHAR_MATCHES(p, c);
  // fprintf(stderr, "[xmltok.c] exit big2_charMatches 1\n");
}

static int PTRFASTCALL
big2_isNameMin(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter big2_isNameMin 1\n");
  UNUSED_P(enc);
  return BIG2_IS_NAME_CHAR_MINBPC(p);
  // fprintf(stderr, "[xmltok.c] exit big2_isNameMin 1\n");
}

static int PTRFASTCALL
big2_isNmstrtMin(const ENCODING *enc, const char *p) {
  fprintf(stderr, "[xmltok.c] enter big2_isNmstrtMin 1\n");
  UNUSED_P(enc);
  return BIG2_IS_NMSTRT_CHAR_MINBPC(p);
  // fprintf(stderr, "[xmltok.c] exit big2_isNmstrtMin 1\n");
}

#  undef VTABLE
#  define VTABLE VTABLE1, big2_toUtf8, big2_toUtf16

#else /* not XML_MIN_SIZE */

#  undef PREFIX
#  define PREFIX(ident) big2_##ident
#  define MINBPC(enc) 2
/* CHAR_MATCHES is guaranteed to have MINBPC bytes available. */
#  define BYTE_TYPE(enc, p) BIG2_BYTE_TYPE(enc, p)
#  define BYTE_TO_ASCII(enc, p) BIG2_BYTE_TO_ASCII(p)
#  define CHAR_MATCHES(enc, p, c) BIG2_CHAR_MATCHES(p, c)
#  define IS_NAME_CHAR(enc, p, n) 0
#  define IS_NAME_CHAR_MINBPC(enc, p) BIG2_IS_NAME_CHAR_MINBPC(p)
#  define IS_NMSTRT_CHAR(enc, p, n) (0)
#  define IS_NMSTRT_CHAR_MINBPC(enc, p) BIG2_IS_NMSTRT_CHAR_MINBPC(p)

#  define XML_TOK_IMPL_C
#  include "xmltok_impl.c"
#  undef XML_TOK_IMPL_C

#  undef MINBPC
#  undef BYTE_TYPE
#  undef BYTE_TO_ASCII
#  undef CHAR_MATCHES
#  undef IS_NAME_CHAR
#  undef IS_NAME_CHAR_MINBPC
#  undef IS_NMSTRT_CHAR
#  undef IS_NMSTRT_CHAR_MINBPC
#  undef IS_INVALID_CHAR

#endif /* not XML_MIN_SIZE */

#ifdef XML_NS

static const struct normal_encoding big2_encoding_ns
    = {{VTABLE, 2, 0,
#  if BYTEORDER == 4321
        1
#  else
        0
#  endif
       },
       {
#  include "asciitab.h"
#  include "latin1tab.h"
       },
       STANDARD_VTABLE(big2_) NULL_VTABLE};

#endif

static const struct normal_encoding big2_encoding
    = {{VTABLE, 2, 0,
#if BYTEORDER == 4321
        1
#else
        0
#endif
       },
       {
#define BT_COLON BT_NMSTRT
#include "asciitab.h"
#undef BT_COLON
#include "latin1tab.h"
       },
       STANDARD_VTABLE(big2_) NULL_VTABLE};

#if BYTEORDER != 1234

#  ifdef XML_NS

static const struct normal_encoding internal_big2_encoding_ns
    = {{VTABLE, 2, 0, 1},
       {
#    include "iasciitab.h"
#    include "latin1tab.h"
       },
       STANDARD_VTABLE(big2_) NULL_VTABLE};

#  endif

static const struct normal_encoding internal_big2_encoding
    = {{VTABLE, 2, 0, 1},
       {
#  define BT_COLON BT_NMSTRT
#  include "iasciitab.h"
#  undef BT_COLON
#  include "latin1tab.h"
       },
       STANDARD_VTABLE(big2_) NULL_VTABLE};

#endif

#undef PREFIX

static int FASTCALL
streqci(const char *s1, const char *s2) {
  fprintf(stderr, "[xmltok.c] enter streqci 1\n");
  for (;;) {
    fprintf(stderr, "[xmltok.c] enter streqci 2\n");
    char c1 = *s1++;
    char c2 = *s2++;
    if (ASCII_a <= c1 && c1 <= ASCII_z)
      c1 += ASCII_A - ASCII_a;
    if (ASCII_a <= c2 && c2 <= ASCII_z)
      /* The following line will never get executed.  streqci() is
       * only called from two places, both of which guarantee to put
       * upper-case strings into s2.
       */
      c2 += ASCII_A - ASCII_a; /* LCOV_EXCL_LINE */
    if (c1 != c2) {
      fprintf(stderr, "[xmltok.c] enter streqci 3\n");
      return 0;
      // fprintf(stderr, "[xmltok.c] exit streqci 3\n");
    }
    if (! c1) {
      fprintf(stderr, "[xmltok.c] enter streqci 4\n");
      break;
      // fprintf(stderr, "[xmltok.c] exit streqci 4\n");
    }
    // fprintf(stderr, "[xmltok.c] exit streqci 2\n");
  }
  fprintf(stderr, "[xmltok.c] enter streqci 5\n");
  return 1;
  // fprintf(stderr, "[xmltok.c] exit streqci 5\n");
  // fprintf(stderr, "[xmltok.c] exit streqci 1\n");
}

static void PTRCALL
initUpdatePosition(const ENCODING *enc, const char *ptr, const char *end,
                   POSITION *pos) {
  fprintf(stderr, "[xmltok.c] enter initUpdatePosition 1\n");
  UNUSED_P(enc);
  normal_updatePosition(&utf8_encoding.enc, ptr, end, pos);
  // fprintf(stderr, "[xmltok.c] exit initUpdatePosition 1\n");
}

static int
toAscii(const ENCODING *enc, const char *ptr, const char *end) {
  fprintf(stderr, "[xmltok.c] enter toAscii 1\n");
  char buf[1];
  char *p = buf;
  XmlUtf8Convert(enc, &ptr, end, &p, p + 1);
  if (p == buf) {
    fprintf(stderr, "[xmltok.c] enter toAscii 2\n");
    return -1;
    // fprintf(stderr, "[xmltok.c] exit toAscii 2\n");
  }
  else {
    fprintf(stderr, "[xmltok.c] enter toAscii 3\n");
    return buf[0];
    // fprintf(stderr, "[xmltok.c] exit toAscii 3\n");
  }
  // fprintf(stderr, "[xmltok.c] exit toAscii 1\n");
}

static int FASTCALL
isSpace(int c) {
  fprintf(stderr, "[xmltok.c] enter isSpace 1\n");
  switch (c) {
  case 0x20:
  case 0xD:
  case 0xA:
  case 0x9:
    fprintf(stderr, "[xmltok.c] enter isSpace 2\n");
    return 1;
    // fprintf(stderr, "[xmltok.c] exit isSpace 2\n");
  }
  fprintf(stderr, "[xmltok.c] enter isSpace 3\n");
  return 0;
  // fprintf(stderr, "[xmltok.c] exit isSpace 3\n");
  // fprintf(stderr, "[xmltok.c] exit isSpace 1\n");
}
static enum XML_Convert_Result PTRCALL
unknown_toUtf16(const ENCODING *enc, const char **fromP, const char *fromLim,
                unsigned short **toP, const unsigned short *toLim) {
  fprintf(stderr, "[xmltok.c] enter unknown_toUtf16 1\n");
  const struct unknown_encoding *uenc = AS_UNKNOWN_ENCODING(enc);
  // fprintf(stderr, "[xmltok.c] exit unknown_toUtf16 1\n");
  
  while (*fromP < fromLim && *toP < toLim) {
    fprintf(stderr, "[xmltok.c] enter unknown_toUtf16 2\n");
    unsigned short c = uenc->utf16[(unsigned char)**fromP];
    // fprintf(stderr, "[xmltok.c] exit unknown_toUtf16 2\n");
    
    if (c == 0) {
      fprintf(stderr, "[xmltok.c] enter unknown_toUtf16 3\n");
      c = (unsigned short)uenc->convert(uenc->userData, *fromP);
      *fromP += (AS_NORMAL_ENCODING(enc)->type[(unsigned char)**fromP]
                 - (BT_LEAD2 - 2));
      // fprintf(stderr, "[xmltok.c] exit unknown_toUtf16 3\n");
    } else {
      fprintf(stderr, "[xmltok.c] enter unknown_toUtf16 4\n");
      (*fromP)++;
      // fprintf(stderr, "[xmltok.c] exit unknown_toUtf16 4\n");
    }
    
    fprintf(stderr, "[xmltok.c] enter unknown_toUtf16 5\n");
    *(*toP)++ = c;
    // fprintf(stderr, "[xmltok.c] exit unknown_toUtf16 5\n");
  }

  if ((*toP == toLim) && (*fromP < fromLim)) {
    fprintf(stderr, "[xmltok.c] enter unknown_toUtf16 6\n");
    return XML_CONVERT_OUTPUT_EXHAUSTED;
    // fprintf(stderr, "[xmltok.c] exit unknown_toUtf16 6\n");
  }
  else {
    fprintf(stderr, "[xmltok.c] enter unknown_toUtf16 7\n");
    return XML_CONVERT_COMPLETED;
    // fprintf(stderr, "[xmltok.c] exit unknown_toUtf16 7\n");
  }
}

ENCODING *
XmlInitUnknownEncoding(void *mem, const int *table, CONVERTER convert,
                       void *userData) {
  fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 1\n");
  int i;
  struct unknown_encoding *e = (struct unknown_encoding *)mem;
  memcpy(mem, &latin1_encoding, sizeof(struct normal_encoding));
  // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 1\n");
  
  for (i = 0; i < 128; i++) {
    fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 2\n");
    if (latin1_encoding.type[i] != BT_OTHER
        && latin1_encoding.type[i] != BT_NONXML && table[i] != i) {
      fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 3\n");
      return 0;
      // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 3\n");
    }
    // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 2\n");
  }
  
  for (i = 0; i < 256; i++) {
    fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 4\n");
    int c = table[i];
    // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 4\n");
    
    if (c == -1) {
      fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 5\n");
      e->normal.type[i] = BT_MALFORM;
      /* This shouldn't really get used. */
      e->utf16[i] = 0xFFFF;
      e->utf8[i][0] = 1;
      e->utf8[i][1] = 0;
      // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 5\n");
    } else if (c < 0) {
      fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 6\n");
      if (c < -4) {
        fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 7\n");
        return 0;
        // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 7\n");
      }
      /* Multi-byte sequences need a converter function */
      if (! convert) {
        fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 8\n");
        return 0;
        // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 8\n");
      }
      e->normal.type[i] = (unsigned char)(BT_LEAD2 - (c + 2));
      e->utf8[i][0] = 0;
      e->utf16[i] = 0;
      // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 6\n");
    } else if (c < 0x80) {
      fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 9\n");
      if (latin1_encoding.type[c] != BT_OTHER
          && latin1_encoding.type[c] != BT_NONXML && c != i) {
        fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 10\n");
        return 0;
        // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 10\n");
      }
      e->normal.type[i] = latin1_encoding.type[c];
      e->utf8[i][0] = 1;
      e->utf8[i][1] = (char)c;
      e->utf16[i] = (unsigned short)(c == 0 ? 0xFFFF : c);
      // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 9\n");
    } else if (checkCharRefNumber(c) < 0) {
      fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 11\n");
      e->normal.type[i] = BT_NONXML;
      /* This shouldn't really get used. */
      e->utf16[i] = 0xFFFF;
      e->utf8[i][0] = 1;
      e->utf8[i][1] = 0;
      // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 11\n");
    } else {
      fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 12\n");
      if (c > 0xFFFF) {
        fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 13\n");
        return 0;
        // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 13\n");
      }
      if (UCS2_GET_NAMING(nmstrtPages, c >> 8, c & 0xff))
        e->normal.type[i] = BT_NMSTRT;
      else if (UCS2_GET_NAMING(namePages, c >> 8, c & 0xff))
        e->normal.type[i] = BT_NAME;
      else
        e->normal.type[i] = BT_OTHER;
      e->utf8[i][0] = (char)XmlUtf8Encode(c, e->utf8[i] + 1);
      e->utf16[i] = (unsigned short)c;
      // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 12\n");
    }
  }
  
  fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 14\n");
  e->userData = userData;
  e->convert = convert;
  // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 14\n");
  
  if (convert) {
    fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 15\n");
    e->normal.isName2 = unknown_isName;
    e->normal.isName3 = unknown_isName;
    e->normal.isName4 = unknown_isName;
    e->normal.isNmstrt2 = unknown_isNmstrt;
    e->normal.isNmstrt3 = unknown_isNmstrt;
    e->normal.isNmstrt4 = unknown_isNmstrt;
    e->normal.isInvalid2 = unknown_isInvalid;
    e->normal.isInvalid3 = unknown_isInvalid;
    e->normal.isInvalid4 = unknown_isInvalid;
    // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 15\n");
  }
  
  fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncoding 16\n");
  e->normal.enc.utf8Convert = unknown_toUtf8;
  e->normal.enc.utf16Convert = unknown_toUtf16;
  return &(e->normal.enc);
  // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncoding 16\n");
}

/* If this enumeration is changed, getEncodingIndex and encodings
must also be changed. */
enum {
  UNKNOWN_ENC = -1,
  ISO_8859_1_ENC = 0,
  US_ASCII_ENC,
  UTF_8_ENC,
  UTF_16_ENC,
  UTF_16BE_ENC,
  UTF_16LE_ENC,
  /* must match encodingNames up to here */
  NO_ENC
};

static const char KW_ISO_8859_1[]
    = {ASCII_I, ASCII_S, ASCII_O,     ASCII_MINUS, ASCII_8, ASCII_8,
       ASCII_5, ASCII_9, ASCII_MINUS, ASCII_1,     '\0'};
static const char KW_US_ASCII[]
    = {ASCII_U, ASCII_S, ASCII_MINUS, ASCII_A, ASCII_S,
       ASCII_C, ASCII_I, ASCII_I,     '\0'};
static const char KW_UTF_8[]
    = {ASCII_U, ASCII_T, ASCII_F, ASCII_MINUS, ASCII_8, '\0'};
static const char KW_UTF_16[]
    = {ASCII_U, ASCII_T, ASCII_F, ASCII_MINUS, ASCII_1, ASCII_6, '\0'};
static const char KW_UTF_16BE[]
    = {ASCII_U, ASCII_T, ASCII_F, ASCII_MINUS, ASCII_1,
       ASCII_6, ASCII_B, ASCII_E, '\0'};
static const char KW_UTF_16LE[]
    = {ASCII_U, ASCII_T, ASCII_F, ASCII_MINUS, ASCII_1,
       ASCII_6, ASCII_L, ASCII_E, '\0'};

static int FASTCALL
getEncodingIndex(const char *name) {
  fprintf(stderr, "[xmltok.c] enter getEncodingIndex 1\n");
  static const char *const encodingNames[] = {
      KW_ISO_8859_1, KW_US_ASCII, KW_UTF_8, KW_UTF_16, KW_UTF_16BE, KW_UTF_16LE,
  };
  int i;
  // fprintf(stderr, "[xmltok.c] exit getEncodingIndex 1\n");
  
  if (name == NULL) {
    fprintf(stderr, "[xmltok.c] enter getEncodingIndex 2\n");
    return NO_ENC;
    // fprintf(stderr, "[xmltok.c] exit getEncodingIndex 2\n");
  }
  
  for (i = 0; i < (int)(sizeof(encodingNames) / sizeof(encodingNames[0])); i++) {
    fprintf(stderr, "[xmltok.c] enter getEncodingIndex 3\n");
    if (streqci(name, encodingNames[i])) {
      fprintf(stderr, "[xmltok.c] enter getEncodingIndex 4\n");
      return i;
      // fprintf(stderr, "[xmltok.c] exit getEncodingIndex 4\n");
    }
    // fprintf(stderr, "[xmltok.c] exit getEncodingIndex 3\n");
  }
  
  fprintf(stderr, "[xmltok.c] enter getEncodingIndex 5\n");
  return UNKNOWN_ENC;
  // fprintf(stderr, "[xmltok.c] exit getEncodingIndex 5\n");
}

/* For binary compatibility, we store the index of the encoding
   specified at initialization in the isUtf16 member.
*/

#define INIT_ENC_INDEX(enc) ((int)(enc)->initEnc.isUtf16)
#define SET_INIT_ENC_INDEX(enc, i) ((enc)->initEnc.isUtf16 = (char)i)

/* This is what detects the encoding.  encodingTable maps from
   encoding indices to encodings; INIT_ENC_INDEX(enc) is the index of
   the external (protocol) specified encoding; state is
   XML_CONTENT_STATE if we're parsing an external text entity, and
   XML_PROLOG_STATE otherwise.
*/

static int
initScan(const ENCODING *const *encodingTable, const INIT_ENCODING *enc,
         int state, const char *ptr, const char *end, const char **nextTokPtr) {
  fprintf(stderr, "[xmltok.c] enter initScan 1\n");
  const ENCODING **encPtr;
  // fprintf(stderr, "[xmltok.c] exit initScan 1\n");

  if (ptr >= end) {
    fprintf(stderr, "[xmltok.c] enter initScan 2\n");
    return XML_TOK_NONE;
    // fprintf(stderr, "[xmltok.c] exit initScan 2\n");
  }
  
  fprintf(stderr, "[xmltok.c] enter initScan 3\n");
  encPtr = enc->encPtr;
  // fprintf(stderr, "[xmltok.c] exit initScan 3\n");
  
  if (ptr + 1 == end) {
    fprintf(stderr, "[xmltok.c] enter initScan 4\n");
    /* only a single byte available for auto-detection */
#ifndef XML_DTD /* FIXME */
    /* a well-formed document entity must have more than one byte */
    if (state != XML_CONTENT_STATE) {
      fprintf(stderr, "[xmltok.c] enter initScan 5\n");
      return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok.c] exit initScan 5\n");
    }
#endif
    /* so we're parsing an external text entity... */
    /* if UTF-16 was externally specified, then we need at least 2 bytes */
    switch (INIT_ENC_INDEX(enc)) {
    case UTF_16_ENC:
    case UTF_16LE_ENC:
    case UTF_16BE_ENC:
      fprintf(stderr, "[xmltok.c] enter initScan 6\n");
      return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok.c] exit initScan 6\n");
    }
    switch ((unsigned char)*ptr) {
    case 0xFE:
    case 0xFF:
    case 0xEF: /* possibly first byte of UTF-8 BOM */
      fprintf(stderr, "[xmltok.c] enter initScan 7\n");
      if (INIT_ENC_INDEX(enc) == ISO_8859_1_ENC && state == XML_CONTENT_STATE)
        break;
      /* fall through */
      // fprintf(stderr, "[xmltok.c] exit initScan 7\n");
    case 0x00:
    case 0x3C:
      fprintf(stderr, "[xmltok.c] enter initScan 8\n");
      return XML_TOK_PARTIAL;
      // fprintf(stderr, "[xmltok.c] exit initScan 8\n");
    }
    // fprintf(stderr, "[xmltok.c] exit initScan 4\n");
  } else {
    fprintf(stderr, "[xmltok.c] enter initScan 9\n");
    switch (((unsigned char)ptr[0] << 8) | (unsigned char)ptr[1]) {
    case 0xFEFF:
      fprintf(stderr, "[xmltok.c] enter initScan 10\n");
      if (INIT_ENC_INDEX(enc) == ISO_8859_1_ENC && state == XML_CONTENT_STATE)
        break;
      *nextTokPtr = ptr + 2;
      *encPtr = encodingTable[UTF_16BE_ENC];
      return XML_TOK_BOM;
      // fprintf(stderr, "[xmltok.c] exit initScan 10\n");
    /* 00 3C is handled in the default case */
    case 0x3C00:
      fprintf(stderr, "[xmltok.c] enter initScan 11\n");
      if ((INIT_ENC_INDEX(enc) == UTF_16BE_ENC
           || INIT_ENC_INDEX(enc) == UTF_16_ENC)
          && state == XML_CONTENT_STATE)
        break;
      *encPtr = encodingTable[UTF_16LE_ENC];
      return XmlTok(*encPtr, state, ptr, end, nextTokPtr);
      // fprintf(stderr, "[xmltok.c] exit initScan 11\n");
    case 0xFFFE:
      fprintf(stderr, "[xmltok.c] enter initScan 12\n");
      if (INIT_ENC_INDEX(enc) == ISO_8859_1_ENC && state == XML_CONTENT_STATE)
        break;
      *nextTokPtr = ptr + 2;
      *encPtr = encodingTable[UTF_16LE_ENC];
      return XML_TOK_BOM;
      // fprintf(stderr, "[xmltok.c] exit initScan 12\n");
    case 0xEFBB:
      fprintf(stderr, "[xmltok.c] enter initScan 13\n");
      /* Maybe a UTF-8 BOM (EF BB BF) */
      /* If there's an explicitly specified (external) encoding
         of ISO-8859-1 or some flavour of UTF-16
         and this is an external text entity,
         don't look for the BOM,
         because it might be a legal data.
      */
      if (state == XML_CONTENT_STATE) {
        fprintf(stderr, "[xmltok.c] enter initScan 14\n");
        int e = INIT_ENC_INDEX(enc);
        if (e == ISO_8859_1_ENC || e == UTF_16BE_ENC || e == UTF_16LE_ENC
            || e == UTF_16_ENC)
          break;
        // fprintf(stderr, "[xmltok.c] exit initScan 14\n");
      }
      if (ptr + 2 == end) {
        fprintf(stderr, "[xmltok.c] enter initScan 15\n");
        return XML_TOK_PARTIAL;
        // fprintf(stderr, "[xmltok.c] exit initScan 15\n");
      }
      if ((unsigned char)ptr[2] == 0xBF) {
        fprintf(stderr, "[xmltok.c] enter initScan 16\n");
        *nextTokPtr = ptr + 3;
        *encPtr = encodingTable[UTF_8_ENC];
        return XML_TOK_BOM;
        // fprintf(stderr, "[xmltok.c] exit initScan 16\n");
      }
      break;
      // fprintf(stderr, "[xmltok.c] exit initScan 13\n");
    default:
      fprintf(stderr, "[xmltok.c] enter initScan 17\n");
      if (ptr[0] == '\0') {
        fprintf(stderr, "[xmltok.c] enter initScan 18\n");
        /* 0 isn't a legal data character. Furthermore a document
           entity can only start with ASCII characters.  So the only
           way this can fail to be big-endian UTF-16 if it it's an
           external parsed general entity that's labelled as
           UTF-16LE.
        */
        if (state == XML_CONTENT_STATE && INIT_ENC_INDEX(enc) == UTF_16LE_ENC)
          break;
        *encPtr = encodingTable[UTF_16BE_ENC];
        return XmlTok(*encPtr, state, ptr, end, nextTokPtr);
        // fprintf(stderr, "[xmltok.c] exit initScan 18\n");
      } else if (ptr[1] == '\0') {
        fprintf(stderr, "[xmltok.c] enter initScan 19\n");
        /* We could recover here in the case:
            - parsing an external entity
            - second byte is 0
            - no externally specified encoding
            - no encoding declaration
           by assuming UTF-16LE.  But we don't, because this would mean when
           presented just with a single byte, we couldn't reliably determine
           whether we needed further bytes.
        */
        if (state == XML_CONTENT_STATE)
          break;
        *encPtr = encodingTable[UTF_16LE_ENC];
        return XmlTok(*encPtr, state, ptr, end, nextTokPtr);
        // fprintf(stderr, "[xmltok.c] exit initScan 19\n");
      }
      break;
      // fprintf(stderr, "[xmltok.c] exit initScan 17\n");
    }
    // fprintf(stderr, "[xmltok.c] exit initScan 9\n");
  }
  
  fprintf(stderr, "[xmltok.c] enter initScan 20\n");
  *encPtr = encodingTable[INIT_ENC_INDEX(enc)];
  return XmlTok(*encPtr, state, ptr, end, nextTokPtr);
  // fprintf(stderr, "[xmltok.c] exit initScan 20\n");
}

#define NS(x) x
#define ns(x) x
#define XML_TOK_NS_C
#include "xmltok_ns.c"
#undef XML_TOK_NS_C
#undef NS
#undef ns

#ifdef XML_NS

#  define NS(x) x##NS
#  define ns(x) x##_ns

#  define XML_TOK_NS_C
#  include "xmltok_ns.c"
#  undef XML_TOK_NS_C

#  undef NS
#  undef ns

ENCODING *
XmlInitUnknownEncodingNS(void *mem, const int *table, CONVERTER convert,
                         void *userData) {
  fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncodingNS 1\n");
  ENCODING *enc = XmlInitUnknownEncoding(mem, table, convert, userData);
  if (enc) {
    fprintf(stderr, "[xmltok.c] enter XmlInitUnknownEncodingNS 2\n");
    ((struct normal_encoding *)enc)->type[ASCII_COLON] = BT_COLON;
    // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncodingNS 2\n");
  }
  return enc;
  // fprintf(stderr, "[xmltok.c] exit XmlInitUnknownEncodingNS 1\n");
}

#endif /* XML_NS */
// Total cost: 2.267459
// Total split cost: 0.249510, input tokens: 158056, output tokens: 1678, cache read tokens: 158033, cache write tokens: 47163, split chunks: [(0, 594), (594, 1377), (1377, 1671)]
// Total instrumented cost: 2.017949, input tokens: 130305, output tokens: 92087, cache read tokens: 130281, cache write tokens: 159330
