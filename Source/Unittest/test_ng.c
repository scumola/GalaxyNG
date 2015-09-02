#include <testsuite.h>
#include <rbt_assert.h>
#include <stdlib.h>
#include "test_wopr.h"
#include "galaxy.h"
#include "galaxyng.h"


void NG_Test( void )
{
    char** argv = calloc( 1, sizeof(char *) );
    int    argc = 1;
    int    result;

    logLevel = LNONE;
    SetDirectoryVariables();

    result = CMD_check( argc, argv, CMD_CHECK_REAL );
    RBT_Assert( result == EXIT_FAILURE );
    free( argv );
}


void NG_Create_NG_TestSuite( void )
{
    RBT_TestSuite* suite = RBT_CreateSuite( "NG_TestSuite" );
    RBT_AddCase( suite, NG_Test );
}

