#include <relp_ext.h>

static VALUE rb_relp_intern_call;

static VALUE rb_relp_on_receive_callback0(VALUE *args)
{
    rb_funcall(args[0], args[1], 3, args[2], args[3], args[4]);
}

#define rb_uchar_to_str(str) \
    RelpEncode(rb_str_new2((char *)str))

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

void rb_relp_mark_server_gc(void *ptr)
{
    relp_server_wrapper *server = (relp_server_wrapper *)ptr;
    if (ptr) rb_gc_mark(server->callback);
}

void rb_relp_free_server_gc(void *ptr)
{
    if (ptr) xfree(ptr);
}

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

static VALUE rb_relp_server_destroy(VALUE obj)
{
    RelpGetServer(obj);
    return Qnil;
}

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

static VALUE rb_relp_server_on_receive(VALUE obj, VALUE callback)
{
    RelpGetServer(obj);
    /* XXX: validate */
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
