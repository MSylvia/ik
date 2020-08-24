#include "ik/log.h"
#include "ik/tests.h"
#include <stdio.h>

/* ------------------------------------------------------------------------- */
int
ik_tests_run(int* argc, char** argv)
{
    ik_log_printf(IK_ERROR, "The IK library was built without unit tests. Recompile with -DIK_TESTS=ON if you want this functionality.");
    return -1;
}
