#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

int debugLevel  = 0;

void loggerInit() {
  debugLevel = LOGLEVEL ;
}
void logger(const char *format, ...) {
  va_list args ;
  va_start(args, format) ;

#ifdef PRINTLOG
  vfprintf(stdout,format, args);
#endif

  va_end(args) ;

}


void loginfo(int logLevel,const char *format, ...) {
    va_list args ;
    if (logLevel > debugLevel) {
      va_start(args, format) ;
#ifdef PRINTLOG
      vfprintf(stdout,format, args);
#endif
      va_end(args) ;
    }
}
