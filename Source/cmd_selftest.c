#include "galaxyng.h"

/****f* CLI/CMD_selftest
 * NAME
 *   CMD_selftest -- run a series of selftests
 * FUNCTION
 *   Run a series of selftest.  Used for debug purposes.
 *****
 */

int CMD_selftest() {
    getstrTest();
    frandTest();
    frand2Test();
    frand3Test();
	
    return 0;
}

