#ifndef _STACK_H
#define _STACK_H

#include <command.h>

#define MAXSTACK 16

typedef struct _stack {
  void *data[MAXSTACK] ;
  int top ;
} Stack ;

#define STACK_INIT {.top = -1}
Stack *StackCreate(void) ;
int StackPush(Stack *stk, void *data) ;
void *StackPop(Stack *stk) ;
void *StackPeek(Stack *stk) ;
int StackIsEmpty(Stack *stk) ;
int StackSize(Stack *stk) ;
void StackDebug(Stack  *stk) ;

#define peek()  StackPeek(&callStack)
#define push(_a)  StackPush(&callStack,_a)

extern Stack callStack  ;


#endif
