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

#include <relp_ext.h>

static VALUE rb_relp_intern_call;

/*
 * :nodoc:
 *  Invoke our user callback
 *
*/
static VALUE rb_relp_on_receive_callback0(VALUE *args)
{
    rb_funcall(args[0], args[1], 3, args[2], args[3], args[4]);
}

#define rb_uchar_to_str(str) \
    RelpEncode(rb_str_new2((char *)str))

/*
 * :nodoc:
 *  Low level Relp engine command handler.
 *
*/
static relpRetVal rb_relp_on_receive_callback(void *obj, unsigned char *host, unsigned char *ip, unsigned char *msg, size_t len)
{
    int status;
    volatile VALUE ret;
    VALUE args[5];
    RelpGetServer((VALUE)obj);
    status = 0;
    if (NIL_P(server->callback)) {
        rb_warn("No receive callback configured for Relp::Server instance %p! Got '%s [%s] %s'", obj, host, ip, msg);
        return RELP_RET_OK;
    } else {
        args[0] = (VALUE)server->callback;
        args[1] = rb_relp_intern_call;
        args[2] = rb_uchar_to_str(host);
        args[3] = rb_uchar_to_str(ip);
        args[4] = rb_uchar_to_str(msg);
        ret = rb_protect((VALUE(*)(VALUE))rb_relp_on_receive_callback0, (VALUE)args, &status);
        if (status) {
           relpEngineDestruct(&server->engine);
           rb_jump_tag(status);
           return RELP_RET_INVALID_PARAM;
        }
        return RELP_RET_OK;
    }
}

/*
 * :nodoc:
 *  GC mark callback
 *
*/
void rb_relp_mark_server_gc(void *ptr)
{
    relp_server_wrapper *server = (relp_server_wrapper *)ptr;
    if (ptr) rb_gc_mark(server->callback);
}

/*
 * :nodoc:
 *  GC free callback
 *
*/
void rb_relp_free_server_gc(void *ptr)
{
    if (ptr) xfree(ptr);
}

/*
 *  call-seq:
 *     Relp::Server.new(engine)    =>  Relp::Server
 *
 *  Allocates a new Relp::Server instance and registers low level command callbacks with the engine.
 *
 * === Examples
 *     engine = Relp::Engine.new          =>  Relp::Engine
 *     server = Relp::Server.new(engine)  =>  Relp::Server
 *
*/
static VALUE rb_relp_server_s_new(VALUE klass, VALUE engine_obj)
{
    VALUE obj;
    relp_server_wrapper *server = NULL;
    relpRetVal ret;
    RelpGetEngine(engine_obj);

    obj = Data_Make_Struct(rb_cRelpServer, relp_server_wrapper, 0, rb_relp_free_server_gc, server);
    ret = relpEngineSetSyslogRcv2(engine->engine, rb_relp_on_receive_callback);
    RelpAssert(ret);
    server->engine = engine->engine;
    server->callback = Qnil;
    rb_obj_call_init(obj, 0, NULL);
    return obj;
}

/*
 *  call-seq:
 *     server.destroy    =>  nil
 *
 *  API compat with Relp::Client and Relp::Engine - noop. This is a low level API.
 *
 * === Examples
 *     engine = Relp::Engine.new          =>  Relp::Engine
 *     server = Relp::Server.new(engine)  =>  Relp::Server
 *     server.destroy                     =>  nil
 *
*/
static VALUE rb_relp_server_destroy(VALUE obj)
{
    RelpGetServer(obj);
    return Qnil;
}

/*
 *  call-seq:
 *     server.bind(518)    =>  boolean
 *
 *  Binds a Relp engine listner to a given port. Note that this only binds to a port and doesn't start
 *  any message exchange yet.
 *
 * === Examples
 *     engine = Relp::Engine.new          =>  Relp::Engine
 *     server = Relp::Server.new(engine)  =>  Relp::Server
 *     server.bind(518)                   =>  true
 *
*/
static VALUE rb_relp_server_bind(VALUE obj, VALUE port)
{
    relpRetVal ret;
    RelpGetServer(obj);
    port = rb_obj_as_string(port);
    Check_Type(port, T_STRING);
    ret = relpEngineAddListner2(server->engine, (unsigned char*)StringValueCStr(port), (void *)obj);
    RelpAssert(ret);
    server->flags |= RELP_SERVER_BOUND;
    return Qtrue;
}

/*
 *  call-seq:
 *     server.on_receive Proc    =>  boolean
 *
 *  Registers an on_receive callback for this Relp::Server instance. Accepts a Proc with an arity of 3 for
 *  host, ip and message arguments respectively.
 *
 * === Examples
 *     engine = Relp::Engine.new          =>  Relp::Engine
 *     server = Relp::Server.new(engine)  =>  Relp::Server
 *     server.bind(518)                   =>  true
 *     server.on_receive Proc.new{|host,ip,msg| p "got #{msg} from #{ip}" }  =>  true
 *
*/
static VALUE rb_relp_server_on_receive(VALUE obj, VALUE callback)
{
    RelpGetServer(obj);
    /* XXX: validate for arity + #call contract */
    server->callback = callback;
    return Qtrue;
}

void _init_rb_relp_server()
{
    rb_relp_intern_call = rb_intern("call");

    rb_cRelpServer = rb_define_class_under(rb_mRelp, "Server", rb_cObject);

    rb_define_singleton_method(rb_cRelpServer, "new", rb_relp_server_s_new, 1);
    rb_define_method(rb_cRelpServer, "destroy", rb_relp_server_destroy, 0);
    rb_define_method(rb_cRelpServer, "bind", rb_relp_server_bind, 1);
    rb_define_method(rb_cRelpServer, "on_receive", rb_relp_server_on_receive, 1);
}
