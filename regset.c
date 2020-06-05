#include <stdio.h>
#include <stdlib.h>

#include "regset.h"
#include "stack.h"
#include "logger.h"
#include "usbservice.h"

#define REGSET_LOGLEVEL 12
#define REGSET_LOGOFF 0

typedef struct {
  command *cmd ;
  int loopcntr ;
} RepeatCmd ;

Stack repeatStack = STACK_INIT ;
Stack callStack = STACK_INIT ;


RepeatCmd * createRepCommand(command *cmd, int cntr) {
  RepeatCmd *rcmd = (RepeatCmd *)malloc(sizeof(RepeatCmd)) ;
  rcmd->cmd = cmd ;
  rcmd->loopcntr = cntr ;
  return rcmd ;
}
regset *createRegSet(command *cmd) {
    regset *rset = (regset *)malloc(sizeof(regset)) ;

    if (rset != NULL) {
      rset->pc = cmd ;
      rset->id = -1 ;
    }
    return rset ;

}

void setID(regset *rset,int id) {
  rset->id = id ;
}

command *getCurrentCommand(regset *rset) {
  return rset->pc ;
}

void nextCommand(regset *rset) {
    rset->pc++;
}

void joystickInstructionCmd(command *cmd) {

  loginfo(REGSET_LOGLEVEL, "***Joystick*** (%s,%d) %p\n",stringifyCommand(cmd->mcmd), cmd->parameter,cmd) ;
  usbJoyStick(cmd->mcmd, cmd->parameter) ;

}

void joystickInstruction(MacroCmd_t mcmd,int duration) {
  loginfo(REGSET_LOGLEVEL, "***Joystick*** (%s,%d)\n",stringifyCommand(mcmd), duration) ;
  usbJoyStick(mcmd, duration) ;

}


void debugRegSet(regset *rset) {
  loginfo(REGSET_LOGOFF ,"***RegSet*** (PC = %p,id = %d)\n",rset->pc,rset->id) ;
  commandPrint(rset->pc) ;

}
char *stringify(command *cmd) ;

int execute(regset *rset) {


  command *cmd = getCurrentCommand(rset) ;
  MacroCmd_t mcmd = cmd->mcmd ;
  int16_t parameter = cmd->parameter ;

  loginfo(REGSET_LOGOFF,"execute - regset---> ") ;
  debugRegSet(rset) ;

  switch(mcmd) {

    case REPT:
            // Enter a Repeat Loop - Note current 'program counter'/command and the loop counter, held in 'parameter'.
          loginfo(REGSET_LOGOFF,"Repeat Loop - COUNT %d pc start %p\n",parameter,rset->pc) ;
          StackPush(&repeatStack,createRepCommand(rset->pc,parameter)) ;
          break ;

    case RETURN:
          rset = StackPop(&callStack) ;
          loginfo(REGSET_LOGOFF,"RETURN\n") ;
          break ;

    case ENDRPT:
          {
            RepeatCmd *repCmd=StackPeek(&repeatStack) ;
            loginfo(REGSET_LOGOFF,"***ENDREPT CHECK *** (%d)\n",repCmd->loopcntr) ;
            if (repCmd->loopcntr < 0 || --repCmd->loopcntr > 0) {
                rset->pc = repCmd->cmd ;
            } else {

              StackPop(&repeatStack) ;

              if (!StackIsEmpty(&repeatStack)) {
                repCmd = StackPeek(&repeatStack) ;
                if (!repCmd) {
                  logger(REGSET_LOGOFF,"ENDRPT (LOOP COMPLETE)- POP OFF (%d) start %p\n",repCmd->loopcntr,repCmd->cmd) ;
                }
              }


            }
          }
          break ;

    case IDENT:
          rset->id = parameter ;
          loginfo(REGSET_LOGLEVEL,"ID %d\n",parameter) ;
          break ;

    case CALL:
          {
            command *cmd = getCall(parameter) ;
            loginfo(REGSET_LOGOFF,"rst(%d) addr = %p\n",parameter,cmd) ;
            // ***Note*** We are pushing the address prior (i.e decrement by 1) to the actual 1st cmd being called
            // since we will be bumping this cmd ptr prior to the actual execution!
            StackPush(&callStack,createRegSet(cmd - 1)) ;
          }
          break ;


    case ENDMACRO:
          loginfo(REGSET_LOGLEVEL,"ENDMACRO\n");
          break ;

    case NOTHING:
          //loginfo(REGSET_LOGLEVEL,"***NOTHING (%d)***\n",parameter) ;
          joystickInstruction(mcmd,parameter) ;
          //joystickInstructionCmd(cmd) ;
          break ;

    default:
          //loginfo(REGSET_LOGLEVEL, "***Joystick*** (%s,%d)\n",stringifyCommand(mcmd), parameter) ;
          joystickInstruction(mcmd,parameter) ;
          //joystickInstructionCmd(cmd) ;
          break ;
  }
  return 0 ;
}
