/***********************************************************************************************************************************
Posix Extended Attribute Test Harness

Shim functions for getting and setting extended attributes that cannot normally be set.
***********************************************************************************************************************************/
#include <sys/stat.h>
#include <unistd.h>

#include "common/assert.h"
#include "common/type/keyValue.h"
#include "storage/posix/xattr.h"

#include "common/harnessDebug.h"

/***********************************************************************************************************************************
Extended attribute constants (duplicated so shim works)
***********************************************************************************************************************************/
VARIANT_STRDEF_EXTERN(STORAGE_POSIX_XATTR_KEY_VAR,                  STORAGE_POSIX_XATTR_KEY);

/***********************************************************************************************************************************
Local variables
***********************************************************************************************************************************/
static struct HarnessXAttrLocal
{
    MemContext *memContext;                                         // Mem context for storage helper
    KeyValue *xAttr;                                                // Key value store extended attributes
} harnessXAttrLocal;

/***********************************************************************************************************************************
***********************************************************************************************************************************/
static String *
hrnXAttrLinkToPath(const String *path, bool followLink)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRING, path);
        FUNCTION_HARNESS_PARAM(BOOL, followLink);
    FUNCTION_HARNESS_END();

    String *result = strDup(path);

    if (followLink)
    {
        struct stat statFile;

        THROW_ON_SYS_ERROR_FMT(lstat(strZ(path), &statFile) == -1, FileOpenError, "unable to stat '%s'", strZ(path));

        if (S_ISLNK(statFile.st_mode))
        {
            char linkDestination[PATH_MAX];
            ssize_t linkDestinationSize = 0;

            THROW_ON_SYS_ERROR_FMT(
                (linkDestinationSize = readlink(strZ(path), linkDestination, sizeof(linkDestination) - 1)) == -1,
                FileReadError, "unable to get destination for link '%s'", strZ(path));

            result = strNewN(linkDestination, (size_t)linkDestinationSize);
        }
    }


    FUNCTION_HARNESS_RESULT(STRING, result);
}

/**********************************************************************************************************************************/
String *
storagePosixInfoXAttr(const String *path, bool followLink, const String *name)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRING, path);
        FUNCTION_HARNESS_PARAM(BOOL, followLink);
        FUNCTION_HARNESS_PARAM(STRING, name);
    FUNCTION_HARNESS_END();

    ASSERT(path != NULL);
    ASSERT(name != NULL);

    String *result = NULL;

    if (harnessXAttrLocal.xAttr != NULL)
    {
        // Get link destination if this is a link and links are followed
        path = hrnXAttrLinkToPath(path, followLink);

        result = strDup(varStr(kvGet(harnessXAttrLocal.xAttr, VARSTR(strNewFmt("%s:%s", strZ(path), strZ(name))))));
    }

    FUNCTION_HARNESS_RESULT(STRING, result);
}

/**********************************************************************************************************************************/
void
storagePosixInfoXAttrSet(const String *path, bool followLink, const String *name, const Buffer *value)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRING, path);
        FUNCTION_HARNESS_PARAM(BOOL, followLink);
        FUNCTION_HARNESS_PARAM(STRING, name);
        FUNCTION_HARNESS_PARAM(BUFFER, value);
    FUNCTION_HARNESS_END();

    ASSERT(path != NULL);
    ASSERT(name != NULL);
    ASSERT(value != NULL);

    if (harnessXAttrLocal.memContext == NULL)
    {
        MEM_CONTEXT_BEGIN(memContextTop())
        {
            harnessXAttrLocal.memContext = memContextNew("HarnessXAttrShim");

            MEM_CONTEXT_BEGIN(harnessXAttrLocal.memContext)
            {
                harnessXAttrLocal.xAttr = kvNew();
            }
            MEM_CONTEXT_END();

            memContextKeep();
        }
        MEM_CONTEXT_END();
    }

    // if (strEqZ(path, "/home/vagrant/test/test-0/pg/pg_wal") && followLink)
    //     THROW_FMT(AssertError, "FOLLOWING PATH LINK %s", strZ(hrnXAttrLinkToPath(path, followLink)));

    // Get link destination if this is a link and links are followed
    path = hrnXAttrLinkToPath(path, followLink);


    kvPut(harnessXAttrLocal.xAttr, VARSTR(strNewFmt("%s:%s", strZ(path), strZ(name))), VARSTR(strNewBuf(value)));

    FUNCTION_HARNESS_RESULT_VOID();
}
