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