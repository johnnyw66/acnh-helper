#include "usbservice.h"
#include "logger.h"
#include "command.h"
int usbDelay = 0 ;
int usbCmd = -1 ;

#define LOGLEVELUSB 0

void usbInit() {
  usbDelay = 0 ;
}

int usbReady() {
  loginfo(LOGLEVELUSB,"usbReady\n") ;
  return (usbDelay == 0) ;
}

int usbHandler() {
  if (usbDelay != 0) {
    if (usbCmd != -1) {
      loginfo(LOGLEVELUSB,"usbHandler %d %d (%s)\n",usbCmd,usbDelay,stringifyCommand(usbCmd)) ;
    }
    // update
    --usbDelay ;
  }
  // Get Current JoyStick Settings and hit that key/pad (if any)

  return 0 ;
}

int getCurrentJoyStick() {
    return usbCmd ;
}

void usbJoyStick(int cmd,int duration) {

  usbDelay = duration ;
  usbCmd = cmd ;

}
