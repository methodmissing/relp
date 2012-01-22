#ifndef RELP_SERVER_H
#define RELP_SERVER_H

#define RELP_SERVER_DESTROYED 0x01
#define RELP_SERVER_BOUND 0x02

typedef struct {
    relpEngine_t *engine;
    VALUE callback;
    int flags;
} relp_server_wrapper;

#define RelpAssertServer(obj) RelpAssertType(obj, rb_cRelpServer, "Relp::Server")
#define RelpGetServer(obj) \
    relp_server_wrapper *server = NULL; \
    RelpAssertServer(obj); \
    Data_Get_Struct(obj, relp_server_wrapper, server); \
    if (!server) rb_raise(rb_eTypeError, "uninitialized RELP server!");

void _init_rb_relp_server();

#endif