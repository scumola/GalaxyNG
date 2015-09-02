#if !defined(LOGGING_H_)
#define LOGGING_H_

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/****d* GalaxyNG/DebugLevels
 * NAME
 *   DebugLevels --
 * FUNCTION
 *   DFULL
 *   DWARN
 *   DSTOP
 *   DERR
 ******
 */

enum DebugLevels {DNONE, DERR, DSTOP, DWARN, DFULL, DFULL2};

/****d* GalaxyNG/LogLevels
 * NAME
 *   LogLevels --
 * FUNCTION
 *   LFULL  -- Log everything. This creates huge log files.
 *   LPART  -- Partial log, used when a turn is ran, messages at this level
 *             end up in the GM report.
 *   LBRIEF -- Be brief. Used during an orders check
 *   LNONE  -- Don't log anything.
 * SEE ALSO
 *   plog()
 * SOURCE
 */

enum LogLevels {LNONE, LBRIEF, LPART, LFULL};

/******/

void pdebug(enum DebugLevels level, char *format, ...);
void plog(enum LogLevels level, char *format, ...);
void plogtime(enum LogLevels level);
int  openLog(char* filename, char* mode);
void closeLog(void);
enum LogLevels setLogLevel(enum LogLevels level);
enum DebugLevels setDebugLevel(enum DebugLevels level);

#endif

