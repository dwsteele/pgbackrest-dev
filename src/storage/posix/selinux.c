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
SELinux constants
***********************************************************************************************************************************/
// MLS extension keys
VARIANT_STRDEF_EXTERN(STORAGE_POSIX_MLS_KEY_VAR,                                STORAGE_POSIX_MLS_KEY);
VARIANT_STRDEF_EXTERN(STORAGE_POSIX_MLS_SELINUX_CONTEXT_RAW_KEY_VAR,            STORAGE_POSIX_MLS_SELINUX_CONTEXT_RAW_KEY);
VARIANT_STRDEF_EXTERN(STORAGE_POSIX_MLS_SELINUX_CONTEXT_TRANSLATED_KEY_VAR,     STORAGE_POSIX_MLS_SELINUX_CONTEXT_TRANSLATED_KEY);

// Extended attribute
STRING_EXTERN(STORAGE_POSIX_SELINUX_XATTR_CONTEXT_STR,              STORAGE_POSIX_SELINUX_XATTR_CONTEXT);

#ifdef HAVE_LIBSELINUX

/**********************************************************************************************************************************/
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
        if (selinux_trans_to_raw_context(UNCONSTIFY(char *, strZ(translated)), &raw) != 0)
            THROW_FMT(KernelError, "unable to convert translated context '%s' to raw", strZ(translated));

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
        if (selinux_raw_to_trans_context(UNCONSTIFY(char *, strZ(raw)), &translated) != 0)
            THROW_FMT(KernelError, "unable to convert raw context '%s' to translated", strZ(raw));

        result = strNew(translated);
        freecon(translated);
    }

    FUNCTION_LOG_RETURN(STRING, result);
}

#endif // HAVE_LIBSELINUX
