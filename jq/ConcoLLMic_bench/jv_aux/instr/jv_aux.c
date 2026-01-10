#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "jv.h"
#include "jv_alloc.h"
#include "jv_private.h"

// making this static verbose function here
// until we introduce a less confusing naming scheme
// of jv_* API with regards to the memory management
static double jv_number_get_value_and_consume(jv number) {
  fprintf(stderr, "[jv_aux.c] enter jv_number_get_value_and_consume 1\n");
  double value = jv_number_value(number);
  jv_free(number);
  return value;
  // fprintf(stderr, "[jv_aux.c] exit jv_number_get_value_and_consume 1\n");
}

static jv parse_slice(jv j, jv slice, int* pstart, int* pend) {
  fprintf(stderr, "[jv_aux.c] enter parse_slice 1\n");
  // Array slices
  jv start_jv = jv_object_get(jv_copy(slice), jv_string("start"));
  jv end_jv = jv_object_get(slice, jv_string("end"));
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 1\n");
  if (jv_get_kind(start_jv) == JV_KIND_NULL) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 2\n");
    jv_free(start_jv);
    start_jv = jv_number(0);
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 2\n");
  }
  fprintf(stderr, "[jv_aux.c] enter parse_slice 3\n");
  int len;
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 3\n");
  if (jv_get_kind(j) == JV_KIND_ARRAY) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 4\n");
    len = jv_array_length(j);
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 4\n");
  } else if (jv_get_kind(j) == JV_KIND_STRING) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 5\n");
    len = jv_string_length_codepoints(j);
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 5\n");
  } else {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 6\n");
    /*
     * XXX This should be dead code because callers shouldn't call this
     * function if `j' is neither an array nor a string.
     */
    jv_free(j);
    jv_free(start_jv);
    jv_free(end_jv);
    return jv_invalid_with_msg(jv_string("Only arrays and strings can be sliced"));
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 6\n");
  }
  fprintf(stderr, "[jv_aux.c] enter parse_slice 7\n");
  if (jv_get_kind(end_jv) == JV_KIND_NULL) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 8\n");
    jv_free(end_jv);
    end_jv = jv_number(len);
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 8\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 7\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 9\n");
  if (jv_get_kind(start_jv) != JV_KIND_NUMBER ||
      jv_get_kind(end_jv) != JV_KIND_NUMBER) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 10\n");
    jv_free(start_jv);
    jv_free(end_jv);
    return jv_invalid_with_msg(jv_string("Array/string slice indices must be integers"));
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 10\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 9\n");

  fprintf(stderr, "[jv_aux.c] enter parse_slice 11\n");
  double dstart = jv_number_value(start_jv);
  double dend = jv_number_value(end_jv);
  int start, end;

  jv_free(start_jv);
  jv_free(end_jv);
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 11\n");
  if (isnan(dstart)) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 12\n");
    dstart = 0;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 12\n");
  }
  fprintf(stderr, "[jv_aux.c] enter parse_slice 13\n");
  if (dstart < 0) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 14\n");
    dstart += len;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 14\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 13\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 15\n");
  if (dstart < 0) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 16\n");
    dstart = 0;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 16\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 15\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 17\n");
  if (dstart > len) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 18\n");
    dstart = len;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 18\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 17\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 19\n");
  start = dstart > INT_MAX ? INT_MAX : (int)dstart; // Rounds down
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 19\n");

  fprintf(stderr, "[jv_aux.c] enter parse_slice 20\n");
  if (isnan(dend)) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 21\n");
    dend = len;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 21\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 20\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 22\n");
  if (dend < 0) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 23\n");
    dend += len;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 23\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 22\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 24\n");
  if (dend < 0) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 25\n");
    dend  = start;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 25\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 24\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 26\n");
  end = dend > INT_MAX ? INT_MAX : (int)dend;
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 26\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 27\n");
  if (end > len) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 28\n");
    end = len;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 28\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 27\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 29\n");
  if (end < len) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 30\n");
    end += end < dend ? 1 : 0; // We round start down
                                                // but round end up
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 30\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 29\n");

  fprintf(stderr, "[jv_aux.c] enter parse_slice 31\n");
  if (end < start) {
    fprintf(stderr, "[jv_aux.c] enter parse_slice 32\n");
    end = start;
    // fprintf(stderr, "[jv_aux.c] exit parse_slice 32\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 31\n");
  fprintf(stderr, "[jv_aux.c] enter parse_slice 33\n");
  assert(0 <= start && start <= end && end <= len);
  *pstart = start;
  *pend = end;
  return jv_true();
  // fprintf(stderr, "[jv_aux.c] exit parse_slice 33\n");
}

jv jv_get(jv t, jv k) {
  fprintf(stderr, "[jv_aux.c] enter jv_get 1\n");
  jv v;
  // fprintf(stderr, "[jv_aux.c] exit jv_get 1\n");
  if (jv_get_kind(t) == JV_KIND_OBJECT && jv_get_kind(k) == JV_KIND_STRING) {
    fprintf(stderr, "[jv_aux.c] enter jv_get 2\n");
    v = jv_object_get(t, k);
    // fprintf(stderr, "[jv_aux.c] exit jv_get 2\n");
    if (!jv_is_valid(v)) {
      fprintf(stderr, "[jv_aux.c] enter jv_get 3\n");
      jv_free(v);
      v = jv_null();
      // fprintf(stderr, "[jv_aux.c] exit jv_get 3\n");
    }
  } else if (jv_get_kind(t) == JV_KIND_ARRAY && jv_get_kind(k) == JV_KIND_NUMBER) {
    fprintf(stderr, "\n");
    if (jvp_number_is_nan(k)) {
      fprintf(stderr, "[jv_aux.c] enter jv_get 5\n");
      jv_free(t);
      v = jv_null();
      // fprintf(stderr, "[jv_aux.c] exit jv_get 5\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_get 6\n");
      double didx = jv_number_value(k);
      // fprintf(stderr, "[jv_aux.c] exit jv_get 6\n");
      if (jvp_number_is_nan(k)) {
        fprintf(stderr, "[jv_aux.c] enter jv_get 7\n");
        v = jv_null();
        // fprintf(stderr, "[jv_aux.c] exit jv_get 7\n");
      } else {
        fprintf(stderr, "[jv_aux.c] enter jv_get 8\n");
        if (didx < INT_MIN) {
          fprintf(stderr, "[jv_aux.c] enter jv_get 9\n");
          didx = INT_MIN;
          // fprintf(stderr, "[jv_aux.c] exit jv_get 9\n");
        }
        // fprintf(stderr, "[jv_aux.c] exit jv_get 8\n");
        fprintf(stderr, "[jv_aux.c] enter jv_get 10\n");
        if (didx > INT_MAX) {
          fprintf(stderr, "[jv_aux.c] enter jv_get 11\n");
          didx = INT_MAX;
          // fprintf(stderr, "[jv_aux.c] exit jv_get 11\n");
        }
        // fprintf(stderr, "[jv_aux.c] exit jv_get 10\n");
        fprintf(stderr, "[jv_aux.c] enter jv_get 12\n");
        int idx = (int)didx;
        // fprintf(stderr, "[jv_aux.c] exit jv_get 12\n");
        if (idx < 0) {
          fprintf(stderr, "[jv_aux.c] enter jv_get 13\n");
          idx += jv_array_length(jv_copy(t));
          // fprintf(stderr, "[jv_aux.c] exit jv_get 13\n");
        }
        fprintf(stderr, "[jv_aux.c] enter jv_get 14\n");
        v = jv_array_get(t, idx);
        // fprintf(stderr, "[jv_aux.c] exit jv_get 14\n");
        if (!jv_is_valid(v)) {
          fprintf(stderr, "[jv_aux.c] enter jv_get 15\n");
          jv_free(v);
          v = jv_null();
          // fprintf(stderr, "[jv_aux.c] exit jv_get 15\n");
        }
      }
    }
    fprintf(stderr, "[jv_aux.c] enter jv_get 16\n");
    jv_free(k);
    // fprintf(stderr, "[jv_aux.c] exit jv_get 16\n");
  } else if (jv_get_kind(t) == JV_KIND_ARRAY && jv_get_kind(k) == JV_KIND_OBJECT) {
    fprintf(stderr, "[jv_aux.c] enter jv_get 17\n");
    int start, end;
    jv e = parse_slice(jv_copy(t), k, &start, &end);
    // fprintf(stderr, "[jv_aux.c] exit jv_get 17\n");
    if (jv_get_kind(e) == JV_KIND_TRUE) {
      fprintf(stderr, "[jv_aux.c] enter jv_get 18\n");
      v = jv_array_slice(t, start, end);
      // fprintf(stderr, "[jv_aux.c] exit jv_get 18\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_get 19\n");
      jv_free(t);
      v = e;
      // fprintf(stderr, "[jv_aux.c] exit jv_get 19\n");
    }
  } else if (jv_get_kind(t) == JV_KIND_STRING && jv_get_kind(k) == JV_KIND_OBJECT) {
    fprintf(stderr, "[jv_aux.c] enter jv_get 20\n");
    int start, end;
    jv e = parse_slice(jv_copy(t), k, &start, &end);
    // fprintf(stderr, "[jv_aux.c] exit jv_get 20\n");
    if (jv_get_kind(e) == JV_KIND_TRUE) {
      fprintf(stderr, "[jv_aux.c] enter jv_get 21\n");
      v = jv_string_slice(t, start, end);
      // fprintf(stderr, "[jv_aux.c] exit jv_get 21\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_get 22\n");
      jv_free(t);
      v = e;
      // fprintf(stderr, "[jv_aux.c] exit jv_get 22\n");
    }
  } else if (jv_get_kind(t) == JV_KIND_ARRAY && jv_get_kind(k) == JV_KIND_ARRAY) {
    fprintf(stderr, "[jv_aux.c] enter jv_get 23\n");
    v = jv_array_indexes(t, k);
    // fprintf(stderr, "[jv_aux.c] exit jv_get 23\n");
  } else if (jv_get_kind(t) == JV_KIND_NULL &&
             (jv_get_kind(k) == JV_KIND_STRING ||
              jv_get_kind(k) == JV_KIND_NUMBER ||
              jv_get_kind(k) == JV_KIND_OBJECT)) {
    fprintf(stderr, "[jv_aux.c] enter jv_get 24\n");
    jv_free(t);
    jv_free(k);
    v = jv_null();
    // fprintf(stderr, "[jv_aux.c] exit jv_get 24\n");
  } else {
    fprintf(stderr, "[jv_aux.c] enter jv_get 25\n");
    /*
     * If k is a short string it's probably from a jq .foo expression or
     * similar, in which case putting it in the invalid msg may help the
     * user.  The length 30 is arbitrary.
     */
    if (jv_get_kind(k) == JV_KIND_STRING && jv_string_length_bytes(jv_copy(k)) < 30) {
      fprintf(stderr, "[jv_aux.c] enter jv_get 26\n");
      v = jv_invalid_with_msg(jv_string_fmt("Cannot index %s with string \"%s\"",
                                            jv_kind_name(jv_get_kind(t)),
                                            jv_string_value(k)));
      // fprintf(stderr, "[jv_aux.c] exit jv_get 26\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_get 27\n");
      v = jv_invalid_with_msg(jv_string_fmt("Cannot index %s with %s",
                                            jv_kind_name(jv_get_kind(t)),
                                            jv_kind_name(jv_get_kind(k))));
      // fprintf(stderr, "[jv_aux.c] exit jv_get 27\n");
    }
    jv_free(t);
    jv_free(k);
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_get 25\n");
  fprintf(stderr, "[jv_aux.c] enter jv_get 28\n");
  return v;
  // fprintf(stderr, "[jv_aux.c] exit jv_get 28\n");
}
jv jv_set(jv t, jv k, jv v) {
  fprintf(stderr, "[jv_aux.c] enter jv_set 1\n");
  if (!jv_is_valid(v)) {
    fprintf(stderr, "[jv_aux.c] enter jv_set 2\n");
    jv_free(t);
    jv_free(k);
    return v;
    // fprintf(stderr, "[jv_aux.c] exit jv_set 2\n");
  }
  fprintf(stderr, "[jv_aux.c] enter jv_set 3\n");
  int isnull = jv_get_kind(t) == JV_KIND_NULL;
  // fprintf(stderr, "[jv_aux.c] exit jv_set 3\n");
  if (jv_get_kind(k) == JV_KIND_STRING &&
      (jv_get_kind(t) == JV_KIND_OBJECT || isnull)) {
    fprintf(stderr, "[jv_aux.c] enter jv_set 4\n");
    if (isnull) t = jv_object();
    t = jv_object_set(t, k, v);
    // fprintf(stderr, "[jv_aux.c] exit jv_set 4\n");
  } else if (jv_get_kind(k) == JV_KIND_NUMBER &&
             (jv_get_kind(t) == JV_KIND_ARRAY || isnull)) {
    fprintf(stderr, "[jv_aux.c] enter jv_set 5\n");
    if (jvp_number_is_nan(k)) {
      fprintf(stderr, "[jv_aux.c] enter jv_set 6\n");
      jv_free(t);
      jv_free(k);
      t = jv_invalid_with_msg(jv_string("Cannot set array element at NaN index"));
      // fprintf(stderr, "[jv_aux.c] exit jv_set 6\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_set 7\n");
      double didx = jv_number_value(k);
      if (didx < INT_MIN) didx = INT_MIN;
      if (didx > INT_MAX) didx = INT_MAX;
      if (isnull) t = jv_array();
      t = jv_array_set(t, (int)didx, v);
      jv_free(k);
      // fprintf(stderr, "[jv_aux.c] exit jv_set 7\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_set 5\n");
  } else if (jv_get_kind(k) == JV_KIND_OBJECT &&
             (jv_get_kind(t) == JV_KIND_ARRAY || isnull)) {
    fprintf(stderr, "[jv_aux.c] enter jv_set 8\n");
    if (isnull) t = jv_array();
    int start, end;
    jv e = parse_slice(jv_copy(t), k, &start, &end);
    if (jv_get_kind(e) == JV_KIND_TRUE) {
      fprintf(stderr, "[jv_aux.c] enter jv_set 9\n");
      if (jv_get_kind(v) == JV_KIND_ARRAY) {
        fprintf(stderr, "[jv_aux.c] enter jv_set 10\n");
        int array_len = jv_array_length(jv_copy(t));
        assert(0 <= start && start <= end && end <= array_len);
        int slice_len = end - start;
        int insert_len = jv_array_length(jv_copy(v));
        if (slice_len < insert_len) {
          fprintf(stderr, "[jv_aux.c] enter jv_set 11\n");
          // array is growing
          int shift = insert_len - slice_len;
          for (int i = array_len - 1; i >= end && jv_is_valid(t); i--) {
            fprintf(stderr, "[jv_aux.c] enter jv_set 12\n");
            t = jv_array_set(t, i + shift, jv_array_get(jv_copy(t), i));
            // fprintf(stderr, "[jv_aux.c] exit jv_set 12\n");
          }
          // fprintf(stderr, "[jv_aux.c] exit jv_set 11\n");
        } else if (slice_len > insert_len) {
          fprintf(stderr, "[jv_aux.c] enter jv_set 13\n");
          // array is shrinking
          int shift = slice_len - insert_len;
          for (int i = end; i < array_len && jv_is_valid(t); i++) {
            fprintf(stderr, "[jv_aux.c] enter jv_set 14\n");
            t = jv_array_set(t, i - shift, jv_array_get(jv_copy(t), i));
            // fprintf(stderr, "[jv_aux.c] exit jv_set 14\n");
          }
          if (jv_is_valid(t))
            t = jv_array_slice(t, 0, array_len - shift);
          // fprintf(stderr, "[jv_aux.c] exit jv_set 13\n");
        }
        fprintf(stderr, "[jv_aux.c] enter jv_set 15\n");
        for (int i = 0; i < insert_len && jv_is_valid(t); i++) {
          fprintf(stderr, "[jv_aux.c] enter jv_set 16\n");
          t = jv_array_set(t, start + i, jv_array_get(jv_copy(v), i));
          // fprintf(stderr, "[jv_aux.c] exit jv_set 16\n");
        }
        jv_free(v);
        // fprintf(stderr, "[jv_aux.c] exit jv_set 15\n");
        // fprintf(stderr, "[jv_aux.c] exit jv_set 10\n");
      } else {
        fprintf(stderr, "[jv_aux.c] enter jv_set 17\n");
        jv_free(t);
        jv_free(v);
        t = jv_invalid_with_msg(jv_string_fmt("A slice of an array can only be assigned another array"));
        // fprintf(stderr, "[jv_aux.c] exit jv_set 17\n");
      }
      // fprintf(stderr, "[jv_aux.c] exit jv_set 9\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_set 18\n");
      jv_free(t);
      jv_free(v);
      t = e;
      // fprintf(stderr, "[jv_aux.c] exit jv_set 18\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_set 8\n");
  } else if (jv_get_kind(k) == JV_KIND_OBJECT && jv_get_kind(t) == JV_KIND_STRING) {
    fprintf(stderr, "[jv_aux.c] enter jv_set 19\n");
    jv_free(t);
    jv_free(k);
    jv_free(v);
    /* Well, why not?  We should implement this... */
    t = jv_invalid_with_msg(jv_string_fmt("Cannot update string slices"));
    // fprintf(stderr, "[jv_aux.c] exit jv_set 19\n");
  } else {
    fprintf(stderr, "[jv_aux.c] enter jv_set 20\n");
    jv err = jv_invalid_with_msg(jv_string_fmt("Cannot update field at %s index of %s",
                                               jv_kind_name(jv_get_kind(k)),
                                               jv_kind_name(jv_get_kind(t))));
    jv_free(t);
    jv_free(k);
    jv_free(v);
    t = err;
    // fprintf(stderr, "[jv_aux.c] exit jv_set 20\n");
  }
  return t;
  // fprintf(stderr, "[jv_aux.c] exit jv_set 1\n");
}

jv jv_has(jv t, jv k) {
  fprintf(stderr, "[jv_aux.c] enter jv_has 1\n");
  assert(jv_is_valid(t));
  assert(jv_is_valid(k));
  jv ret;
  if (jv_get_kind(t) == JV_KIND_NULL) {
    fprintf(stderr, "[jv_aux.c] enter jv_has 2\n");
    jv_free(t);
    jv_free(k);
    ret = jv_false();
    // fprintf(stderr, "[jv_aux.c] exit jv_has 2\n");
  } else if (jv_get_kind(t) == JV_KIND_OBJECT &&
             jv_get_kind(k) == JV_KIND_STRING) {
    fprintf(stderr, "[jv_aux.c] enter jv_has 3\n");
    jv elem = jv_object_get(t, k);
    ret = jv_bool(jv_is_valid(elem));
    jv_free(elem);
    // fprintf(stderr, "[jv_aux.c] exit jv_has 3\n");
  } else if (jv_get_kind(t) == JV_KIND_ARRAY &&
             jv_get_kind(k) == JV_KIND_NUMBER) {
    fprintf(stderr, "[jv_aux.c] enter jv_has 4\n");
    if (jvp_number_is_nan(k)) {
      fprintf(stderr, "[jv_aux.c] enter jv_has 5\n");
      jv_free(t);
      ret = jv_false();
      // fprintf(stderr, "[jv_aux.c] exit jv_has 5\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_has 6\n");
      jv elem = jv_array_get(t, (int)jv_number_value(k));
      ret = jv_bool(jv_is_valid(elem));
      jv_free(elem);
      // fprintf(stderr, "[jv_aux.c] exit jv_has 6\n");
    }
    jv_free(k);
    // fprintf(stderr, "[jv_aux.c] exit jv_has 4\n");
  } else {
    fprintf(stderr, "[jv_aux.c] enter jv_has 7\n");
    ret = jv_invalid_with_msg(jv_string_fmt("Cannot check whether %s has a %s key",
                                            jv_kind_name(jv_get_kind(t)),
                                            jv_kind_name(jv_get_kind(k))));
    jv_free(t);
    jv_free(k);
    // fprintf(stderr, "[jv_aux.c] exit jv_has 7\n");
  }
  return ret;
  // fprintf(stderr, "[jv_aux.c] exit jv_has 1\n");
}
// assumes keys is a sorted array
static jv jv_dels(jv t, jv keys) {
  fprintf(stderr, "[jv_aux.c] enter jv_dels 1\n");
  assert(jv_get_kind(keys) == JV_KIND_ARRAY);
  assert(jv_is_valid(t));
  // fprintf(stderr, "[jv_aux.c] exit jv_dels 1\n");

  if (jv_get_kind(t) == JV_KIND_NULL || jv_array_length(jv_copy(keys)) == 0) {
    fprintf(stderr, "[jv_aux.c] enter jv_dels 2\n");
    // no change
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 2\n");
  } else if (jv_get_kind(t) == JV_KIND_ARRAY) {
    fprintf(stderr, "[jv_aux.c] enter jv_dels 3\n");
    // extract slices, they must be handled differently
    jv neg_keys = jv_array();
    jv nonneg_keys = jv_array();
    jv new_array = jv_array();
    jv starts = jv_array(), ends = jv_array();
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 3\n");
    
    fprintf(stderr, "[jv_aux.c] enter jv_dels 4\n");
    jv_array_foreach(keys, i, key) {
      fprintf(stderr, "[jv_aux.c] enter jv_dels 5\n");
      if (jv_get_kind(key) == JV_KIND_NUMBER) {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 6\n");
        if (jv_number_value(key) < 0) {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 7\n");
          neg_keys = jv_array_append(neg_keys, key);
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 7\n");
        } else {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 8\n");
          nonneg_keys = jv_array_append(nonneg_keys, key);
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 8\n");
        }
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 6\n");
      } else if (jv_get_kind(key) == JV_KIND_OBJECT) {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 9\n");
        int start, end;
        jv e = parse_slice(jv_copy(t), key, &start, &end);
        if (jv_get_kind(e) == JV_KIND_TRUE) {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 10\n");
          starts = jv_array_append(starts, jv_number(start));
          ends = jv_array_append(ends, jv_number(end));
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 10\n");
        } else {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 11\n");
          jv_free(new_array);
          jv_free(key);
          new_array = e;
          goto arr_out;
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 11\n");
        }
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 9\n");
      } else {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 12\n");
        jv_free(new_array);
        new_array = jv_invalid_with_msg(jv_string_fmt("Cannot delete %s element of array",
                                                      jv_kind_name(jv_get_kind(key))));
        jv_free(key);
        goto arr_out;
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 12\n");
      }
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 5\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 4\n");

    fprintf(stderr, "[jv_aux.c] enter jv_dels 13\n");
    int neg_idx = 0;
    int nonneg_idx = 0;
    int len = jv_array_length(jv_copy(t));
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 13\n");
    
    fprintf(stderr, "[jv_aux.c] enter jv_dels 14\n");
    for (int i = 0; i < len; ++i) {
      fprintf(stderr, "[jv_aux.c] enter jv_dels 15\n");
      int del = 0;
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 15\n");
      
      fprintf(stderr, "[jv_aux.c] enter jv_dels 16\n");
      while (neg_idx < jv_array_length(jv_copy(neg_keys))) {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 17\n");
        int delidx = len + (int)jv_number_get_value_and_consume(jv_array_get(jv_copy(neg_keys), neg_idx));
        if (i == delidx) {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 18\n");
          del = 1;
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 18\n");
        }
        if (i < delidx) {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 19\n");
          break;
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 19\n");
        }
        fprintf(stderr, "[jv_aux.c] enter jv_dels 20\n");
        neg_idx++;
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 20\n");
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 17\n");
      }
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 16\n");
      
      fprintf(stderr, "[jv_aux.c] enter jv_dels 21\n");
      while (nonneg_idx < jv_array_length(jv_copy(nonneg_keys))) {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 22\n");
        int delidx = (int)jv_number_get_value_and_consume(jv_array_get(jv_copy(nonneg_keys), nonneg_idx));
        if (i == delidx) {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 23\n");
          del = 1;
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 23\n");
        }
        if (i < delidx) {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 24\n");
          break;
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 24\n");
        }
        fprintf(stderr, "[jv_aux.c] enter jv_dels 25\n");
        nonneg_idx++;
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 25\n");
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 22\n");
      }
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 21\n");
      
      fprintf(stderr, "[jv_aux.c] enter jv_dels 26\n");
      for (int sidx=0; !del && sidx<jv_array_length(jv_copy(starts)); sidx++) {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 27\n");
        if ((int)jv_number_get_value_and_consume(jv_array_get(jv_copy(starts), sidx)) <= i &&
            i < (int)jv_number_get_value_and_consume(jv_array_get(jv_copy(ends), sidx))) {
          fprintf(stderr, "[jv_aux.c] enter jv_dels 28\n");
          del = 1;
          // fprintf(stderr, "[jv_aux.c] exit jv_dels 28\n");
        }
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 27\n");
      }
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 26\n");
      
      fprintf(stderr, "[jv_aux.c] enter jv_dels 29\n");
      if (!del) {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 30\n");
        new_array = jv_array_append(new_array, jv_array_get(jv_copy(t), i));
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 30\n");
      }
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 29\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 14\n");

  arr_out:
    fprintf(stderr, "[jv_aux.c] enter jv_dels 31\n");
    jv_free(neg_keys);
    jv_free(nonneg_keys);
    jv_free(starts);
    jv_free(ends);
    jv_free(t);
    t = new_array;
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 31\n");
  } else if (jv_get_kind(t) == JV_KIND_OBJECT) {
    fprintf(stderr, "[jv_aux.c] enter jv_dels 32\n");
    jv_array_foreach(keys, i, k) {
      fprintf(stderr, "[jv_aux.c] enter jv_dels 33\n");
      if (jv_get_kind(k) != JV_KIND_STRING) {
        fprintf(stderr, "[jv_aux.c] enter jv_dels 34\n");
        jv_free(t);
        t = jv_invalid_with_msg(jv_string_fmt("Cannot delete %s field of object",
                                              jv_kind_name(jv_get_kind(k))));
        jv_free(k);
        break;
        // fprintf(stderr, "[jv_aux.c] exit jv_dels 34\n");
      }
      fprintf(stderr, "[jv_aux.c] enter jv_dels 35\n");
      t = jv_object_delete(t, k);
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 35\n");
      // fprintf(stderr, "[jv_aux.c] exit jv_dels 33\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 32\n");
  } else {
    fprintf(stderr, "[jv_aux.c] enter jv_dels 36\n");
    jv err = jv_invalid_with_msg(jv_string_fmt("Cannot delete fields from %s",
                                               jv_kind_name(jv_get_kind(t))));
    jv_free(t);
    t = err;
    // fprintf(stderr, "[jv_aux.c] exit jv_dels 36\n");
  }
  fprintf(stderr, "[jv_aux.c] enter jv_dels 37\n");
  jv_free(keys);
  return t;
  // fprintf(stderr, "[jv_aux.c] exit jv_dels 37\n");
}

jv jv_setpath(jv root, jv path, jv value) {
  fprintf(stderr, "[jv_aux.c] enter jv_setpath 1\n");
  if (jv_get_kind(path) != JV_KIND_ARRAY) {
    fprintf(stderr, "[jv_aux.c] enter jv_setpath 2\n");
    jv_free(value);
    jv_free(root);
    jv_free(path);
    return jv_invalid_with_msg(jv_string("Path must be specified as an array"));
    // fprintf(stderr, "[jv_aux.c] exit jv_setpath 2\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_setpath 1\n");
  
  fprintf(stderr, "[jv_aux.c] enter jv_setpath 3\n");
  if (!jv_is_valid(root)){
    fprintf(stderr, "[jv_aux.c] enter jv_setpath 4\n");
    jv_free(value);
    jv_free(path);
    return root;
    // fprintf(stderr, "[jv_aux.c] exit jv_setpath 4\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_setpath 3\n");
  
  fprintf(stderr, "[jv_aux.c] enter jv_setpath 5\n");
  if (jv_array_length(jv_copy(path)) == 0) {
    fprintf(stderr, "[jv_aux.c] enter jv_setpath 6\n");
    jv_free(path);
    jv_free(root);
    return value;
    // fprintf(stderr, "[jv_aux.c] exit jv_setpath 6\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_setpath 5\n");
  
  fprintf(stderr, "[jv_aux.c] enter jv_setpath 7\n");
  jv pathcurr = jv_array_get(jv_copy(path), 0);
  jv pathrest = jv_array_slice(path, 1, jv_array_length(jv_copy(path)));
  // fprintf(stderr, "[jv_aux.c] exit jv_setpath 7\n");

  /*
   * We need to be careful not to make extra copies since that leads to
   * quadratic behavior (e.g., when growing large data structures in a
   * reduction with `setpath/2`, i.e., with `|=`.
   */
  if (jv_get_kind(pathcurr) == JV_KIND_OBJECT) {
    fprintf(stderr, "[jv_aux.c] enter jv_setpath 8\n");
    // Assignment to slice -- dunno yet how to avoid the extra copy
    return jv_set(root, pathcurr,
                  jv_setpath(jv_get(jv_copy(root), jv_copy(pathcurr)), pathrest, value));
    // fprintf(stderr, "[jv_aux.c] exit jv_setpath 8\n");
  }

  fprintf(stderr, "[jv_aux.c] enter jv_setpath 9\n");
  jv subroot = jv_get(jv_copy(root), jv_copy(pathcurr));
  if (!jv_is_valid(subroot)) {
    fprintf(stderr, "[jv_aux.c] enter jv_setpath 10\n");
    jv_free(root);
    jv_free(pathcurr);
    jv_free(pathrest);
    jv_free(value);
    return subroot;
    // fprintf(stderr, "[jv_aux.c] exit jv_setpath 10\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_setpath 9\n");

  fprintf(stderr, "[jv_aux.c] enter jv_setpath 11\n");
  // To avoid the extra copy we drop the reference from `root` by setting that
  // to null first.
  root = jv_set(root, jv_copy(pathcurr), jv_null());
  if (!jv_is_valid(root)) {
    fprintf(stderr, "[jv_aux.c] enter jv_setpath 12\n");
    jv_free(subroot);
    jv_free(pathcurr);
    jv_free(pathrest);
    jv_free(value);
    return root;
    // fprintf(stderr, "[jv_aux.c] exit jv_setpath 12\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_setpath 11\n");
  
  fprintf(stderr, "[jv_aux.c] enter jv_setpath 13\n");
  return jv_set(root, pathcurr, jv_setpath(subroot, pathrest, value));
  // fprintf(stderr, "[jv_aux.c] exit jv_setpath 13\n");
}

jv jv_getpath(jv root, jv path) {
  fprintf(stderr, "[jv_aux.c] enter jv_getpath 1\n");
  if (jv_get_kind(path) != JV_KIND_ARRAY) {
    fprintf(stderr, "[jv_aux.c] enter jv_getpath 2\n");
    jv_free(root);
    jv_free(path);
    return jv_invalid_with_msg(jv_string("Path must be specified as an array"));
    // fprintf(stderr, "[jv_aux.c] exit jv_getpath 2\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_getpath 1\n");
  
  fprintf(stderr, "[jv_aux.c] enter jv_getpath 3\n");
  if (!jv_is_valid(root)) {
    fprintf(stderr, "[jv_aux.c] enter jv_getpath 4\n");
    jv_free(path);
    return root;
    // fprintf(stderr, "[jv_aux.c] exit jv_getpath 4\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_getpath 3\n");
  
  fprintf(stderr, "[jv_aux.c] enter jv_getpath 5\n");
  if (jv_array_length(jv_copy(path)) == 0) {
    fprintf(stderr, "[jv_aux.c] enter jv_getpath 6\n");
    jv_free(path);
    return root;
    // fprintf(stderr, "[jv_aux.c] exit jv_getpath 6\n");
  }
  // fprintf(stderr, "[jv_aux.c] exit jv_getpath 5\n");
  
  fprintf(stderr, "[jv_aux.c] enter jv_getpath 7\n");
  jv pathcurr = jv_array_get(jv_copy(path), 0);
  jv pathrest = jv_array_slice(path, 1, jv_array_length(jv_copy(path)));
  return jv_getpath(jv_get(root, pathcurr), pathrest);
  // fprintf(stderr, "[jv_aux.c] exit jv_getpath 7\n");
}
// assumes paths is a sorted array of arrays
static jv delpaths_sorted(jv object, jv paths, int start) {
  fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 1\n");
  jv delkeys = jv_array();
  // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 1\n");
  for (int i=0; i<jv_array_length(jv_copy(paths));) {
    fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 2\n");
    int j = i;
    assert(jv_array_length(jv_array_get(jv_copy(paths), i)) > start);
    int delkey = jv_array_length(jv_array_get(jv_copy(paths), i)) == start + 1;
    jv key = jv_array_get(jv_array_get(jv_copy(paths), i), start);
    // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 2\n");
    while (j < jv_array_length(jv_copy(paths)) &&
           jv_equal(jv_copy(key), jv_array_get(jv_array_get(jv_copy(paths), j), start)))
    {
      fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 3\n");
      j++;
      // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 3\n");
    }
    fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 4\n");
    // if i <= entry < j, then entry starts with key
    // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 4\n");
    if (delkey) {
      fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 5\n");
      // deleting this entire key, we don't care about any more specific deletions
      delkeys = jv_array_append(delkeys, key);
      // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 5\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 6\n");
      // deleting certain sub-parts of this key
      jv subobject = jv_get(jv_copy(object), jv_copy(key));
      // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 6\n");
      if (!jv_is_valid(subobject)) {
        fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 7\n");
        jv_free(key);
        jv_free(object);
        object = subobject;
        break;
        // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 7\n");
      } else if (jv_get_kind(subobject) == JV_KIND_NULL) {
        fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 8\n");
        jv_free(key);
        jv_free(subobject);
        // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 8\n");
      } else {
        fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 9\n");
        jv newsubobject = delpaths_sorted(subobject, jv_array_slice(jv_copy(paths), i, j), start+1);
        // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 9\n");
        if (!jv_is_valid(newsubobject)) {
          fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 10\n");
          jv_free(key);
          jv_free(object);
          object = newsubobject;
          break;
          // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 10\n");
        }
        fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 11\n");
        object = jv_set(object, key, newsubobject);
        // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 11\n");
      }
      fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 12\n");
      if (!jv_is_valid(object)) break;
      // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 12\n");
    }
    fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 13\n");
    i = j;
    // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 13\n");
  }
  fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 14\n");
  jv_free(paths);
  // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 14\n");
  if (jv_is_valid(object))
  {
    fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 15\n");
    object = jv_dels(object, delkeys);
    // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 15\n");
  }
  else
  {
    fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 16\n");
    jv_free(delkeys);
    // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 16\n");
  }
  fprintf(stderr, "[jv_aux.c] enter delpaths_sorted 17\n");
  return object;
  // fprintf(stderr, "[jv_aux.c] exit delpaths_sorted 17\n");
}

jv jv_delpaths(jv object, jv paths) {
  fprintf(stderr, "\n");
  // fprintf(stderr, "\n");
  if (jv_get_kind(paths) != JV_KIND_ARRAY) {
    fprintf(stderr, "[jv_aux.c] enter jv_delpaths 2\n");
    jv_free(object);
    jv_free(paths);
    return jv_invalid_with_msg(jv_string("Paths must be specified as an array"));
    // fprintf(stderr, "[jv_aux.c] exit jv_delpaths 2\n");
  }
  fprintf(stderr, "[jv_aux.c] enter jv_delpaths 3\n");
  paths = jv_sort(paths, jv_copy(paths));
  // fprintf(stderr, "[jv_aux.c] exit jv_delpaths 3\n");
  jv_array_foreach(paths, i, elem) {
    fprintf(stderr, "\n");
    // fprintf(stderr, "\n");
    if (jv_get_kind(elem) != JV_KIND_ARRAY) {
      fprintf(stderr, "[jv_aux.c] enter jv_delpaths 5\n");
      jv_free(object);
      jv_free(paths);
      jv err = jv_invalid_with_msg(jv_string_fmt("Path must be specified as array, not %s",
                                                 jv_kind_name(jv_get_kind(elem))));
      jv_free(elem);
      return err;
      // fprintf(stderr, "[jv_aux.c] exit jv_delpaths 5\n");
    }
    fprintf(stderr, "[jv_aux.c] enter jv_delpaths 6\n");
    jv_free(elem);
    // fprintf(stderr, "[jv_aux.c] exit jv_delpaths 6\n");
  }
  fprintf(stderr, "\n");
  // fprintf(stderr, "\n");
  if (jv_array_length(jv_copy(paths)) == 0) {
    fprintf(stderr, "[jv_aux.c] enter jv_delpaths 8\n");
    // nothing is being deleted
    jv_free(paths);
    return object;
    // fprintf(stderr, "[jv_aux.c] exit jv_delpaths 8\n");
  }
  fprintf(stderr, "\n");
  // fprintf(stderr, "\n");
  if (jv_array_length(jv_array_get(jv_copy(paths), 0)) == 0) {
    fprintf(stderr, "[jv_aux.c] enter jv_delpaths 10\n");
    // everything is being deleted
    jv_free(paths);
    jv_free(object);
    return jv_null();
    // fprintf(stderr, "[jv_aux.c] exit jv_delpaths 10\n");
  }
  fprintf(stderr, "[jv_aux.c] enter jv_delpaths 11\n");
  return delpaths_sorted(object, paths, 0);
  // fprintf(stderr, "[jv_aux.c] exit jv_delpaths 11\n");
}


static int string_cmp(const void* pa, const void* pb){
  fprintf(stderr, "[jv_aux.c] enter string_cmp 1\n");
  const jv* a = pa;
  const jv* b = pb;
  int lena = jv_string_length_bytes(jv_copy(*a));
  int lenb = jv_string_length_bytes(jv_copy(*b));
  int minlen = lena < lenb ? lena : lenb;
  int r = memcmp(jv_string_value(*a), jv_string_value(*b), minlen);
  if (r == 0) r = lena - lenb;
  return r;
  // fprintf(stderr, "[jv_aux.c] exit string_cmp 1\n");
}

jv jv_keys_unsorted(jv x) {
  fprintf(stderr, "\n");
  // fprintf(stderr, "\n");
  if (jv_get_kind(x) != JV_KIND_OBJECT) {
    fprintf(stderr, "[jv_aux.c] enter jv_keys_unsorted 2\n");
    return jv_keys(x);
    // fprintf(stderr, "[jv_aux.c] exit jv_keys_unsorted 2\n");
  }
  fprintf(stderr, "[jv_aux.c] enter jv_keys_unsorted 3\n");
  jv answer = jv_array_sized(jv_object_length(jv_copy(x)));
  // fprintf(stderr, "[jv_aux.c] exit jv_keys_unsorted 3\n");
  jv_object_foreach(x, key, value) {
    fprintf(stderr, "[jv_aux.c] enter jv_keys_unsorted 4\n");
    answer = jv_array_append(answer, key);
    jv_free(value);
    // fprintf(stderr, "[jv_aux.c] exit jv_keys_unsorted 4\n");
  }
  fprintf(stderr, "[jv_aux.c] enter jv_keys_unsorted 5\n");
  jv_free(x);
  return answer;
  // fprintf(stderr, "[jv_aux.c] exit jv_keys_unsorted 5\n");
}

jv jv_keys(jv x) {
  fprintf(stderr, "\n");
  // fprintf(stderr, "\n");
  if (jv_get_kind(x) == JV_KIND_OBJECT) {
    fprintf(stderr, "[jv_aux.c] enter jv_keys 2\n");
    int nkeys = jv_object_length(jv_copy(x));
    // fprintf(stderr, "[jv_aux.c] exit jv_keys 2\n");
    if (nkeys == 0) {
      fprintf(stderr, "[jv_aux.c] enter jv_keys 3\n");
      jv_free(x);
      return jv_array();
      // fprintf(stderr, "[jv_aux.c] exit jv_keys 3\n");
    }
    fprintf(stderr, "[jv_aux.c] enter jv_keys 4\n");
    jv* keys = jv_mem_calloc(nkeys, sizeof(jv));
    int kidx = 0;
    // fprintf(stderr, "[jv_aux.c] exit jv_keys 4\n");
    jv_object_foreach(x, key, value) {
      fprintf(stderr, "[jv_aux.c] enter jv_keys 5\n");
      keys[kidx++] = key;
      jv_free(value);
      // fprintf(stderr, "[jv_aux.c] exit jv_keys 5\n");
    }
    fprintf(stderr, "[jv_aux.c] enter jv_keys 6\n");
    qsort(keys, nkeys, sizeof(jv), string_cmp);
    jv answer = jv_array_sized(nkeys);
    // fprintf(stderr, "[jv_aux.c] exit jv_keys 6\n");
    for (int i = 0; i<nkeys; i++) {
      fprintf(stderr, "[jv_aux.c] enter jv_keys 7\n");
      answer = jv_array_append(answer, keys[i]);
      // fprintf(stderr, "[jv_aux.c] exit jv_keys 7\n");
    }
    fprintf(stderr, "[jv_aux.c] enter jv_keys 8\n");
    jv_mem_free(keys);
    jv_free(x);
    return answer;
    // fprintf(stderr, "[jv_aux.c] exit jv_keys 8\n");
  } else if (jv_get_kind(x) == JV_KIND_ARRAY) {
    fprintf(stderr, "[jv_aux.c] enter jv_keys 9\n");
    int n = jv_array_length(x);
    jv answer = jv_array();
    // fprintf(stderr, "[jv_aux.c] exit jv_keys 9\n");
    for (int i=0; i<n; i++){
      fprintf(stderr, "[jv_aux.c] enter jv_keys 10\n");
      answer = jv_array_set(answer, i, jv_number(i));
      // fprintf(stderr, "[jv_aux.c] exit jv_keys 10\n");
    }
    fprintf(stderr, "[jv_aux.c] enter jv_keys 11\n");
    return answer;
    // fprintf(stderr, "[jv_aux.c] exit jv_keys 11\n");
  } else {
    fprintf(stderr, "[jv_aux.c] enter jv_keys 12\n");
    assert(0 && "jv_keys passed something neither object nor array");
    return jv_invalid();
    // fprintf(stderr, "[jv_aux.c] exit jv_keys 12\n");
  }
}
int jv_cmp(jv a, jv b) {
  fprintf(stderr, "\n");
  if (jv_get_kind(a) != jv_get_kind(b)) {
    fprintf(stderr, "[jv_aux.c] enter jv_cmp 2\n");
    int r = (int)jv_get_kind(a) - (int)jv_get_kind(b);
    jv_free(a);
    jv_free(b);
    return r;
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 2\n");
  }
  fprintf(stderr, "[jv_aux.c] enter jv_cmp 3\n");
  int r = 0;
  // fprintf(stderr, "[jv_aux.c] exit jv_cmp 3\n");
  switch (jv_get_kind(a)) {
  default:
    fprintf(stderr, "[jv_aux.c] enter jv_cmp 4\n");
    assert(0 && "invalid kind passed to jv_cmp");
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 4\n");
  case JV_KIND_NULL:
  case JV_KIND_FALSE:
  case JV_KIND_TRUE:
    fprintf(stderr, "[jv_aux.c] enter jv_cmp 5\n");
    // there's only one of each of these values
    r = 0;
    break;
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 5\n");

  case JV_KIND_NUMBER: {
    fprintf(stderr, "[jv_aux.c] enter jv_cmp 6\n");
    if (jvp_number_is_nan(a)) {
      fprintf(stderr, "[jv_aux.c] enter jv_cmp 7\n");
      r = jv_cmp(jv_null(), jv_copy(b));
      // fprintf(stderr, "[jv_aux.c] exit jv_cmp 7\n");
    } else if (jvp_number_is_nan(b)) {
      fprintf(stderr, "[jv_aux.c] enter jv_cmp 8\n");
      r = jv_cmp(jv_copy(a), jv_null());
      // fprintf(stderr, "[jv_aux.c] exit jv_cmp 8\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_cmp 9\n");
      r = jvp_number_cmp(a, b);
      // fprintf(stderr, "[jv_aux.c] exit jv_cmp 9\n");
    }
    break;
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 6\n");
  }

  case JV_KIND_STRING: {
    fprintf(stderr, "[jv_aux.c] enter jv_cmp 10\n");
    r = string_cmp(&a, &b);
    break;
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 10\n");
  }

  case JV_KIND_ARRAY: {
    fprintf(stderr, "[jv_aux.c] enter jv_cmp 11\n");
    // Lexical ordering of arrays
    int i = 0;
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 11\n");
    
    while (r == 0) {
      fprintf(stderr, "[jv_aux.c] enter jv_cmp 12\n");
      int a_done = i >= jv_array_length(jv_copy(a));
      int b_done = i >= jv_array_length(jv_copy(b));
      if (a_done || b_done) {
        fprintf(stderr, "[jv_aux.c] enter jv_cmp 13\n");
        r = b_done - a_done; //suddenly, logic
        break;
        // fprintf(stderr, "[jv_aux.c] exit jv_cmp 13\n");
      }
      fprintf(stderr, "[jv_aux.c] enter jv_cmp 14\n");
      jv xa = jv_array_get(jv_copy(a), i);
      jv xb = jv_array_get(jv_copy(b), i);
      r = jv_cmp(xa, xb);
      i++;
      // fprintf(stderr, "[jv_aux.c] exit jv_cmp 14\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 12\n");
    break;
  }

  case JV_KIND_OBJECT: {
    fprintf(stderr, "[jv_aux.c] enter jv_cmp 15\n");
    jv keys_a = jv_keys(jv_copy(a));
    jv keys_b = jv_keys(jv_copy(b));
    r = jv_cmp(jv_copy(keys_a), keys_b);
    if (r == 0) {
      fprintf(stderr, "[jv_aux.c] enter jv_cmp 16\n");
      jv_array_foreach(keys_a, i, key) {
        fprintf(stderr, "[jv_aux.c] enter jv_cmp 17\n");
        jv xa = jv_object_get(jv_copy(a), jv_copy(key));
        jv xb = jv_object_get(jv_copy(b), key);
        r = jv_cmp(xa, xb);
        if (r) break;
        // fprintf(stderr, "[jv_aux.c] exit jv_cmp 17\n");
      }
      // fprintf(stderr, "[jv_aux.c] exit jv_cmp 16\n");
    }
    jv_free(keys_a);
    break;
    // fprintf(stderr, "[jv_aux.c] exit jv_cmp 15\n");
  }
  }

  fprintf(stderr, "[jv_aux.c] enter jv_cmp 18\n");
  jv_free(a);
  jv_free(b);
  return r;
  // fprintf(stderr, "[jv_aux.c] exit jv_cmp 18\n");
}


struct sort_entry {
  jv object;
  jv key;
  int index;
};

static int sort_cmp(const void* pa, const void* pb) {
  fprintf(stderr, "[jv_aux.c] enter sort_cmp 1\n");
  const struct sort_entry* a = pa;
  const struct sort_entry* b = pb;
  int r = jv_cmp(jv_copy(a->key), jv_copy(b->key));
  // comparing by index if r == 0 makes the sort stable
  return r ? r : (a->index - b->index);
  // fprintf(stderr, "[jv_aux.c] exit sort_cmp 1\n");
}

static struct sort_entry* sort_items(jv objects, jv keys) {
  fprintf(stderr, "\n");
  assert(jv_get_kind(objects) == JV_KIND_ARRAY);
  assert(jv_get_kind(keys) == JV_KIND_ARRAY);
  assert(jv_array_length(jv_copy(objects)) == jv_array_length(jv_copy(keys)));
  int n = jv_array_length(jv_copy(objects));
  if (n == 0) {
    fprintf(stderr, "[jv_aux.c] enter sort_items 2\n");
    jv_free(objects);
    jv_free(keys);
    return NULL;
    // fprintf(stderr, "[jv_aux.c] exit sort_items 2\n");
  }
  fprintf(stderr, "[jv_aux.c] enter sort_items 3\n");
  struct sort_entry* entries = jv_mem_calloc(n, sizeof(struct sort_entry));
  for (int i=0; i<n; i++) {
    fprintf(stderr, "[jv_aux.c] enter sort_items 4\n");
    entries[i].object = jv_array_get(jv_copy(objects), i);
    entries[i].key = jv_array_get(jv_copy(keys), i);
    entries[i].index = i;
    // fprintf(stderr, "[jv_aux.c] exit sort_items 4\n");
  }
  jv_free(objects);
  jv_free(keys);
  qsort(entries, n, sizeof(struct sort_entry), sort_cmp);
  return entries;
  // fprintf(stderr, "[jv_aux.c] exit sort_items 3\n");
}

jv jv_sort(jv objects, jv keys) {
  fprintf(stderr, "[jv_aux.c] enter jv_sort 1\n");
  assert(jv_get_kind(objects) == JV_KIND_ARRAY);
  assert(jv_get_kind(keys) == JV_KIND_ARRAY);
  assert(jv_array_length(jv_copy(objects)) == jv_array_length(jv_copy(keys)));
  int n = jv_array_length(jv_copy(objects));
  struct sort_entry* entries = sort_items(objects, keys);
  jv ret = jv_array();
  for (int i=0; i<n; i++) {
    fprintf(stderr, "[jv_aux.c] enter jv_sort 2\n");
    jv_free(entries[i].key);
    ret = jv_array_set(ret, i, entries[i].object);
    // fprintf(stderr, "[jv_aux.c] exit jv_sort 2\n");
  }
  jv_mem_free(entries);
  return ret;
  // fprintf(stderr, "[jv_aux.c] exit jv_sort 1\n");
}

jv jv_group(jv objects, jv keys) {
  fprintf(stderr, "[jv_aux.c] enter jv_group 1\n");
  assert(jv_get_kind(objects) == JV_KIND_ARRAY);
  assert(jv_get_kind(keys) == JV_KIND_ARRAY);
  assert(jv_array_length(jv_copy(objects)) == jv_array_length(jv_copy(keys)));
  int n = jv_array_length(jv_copy(objects));
  struct sort_entry* entries = sort_items(objects, keys);
  jv ret = jv_array();
  if (n > 0) {
    fprintf(stderr, "[jv_aux.c] enter jv_group 2\n");
    jv curr_key = entries[0].key;
    jv group = jv_array_append(jv_array(), entries[0].object);
    // fprintf(stderr, "[jv_aux.c] exit jv_group 2\n");
    
    for (int i = 1; i < n; i++) {
      fprintf(stderr, "[jv_aux.c] enter jv_group 3\n");
      if (jv_equal(jv_copy(curr_key), jv_copy(entries[i].key))) {
        fprintf(stderr, "[jv_aux.c] enter jv_group 4\n");
        jv_free(entries[i].key);
        // fprintf(stderr, "[jv_aux.c] exit jv_group 4\n");
      } else {
        fprintf(stderr, "[jv_aux.c] enter jv_group 5\n");
        jv_free(curr_key);
        curr_key = entries[i].key;
        ret = jv_array_append(ret, group);
        group = jv_array();
        // fprintf(stderr, "[jv_aux.c] exit jv_group 5\n");
      }
      fprintf(stderr, "[jv_aux.c] enter jv_group 6\n");
      group = jv_array_append(group, entries[i].object);
      // fprintf(stderr, "[jv_aux.c] exit jv_group 6\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_group 3\n");
    
    fprintf(stderr, "[jv_aux.c] enter jv_group 7\n");
    jv_free(curr_key);
    ret = jv_array_append(ret, group);
    // fprintf(stderr, "[jv_aux.c] exit jv_group 7\n");
  }
  jv_mem_free(entries);
  return ret;
  // fprintf(stderr, "[jv_aux.c] exit jv_group 1\n");
}

jv jv_unique(jv objects, jv keys) {
  fprintf(stderr, "[jv_aux.c] enter jv_unique 1\n");
  assert(jv_get_kind(objects) == JV_KIND_ARRAY);
  assert(jv_get_kind(keys) == JV_KIND_ARRAY);
  assert(jv_array_length(jv_copy(objects)) == jv_array_length(jv_copy(keys)));
  int n = jv_array_length(jv_copy(objects));
  struct sort_entry* entries = sort_items(objects, keys);
  jv ret = jv_array();
  jv curr_key = jv_invalid();
  for (int i = 0; i < n; i++) {
    fprintf(stderr, "[jv_aux.c] enter jv_unique 2\n");
    if (jv_equal(jv_copy(curr_key), jv_copy(entries[i].key))) {
      fprintf(stderr, "[jv_aux.c] enter jv_unique 3\n");
      jv_free(entries[i].key);
      jv_free(entries[i].object);
      // fprintf(stderr, "[jv_aux.c] exit jv_unique 3\n");
    } else {
      fprintf(stderr, "[jv_aux.c] enter jv_unique 4\n");
      jv_free(curr_key);
      curr_key = entries[i].key;
      ret = jv_array_append(ret, entries[i].object);
      // fprintf(stderr, "[jv_aux.c] exit jv_unique 4\n");
    }
    // fprintf(stderr, "[jv_aux.c] exit jv_unique 2\n");
  }
  jv_free(curr_key);
  jv_mem_free(entries);
  return ret;
  // fprintf(stderr, "[jv_aux.c] exit jv_unique 1\n");
}
// Total cost: 0.089321
// Total split cost: 0.005251, input tokens: 17797, output tokens: 405, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 157), (157, 274), (274, 449), (449, 587), (587, 763)]
// Total instrumented cost: 0.084071, input tokens: 117932, output tokens: 47481, cache read tokens: 0, cache write tokens: 0
