#include <stdio.h>

/* This file is included!
                            __  __            _
                         ___\ \/ /_ __   __ _| |_
                        / _ \\  /| '_ \ / _` | __|
                       |  __//  \| |_) | (_| | |_
                        \___/_/\_\ .__/ \__,_|\__|
                                 |_| XML parser

   Copyright (c) 1997-2000 Thai Open Source Software Center Ltd
   Copyright (c) 2000      Clark Cooper <coopercc@users.sourceforge.net>
   Copyright (c) 2002      Greg Stein <gstein@users.sourceforge.net>
   Copyright (c) 2002      Fred L. Drake, Jr. <fdrake@users.sourceforge.net>
   Copyright (c) 2002-2006 Karl Waclawek <karl@waclawek.net>
   Copyright (c) 2017-2021 Sebastian Pipping <sebastian@pipping.org>
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

#ifdef XML_TOK_NS_C

const ENCODING *
NS(XmlGetUtf8InternalEncoding)(void) {
  fprintf(stderr, "[xmltok_ns.c] enter XmlGetUtf8InternalEncoding 1\n");
  return &ns(internal_utf8_encoding).enc;
  // fprintf(stderr, "[xmltok_ns.c] exit XmlGetUtf8InternalEncoding 1\n");
}

const ENCODING *
NS(XmlGetUtf16InternalEncoding)(void) {
#  if BYTEORDER == 1234
  fprintf(stderr, "[xmltok_ns.c] enter XmlGetUtf16InternalEncoding 1\n");
  return &ns(internal_little2_encoding).enc;
  // fprintf(stderr, "[xmltok_ns.c] exit XmlGetUtf16InternalEncoding 1\n");
#  elif BYTEORDER == 4321
  fprintf(stderr, "[xmltok_ns.c] enter XmlGetUtf16InternalEncoding 2\n");
  return &ns(internal_big2_encoding).enc;
  // fprintf(stderr, "[xmltok_ns.c] exit XmlGetUtf16InternalEncoding 2\n");
#  else
  fprintf(stderr, "[xmltok_ns.c] enter XmlGetUtf16InternalEncoding 3\n");
  const short n = 1;
  return (*(const char *)&n ? &ns(internal_little2_encoding).enc
                            : &ns(internal_big2_encoding).enc);
  // fprintf(stderr, "[xmltok_ns.c] exit XmlGetUtf16InternalEncoding 3\n");
#  endif
}

static const ENCODING *const NS(encodings)[] = {
    &ns(latin1_encoding).enc, &ns(ascii_encoding).enc,
    &ns(utf8_encoding).enc,   &ns(big2_encoding).enc,
    &ns(big2_encoding).enc,   &ns(little2_encoding).enc,
    &ns(utf8_encoding).enc /* NO_ENC */
};

static int PTRCALL
NS(initScanProlog)(const ENCODING *enc, const char *ptr, const char *end,
                   const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_ns.c] enter initScanProlog 1\n");
  return initScan(NS(encodings), (const INIT_ENCODING *)enc, XML_PROLOG_STATE,
                  ptr, end, nextTokPtr);
  // fprintf(stderr, "[xmltok_ns.c] exit initScanProlog 1\n");
}

static int PTRCALL
NS(initScanContent)(const ENCODING *enc, const char *ptr, const char *end,
                    const char **nextTokPtr) {
  fprintf(stderr, "[xmltok_ns.c] enter initScanContent 1\n");
  return initScan(NS(encodings), (const INIT_ENCODING *)enc, XML_CONTENT_STATE,
                  ptr, end, nextTokPtr);
  // fprintf(stderr, "[xmltok_ns.c] exit initScanContent 1\n");
}

int
NS(XmlInitEncoding)(INIT_ENCODING *p, const ENCODING **encPtr,
                    const char *name) {
  fprintf(stderr, "[xmltok_ns.c] enter XmlInitEncoding 1\n");
  int i = getEncodingIndex(name);
  // fprintf(stderr, "[xmltok_ns.c] exit XmlInitEncoding 1\n");
  
  if (i == UNKNOWN_ENC)
  {
    fprintf(stderr, "[xmltok_ns.c] enter XmlInitEncoding 2\n");
    return 0;
    // fprintf(stderr, "[xmltok_ns.c] exit XmlInitEncoding 2\n");
  }
  
  fprintf(stderr, "[xmltok_ns.c] enter XmlInitEncoding 3\n");
  SET_INIT_ENC_INDEX(p, i);
  p->initEnc.scanners[XML_PROLOG_STATE] = NS(initScanProlog);
  p->initEnc.scanners[XML_CONTENT_STATE] = NS(initScanContent);
  p->initEnc.updatePosition = initUpdatePosition;
  p->encPtr = encPtr;
  *encPtr = &(p->initEnc);
  return 1;
  // fprintf(stderr, "[xmltok_ns.c] exit XmlInitEncoding 3\n");
}

static const ENCODING *
NS(findEncoding)(const ENCODING *enc, const char *ptr, const char *end) {
  fprintf(stderr, "[xmltok_ns.c] enter findEncoding 1\n");
#  define ENCODING_MAX 128
  char buf[ENCODING_MAX] = "";
  char *p = buf;
  int i;
  XmlUtf8Convert(enc, &ptr, end, &p, p + ENCODING_MAX - 1);
  // fprintf(stderr, "[xmltok_ns.c] exit findEncoding 1\n");
  
  if (ptr != end)
  {
    fprintf(stderr, "[xmltok_ns.c] enter findEncoding 2\n");
    return NULL;
    // fprintf(stderr, "[xmltok_ns.c] exit findEncoding 2\n");
  }
  
  fprintf(stderr, "[xmltok_ns.c] enter findEncoding 3\n");
  *p = 0;
  // fprintf(stderr, "[xmltok_ns.c] exit findEncoding 3\n");
  
  if (streqci(buf, KW_UTF_16) && enc->minBytesPerChar == 2)
  {
    fprintf(stderr, "[xmltok_ns.c] enter findEncoding 4\n");
    return enc;
    // fprintf(stderr, "[xmltok_ns.c] exit findEncoding 4\n");
  }
  
  fprintf(stderr, "[xmltok_ns.c] enter findEncoding 5\n");
  i = getEncodingIndex(buf);
  // fprintf(stderr, "[xmltok_ns.c] exit findEncoding 5\n");
  
  if (i == UNKNOWN_ENC)
  {
    fprintf(stderr, "[xmltok_ns.c] enter findEncoding 6\n");
    return NULL;
    // fprintf(stderr, "[xmltok_ns.c] exit findEncoding 6\n");
  }
  
  fprintf(stderr, "[xmltok_ns.c] enter findEncoding 7\n");
  return NS(encodings)[i];
  // fprintf(stderr, "[xmltok_ns.c] exit findEncoding 7\n");
}

int
NS(XmlParseXmlDecl)(int isGeneralTextEntity, const ENCODING *enc,
                    const char *ptr, const char *end, const char **badPtr,
                    const char **versionPtr, const char **versionEndPtr,
                    const char **encodingName, const ENCODING **encoding,
                    int *standalone) {
  fprintf(stderr, "[xmltok_ns.c] enter XmlParseXmlDecl 1\n");
  return doParseXmlDecl(NS(findEncoding), isGeneralTextEntity, enc, ptr, end,
                        badPtr, versionPtr, versionEndPtr, encodingName,
                        encoding, standalone);
  // fprintf(stderr, "[xmltok_ns.c] exit XmlParseXmlDecl 1\n");
}

#endif /* XML_TOK_NS_C */
// Total cost: 0.107144
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 122)]
// Total instrumented cost: 0.107144, input tokens: 6, output tokens: 4571, cache read tokens: 0, cache write tokens: 10283
