/*
                            __  __            _
                         ___\ \/ /_ __   __ _| |_
                        / _ \\  /| '_ \ / _` | __|
                       |  __//  \| |_) | (_| | |_
                        \___/_/\_\ .__/ \__,_|\__|
                                 |_| XML parser

   Copyright (c) 1997-2000 Thai Open Source Software Center Ltd
   Copyright (c) 2000      Clark Cooper <coopercc@users.sourceforge.net>
   Copyright (c) 2002      Greg Stein <gstein@users.sourceforge.net>
   Copyright (c) 2002-2006 Karl Waclawek <karl@waclawek.net>
   Copyright (c) 2002-2003 Fred L. Drake, Jr. <fdrake@users.sourceforge.net>
   Copyright (c) 2005-2009 Steven Solie <steven@solie.ca>
   Copyright (c) 2016-2023 Sebastian Pipping <sebastian@pipping.org>
   Copyright (c) 2017      Rhodri James <rhodri@wildebeest.org.uk>
   Copyright (c) 2019      David Loffredo <loffredo@steptools.com>
   Copyright (c) 2021      Donghee Na <donghee.na@python.org>
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

   THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY  OF  ANY  KIND,
   EXPRESS  OR IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO  THE WARRANTIES  OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
   NO EVENT SHALL THE AUTHORS OR  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
   DAMAGES OR  OTHER LIABILITY, WHETHER  IN AN  ACTION OF CONTRACT,  TORT OR
   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
   USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "expat_config.h"

#include <stddef.h>

#ifdef _WIN32
#  include "winconfig.h"
#endif

#include "internal.h"
#include "xmlrole.h"
#include "ascii.h"
#include <stdio.h>

/* Doesn't check:

 that ,| are not mixed in a model group
 content of literals

*/

static const char KW_ANY[] = {ASCII_A, ASCII_N, ASCII_Y, '\0'};
static const char KW_ATTLIST[]
    = {ASCII_A, ASCII_T, ASCII_T, ASCII_L, ASCII_I, ASCII_S, ASCII_T, '\0'};
static const char KW_CDATA[]
    = {ASCII_C, ASCII_D, ASCII_A, ASCII_T, ASCII_A, '\0'};
static const char KW_DOCTYPE[]
    = {ASCII_D, ASCII_O, ASCII_C, ASCII_T, ASCII_Y, ASCII_P, ASCII_E, '\0'};
static const char KW_ELEMENT[]
    = {ASCII_E, ASCII_L, ASCII_E, ASCII_M, ASCII_E, ASCII_N, ASCII_T, '\0'};
static const char KW_EMPTY[]
    = {ASCII_E, ASCII_M, ASCII_P, ASCII_T, ASCII_Y, '\0'};
static const char KW_ENTITIES[] = {ASCII_E, ASCII_N, ASCII_T, ASCII_I, ASCII_T,
                                   ASCII_I, ASCII_E, ASCII_S, '\0'};
static const char KW_ENTITY[]
    = {ASCII_E, ASCII_N, ASCII_T, ASCII_I, ASCII_T, ASCII_Y, '\0'};
static const char KW_FIXED[]
    = {ASCII_F, ASCII_I, ASCII_X, ASCII_E, ASCII_D, '\0'};
static const char KW_ID[] = {ASCII_I, ASCII_D, '\0'};
static const char KW_IDREF[]
    = {ASCII_I, ASCII_D, ASCII_R, ASCII_E, ASCII_F, '\0'};
static const char KW_IDREFS[]
    = {ASCII_I, ASCII_D, ASCII_R, ASCII_E, ASCII_F, ASCII_S, '\0'};
#ifdef XML_DTD
static const char KW_IGNORE[]
    = {ASCII_I, ASCII_G, ASCII_N, ASCII_O, ASCII_R, ASCII_E, '\0'};
#endif
static const char KW_IMPLIED[]
    = {ASCII_I, ASCII_M, ASCII_P, ASCII_L, ASCII_I, ASCII_E, ASCII_D, '\0'};
#ifdef XML_DTD
static const char KW_INCLUDE[]
    = {ASCII_I, ASCII_N, ASCII_C, ASCII_L, ASCII_U, ASCII_D, ASCII_E, '\0'};
#endif
static const char KW_NDATA[]
    = {ASCII_N, ASCII_D, ASCII_A, ASCII_T, ASCII_A, '\0'};
static const char KW_NMTOKEN[]
    = {ASCII_N, ASCII_M, ASCII_T, ASCII_O, ASCII_K, ASCII_E, ASCII_N, '\0'};
static const char KW_NMTOKENS[] = {ASCII_N, ASCII_M, ASCII_T, ASCII_O, ASCII_K,
                                   ASCII_E, ASCII_N, ASCII_S, '\0'};
static const char KW_NOTATION[] = {ASCII_N, ASCII_O, ASCII_T, ASCII_A, ASCII_T,
                                   ASCII_I, ASCII_O, ASCII_N, '\0'};
static const char KW_PCDATA[]
    = {ASCII_P, ASCII_C, ASCII_D, ASCII_A, ASCII_T, ASCII_A, '\0'};
static const char KW_PUBLIC[]
    = {ASCII_P, ASCII_U, ASCII_B, ASCII_L, ASCII_I, ASCII_C, '\0'};
static const char KW_REQUIRED[] = {ASCII_R, ASCII_E, ASCII_Q, ASCII_U, ASCII_I,
                                   ASCII_R, ASCII_E, ASCII_D, '\0'};
static const char KW_SYSTEM[]
    = {ASCII_S, ASCII_Y, ASCII_S, ASCII_T, ASCII_E, ASCII_M, '\0'};

#ifndef MIN_BYTES_PER_CHAR
#  define MIN_BYTES_PER_CHAR(enc) ((enc)->minBytesPerChar)
#endif

#ifdef XML_DTD
#  define setTopLevel(state)                                                   \
    ((state)->handler                                                          \
     = ((state)->documentEntity ? internalSubset : externalSubset1))
#else /* not XML_DTD */
#  define setTopLevel(state) ((state)->handler = internalSubset)
#endif /* not XML_DTD */

typedef int PTRCALL PROLOG_HANDLER(PROLOG_STATE *state, int tok,
                                   const char *ptr, const char *end,
                                   const ENCODING *enc);

static PROLOG_HANDLER prolog0, prolog1, prolog2, doctype0, doctype1, doctype2,
    doctype3, doctype4, doctype5, internalSubset, entity0, entity1, entity2,
    entity3, entity4, entity5, entity6, entity7, entity8, entity9, entity10,
    notation0, notation1, notation2, notation3, notation4, attlist0, attlist1,
    attlist2, attlist3, attlist4, attlist5, attlist6, attlist7, attlist8,
    attlist9, element0, element1, element2, element3, element4, element5,
    element6, element7,
#ifdef XML_DTD
    externalSubset0, externalSubset1, condSect0, condSect1, condSect2,
#endif /* XML_DTD */
    declClose, error;

static int FASTCALL common(PROLOG_STATE *state, int tok);

static int PTRCALL
prolog0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter prolog0 1\n");
    state->handler = prolog1;
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit prolog0 1\n");
  case XML_TOK_XML_DECL:
    fprintf(stderr, "[xmlrole.c] enter prolog0 2\n");
    state->handler = prolog1;
    return XML_ROLE_XML_DECL;
    // fprintf(stderr, "[xmlrole.c] exit prolog0 2\n");
  case XML_TOK_PI:
    fprintf(stderr, "[xmlrole.c] enter prolog0 3\n");
    state->handler = prolog1;
    return XML_ROLE_PI;
    // fprintf(stderr, "[xmlrole.c] exit prolog0 3\n");
  case XML_TOK_COMMENT:
    fprintf(stderr, "[xmlrole.c] enter prolog0 4\n");
    state->handler = prolog1;
    return XML_ROLE_COMMENT;
    // fprintf(stderr, "[xmlrole.c] exit prolog0 4\n");
  case XML_TOK_BOM:
    fprintf(stderr, "[xmlrole.c] enter prolog0 5\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit prolog0 5\n");
  case XML_TOK_DECL_OPEN:
    if (! XmlNameMatchesAscii(enc, ptr + 2 * MIN_BYTES_PER_CHAR(enc), end,
                              KW_DOCTYPE))
    {
      fprintf(stderr, "[xmlrole.c] enter prolog0 6\n");
      break;
      // fprintf(stderr, "[xmlrole.c] exit prolog0 6\n");
    }
    fprintf(stderr, "[xmlrole.c] enter prolog0 7\n");
    state->handler = doctype0;
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit prolog0 7\n");
  case XML_TOK_INSTANCE_START:
    fprintf(stderr, "[xmlrole.c] enter prolog0 8\n");
    state->handler = error;
    return XML_ROLE_INSTANCE_START;
    // fprintf(stderr, "[xmlrole.c] exit prolog0 8\n");
  }
  fprintf(stderr, "[xmlrole.c] enter prolog0 9\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit prolog0 9\n");
}
static int PTRCALL
prolog1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter prolog1 1\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit prolog1 1\n");
  case XML_TOK_PI:
    fprintf(stderr, "[xmlrole.c] enter prolog1 2\n");
    return XML_ROLE_PI;
    // fprintf(stderr, "[xmlrole.c] exit prolog1 2\n");
  case XML_TOK_COMMENT:
    fprintf(stderr, "[xmlrole.c] enter prolog1 3\n");
    return XML_ROLE_COMMENT;
    // fprintf(stderr, "[xmlrole.c] exit prolog1 3\n");
  case XML_TOK_BOM:
    /* This case can never arise.  To reach this role function, the
     * parse must have passed through prolog0 and therefore have had
     * some form of input, even if only a space.  At that point, a
     * byte order mark is no longer a valid character (though
     * technically it should be interpreted as a non-breaking space),
     * so will be rejected by the tokenizing stages.
     */
    fprintf(stderr, "[xmlrole.c] enter prolog1 4\n");
    return XML_ROLE_NONE; /* LCOV_EXCL_LINE */
    // fprintf(stderr, "[xmlrole.c] exit prolog1 4\n");
  case XML_TOK_DECL_OPEN:
    if (! XmlNameMatchesAscii(enc, ptr + 2 * MIN_BYTES_PER_CHAR(enc), end,
                              KW_DOCTYPE)) {
      fprintf(stderr, "[xmlrole.c] enter prolog1 5\n");
      break;
      // fprintf(stderr, "[xmlrole.c] exit prolog1 5\n");
    }
    fprintf(stderr, "[xmlrole.c] enter prolog1 6\n");
    state->handler = doctype0;
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit prolog1 6\n");
  case XML_TOK_INSTANCE_START:
    fprintf(stderr, "[xmlrole.c] enter prolog1 7\n");
    state->handler = error;
    return XML_ROLE_INSTANCE_START;
    // fprintf(stderr, "[xmlrole.c] exit prolog1 7\n");
  }
  fprintf(stderr, "[xmlrole.c] enter prolog1 8\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit prolog1 8\n");
}

static int PTRCALL
prolog2(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter prolog2 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit prolog2 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter prolog2 2\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit prolog2 2\n");
  case XML_TOK_PI:
    fprintf(stderr, "[xmlrole.c] enter prolog2 3\n");
    return XML_ROLE_PI;
    // fprintf(stderr, "[xmlrole.c] exit prolog2 3\n");
  case XML_TOK_COMMENT:
    fprintf(stderr, "[xmlrole.c] enter prolog2 4\n");
    return XML_ROLE_COMMENT;
    // fprintf(stderr, "[xmlrole.c] exit prolog2 4\n");
  case XML_TOK_INSTANCE_START:
    fprintf(stderr, "[xmlrole.c] enter prolog2 5\n");
    state->handler = error;
    return XML_ROLE_INSTANCE_START;
    // fprintf(stderr, "[xmlrole.c] exit prolog2 5\n");
  }
  fprintf(stderr, "[xmlrole.c] enter prolog2 6\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit prolog2 6\n");
}

static int PTRCALL
doctype0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter doctype0 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit doctype0 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter doctype0 2\n");
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit doctype0 2\n");
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter doctype0 3\n");
    state->handler = doctype1;
    return XML_ROLE_DOCTYPE_NAME;
    // fprintf(stderr, "[xmlrole.c] exit doctype0 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter doctype0 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit doctype0 4\n");
}

static int PTRCALL
doctype1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter doctype1 1\n");
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit doctype1 1\n");
  case XML_TOK_OPEN_BRACKET:
    fprintf(stderr, "[xmlrole.c] enter doctype1 2\n");
    state->handler = internalSubset;
    return XML_ROLE_DOCTYPE_INTERNAL_SUBSET;
    // fprintf(stderr, "[xmlrole.c] exit doctype1 2\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter doctype1 3\n");
    state->handler = prolog2;
    return XML_ROLE_DOCTYPE_CLOSE;
    // fprintf(stderr, "[xmlrole.c] exit doctype1 3\n");
  case XML_TOK_NAME:
    if (XmlNameMatchesAscii(enc, ptr, end, KW_SYSTEM)) {
      fprintf(stderr, "[xmlrole.c] enter doctype1 4\n");
      state->handler = doctype3;
      return XML_ROLE_DOCTYPE_NONE;
      // fprintf(stderr, "[xmlrole.c] exit doctype1 4\n");
    }
    if (XmlNameMatchesAscii(enc, ptr, end, KW_PUBLIC)) {
      fprintf(stderr, "[xmlrole.c] enter doctype1 5\n");
      state->handler = doctype2;
      return XML_ROLE_DOCTYPE_NONE;
      // fprintf(stderr, "[xmlrole.c] exit doctype1 5\n");
    }
    fprintf(stderr, "[xmlrole.c] enter doctype1 6\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit doctype1 6\n");
  }
  fprintf(stderr, "[xmlrole.c] enter doctype1 7\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit doctype1 7\n");
}

static int PTRCALL
doctype2(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter doctype2 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit doctype2 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter doctype2 2\n");
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit doctype2 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter doctype2 3\n");
    state->handler = doctype3;
    return XML_ROLE_DOCTYPE_PUBLIC_ID;
    // fprintf(stderr, "[xmlrole.c] exit doctype2 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter doctype2 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit doctype2 4\n");
}

static int PTRCALL
doctype3(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter doctype3 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit doctype3 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter doctype3 2\n");
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit doctype3 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter doctype3 3\n");
    state->handler = doctype4;
    return XML_ROLE_DOCTYPE_SYSTEM_ID;
    // fprintf(stderr, "[xmlrole.c] exit doctype3 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter doctype3 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit doctype3 4\n");
}

static int PTRCALL
doctype4(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter doctype4 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit doctype4 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter doctype4 2\n");
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit doctype4 2\n");
  case XML_TOK_OPEN_BRACKET:
    fprintf(stderr, "[xmlrole.c] enter doctype4 3\n");
    state->handler = internalSubset;
    return XML_ROLE_DOCTYPE_INTERNAL_SUBSET;
    // fprintf(stderr, "[xmlrole.c] exit doctype4 3\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter doctype4 4\n");
    state->handler = prolog2;
    return XML_ROLE_DOCTYPE_CLOSE;
    // fprintf(stderr, "[xmlrole.c] exit doctype4 4\n");
  }
  fprintf(stderr, "[xmlrole.c] enter doctype4 5\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit doctype4 5\n");
}

static int PTRCALL
doctype5(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter doctype5 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit doctype5 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter doctype5 2\n");
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit doctype5 2\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter doctype5 3\n");
    state->handler = prolog2;
    return XML_ROLE_DOCTYPE_CLOSE;
    // fprintf(stderr, "[xmlrole.c] exit doctype5 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter doctype5 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit doctype5 4\n");
}
static int PTRCALL
internalSubset(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
               const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter internalSubset 1\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit internalSubset 1\n");
  case XML_TOK_DECL_OPEN:
    if (XmlNameMatchesAscii(enc, ptr + 2 * MIN_BYTES_PER_CHAR(enc), end,
                            KW_ENTITY)) {
      fprintf(stderr, "[xmlrole.c] enter internalSubset 2\n");
      state->handler = entity0;
      return XML_ROLE_ENTITY_NONE;
      // fprintf(stderr, "[xmlrole.c] exit internalSubset 2\n");
    }
    if (XmlNameMatchesAscii(enc, ptr + 2 * MIN_BYTES_PER_CHAR(enc), end,
                            KW_ATTLIST)) {
      fprintf(stderr, "[xmlrole.c] enter internalSubset 3\n");
      state->handler = attlist0;
      return XML_ROLE_ATTLIST_NONE;
      // fprintf(stderr, "[xmlrole.c] exit internalSubset 3\n");
    }
    if (XmlNameMatchesAscii(enc, ptr + 2 * MIN_BYTES_PER_CHAR(enc), end,
                            KW_ELEMENT)) {
      fprintf(stderr, "[xmlrole.c] enter internalSubset 4\n");
      state->handler = element0;
      return XML_ROLE_ELEMENT_NONE;
      // fprintf(stderr, "[xmlrole.c] exit internalSubset 4\n");
    }
    if (XmlNameMatchesAscii(enc, ptr + 2 * MIN_BYTES_PER_CHAR(enc), end,
                            KW_NOTATION)) {
      fprintf(stderr, "[xmlrole.c] enter internalSubset 5\n");
      state->handler = notation0;
      return XML_ROLE_NOTATION_NONE;
      // fprintf(stderr, "[xmlrole.c] exit internalSubset 5\n");
    }
    fprintf(stderr, "[xmlrole.c] enter internalSubset 6\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit internalSubset 6\n");
  case XML_TOK_PI:
    fprintf(stderr, "[xmlrole.c] enter internalSubset 7\n");
    return XML_ROLE_PI;
    // fprintf(stderr, "[xmlrole.c] exit internalSubset 7\n");
  case XML_TOK_COMMENT:
    fprintf(stderr, "[xmlrole.c] enter internalSubset 8\n");
    return XML_ROLE_COMMENT;
    // fprintf(stderr, "[xmlrole.c] exit internalSubset 8\n");
  case XML_TOK_PARAM_ENTITY_REF:
    fprintf(stderr, "[xmlrole.c] enter internalSubset 9\n");
    return XML_ROLE_PARAM_ENTITY_REF;
    // fprintf(stderr, "[xmlrole.c] exit internalSubset 9\n");
  case XML_TOK_CLOSE_BRACKET:
    fprintf(stderr, "[xmlrole.c] enter internalSubset 10\n");
    state->handler = doctype5;
    return XML_ROLE_DOCTYPE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit internalSubset 10\n");
  case XML_TOK_NONE:
    fprintf(stderr, "[xmlrole.c] enter internalSubset 11\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit internalSubset 11\n");
  }
  fprintf(stderr, "[xmlrole.c] enter internalSubset 12\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit internalSubset 12\n");
}

#ifdef XML_DTD

static int PTRCALL
externalSubset0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
                const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter externalSubset0 1\n");
  state->handler = externalSubset1;
  // fprintf(stderr, "[xmlrole.c] exit externalSubset0 1\n");
  if (tok == XML_TOK_XML_DECL) {
    fprintf(stderr, "[xmlrole.c] enter externalSubset0 2\n");
    return XML_ROLE_TEXT_DECL;
    // fprintf(stderr, "[xmlrole.c] exit externalSubset0 2\n");
  }
  fprintf(stderr, "[xmlrole.c] enter externalSubset0 3\n");
  return externalSubset1(state, tok, ptr, end, enc);
  // fprintf(stderr, "[xmlrole.c] exit externalSubset0 3\n");
}

static int PTRCALL
externalSubset1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
                const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_COND_SECT_OPEN:
    fprintf(stderr, "[xmlrole.c] enter externalSubset1 1\n");
    state->handler = condSect0;
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit externalSubset1 1\n");
  case XML_TOK_COND_SECT_CLOSE:
    if (state->includeLevel == 0) {
      fprintf(stderr, "[xmlrole.c] enter externalSubset1 2\n");
      break;
      // fprintf(stderr, "[xmlrole.c] exit externalSubset1 2\n");
    }
    fprintf(stderr, "[xmlrole.c] enter externalSubset1 3\n");
    state->includeLevel -= 1;
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit externalSubset1 3\n");
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter externalSubset1 4\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit externalSubset1 4\n");
  case XML_TOK_CLOSE_BRACKET:
    fprintf(stderr, "[xmlrole.c] enter externalSubset1 5\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit externalSubset1 5\n");
  case XML_TOK_NONE:
    if (state->includeLevel) {
      fprintf(stderr, "[xmlrole.c] enter externalSubset1 6\n");
      break;
      // fprintf(stderr, "[xmlrole.c] exit externalSubset1 6\n");
    }
    fprintf(stderr, "[xmlrole.c] enter externalSubset1 7\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit externalSubset1 7\n");
  default:
    fprintf(stderr, "[xmlrole.c] enter externalSubset1 8\n");
    return internalSubset(state, tok, ptr, end, enc);
    // fprintf(stderr, "[xmlrole.c] exit externalSubset1 8\n");
  }
  fprintf(stderr, "[xmlrole.c] enter externalSubset1 9\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit externalSubset1 9\n");
}

#endif /* XML_DTD */

static int PTRCALL
entity0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity0 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity0 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity0 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity0 2\n");
  case XML_TOK_PERCENT:
    fprintf(stderr, "[xmlrole.c] enter entity0 3\n");
    state->handler = entity1;
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity0 3\n");
  case XML_TOK_NAME:
    fprintf(stderr, "[xmlrole.c] enter entity0 4\n");
    state->handler = entity2;
    return XML_ROLE_GENERAL_ENTITY_NAME;
    // fprintf(stderr, "[xmlrole.c] exit entity0 4\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity0 5\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity0 5\n");
}

static int PTRCALL
entity1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity1 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity1 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity1 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity1 2\n");
  case XML_TOK_NAME:
    fprintf(stderr, "[xmlrole.c] enter entity1 3\n");
    state->handler = entity7;
    return XML_ROLE_PARAM_ENTITY_NAME;
    // fprintf(stderr, "[xmlrole.c] exit entity1 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity1 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity1 4\n");
}

static int PTRCALL
entity2(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity2 1\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity2 1\n");
  case XML_TOK_NAME:
    if (XmlNameMatchesAscii(enc, ptr, end, KW_SYSTEM)) {
      fprintf(stderr, "[xmlrole.c] enter entity2 2\n");
      state->handler = entity4;
      return XML_ROLE_ENTITY_NONE;
      // fprintf(stderr, "[xmlrole.c] exit entity2 2\n");
    }
    if (XmlNameMatchesAscii(enc, ptr, end, KW_PUBLIC)) {
      fprintf(stderr, "[xmlrole.c] enter entity2 3\n");
      state->handler = entity3;
      return XML_ROLE_ENTITY_NONE;
      // fprintf(stderr, "[xmlrole.c] exit entity2 3\n");
    }
    fprintf(stderr, "[xmlrole.c] enter entity2 4\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit entity2 4\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter entity2 5\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_ENTITY_NONE;
    return XML_ROLE_ENTITY_VALUE;
    // fprintf(stderr, "[xmlrole.c] exit entity2 5\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity2 6\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity2 6\n");
}

static int PTRCALL
entity3(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity3 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity3 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity3 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity3 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter entity3 3\n");
    state->handler = entity4;
    return XML_ROLE_ENTITY_PUBLIC_ID;
    // fprintf(stderr, "[xmlrole.c] exit entity3 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity3 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity3 4\n");
}

static int PTRCALL
entity4(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity4 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity4 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity4 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity4 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter entity4 3\n");
    state->handler = entity5;
    return XML_ROLE_ENTITY_SYSTEM_ID;
    // fprintf(stderr, "[xmlrole.c] exit entity4 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity4 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity4 4\n");
}

static int PTRCALL
entity5(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity5 1\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity5 1\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter entity5 2\n");
    setTopLevel(state);
    return XML_ROLE_ENTITY_COMPLETE;
    // fprintf(stderr, "[xmlrole.c] exit entity5 2\n");
  case XML_TOK_NAME:
    if (XmlNameMatchesAscii(enc, ptr, end, KW_NDATA)) {
      fprintf(stderr, "[xmlrole.c] enter entity5 3\n");
      state->handler = entity6;
      return XML_ROLE_ENTITY_NONE;
      // fprintf(stderr, "[xmlrole.c] exit entity5 3\n");
    }
    fprintf(stderr, "[xmlrole.c] enter entity5 4\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit entity5 4\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity5 5\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity5 5\n");
}
static int PTRCALL
entity6(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity6 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity6 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity6 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity6 2\n");
  case XML_TOK_NAME:
    fprintf(stderr, "[xmlrole.c] enter entity6 3\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_ENTITY_NONE;
    return XML_ROLE_ENTITY_NOTATION_NAME;
    // fprintf(stderr, "[xmlrole.c] exit entity6 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity6 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity6 4\n");
}

static int PTRCALL
entity7(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity7 1\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity7 1\n");
  case XML_TOK_NAME:
    if (XmlNameMatchesAscii(enc, ptr, end, KW_SYSTEM)) {
      fprintf(stderr, "[xmlrole.c] enter entity7 2\n");
      state->handler = entity9;
      return XML_ROLE_ENTITY_NONE;
      // fprintf(stderr, "[xmlrole.c] exit entity7 2\n");
    }
    if (XmlNameMatchesAscii(enc, ptr, end, KW_PUBLIC)) {
      fprintf(stderr, "[xmlrole.c] enter entity7 3\n");
      state->handler = entity8;
      return XML_ROLE_ENTITY_NONE;
      // fprintf(stderr, "[xmlrole.c] exit entity7 3\n");
    }
    fprintf(stderr, "[xmlrole.c] enter entity7 4\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit entity7 4\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter entity7 5\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_ENTITY_NONE;
    return XML_ROLE_ENTITY_VALUE;
    // fprintf(stderr, "[xmlrole.c] exit entity7 5\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity7 6\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity7 6\n");
}

static int PTRCALL
entity8(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity8 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity8 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity8 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity8 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter entity8 3\n");
    state->handler = entity9;
    return XML_ROLE_ENTITY_PUBLIC_ID;
    // fprintf(stderr, "[xmlrole.c] exit entity8 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity8 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity8 4\n");
}

static int PTRCALL
entity9(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
        const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity9 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity9 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity9 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity9 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter entity9 3\n");
    state->handler = entity10;
    return XML_ROLE_ENTITY_SYSTEM_ID;
    // fprintf(stderr, "[xmlrole.c] exit entity9 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity9 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity9 4\n");
}

static int PTRCALL
entity10(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter entity10 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit entity10 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter entity10 2\n");
    return XML_ROLE_ENTITY_NONE;
    // fprintf(stderr, "[xmlrole.c] exit entity10 2\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter entity10 3\n");
    setTopLevel(state);
    return XML_ROLE_ENTITY_COMPLETE;
    // fprintf(stderr, "[xmlrole.c] exit entity10 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter entity10 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit entity10 4\n");
}

static int PTRCALL
notation0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter notation0 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit notation0 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter notation0 2\n");
    return XML_ROLE_NOTATION_NONE;
    // fprintf(stderr, "[xmlrole.c] exit notation0 2\n");
  case XML_TOK_NAME:
    fprintf(stderr, "[xmlrole.c] enter notation0 3\n");
    state->handler = notation1;
    return XML_ROLE_NOTATION_NAME;
    // fprintf(stderr, "[xmlrole.c] exit notation0 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter notation0 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit notation0 4\n");
}

static int PTRCALL
notation1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter notation1 1\n");
    return XML_ROLE_NOTATION_NONE;
    // fprintf(stderr, "[xmlrole.c] exit notation1 1\n");
  case XML_TOK_NAME:
    if (XmlNameMatchesAscii(enc, ptr, end, KW_SYSTEM)) {
      fprintf(stderr, "[xmlrole.c] enter notation1 2\n");
      state->handler = notation3;
      return XML_ROLE_NOTATION_NONE;
      // fprintf(stderr, "[xmlrole.c] exit notation1 2\n");
    }
    if (XmlNameMatchesAscii(enc, ptr, end, KW_PUBLIC)) {
      fprintf(stderr, "[xmlrole.c] enter notation1 3\n");
      state->handler = notation2;
      return XML_ROLE_NOTATION_NONE;
      // fprintf(stderr, "[xmlrole.c] exit notation1 3\n");
    }
    fprintf(stderr, "[xmlrole.c] enter notation1 4\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit notation1 4\n");
  }
  fprintf(stderr, "[xmlrole.c] enter notation1 5\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit notation1 5\n");
}

static int PTRCALL
notation2(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter notation2 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit notation2 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter notation2 2\n");
    return XML_ROLE_NOTATION_NONE;
    // fprintf(stderr, "[xmlrole.c] exit notation2 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter notation2 3\n");
    state->handler = notation4;
    return XML_ROLE_NOTATION_PUBLIC_ID;
    // fprintf(stderr, "[xmlrole.c] exit notation2 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter notation2 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit notation2 4\n");
}

static int PTRCALL
notation3(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter notation3 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit notation3 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter notation3 2\n");
    return XML_ROLE_NOTATION_NONE;
    // fprintf(stderr, "[xmlrole.c] exit notation3 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter notation3 3\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_NOTATION_NONE;
    return XML_ROLE_NOTATION_SYSTEM_ID;
    // fprintf(stderr, "[xmlrole.c] exit notation3 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter notation3 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit notation3 4\n");
}

static int PTRCALL
notation4(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter notation4 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit notation4 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter notation4 2\n");
    return XML_ROLE_NOTATION_NONE;
    // fprintf(stderr, "[xmlrole.c] exit notation4 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter notation4 3\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_NOTATION_NONE;
    return XML_ROLE_NOTATION_SYSTEM_ID;
    // fprintf(stderr, "[xmlrole.c] exit notation4 3\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter notation4 4\n");
    setTopLevel(state);
    return XML_ROLE_NOTATION_NO_SYSTEM_ID;
    // fprintf(stderr, "[xmlrole.c] exit notation4 4\n");
  }
  fprintf(stderr, "[xmlrole.c] enter notation4 5\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit notation4 5\n");
}

static int PTRCALL
attlist0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter attlist0 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit attlist0 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist0 2\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist0 2\n");
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter attlist0 3\n");
    state->handler = attlist1;
    return XML_ROLE_ATTLIST_ELEMENT_NAME;
    // fprintf(stderr, "[xmlrole.c] exit attlist0 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist0 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist0 4\n");
}
static int PTRCALL
attlist1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist1 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist1 1\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter attlist1 2\n");
    setTopLevel(state);
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist1 2\n");
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter attlist1 3\n");
    state->handler = attlist2;
    return XML_ROLE_ATTRIBUTE_NAME;
    // fprintf(stderr, "[xmlrole.c] exit attlist1 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist1 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist1 4\n");
}

static int PTRCALL
attlist2(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist2 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist2 1\n");
  case XML_TOK_NAME: {
    fprintf(stderr, "[xmlrole.c] enter attlist2 2\n");
    static const char *const types[] = {
        KW_CDATA,  KW_ID,       KW_IDREF,   KW_IDREFS,
        KW_ENTITY, KW_ENTITIES, KW_NMTOKEN, KW_NMTOKENS,
    };
    int i;
    for (i = 0; i < (int)(sizeof(types) / sizeof(types[0])); i++)
      if (XmlNameMatchesAscii(enc, ptr, end, types[i])) {
        state->handler = attlist8;
        return XML_ROLE_ATTRIBUTE_TYPE_CDATA + i;
      }
  }
    if (XmlNameMatchesAscii(enc, ptr, end, KW_NOTATION)) {
      state->handler = attlist5;
      return XML_ROLE_ATTLIST_NONE;
    }
    break;
    // fprintf(stderr, "[xmlrole.c] exit attlist2 2\n");
  case XML_TOK_OPEN_PAREN:
    fprintf(stderr, "[xmlrole.c] enter attlist2 3\n");
    state->handler = attlist3;
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist2 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist2 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist2 4\n");
}

static int PTRCALL
attlist3(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist3 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist3 1\n");
  case XML_TOK_NMTOKEN:
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter attlist3 2\n");
    state->handler = attlist4;
    return XML_ROLE_ATTRIBUTE_ENUM_VALUE;
    // fprintf(stderr, "[xmlrole.c] exit attlist3 2\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist3 3\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist3 3\n");
}

static int PTRCALL
attlist4(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist4 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist4 1\n");
  case XML_TOK_CLOSE_PAREN:
    fprintf(stderr, "[xmlrole.c] enter attlist4 2\n");
    state->handler = attlist8;
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist4 2\n");
  case XML_TOK_OR:
    fprintf(stderr, "[xmlrole.c] enter attlist4 3\n");
    state->handler = attlist3;
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist4 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist4 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist4 4\n");
}

static int PTRCALL
attlist5(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist5 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist5 1\n");
  case XML_TOK_OPEN_PAREN:
    fprintf(stderr, "[xmlrole.c] enter attlist5 2\n");
    state->handler = attlist6;
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist5 2\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist5 3\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist5 3\n");
}

static int PTRCALL
attlist6(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist6 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist6 1\n");
  case XML_TOK_NAME:
    fprintf(stderr, "[xmlrole.c] enter attlist6 2\n");
    state->handler = attlist7;
    return XML_ROLE_ATTRIBUTE_NOTATION_VALUE;
    // fprintf(stderr, "[xmlrole.c] exit attlist6 2\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist6 3\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist6 3\n");
}

static int PTRCALL
attlist7(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist7 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist7 1\n");
  case XML_TOK_CLOSE_PAREN:
    fprintf(stderr, "[xmlrole.c] enter attlist7 2\n");
    state->handler = attlist8;
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist7 2\n");
  case XML_TOK_OR:
    fprintf(stderr, "[xmlrole.c] enter attlist7 3\n");
    state->handler = attlist6;
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist7 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist7 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist7 4\n");
}

/* default value */
static int PTRCALL
attlist8(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist8 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist8 1\n");
  case XML_TOK_POUND_NAME:
    fprintf(stderr, "[xmlrole.c] enter attlist8 2\n");
    if (XmlNameMatchesAscii(enc, ptr + MIN_BYTES_PER_CHAR(enc), end,
                            KW_IMPLIED)) {
      state->handler = attlist1;
      return XML_ROLE_IMPLIED_ATTRIBUTE_VALUE;
    }
    if (XmlNameMatchesAscii(enc, ptr + MIN_BYTES_PER_CHAR(enc), end,
                            KW_REQUIRED)) {
      state->handler = attlist1;
      return XML_ROLE_REQUIRED_ATTRIBUTE_VALUE;
    }
    if (XmlNameMatchesAscii(enc, ptr + MIN_BYTES_PER_CHAR(enc), end,
                            KW_FIXED)) {
      state->handler = attlist9;
      return XML_ROLE_ATTLIST_NONE;
    }
    break;
    // fprintf(stderr, "[xmlrole.c] exit attlist8 2\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter attlist8 3\n");
    state->handler = attlist1;
    return XML_ROLE_DEFAULT_ATTRIBUTE_VALUE;
    // fprintf(stderr, "[xmlrole.c] exit attlist8 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist8 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist8 4\n");
}

static int PTRCALL
attlist9(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter attlist9 1\n");
    return XML_ROLE_ATTLIST_NONE;
    // fprintf(stderr, "[xmlrole.c] exit attlist9 1\n");
  case XML_TOK_LITERAL:
    fprintf(stderr, "[xmlrole.c] enter attlist9 2\n");
    state->handler = attlist1;
    return XML_ROLE_FIXED_ATTRIBUTE_VALUE;
    // fprintf(stderr, "[xmlrole.c] exit attlist9 2\n");
  }
  fprintf(stderr, "[xmlrole.c] enter attlist9 3\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit attlist9 3\n");
}
static int PTRCALL
element0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter element0 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit element0 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element0 2\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element0 2\n");
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter element0 3\n");
    state->handler = element1;
    return XML_ROLE_ELEMENT_NAME;
    // fprintf(stderr, "[xmlrole.c] exit element0 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element0 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element0 4\n");
}

static int PTRCALL
element1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element1 1\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element1 1\n");
  case XML_TOK_NAME:
    if (XmlNameMatchesAscii(enc, ptr, end, KW_EMPTY)) {
      fprintf(stderr, "[xmlrole.c] enter element1 2\n");
      state->handler = declClose;
      state->role_none = XML_ROLE_ELEMENT_NONE;
      return XML_ROLE_CONTENT_EMPTY;
      // fprintf(stderr, "[xmlrole.c] exit element1 2\n");
    }
    if (XmlNameMatchesAscii(enc, ptr, end, KW_ANY)) {
      fprintf(stderr, "[xmlrole.c] enter element1 3\n");
      state->handler = declClose;
      state->role_none = XML_ROLE_ELEMENT_NONE;
      return XML_ROLE_CONTENT_ANY;
      // fprintf(stderr, "[xmlrole.c] exit element1 3\n");
    }
    fprintf(stderr, "[xmlrole.c] enter element1 4\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit element1 4\n");
  case XML_TOK_OPEN_PAREN:
    fprintf(stderr, "[xmlrole.c] enter element1 5\n");
    state->handler = element2;
    state->level = 1;
    return XML_ROLE_GROUP_OPEN;
    // fprintf(stderr, "[xmlrole.c] exit element1 5\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element1 6\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element1 6\n");
}

static int PTRCALL
element2(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element2 1\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element2 1\n");
  case XML_TOK_POUND_NAME:
    if (XmlNameMatchesAscii(enc, ptr + MIN_BYTES_PER_CHAR(enc), end,
                            KW_PCDATA)) {
      fprintf(stderr, "[xmlrole.c] enter element2 2\n");
      state->handler = element3;
      return XML_ROLE_CONTENT_PCDATA;
      // fprintf(stderr, "[xmlrole.c] exit element2 2\n");
    }
    fprintf(stderr, "[xmlrole.c] enter element2 3\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit element2 3\n");
  case XML_TOK_OPEN_PAREN:
    fprintf(stderr, "[xmlrole.c] enter element2 4\n");
    state->level = 2;
    state->handler = element6;
    return XML_ROLE_GROUP_OPEN;
    // fprintf(stderr, "[xmlrole.c] exit element2 4\n");
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter element2 5\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT;
    // fprintf(stderr, "[xmlrole.c] exit element2 5\n");
  case XML_TOK_NAME_QUESTION:
    fprintf(stderr, "[xmlrole.c] enter element2 6\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT_OPT;
    // fprintf(stderr, "[xmlrole.c] exit element2 6\n");
  case XML_TOK_NAME_ASTERISK:
    fprintf(stderr, "[xmlrole.c] enter element2 7\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT_REP;
    // fprintf(stderr, "[xmlrole.c] exit element2 7\n");
  case XML_TOK_NAME_PLUS:
    fprintf(stderr, "[xmlrole.c] enter element2 8\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT_PLUS;
    // fprintf(stderr, "[xmlrole.c] exit element2 8\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element2 9\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element2 9\n");
}

static int PTRCALL
element3(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter element3 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit element3 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element3 2\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element3 2\n");
  case XML_TOK_CLOSE_PAREN:
    fprintf(stderr, "[xmlrole.c] enter element3 3\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_ELEMENT_NONE;
    return XML_ROLE_GROUP_CLOSE;
    // fprintf(stderr, "[xmlrole.c] exit element3 3\n");
  case XML_TOK_CLOSE_PAREN_ASTERISK:
    fprintf(stderr, "[xmlrole.c] enter element3 4\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_ELEMENT_NONE;
    return XML_ROLE_GROUP_CLOSE_REP;
    // fprintf(stderr, "[xmlrole.c] exit element3 4\n");
  case XML_TOK_OR:
    fprintf(stderr, "[xmlrole.c] enter element3 5\n");
    state->handler = element4;
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element3 5\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element3 6\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element3 6\n");
}

static int PTRCALL
element4(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter element4 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit element4 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element4 2\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element4 2\n");
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter element4 3\n");
    state->handler = element5;
    return XML_ROLE_CONTENT_ELEMENT;
    // fprintf(stderr, "[xmlrole.c] exit element4 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element4 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element4 4\n");
}

static int PTRCALL
element5(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter element5 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit element5 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element5 2\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element5 2\n");
  case XML_TOK_CLOSE_PAREN_ASTERISK:
    fprintf(stderr, "[xmlrole.c] enter element5 3\n");
    state->handler = declClose;
    state->role_none = XML_ROLE_ELEMENT_NONE;
    return XML_ROLE_GROUP_CLOSE_REP;
    // fprintf(stderr, "[xmlrole.c] exit element5 3\n");
  case XML_TOK_OR:
    fprintf(stderr, "[xmlrole.c] enter element5 4\n");
    state->handler = element4;
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element5 4\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element5 5\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element5 5\n");
}

static int PTRCALL
element6(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter element6 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit element6 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element6 2\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element6 2\n");
  case XML_TOK_OPEN_PAREN:
    fprintf(stderr, "[xmlrole.c] enter element6 3\n");
    state->level += 1;
    return XML_ROLE_GROUP_OPEN;
    // fprintf(stderr, "[xmlrole.c] exit element6 3\n");
  case XML_TOK_NAME:
  case XML_TOK_PREFIXED_NAME:
    fprintf(stderr, "[xmlrole.c] enter element6 4\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT;
    // fprintf(stderr, "[xmlrole.c] exit element6 4\n");
  case XML_TOK_NAME_QUESTION:
    fprintf(stderr, "[xmlrole.c] enter element6 5\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT_OPT;
    // fprintf(stderr, "[xmlrole.c] exit element6 5\n");
  case XML_TOK_NAME_ASTERISK:
    fprintf(stderr, "[xmlrole.c] enter element6 6\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT_REP;
    // fprintf(stderr, "[xmlrole.c] exit element6 6\n");
  case XML_TOK_NAME_PLUS:
    fprintf(stderr, "[xmlrole.c] enter element6 7\n");
    state->handler = element7;
    return XML_ROLE_CONTENT_ELEMENT_PLUS;
    // fprintf(stderr, "[xmlrole.c] exit element6 7\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element6 8\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element6 8\n");
}
static int PTRCALL
element7(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
         const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter element7 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit element7 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter element7 2\n");
    return XML_ROLE_ELEMENT_NONE;
    // fprintf(stderr, "[xmlrole.c] exit element7 2\n");
  case XML_TOK_CLOSE_PAREN:
    fprintf(stderr, "[xmlrole.c] enter element7 3\n");
    state->level -= 1;
    // fprintf(stderr, "[xmlrole.c] exit element7 3\n");
    if (state->level == 0) {
      fprintf(stderr, "[xmlrole.c] enter element7 4\n");
      state->handler = declClose;
      state->role_none = XML_ROLE_ELEMENT_NONE;
      // fprintf(stderr, "[xmlrole.c] exit element7 4\n");
    }
    fprintf(stderr, "[xmlrole.c] enter element7 5\n");
    return XML_ROLE_GROUP_CLOSE;
    // fprintf(stderr, "[xmlrole.c] exit element7 5\n");
  case XML_TOK_CLOSE_PAREN_ASTERISK:
    fprintf(stderr, "[xmlrole.c] enter element7 6\n");
    state->level -= 1;
    // fprintf(stderr, "[xmlrole.c] exit element7 6\n");
    if (state->level == 0) {
      fprintf(stderr, "[xmlrole.c] enter element7 7\n");
      state->handler = declClose;
      state->role_none = XML_ROLE_ELEMENT_NONE;
      // fprintf(stderr, "[xmlrole.c] exit element7 7\n");
    }
    fprintf(stderr, "[xmlrole.c] enter element7 8\n");
    return XML_ROLE_GROUP_CLOSE_REP;
    // fprintf(stderr, "[xmlrole.c] exit element7 8\n");
  case XML_TOK_CLOSE_PAREN_QUESTION:
    fprintf(stderr, "[xmlrole.c] enter element7 9\n");
    state->level -= 1;
    // fprintf(stderr, "[xmlrole.c] exit element7 9\n");
    if (state->level == 0) {
      fprintf(stderr, "[xmlrole.c] enter element7 10\n");
      state->handler = declClose;
      state->role_none = XML_ROLE_ELEMENT_NONE;
      // fprintf(stderr, "[xmlrole.c] exit element7 10\n");
    }
    fprintf(stderr, "[xmlrole.c] enter element7 11\n");
    return XML_ROLE_GROUP_CLOSE_OPT;
    // fprintf(stderr, "[xmlrole.c] exit element7 11\n");
  case XML_TOK_CLOSE_PAREN_PLUS:
    fprintf(stderr, "[xmlrole.c] enter element7 12\n");
    state->level -= 1;
    // fprintf(stderr, "[xmlrole.c] exit element7 12\n");
    if (state->level == 0) {
      fprintf(stderr, "[xmlrole.c] enter element7 13\n");
      state->handler = declClose;
      state->role_none = XML_ROLE_ELEMENT_NONE;
      // fprintf(stderr, "[xmlrole.c] exit element7 13\n");
    }
    fprintf(stderr, "[xmlrole.c] enter element7 14\n");
    return XML_ROLE_GROUP_CLOSE_PLUS;
    // fprintf(stderr, "[xmlrole.c] exit element7 14\n");
  case XML_TOK_COMMA:
    fprintf(stderr, "[xmlrole.c] enter element7 15\n");
    state->handler = element6;
    return XML_ROLE_GROUP_SEQUENCE;
    // fprintf(stderr, "[xmlrole.c] exit element7 15\n");
  case XML_TOK_OR:
    fprintf(stderr, "[xmlrole.c] enter element7 16\n");
    state->handler = element6;
    return XML_ROLE_GROUP_CHOICE;
    // fprintf(stderr, "[xmlrole.c] exit element7 16\n");
  }
  fprintf(stderr, "[xmlrole.c] enter element7 17\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit element7 17\n");
}

#ifdef XML_DTD

static int PTRCALL
condSect0(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter condSect0 1\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit condSect0 1\n");
  case XML_TOK_NAME:
    if (XmlNameMatchesAscii(enc, ptr, end, KW_INCLUDE)) {
      fprintf(stderr, "[xmlrole.c] enter condSect0 2\n");
      state->handler = condSect1;
      return XML_ROLE_NONE;
      // fprintf(stderr, "[xmlrole.c] exit condSect0 2\n");
    }
    if (XmlNameMatchesAscii(enc, ptr, end, KW_IGNORE)) {
      fprintf(stderr, "[xmlrole.c] enter condSect0 3\n");
      state->handler = condSect2;
      return XML_ROLE_NONE;
      // fprintf(stderr, "[xmlrole.c] exit condSect0 3\n");
    }
    fprintf(stderr, "[xmlrole.c] enter condSect0 4\n");
    break;
    // fprintf(stderr, "[xmlrole.c] exit condSect0 4\n");
  }
  fprintf(stderr, "[xmlrole.c] enter condSect0 5\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit condSect0 5\n");
}

static int PTRCALL
condSect1(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter condSect1 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit condSect1 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter condSect1 2\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit condSect1 2\n");
  case XML_TOK_OPEN_BRACKET:
    fprintf(stderr, "[xmlrole.c] enter condSect1 3\n");
    state->handler = externalSubset1;
    state->includeLevel += 1;
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit condSect1 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter condSect1 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit condSect1 4\n");
}

static int PTRCALL
condSect2(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter condSect2 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit condSect2 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter condSect2 2\n");
    return XML_ROLE_NONE;
    // fprintf(stderr, "[xmlrole.c] exit condSect2 2\n");
  case XML_TOK_OPEN_BRACKET:
    fprintf(stderr, "[xmlrole.c] enter condSect2 3\n");
    state->handler = externalSubset1;
    return XML_ROLE_IGNORE_SECT;
    // fprintf(stderr, "[xmlrole.c] exit condSect2 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter condSect2 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit condSect2 4\n");
}

#endif /* XML_DTD */

static int PTRCALL
declClose(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
          const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter declClose 1\n");
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  // fprintf(stderr, "[xmlrole.c] exit declClose 1\n");
  switch (tok) {
  case XML_TOK_PROLOG_S:
    fprintf(stderr, "[xmlrole.c] enter declClose 2\n");
    return state->role_none;
    // fprintf(stderr, "[xmlrole.c] exit declClose 2\n");
  case XML_TOK_DECL_CLOSE:
    fprintf(stderr, "[xmlrole.c] enter declClose 3\n");
    setTopLevel(state);
    return state->role_none;
    // fprintf(stderr, "[xmlrole.c] exit declClose 3\n");
  }
  fprintf(stderr, "[xmlrole.c] enter declClose 4\n");
  return common(state, tok);
  // fprintf(stderr, "[xmlrole.c] exit declClose 4\n");
}

/* This function will only be invoked if the internal logic of the
 * parser has broken down.  It is used in two cases:
 *
 * 1: When the XML prolog has been finished.  At this point the
 * processor (the parser level above these role handlers) should
 * switch from prologProcessor to contentProcessor and reinitialise
 * the handler function.
 *
 * 2: When an error has been detected (via common() below).  At this
 * point again the processor should be switched to errorProcessor,
 * which will never call a handler.
 *
 * The result of this is that error() can only be called if the
 * processor switch failed to happen, which is an internal error and
 * therefore we shouldn't be able to provoke it simply by using the
 * library.  It is a necessary backstop, however, so we merely exclude
 * it from the coverage statistics.
 *
 * LCOV_EXCL_START
 */
static int PTRCALL
error(PROLOG_STATE *state, int tok, const char *ptr, const char *end,
      const ENCODING *enc) {
  fprintf(stderr, "[xmlrole.c] enter error 1\n");
  UNUSED_P(state);
  UNUSED_P(tok);
  UNUSED_P(ptr);
  UNUSED_P(end);
  UNUSED_P(enc);
  return XML_ROLE_NONE;
  // fprintf(stderr, "[xmlrole.c] exit error 1\n");
}
/* LCOV_EXCL_STOP */

static int FASTCALL
common(PROLOG_STATE *state, int tok) {
#ifdef XML_DTD
  if (! state->documentEntity && tok == XML_TOK_PARAM_ENTITY_REF)
  {
    fprintf(stderr, "[xmlrole.c] enter common 1\n");
    return XML_ROLE_INNER_PARAM_ENTITY_REF;
    // fprintf(stderr, "[xmlrole.c] exit common 1\n");
  }
#else
  UNUSED_P(tok);
#endif
  fprintf(stderr, "[xmlrole.c] enter common 2\n");
  state->handler = error;
  return XML_ROLE_ERROR;
  // fprintf(stderr, "[xmlrole.c] exit common 2\n");
}

void
XmlPrologStateInit(PROLOG_STATE *state) {
  fprintf(stderr, "[xmlrole.c] enter XmlPrologStateInit 1\n");
  state->handler = prolog0;
  // fprintf(stderr, "[xmlrole.c] exit XmlPrologStateInit 1\n");
#ifdef XML_DTD
  fprintf(stderr, "[xmlrole.c] enter XmlPrologStateInit 2\n");
  state->documentEntity = 1;
  state->includeLevel = 0;
  state->inEntityValue = 0;
  // fprintf(stderr, "[xmlrole.c] exit XmlPrologStateInit 2\n");
#endif /* XML_DTD */
}

#ifdef XML_DTD

void
XmlPrologStateInitExternalEntity(PROLOG_STATE *state) {
  fprintf(stderr, "[xmlrole.c] enter XmlPrologStateInitExternalEntity 1\n");
  state->handler = externalSubset0;
  state->documentEntity = 0;
  state->includeLevel = 0;
  // fprintf(stderr, "[xmlrole.c] exit XmlPrologStateInitExternalEntity 1\n");
}

#endif /* XML_DTD */
// Total cost: 1.150637
// Total split cost: 0.079030, input tokens: 22736, output tokens: 5061, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 169), (169, 331), (331, 523), (523, 718), (718, 903), (903, 1070), (1070, 1254)]
// Total instrumented cost: 1.071607, input tokens: 48530, output tokens: 104248, cache read tokens: 0, cache write tokens: 0
