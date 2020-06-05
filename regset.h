#ifndef _REGSET_H
#define _REGSET_H

#include "command.h"

typedef struct {
        int id ;
        command *pc;
} regset;


regset *createRegSet(command *cmd) ;
void setID(regset *rset,int id) ;
command *getCurrentCommand(regset *rset) ;
void nextCommand(regset *rset) ;
int execute(regset *rset) ;

extern command *getCall(int rstnumber) ;

#endif
