#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "command.h"
#include "logger.h"
/*
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
   ENDR,
   ID,
   ENDMACRO
 */

 char *stringifyCommand(MacroCmd_t mcmd) {
   switch(mcmd) {

   case UP:
           return "UP";

   case DOWN:
           return "DOWN";
   case LEFT:
           return "LEFT";

   case RIGHT:
           return "RIGHT";

   case A:
           return "A";
   case B:
           return "B";
   case X:
           return "X";
   case Y:
           return "Y";

   case L:
           return "L";
   case R:
           return "R";

   case THROW:
           return "THROW";

   case NOTHING:
           return "NOTHING";


   case CONFIRM:
           return "CONFIRM";

   case TRIGGERS:
           return "TRIGGERS";

   case RETURN:
           return "RETURN";

   case REPT:
           return "REPT";

   case ENDRPT:
           return "ENDREPT";

   case IDENT:
           return "ID";

   case CALL:
           return "RST";

   case ENDMACRO:
           return "ENDMACRO";

   default:
           return "??";

   }


 }

char *stringify(command *cmd) {
  return stringifyCommand(cmd->mcmd) ;
}



command *createCMD(MacroCmd_t mcmd, int16_t par) {

        command  *cmd =  (command *) malloc(sizeof(command));
        if (cmd != NULL) {
                cmd->mcmd = mcmd;
                cmd->parameter = par;
        }
        return cmd;
}



void commandPrint(command *cmd) {
        loginfo(LOGLEVEL,"Command (addr %p) CMD = %d (%s) DURATION/PARAMETER = %d\n",cmd, cmd->mcmd, stringify(cmd),cmd->parameter);
}
