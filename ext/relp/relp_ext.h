#ifndef RELP_EXT_H
#define RELP_EXT_H

#include "ruby.h"
#include <librelp.h>

/* Compiler specific */

#if defined(__GNUC__) && (__GNUC__ >= 3)
#define RELP_UNUSED __attribute__ ((unused))
#define RELP_NOINLINE __attribute__ ((noinline))
#else
#define RELP_UNUSED
#define RELP_NOINLINE
#endif

#include <relp_prelude.h>

typedef VALUE(*iterfunc)(ANYARGS);

#define RelpErrorMessage(code) \
    RSTRING_PTR(rb_hash_aref(rb_const_get_at(rb_mRelp, rb_relp_intern_errors), INT2FIX(code)))

#define RelpAssert(ret) \
  if ((ret) != RELP_RET_OK) \
      rb_raise(rb_eRelpError, "[%d] %s\n", ret, RelpErrorMessage(ret));

#define RelpAssertType(obj, type, desc) \
    if (!rb_obj_is_kind_of(obj,type)) \
        rb_raise(rb_eTypeError, "wrong argument type %s (expected %s): %s", rb_obj_classname(obj), desc, RSTRING_PTR(rb_obj_as_string(obj)));

extern VALUE rb_relp_intern_errors;
extern VALUE rb_mRelp;
extern VALUE rb_eRelpError;
extern VALUE rb_cRelpEngine;
extern VALUE rb_cRelpClient;
extern VALUE rb_cRelpServer;

#include <engine.h>
#include <client.h>
#include <server.h>

#endif