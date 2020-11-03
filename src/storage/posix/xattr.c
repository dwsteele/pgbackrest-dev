/***********************************************************************************************************************************
Posix Extended Attributes
***********************************************************************************************************************************/
#include "build.auto.h"

#ifdef HAVE_XATTR
#include <sys/xattr.h>
#endif // HAVE_XATTR

#include "common/debug.h"
#include "common/log.h"
#include "common/memContext.h"
#include "storage/posix/xattr.h"

/***********************************************************************************************************************************
Extended attribute constants
***********************************************************************************************************************************/
VARIANT_STRDEF_EXTERN(STORAGE_POSIX_XATTR_KEY_VAR,                  STORAGE_POSIX_XATTR_KEY);

/**********************************************************************************************************************************/
#ifdef HAVE_XATTR

String *
storagePosixInfoXAttr(const String *path, bool followLink, const String *name)
{
    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(STRING, path);
        FUNCTION_LOG_PARAM(BOOL, followLink);
        FUNCTION_LOG_PARAM(STRING, name);
    FUNCTION_LOG_END();

    ASSERT(path != NULL);
    ASSERT(name != NULL);

    String *result = NULL;

    MEM_CONTEXT_TEMP_BEGIN()
    {
        Buffer *buffer = bufNew(256);
        ssize_t getResult;

        do
        {
            if (followLink)
                getResult = getxattr(strZ(path), strZ(name), bufPtr(buffer), bufSize(buffer) - 1);
            else
                getResult = lgetxattr(strZ(path), strZ(name), bufPtr(buffer), bufSize(buffer) - 1);

            if (getResult == -1)
            {
                // !!! THIS SHOULD BE ENOATTR BUT THAT REQUIRES A SPECIAL PACKAGE TO BE INSTALLED
                if (errno == ENODATA)
                    break;

                if (errno == ERANGE)
                {
                    ssize_t size = followLink ?
                        getxattr(strZ(path), strZ(name), NULL, 0) : lgetxattr(strZ(path), strZ(name), NULL, 0);

                    THROW_ON_SYS_ERROR_FMT(
                        size == -1, FileReadError, "unable to get xattr '%s' size for path '%s'", strZ(name), strZ(path));

                    LOG_DEBUG_FMT("resize to %zd", size);

                    bufResize(buffer, (size_t)size + 1);
                }
                else
                {
                    THROW_SYS_ERROR_CODE_FMT(
                        errno, FileReadError, "unable to get xattr '%s' for path '%s'", strZ(name), strZ(path));
                }
            }
            else
            {
                bufUsedSet(buffer, (size_t)getResult);
                bufPtr(buffer)[getResult] = '\0';

                MEM_CONTEXT_PRIOR_BEGIN()
                {
                    result = strNewBuf(buffer);
                }
                MEM_CONTEXT_PRIOR_END();
            }
        }
        while (getResult == -1);
    }
    MEM_CONTEXT_TEMP_END();

    FUNCTION_LOG_RETURN(STRING, result);
}

#endif // HAVE_XATTR
