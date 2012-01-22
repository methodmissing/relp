#ifndef RELP_RUBINIUS_H
#define RELP_RUBINIUS_H

#define RSTRING_NOT_MODIFIED

/* XXX */
#define RelpEncode(str) str

#define TRAP_BEG
#define TRAP_END

#define THREAD_PASS rb_thread_schedule();

#endif