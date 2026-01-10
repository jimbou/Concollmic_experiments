#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jv.h"

struct nomem_handler {
    jv_nomem_handler_f handler;
    void *data;
};

#if !defined(HAVE_PTHREAD_KEY_CREATE) || \
    !defined(HAVE_PTHREAD_ONCE) || \
    !defined(HAVE_ATEXIT)

/* Try thread-local storage? */

#ifdef _MSC_VER
/* Visual C++: yes */
static __declspec(thread) struct nomem_handler nomem_handler;
#define USE_TLS
#else
#ifdef HAVE___THREAD
/* GCC and friends: yes */
static __thread struct nomem_handler nomem_handler;
#define USE_TLS
#endif /* HAVE___THREAD */
#endif /* _MSC_VER */

#endif /* !HAVE_PTHREAD_KEY_CREATE */

#ifdef USE_TLS
void jv_nomem_handler(jv_nomem_handler_f handler, void *data) {
  fprintf(stderr, "[jv_alloc.c] enter jv_nomem_handler 1\n");
  nomem_handler.handler = handler;
  // fprintf(stderr, "[jv_alloc.c] exit jv_nomem_handler 1\n");
}

static void memory_exhausted(void) {
  fprintf(stderr, "[jv_alloc.c] enter memory_exhausted 1\n");
  if (nomem_handler.handler)
    nomem_handler.handler(nomem_handler.data); // Maybe handler() will longjmp() to safety
  // Or not
  fprintf(stderr, "jq: error: cannot allocate memory\n");
  abort();
  // fprintf(stderr, "[jv_alloc.c] exit memory_exhausted 1\n");
}
#else /* USE_TLS */

#ifdef HAVE_PTHREAD_KEY_CREATE
#include <pthread.h>

static pthread_key_t nomem_handler_key;
static pthread_once_t mem_once = PTHREAD_ONCE_INIT;

/* tsd_fini is called on application exit */
/* it clears the nomem_handler allocated in the main thread */
static void tsd_fini(void) {
  fprintf(stderr, "[jv_alloc.c] enter tsd_fini 1\n");
  struct nomem_handler *nomem_handler;
  nomem_handler = pthread_getspecific(nomem_handler_key);
  if (nomem_handler) {
    (void) pthread_setspecific(nomem_handler_key, NULL);
    free(nomem_handler);
  }
  // fprintf(stderr, "[jv_alloc.c] exit tsd_fini 1\n");
}

/* The tsd_fini_thread is a destructor set by calling */
/* pthread_key_create(&nomem_handler_key, tsd_fini_thread) */
/* It is called when thread ends */
static void tsd_fini_thread(void *nomem_handler) {
  fprintf(stderr, "[jv_alloc.c] enter tsd_fini_thread 1\n");
  free(nomem_handler);
  // fprintf(stderr, "[jv_alloc.c] exit tsd_fini_thread 1\n");
}

static void tsd_init(void) {
  fprintf(stderr, "[jv_alloc.c] enter tsd_init 1\n");
  if (pthread_key_create(&nomem_handler_key, tsd_fini_thread) != 0) {
    fprintf(stderr, "[jv_alloc.c] enter tsd_init 2\n");
    fprintf(stderr, "jq: error: cannot create thread specific key");
    abort();
    // fprintf(stderr, "[jv_alloc.c] exit tsd_init 2\n");
  }
  fprintf(stderr, "[jv_alloc.c] enter tsd_init 3\n");
  if (atexit(tsd_fini) != 0) {
    fprintf(stderr, "[jv_alloc.c] enter tsd_init 4\n");
    fprintf(stderr, "jq: error: cannot set an exit handler");
    abort();
    // fprintf(stderr, "[jv_alloc.c] exit tsd_init 4\n");
  }
  // fprintf(stderr, "[jv_alloc.c] exit tsd_init 3\n");
  // fprintf(stderr, "[jv_alloc.c] exit tsd_init 1\n");
}

static void tsd_init_nomem_handler(void)
{
  fprintf(stderr, "[jv_alloc.c] enter tsd_init_nomem_handler 1\n");
  if (pthread_getspecific(nomem_handler_key) == NULL) {
    fprintf(stderr, "[jv_alloc.c] enter tsd_init_nomem_handler 2\n");
    struct nomem_handler *nomem_handler = calloc(1, sizeof(struct nomem_handler));
    if (pthread_setspecific(nomem_handler_key, nomem_handler) != 0) {
      fprintf(stderr, "[jv_alloc.c] enter tsd_init_nomem_handler 3\n");
      fprintf(stderr, "jq: error: cannot set thread specific data");
      abort();
      // fprintf(stderr, "[jv_alloc.c] exit tsd_init_nomem_handler 3\n");
    }
    // fprintf(stderr, "[jv_alloc.c] exit tsd_init_nomem_handler 2\n");
  }
  // fprintf(stderr, "[jv_alloc.c] exit tsd_init_nomem_handler 1\n");
}

void jv_nomem_handler(jv_nomem_handler_f handler, void *data) {
  fprintf(stderr, "[jv_alloc.c] enter jv_nomem_handler 2838\n");
  pthread_once(&mem_once, tsd_init); // cannot fail
  tsd_init_nomem_handler();

  struct nomem_handler *nomem_handler;

  nomem_handler = pthread_getspecific(nomem_handler_key);
  if (nomem_handler == NULL) {
    fprintf(stderr, "[jv_alloc.c] enter jv_nomem_handler 2\n");
    handler(data);
    fprintf(stderr, "jq: error: cannot allocate memory\n");
    abort();
    // fprintf(stderr, "[jv_alloc.c] exit jv_nomem_handler 2\n");
  }
  fprintf(stderr, "[jv_alloc.c] enter jv_nomem_handler 3\n");
  nomem_handler->handler = handler;
  nomem_handler->data = data;
  // fprintf(stderr, "[jv_alloc.c] exit jv_nomem_handler 3\n");
  // fprintf(stderr, "[jv_alloc.c] exit jv_nomem_handler 2838\n");
}

static void memory_exhausted(void) {
  fprintf(stderr, "[jv_alloc.c] enter memory_exhausted 2998\n");
  struct nomem_handler *nomem_handler;

  pthread_once(&mem_once, tsd_init);
  tsd_init_nomem_handler();

  nomem_handler = pthread_getspecific(nomem_handler_key);
  if (nomem_handler && nomem_handler->handler)
    nomem_handler->handler(nomem_handler->data); // Maybe handler() will longjmp() to safety
  // Or not
  fprintf(stderr, "jq: error: cannot allocate memory\n");
  abort();
  // fprintf(stderr, "[jv_alloc.c] exit memory_exhausted 2998\n");
}

#else

/* No thread-local storage of any kind that we know how to handle */

static struct nomem_handler nomem_handler;
void jv_nomem_handler(jv_nomem_handler_f handler, void *data) {
  fprintf(stderr, "[jv_alloc.c] enter jv_nomem_handler 1132\n");
  nomem_handler.handler = handler;
  nomem_handler.data = data;
  // fprintf(stderr, "[jv_alloc.c] exit jv_nomem_handler 1132\n");
}

static void memory_exhausted(void) {
  fprintf(stderr, "[jv_alloc.c] enter memory_exhausted 2437\n");
  fprintf(stderr, "jq: error: cannot allocate memory\n");
  abort();
  // fprintf(stderr, "[jv_alloc.c] exit memory_exhausted 2437\n");
}

#endif /* HAVE_PTHREAD_KEY_CREATE */
#endif /* USE_TLS */


void* jv_mem_alloc(size_t sz) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_alloc 1\n");
  void* p = malloc(sz);
  if (!p) {
    fprintf(stderr, "[jv_alloc.c] enter jv_mem_alloc 2\n");
    memory_exhausted();
    // fprintf(stderr, "[jv_alloc.c] exit jv_mem_alloc 2\n");
  }
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_alloc 1\n");
  return p;
}

void* jv_mem_alloc_unguarded(size_t sz) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_alloc_unguarded 1\n");
  return malloc(sz);
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_alloc_unguarded 1\n");
}

void* jv_mem_calloc(size_t nemb, size_t sz) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_calloc 1\n");
  assert(nemb > 0 && sz > 0);
  void* p = calloc(nemb, sz);
  if (!p) {
    fprintf(stderr, "[jv_alloc.c] enter jv_mem_calloc 2\n");
    memory_exhausted();
    // fprintf(stderr, "[jv_alloc.c] exit jv_mem_calloc 2\n");
  }
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_calloc 1\n");
  return p;
}

void* jv_mem_calloc_unguarded(size_t nemb, size_t sz) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_calloc_unguarded 1\n");
  assert(nemb > 0 && sz > 0);
  return calloc(nemb, sz);
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_calloc_unguarded 1\n");
}

char* jv_mem_strdup(const char *s) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_strdup 1\n");
  char *p = strdup(s);
  if (!p) {
    fprintf(stderr, "[jv_alloc.c] enter jv_mem_strdup 2\n");
    memory_exhausted();
    // fprintf(stderr, "[jv_alloc.c] exit jv_mem_strdup 2\n");
  }
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_strdup 1\n");
  return p;
}

char* jv_mem_strdup_unguarded(const char *s) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_strdup_unguarded 1\n");
  return strdup(s);
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_strdup_unguarded 1\n");
}

void jv_mem_free(void* p) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_free 1\n");
  free(p);
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_free 1\n");
}

void* jv_mem_realloc(void* p, size_t sz) {
  fprintf(stderr, "[jv_alloc.c] enter jv_mem_realloc 1\n");
  p = realloc(p, sz);
  if (!p) {
    fprintf(stderr, "[jv_alloc.c] enter jv_mem_realloc 2\n");
    memory_exhausted();
    // fprintf(stderr, "[jv_alloc.c] exit jv_mem_realloc 2\n");
  }
  // fprintf(stderr, "[jv_alloc.c] exit jv_mem_realloc 1\n");
  return p;
}
// Total cost: 0.003571
// Total split cost: 0.000000, input tokens: 0, output tokens: 0, cache read tokens: 0, cache write tokens: 0, split chunks: [(0, 189)]
// Total instrumented cost: 0.003571, input tokens: 3505, output tokens: 2386, cache read tokens: 0, cache write tokens: 0
