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

#ifndef RELP_CLIENT_H
#define RELP_CLIENT_H

#define RELP_CLIENT_DESTROYED 0x01
#define RELP_CLIENT_INITIAL_CONNECT 0x02
#define RELP_CLIENT_CONNECTED 0x04

typedef struct {
    relpEngine_t *engine;
    relpClt_t *client;
    int flags;
} relp_client_wrapper;

#define RelpAssertClient(obj) RelpAssertType(obj, rb_cRelpClient, "Relp::Client")
#define RelpGetClient(obj) \
    relp_client_wrapper *client = NULL; \
    RelpAssertClient(obj); \
    Data_Get_Struct(obj, relp_client_wrapper, client); \
    if (!client) rb_raise(rb_eTypeError, "uninitialized RELP client!");

void _init_rb_relp_client();

#endif