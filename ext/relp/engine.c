#include <relp_ext.h>

static VALUE rb_relp_engine_flag_client(VALUE obj, void *args)
{
    if (obj && !NIL_P(obj)){
        RelpGetClient(obj);
        client->flags |= RELP_CLIENT_DESTROYED;
    }
    return Qnil;
}

static void rb_relp_engine_flag_clients(relp_engine_wrapper *engine)
{
    if(!NIL_P(engine->clients))
        rb_iterate(rb_each, engine->clients, (iterfunc)rb_relp_engine_flag_client, (VALUE)NULL);
}

void rb_relp_mark_engine_gc(void *ptr)
{
    relp_engine_wrapper *engine = (relp_engine_wrapper *)ptr;
    rb_gc_mark(engine->clients);
    rb_gc_mark(engine->callback);
}

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

static VALUE rb_relp_engine_destroy(VALUE obj)
{
    relpRetVal ret;
    RelpGetEngine(obj);
    ret = relpEngineDestruct(&engine->engine);
    RelpAssert(ret);
    engine->flags |= RELP_ENGINE_DESTROYED;
    rb_relp_engine_flag_clients(engine);
    return Qnil;
}

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