#ifndef RELP_JRUBY_H
#define RELP_JRUBY_H

/* XXX */
#define RelpEncode(str) str

#ifndef THREAD_PASS
#define THREAD_PASS rb_thread_schedule();
#endif

#define TRAP_BEG
#define TRAP_END

#undef rb_errinfo
#define rb_errinfo() (rb_gv_get("$!"))

#define HAVE_RB_THREAD_BLOCKING_REGION 1

#endif