#include <stdio.h>
#include "stack.h"
#include <stdlib.h>


Stack *StackCreate() {
    Stack *stk = (Stack *)malloc(sizeof(Stack)) ;
    stk->top = -1 ;
    return stk ;
}

int StackPush(Stack *stk, void *data) {

  if (stk->top == MAXSTACK - 1) {
      printf("Stack Overflow\n") ;
      return -1 ;
  }
  //printf("Push %p ", cmd) ;
  stk->data[++stk->top] = data ;
  return 0 ;

}

void *StackPop(Stack *stk) {
  return stk->data[stk->top--] ;
}

void *StackPeek(Stack *stk) {
  return stk->data[stk->top] ;
}

int StackIsEmpty(Stack *stk) {
  return (stk->top == -1) ;
}

int StackSize(Stack *stk) {
  return stk->top  + 1;
}


void StackDebug(Stack *stk) {
  printf("Stack Debug top %d\n",stk->top) ;
  for(int i = 0 ; i < stk->top + 1 ; i++) {
    printf("Entry %d --> %p\n",i,stk->data[i]) ;
  }
}
