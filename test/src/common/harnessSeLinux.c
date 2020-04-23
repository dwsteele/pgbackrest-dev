/***********************************************************************************************************************************
SELinux Test Harness

sudo setcap cap_sys_admin+eip /usr/sbin/setfilecon
setfilecon system_u:object_r:tmp_t:s0 foo.log
getfilecon foo.log
***********************************************************************************************************************************/
// #include <fcntl.h>
// #include <unistd.h>
// #include <regex.h>
// #include <stdio.h>
// #include <string.h>
//
// #include "common/log.h"
// #include "common/memContext.h"
// #include "common/regExp.h"
// #include "common/type/stringList.h"
//
// #include "common/harnessDebug.h"
// #include "common/harnessTest.h"

/***********************************************************************************************************************************
Shim for SELinux setfilecon()
***********************************************************************************************************************************/
int setfilecon(const char *path, const char *context)
{
    (void)path;
    (void)context;

    return 0;
}
