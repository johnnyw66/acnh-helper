#ifndef _COMMAND_H
#define _COMMAND_H
#include <stdint.h>

#define _ID(A) {IDENT,A}
#define _REPEAT(A) {REPT,A}
#define _RST(A)     { CALL, A}
#define _ENDRPT     { ENDRPT, 0}
#define _RETURN     { RETURN, 0}
#define _ENDMACRO {ENDMACRO,0}

typedef enum {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        X,
        Y,
        A,
        B,
        L,
        R,
        THROW,
        NOTHING,
        CONFIRM,
        TRIGGERS,
        CALL,
        RETURN,
        REPT,
        ENDRPT,
        IDENT,
        ENDMACRO
} MacroCmd_t;

typedef struct {
        MacroCmd_t mcmd;
        int16_t parameter;
} command;

void commandPrint(command *cmd) ;
command *createCMD(MacroCmd_t mcmd, int16_t parameter) ;
char *stringifyCommand(MacroCmd_t tcmd) ;
char *executeCMD(command *cmd) ;


#endif
