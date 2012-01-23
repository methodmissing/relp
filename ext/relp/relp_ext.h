/*
 *
 * Author:: Lourens Naudé
 * Homepage::  http://github.com/methodmissing/relp
 * Date:: 20120123
 *
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2012 by Lourens Naudé. All Rights Reserved.
 * Email: lourens at methodmissing dot com
 *
 * (The MIT License)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *---------------------------------------------------------------------------
 *
 */

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