/***********************************************************************************************************************************
Test Posix Extended Attributes
***********************************************************************************************************************************/
#include <unistd.h>

#include "storage/posix/storage.h"

/***********************************************************************************************************************************
Test function to set extended attributes
***********************************************************************************************************************************/
#ifdef HAVE_XATTR

static void
storagePosixInfoXAttrSet(const String *path, const String *name, const Buffer *value)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRING, path);
        FUNCTION_HARNESS_PARAM(STRING, name);
        FUNCTION_HARNESS_PARAM(BUFFER, value);
    FUNCTION_HARNESS_END();

    ASSERT(path != NULL);
    ASSERT(name != NULL);
    ASSERT(value != NULL);

    THROW_ON_SYS_ERROR_FMT(
        lsetxattr(strZ(path), strZ(name), bufPtrConst(value), bufSize(value), 0), FileWriteError,
        "unable to set xattr '%s' on '%s'", strZ(name), strZ(path));

    FUNCTION_HARNESS_RESULT_VOID();
}

#endif // HAVE_XATTR

/***********************************************************************************************************************************
Test Run
***********************************************************************************************************************************/
void
testRun(void)
{
    FUNCTION_HARNESS_VOID();

    // Create default storage object for testing
#ifdef HAVE_XATTR
    Storage *storageTest = storagePosixNewP(strNew(testPath()), .write = true);
#endif // HAVE_XATTR

    // *****************************************************************************************************************************
    if (testBegin("storagePosixInfoXAttr()"))
    {
#ifdef HAVE_XATTR
        const String *attrName = STRDEF("user.pgbackrest");
        const String *fileName = strNewFmt("%s/test.txt", testPath());

        storagePutP(storageNewWriteP(storageTest, fileName), BUFSTRDEF("DATA"));

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("error on missing file");

        TEST_ERROR(
            storagePosixInfoXAttr(STRDEF(BOGUS_STR), false, attrName), FileReadError,
            "unable to get xattr 'user.pgbackrest' for path 'BOGUS': [2] No such file or directory");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("extended attribute does not exist");

        TEST_RESULT_PTR(storagePosixInfoXAttr(fileName, false, attrName), NULL, "check attr");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("short extended attribute");

        const Buffer *attrVal = BUFSTRDEF("sample");

        storagePosixInfoXAttrSet(fileName, attrName, attrVal);
        TEST_RESULT_STR(storagePosixInfoXAttr(fileName, false, attrName), strNewBuf(attrVal), "check attr");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("link to file has no attribute");

        const String *linkName = strNewFmt("%s/link", testPath());

        THROW_ON_SYS_ERROR(symlink(strZ(fileName), strZ(linkName)) == -1, FileOpenError, "unable to create symlink");

        TEST_RESULT_PTR(storagePosixInfoXAttr(linkName, false, attrName), NULL, "check attr");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("link to file with short extended attribute");

        TEST_RESULT_STR(storagePosixInfoXAttr(linkName, true, attrName), strNewBuf(attrVal), "check attr");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("long extended attribute");

        const Buffer *attrValLong = BUFSTRDEF(
            "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
            "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy"
            "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");

        storagePosixInfoXAttrSet(fileName, attrName, attrValLong);
        TEST_RESULT_STR(storagePosixInfoXAttr(fileName, false, attrName), strNewBuf(attrValLong), "check attr");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("link to file with long extended attribute");

        TEST_RESULT_STR(storagePosixInfoXAttr(linkName, true, attrName), strNewBuf(attrValLong), "check attr");
#endif // HAVE_XATTR
    }

    FUNCTION_HARNESS_RESULT_VOID();
}
