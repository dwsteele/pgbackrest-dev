/***********************************************************************************************************************************
SELinux Test Harness

sudo setcap cap_sys_admin+eip /usr/sbin/setfilecon
setfilecon system_u:object_r:tmp_t:s0 foo.log
getfilecon foo.log
***********************************************************************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "common/assert.h"

#include "common/harnessDebug.h"

/***********************************************************************************************************************************
Shim for SELinux selinux_trans_to_raw_context()
***********************************************************************************************************************************/
int
selinux_trans_to_raw_context(const char *trans, char **rawp)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRINGZ, trans);
        FUNCTION_HARNESS_PARAM_P(STRINGZ, rawp);
    FUNCTION_HARNESS_END();

    ASSERT(trans != NULL);
    ASSERT(rawp != NULL);

    if (strcmp(trans, "ERROR") == 0)
        FUNCTION_HARNESS_RESULT(INT, -1);

    *rawp = malloc(strlen(trans) + 1);

    strcpy(*rawp, trans);

    FUNCTION_HARNESS_RESULT(INT, 0);
}

/***********************************************************************************************************************************
Shim for SELinux selinux_raw_to_trans_context()
***********************************************************************************************************************************/
int
selinux_raw_to_trans_context(const char *raw, char **transp)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRINGZ, raw);
        FUNCTION_HARNESS_PARAM_P(STRINGZ, transp);
    FUNCTION_HARNESS_END();

    ASSERT(raw != NULL);
    ASSERT(transp != NULL);

    if (strcmp(raw, "error") == 0)
        FUNCTION_HARNESS_RESULT(INT, -1);

    *transp = malloc(strlen(raw) + 1);

    strcpy(*transp, raw);

    FUNCTION_HARNESS_RESULT(INT, 0);
}

/***********************************************************************************************************************************
Shim for freecon()
***********************************************************************************************************************************/
void freecon(char *con)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRINGZ, con);
    FUNCTION_HARNESS_END();

    free(con);

    FUNCTION_HARNESS_RESULT_VOID();
}
