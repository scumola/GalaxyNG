#include "logging.h"

static enum DebugLevels debugLevel = DNONE;
static enum LogLevels logLevel = LNONE;
static FILE* logFile = NULL;

/****f* logging/pdebug
 * NAME
 *   pdebug -- dump a message to stdout
 * SYNOPSIS
 *   void pdebug(int level, char *format,...)
 * FUNCTION
 *   Dump a debug message to stdout. Each message has a debug level.
 *   Messages are only printed if they have a level below the debug
 *   level (specified in the global debugLevel).
 * INPUTS
 *   level  -- debug level of the message
 *   format -- format string for the messgage
 *   ...    -- other parameters
 * SOURCE
 */
    
void
pdebug(enum DebugLevels level, char *format, ...)
{
  va_list         ap;
    
  if (level <= debugLevel) {
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
  }
}
  
/*******/


/****f* logging/plog
 * NAME
 *   plog -- dump a messgage to the log file.
 * SYNOPSIS
 *   void plog(int level, char *format,...)
 * FUNCTION
 *   Dump a log message to the logfile. Each message has a log level.
 *   Messages are only stored if they have a level below the
 *   log level (specified in the global logLevel).
 * INPUTS
 *   level  -- log level of the message
 *   format -- format string for the messgage
 *   ...    -- other parameters
 * SOURCE
 */
    
void
plog(enum LogLevels level, char *format, ...)
{
  va_list         ap;

  if (level <= logLevel && logFile) {
    va_start(ap, format);
    vfprintf(logFile, format, ap);
    va_end(ap);
  }
}
    
/**********/

  
/****f* logging/plogtime
 * NAME
 *   plogtime -- write current time and date to the log file.
 * SYNOPSIS
 *   void plogtime(int level)
 * SOURCE
 */

void
plogtime(enum LogLevels level)
{
  if (level <= logLevel && logFile) {
    time_t          ttp;
    char            timeBuffer[255];

    time(&ttp);
    strftime(timeBuffer, 255, "%H:%M:%S %a %b %d %Y\n", localtime(&ttp));
    fprintf(logFile, "%s", timeBuffer);
  }
}

/***********/

/****f* logging/setDebugLevel
 * NAME
 *   setDebugLevel -- set the system debugging level
 * SYNOPSIS
 *   enum DebugLevels setDebugLevel(enum DebugLevels level)
 * FUNCTION
 *   set the system debugging level
 * SOURCE
 */

enum DebugLevels setDebugLevel(enum DebugLevels level)
{
    enum DebugLevels lastLevel = debugLevel;

    debugLevel = level;

    return lastLevel;
}

/****f* logging/setLogLevel
 * NAME
 *   setLogLevel -- set the system logging level
 * SYNOPSIS
 *   enum LogLevels setLogLevel(enum LogLevels level)
 * FUNCTION
 *   set the system logging level
 * SOURCE
 */

enum LogLevels setLogLevel(enum LogLevels level)
{
    enum LogLevels lastLevel = logLevel;

    logLevel = level;

    return lastLevel;
}


/****f* logging/openLog
 * NAME
 *   openLog -- open a log file.
 * FUNCTION
 *   Open a log file. Close the previous log file if open.
 * SOURCE
 */

int
openLog(char *name, char* mode)
{
    if (logFile)
	fclose(logFile);
    if ((logFile = fopen(name, mode)) == NULL) {
	fprintf(stderr, "Could not open log file %s\n", name);
	return 1;
    }
    setvbuf(logFile, NULL, _IONBF, 0);
    return 0;
}

/**********/

void
closeLog()
{
    if (logFile) {
	fclose(logFile);
	logFile = NULL;
    }
}

