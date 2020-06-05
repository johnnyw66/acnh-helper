
#ifndef _LOGGER_H
#define _LOGGER_H

#define LOGLEVEL 0
void loggerInit(void) ;

void logger(const char * format, ...) ;
void loginfo(int logLevel,const char *format, ...) ;

#endif
