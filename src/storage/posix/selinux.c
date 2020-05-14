/***********************************************************************************************************************************
SELinux Functions
***********************************************************************************************************************************/
#include "build.auto.h"

#ifdef HAVE_LIBSELINUX
#include <selinux/selinux.h>
#endif // HAVE_LIBSELINUX

#include "common/debug.h"
#include "common/log.h"
#include "common/macro.h"
#include "common/memContext.h"
#include "storage/posix/selinux.h"
#include "version.h"

/***********************************************************************************************************************************
SELinux context extended attribute
***********************************************************************************************************************************/
VARIANT_STRDEF_EXTERN(STORAGE_POSIX_SELINUX_KEY_VAR,                STORAGE_POSIX_SELINUX_KEY);
VARIANT_STRDEF_EXTERN(STORAGE_POSIX_SELINUX_XATTR_CONTEXT_VAR,      "security.selinux");

#ifdef HAVE_LIBSELINUX

VARIANT_STRDEF_EXTERN(STORAGE_POSIX_SELINUX_KEY_CONTEXT_VAR,        STORAGE_POSIX_SELINUX_KEY_CONTEXT);

#endif // HAVE_LIBSELINUX

/**********************************************************************************************************************************/
void
storagePosixSelCheck(void)
{
    FUNCTION_TEST_VOID();

#ifndef HAVE_LIBSELINUX
        THROW_FMT(OptionInvalidValueError, PROJECT_NAME " not compiled with SELinux support");
#endif // HAVE_LIBSELINUX

    FUNCTION_TEST_RETURN_VOID();
}

/**********************************************************************************************************************************/
#ifdef HAVE_LIBSELINUX

String *
storagePosixSelContextTransToRaw(const String *translated)
{
    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(STRING, translated);
    FUNCTION_LOG_END();

    String *result = NULL;

    if (translated != NULL)
    {
        char *raw = NULL;

        // Older versions of SELinux do not accept const translated input
        if (selinux_trans_to_raw_context(UNCONSTIFY(char *, strPtr(translated)), &raw) != 0)
            THROW_FMT(KernelError, "unable to convert translated context '%s' to raw", strPtr(translated));

        result = strNew(raw);
        freecon(raw);
    }

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

        // Older versions of SELinux do not accept const raw input
        if (selinux_raw_to_trans_context(UNCONSTIFY(char *, strPtr(raw)), &translated) != 0)
            THROW_FMT(KernelError, "unable to convert raw context '%s' to translated", strPtr(raw));

        result = strNew(translated);
        freecon(translated);
    }

    FUNCTION_LOG_RETURN(STRING, result);
}

#endif // HAVE_LIBSELINUX
