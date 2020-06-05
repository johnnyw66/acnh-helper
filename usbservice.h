#ifndef _USBSERVICE_H
#define _USBSERVICE_H

void usbInit(void) ;
int usbReady(void) ;
int usbHandler(void) ;
void usbJoyStick(int cmd,int duration) ;
int getCurrentJoyStick(void) ;


#endif
