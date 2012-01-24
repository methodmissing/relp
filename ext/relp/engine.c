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
 *  Flags a client as "cleaned up by librelp"
 *
*/
static VALUE rb_relp_engine_flag_client(VALUE obj, void *args)
{
    if (obj && !NIL_P(obj)){
        RelpGetClient(obj);
        client->flags |= RELP_CLIENT_DESTROYED;
    }
    return Qnil;
}

/*
 * :nodoc:
 *  Iterator that flags any clients registered with this engine instance as "cleaned up by librelp"
 *
*/
static void rb_relp_engine_flag_clients(relp_engine_wrapper *engine)
{
    if(!NIL_P(engine->clients))
        rb_iterate(rb_each, engine->clients, (iterfunc)rb_relp_engine_flag_client, (VALUE)NULL);
}

/*
 * :nodoc:
 *  GC mark callback
 *
*/
void rb_relp_mark_engine_gc(void *ptr)
{
    relp_engine_wrapper *engine = (relp_engine_wrapper *)ptr;
    rb_gc_mark(engine->clients);
    rb_gc_mark(engine->callback);
}

/*
 * :nodoc:
 *  GC free callback
 *
*/
void rb_relp_free_engine_gc(void *ptr)
{
    relp_engine_wrapper *engine = (relp_engine_wrapper *)ptr;
    if (ptr) {
        rb_relp_engine_flag_clients(engine);
        if (!(engine->flags & RELP_ENGINE_DESTROYED))
            relpEngineDestruct(&engine->engine);
        xfree(ptr);
    }
}

/*
 *  call-seq:
 *     Relp::Engine.new    =>  Relp::Engine
 *
 *  Returns a handle to a new Relp engine context, which is a container for all protocol and connection
 *  state.
 *
 * === Examples
 *     Relp::Engine.new    =>  Relp::Engine
 *
*/
static VALUE rb_relp_engine_new(VALUE obj)
{
    relp_engine_wrapper *engine = NULL;
    relpRetVal ret;

    obj = Data_Make_Struct(rb_cRelpEngine, relp_engine_wrapper, rb_relp_mark_engine_gc, rb_relp_free_engine_gc, engine);
    ret = relpEngineConstruct(&engine->engine);
    RelpAssert(ret);
    ret = relpEngineSetDbgprint(engine->engine, printf);
    RelpAssert(ret);
    ret = relpEngineSetEnableCmd(engine->engine, (unsigned char*)"syslog", eRelpCmdState_Required);
    RelpAssert(ret);
    engine->clients = Qnil;
    engine->callback = Qnil;
    rb_obj_call_init(obj, 0, NULL);
    return obj;
}

/*
 *  call-seq:
 *     engine.run    =>  Fixnum
 *
 *  Starts the Relp engine - begins to read and write data from it's peers.
 *
 * === Examples
 *     engine = Relp::Engine.new    =>  Relp::Engine
 *     # blocks the current thread of execution
 *     engine.run                   =>  Fixnum
 *
*/
static VALUE rb_relp_engine_run(VALUE obj)
{
    relpRetVal ret;
    RelpGetEngine(obj);
    THREAD_PASS;
    TRAP_BEG;
    ret = relpEngineRun(engine->engine);
    RelpAssert(ret);
    TRAP_END;
    return INT2NUM(ret);
}

static VALUE rb_relp_nogvl_engine_destroy(void *ptr)
{
    return (VALUE)relpEngineDestruct((relpEngine_t *)&ptr);
}

/*
 *  call-seq:
 *     engine.destroy    =>  nil
 *
 *  Stops the Relp engine and terminates all operations. This is a low level API.
 *
 * === Examples
 *     engine = Relp::Engine.new    =>  Relp::Engine
 *     Thread.new do
 *       sleep 1 && engine.destroy
 *     end
 *     # blocks the current thread of execution
 *     engine.run                   =>   Fixnum
 *
*/
static VALUE rb_relp_engine_destroy(VALUE obj)
{
    relpRetVal ret;
    RelpGetEngine(obj);
    ret = (relpRetVal)rb_thread_blocking_region(rb_relp_nogvl_engine_destroy, (void *)engine->engine, RUBY_UBF_IO, 0);
    RelpAssert(ret);
    engine->flags |= RELP_ENGINE_DESTROYED;
    rb_relp_engine_flag_clients(engine);
    return Qnil;
}

/*
 *  call-seq:
 *     engine.resolve = true    =>  nil
 *
 *  Sets hostname resolution preferences for this Relp engine instance.
 *
 * === Examples
 *     engine = Relp::Engine.new    =>  Relp::Engine
 *     engine.resolve = true  # do hostname lookups
 *     engine.resolve = false # disable hostname lookups
 *
*/
static VALUE rb_relp_engine_resolve_equals(VALUE obj, VALUE resolve)
{
    int mode;
    relpRetVal ret;
    RelpGetEngine(obj);
    if (resolve == Qtrue)
        mode = 1;
    else if (resolve == Qfalse)
        mode = 0;
    else
        rb_raise(rb_eArgError, "only boolean arguments supported for setting DNS preferences!");
    ret = relpEngineSetDnsLookupMode(engine->engine, mode);
    RelpAssert(ret);
    return Qnil;
}

void _init_rb_relp_engine()
{
    rb_cRelpEngine = rb_define_class_under(rb_mRelp, "Engine", rb_cObject);

    rb_define_alloc_func(rb_cRelpEngine, rb_relp_engine_new);
    rb_define_method(rb_cRelpEngine, "run", rb_relp_engine_run, 0);
    rb_define_method(rb_cRelpEngine, "resolve=", rb_relp_engine_resolve_equals, 1);
    rb_define_method(rb_cRelpEngine, "destroy", rb_relp_engine_destroy, 0);
}