/***********************************************************************************************************************************
SELinux Functions
***********************************************************************************************************************************/
#include "build.auto.h"

#ifdef HAVE_LIBSELINUX

#include <selinux/selinux.h>

#include "common/debug.h"
#include "common/log.h"
#include "common/memContext.h"
#include "storage/posix/selinux.h"

/***********************************************************************************************************************************
SELinux context extended attribute
***********************************************************************************************************************************/
STRING_EXTERN(STORAGE_POSIX_SELINUX_CONTEXT_XATTR_STR,              "security.selinux");

/**********************************************************************************************************************************/
String *
storagePosixSelContextTransToRaw(const String *translated)
{
    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(STRING, translated);
    FUNCTION_LOG_END();

    ASSERT(translated != NULL);

    char *raw = NULL;

    if (selinux_trans_to_raw_context(strPtr(translated), &raw) != 0)
        THROW_FMT(KernelError, "unable to convert translated context '%s' to raw", strPtr(translated));

    String *result = strNew(raw);
    freecon(raw);

    FUNCTION_LOG_RETURN(STRING, result);
}

/**********************************************************************************************************************************/
String *
storagePosixSelContextRawToTrans(const String *raw)
{
    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(STRING, raw);
    FUNCTION_LOG_END();

    String *result = NULL;

    if (raw != NULL)
    {
        char *translated = NULL;

        if (selinux_raw_to_trans_context(strPtr(raw), &translated) != 0)
            THROW_FMT(KernelError, "unable to convert raw context '%s' to translated", strPtr(raw));

        result = strNew(translated);
        freecon(translated);
    }

    FUNCTION_LOG_RETURN(STRING, result);
}

#endif // HAVE_LIBSELINUX
