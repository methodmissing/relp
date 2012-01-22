#include <relp_ext.h>

VALUE rb_relp_intern_errors;
VALUE rb_mRelp;
VALUE rb_eRelpError;
VALUE rb_cRelpClient;
VALUE rb_cRelpEngine;
VALUE rb_cRelpServer;

#ifdef HAVE_RUBY_ENCODING_H
rb_encoding *binary_encoding;
#endif

static VALUE rb_relp_m_version(RELP_UNUSED VALUE obj)
{
    return rb_str_new2(relpEngineGetVersion());
}

void Init_relp_ext()
{
    rb_relp_intern_errors = rb_intern("ERRORS");
#ifdef HAVE_RUBY_ENCODING_H
    binary_encoding = rb_enc_find("binary");
#endif

    rb_mRelp = rb_define_module("Relp");

    rb_eRelpError = rb_define_class_under(rb_mRelp, "Error", rb_eStandardError);

    rb_define_module_function(rb_mRelp, "version", rb_relp_m_version, 0);

    rb_define_const(rb_mRelp, "OK", INT2NUM(RELP_RET_OK));
    rb_define_const(rb_mRelp, "OUT_OF_MEMORY", INT2NUM(RELP_RET_OUT_OF_MEMORY));
    rb_define_const(rb_mRelp, "INVALID_FRAME", INT2NUM(RELP_RET_INVALID_FRAME));
    rb_define_const(rb_mRelp, "PARAM_ERROR", INT2NUM(RELP_RET_PARAM_ERROR));
    rb_define_const(rb_mRelp, "INVALID_PORT", INT2NUM(RELP_RET_INVALID_PORT));
    rb_define_const(rb_mRelp, "COULD_NOT_BIND", INT2NUM(RELP_RET_COULD_NOT_BIND));
    rb_define_const(rb_mRelp, "ACCEPT_ERR", INT2NUM(RELP_RET_ACCEPT_ERR));
    rb_define_const(rb_mRelp, "SESSION_BROKEN", INT2NUM(RELP_RET_SESSION_BROKEN));
    rb_define_const(rb_mRelp, "SESSION_CLOSED", INT2NUM(RELP_RET_SESSION_CLOSED));
    rb_define_const(rb_mRelp, "INVALID_CMD", INT2NUM(RELP_RET_INVALID_CMD));
    rb_define_const(rb_mRelp, "DATA_TOO_LONG", INT2NUM(RELP_RET_DATA_TOO_LONG));
    rb_define_const(rb_mRelp, "INVALID_TXNR", INT2NUM(RELP_RET_INVALID_TXNR));
    rb_define_const(rb_mRelp, "INVALID_DATALEN", INT2NUM(RELP_RET_INVALID_DATALEN));
    rb_define_const(rb_mRelp, "PARTIAL_WRITE", INT2NUM(RELP_RET_PARTIAL_WRITE));
    rb_define_const(rb_mRelp, "IO_ERR", INT2NUM(RELP_RET_IO_ERR));
    rb_define_const(rb_mRelp, "TIMED_OUT", INT2NUM(RELP_RET_TIMED_OUT));
    rb_define_const(rb_mRelp, "NOT_FOUND", INT2NUM(RELP_RET_NOT_FOUND));
    rb_define_const(rb_mRelp, "NOT_IMPLEMENTED", INT2NUM(RELP_RET_NOT_IMPLEMENTED));
    rb_define_const(rb_mRelp, "INVALID_RSPHDR", INT2NUM(RELP_RET_INVALID_RSPHDR));
    rb_define_const(rb_mRelp, "END_OF_DATA", INT2NUM(RELP_RET_END_OF_DATA));
    rb_define_const(rb_mRelp, "RSP_STATE_ERR", INT2NUM(RELP_RET_RSP_STATE_ERR));
    rb_define_const(rb_mRelp, "INVALID_OFFER", INT2NUM(RELP_RET_INVALID_OFFER));
    rb_define_const(rb_mRelp, "UNKNOWN_CMD", INT2NUM(RELP_RET_UNKNOWN_CMD));
    rb_define_const(rb_mRelp, "CMD_DISABLED", INT2NUM(RELP_RET_CMD_DISABLED));
    rb_define_const(rb_mRelp, "INVALID_HDL", INT2NUM(RELP_RET_INVALID_HDL));
    rb_define_const(rb_mRelp, "INCOMPAT_OFFERS", INT2NUM(RELP_RET_INCOMPAT_OFFERS));
    rb_define_const(rb_mRelp, "RQD_FEAT_MISSING", INT2NUM(RELP_RET_RQD_FEAT_MISSING));
    rb_define_const(rb_mRelp, "MALICIOUS_HNAME", INT2NUM(RELP_RET_MALICIOUS_HNAME));
    rb_define_const(rb_mRelp, "INVALID_HNAME", INT2NUM(RELP_RET_INVALID_HNAME));
    rb_define_const(rb_mRelp, "ADDR_UNKNOWN", INT2NUM(RELP_RET_ADDR_UNKNOWN));
    rb_define_const(rb_mRelp, "INVALID_PARAM", INT2NUM(RELP_RET_INVALID_PARAM));

    _init_rb_relp_engine();
    _init_rb_relp_client();
    _init_rb_relp_server();
}
