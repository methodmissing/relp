#ifndef RELP_ENGINE_H
#define RELP_ENGINE_H

#define RELP_ENGINE_DESTROYED 0x01

typedef struct {
    relpEngine_t *engine;
    VALUE clients;
    VALUE callback;
    int flags;
} relp_engine_wrapper;

#define RelpAssertEngine(obj) RelpAssertType(obj, rb_cRelpEngine, "Relp::Engine")
#define RelpGetEngine(obj) \
    relp_engine_wrapper *engine = NULL; \
    RelpAssertEngine(obj); \
    Data_Get_Struct(obj, relp_engine_wrapper, engine); \
    if (!engine) rb_raise(rb_eTypeError, "uninitialized RELP engine!");

void _init_rb_relp_engine();

#endif