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

/*
 * :nodoc:
 *  GC free callback
 *
*/
void rb_relp_free_client_gc(void *ptr)
{
    relp_client_wrapper *client = (relp_client_wrapper *)ptr;
    if (ptr) {
        if (!(client->flags & RELP_CLIENT_DESTROYED))
            if (client->engine) relpEngineCltDestruct(client->engine, &client->client);
        xfree(ptr);
    }
}

/*
 *  call-seq:
 *     Relp::Client.new(engine)    =>  Relp::Client
 *
 *  Returns a handle to a new Relp client, registered with the engine context.
 *
 * === Examples
 *     engine = Relp::Engine.new          =>  Relp::Engine
 *     client = Relp::Client.new(engine)  =>  Relp::Client
 *
*/
static VALUE rb_relp_client_s_new(VALUE klass, VALUE engine_obj)
{
    VALUE obj;
    relpRetVal ret;
    relp_client_wrapper *client = NULL;
    RelpGetEngine(engine_obj);

    obj = Data_Make_Struct(rb_cRelpClient, relp_client_wrapper, 0, rb_relp_free_client_gc, client);
    client->engine = engine->engine;
    ret = relpEngineCltConstruct(engine->engine, &client->client);
    RelpAssert(ret);
    client->flags |= RELP_CLIENT_INITIAL_CONNECT;
    rb_obj_call_init(obj, 0, NULL);
    if (NIL_P(engine->clients)) engine->clients = rb_ary_new();
    rb_ary_push(engine->clients, obj);
    return obj;
}

/*
 *  call-seq:
 *     client.destroy    =>  nil
 *
 *  Release a client role from the Relp engine. This is a low level API.
 *
 * === Examples
 *     engine = Relp::Engine.new          =>  Relp::Engine
 *     client = Relp::Client.new(engine)  =>  Relp::Client
 *     client.destroy                     =>  nil
 *
*/
static VALUE rb_relp_client_destroy(VALUE obj)
{
    relpRetVal ret;
    RelpGetClient(obj);
    ret = relpEngineCltDestruct(client->engine, &client->client);
    RelpAssert(ret);
    client->flags |= RELP_CLIENT_DESTROYED;
    return Qnil;
}

/*
 *  call-seq:
 *     client.connect(Socket::AF_INET, 'localhost', 518)  =>  true
 *
 *  Connects to a Relp server role.
 *
 * === Examples
 *     engine = Relp::Engine.new                          =>  Relp::Engine
 *     client = Relp::Client.new(engine)                  =>  Relp::Client
 *     client.connect(Socket::AF_INET, 'localhost', 518)  =>  true
 *
*/
static VALUE rb_relp_client_connect(VALUE obj, VALUE prot_family, VALUE host, VALUE port)
{
    relpRetVal ret;
    RelpGetClient(obj);
    port = rb_obj_as_string(port);
    Check_Type(host, T_STRING);
    Check_Type(port, T_STRING);
    Check_Type(prot_family, T_FIXNUM);
    if (client->flags & RELP_CLIENT_INITIAL_CONNECT) {
        ret = relpCltConnect(client->client, NUM2INT(prot_family), (unsigned char*)StringValueCStr(port), (unsigned char*)StringValueCStr(host));
        if (ret == RELP_RET_OK) client->flags &= ~RELP_CLIENT_INITIAL_CONNECT;
    } else {
        ret = relpCltReconnect(client->client);
    }

    if(ret == RELP_RET_OK) {
        client->flags |= RELP_CLIENT_CONNECTED;
    } else {
        client->flags &= ~RELP_CLIENT_CONNECTED;
    }
    RelpAssert(ret);
    return Qtrue;
}

/*
 *  call-seq:
 *     client.send("message")  =>  true
 *
 *  Sends a message to the connected server peer.
 *
 * === Examples
 *     engine = Relp::Engine.new                          =>  Relp::Engine
 *     client = Relp::Client.new(engine)                  =>  Relp::Client
 *     client.connect(Socket::AF_INET, 'localhost', 518)  =>  true
 *     client.send("message")                             =>  true
 *
*/
static VALUE rb_relp_client_send(VALUE obj, VALUE message)
{
    relpRetVal ret;
    RelpGetClient(obj);
    Check_Type(message, T_STRING);
    ret = relpCltSendSyslog(client->client, (unsigned char*)StringValueCStr(message), RSTRING_LEN(message) + 1);
    RelpAssert(ret);
    return Qtrue;
}

void _init_rb_relp_client()
{
    rb_cRelpClient = rb_define_class_under(rb_mRelp, "Client", rb_cObject);

    rb_define_singleton_method(rb_cRelpClient, "new", rb_relp_client_s_new, 1);
    rb_define_method(rb_cRelpClient, "destroy", rb_relp_client_destroy, 0);
    rb_define_method(rb_cRelpClient, "connect", rb_relp_client_connect, 3);
    rb_define_alias(rb_cRelpClient, "reconnect", "connect");
    rb_define_method(rb_cRelpClient, "send", rb_relp_client_send, 1);
}