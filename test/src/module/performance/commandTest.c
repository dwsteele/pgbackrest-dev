/***********************************************************************************************************************************
Command Performance

Test the performance of various commands.

Generally speaking, the starting values should be high enough to "blow up" in terms of execution time if there are performance
problems without taking very long if everything is running smoothly. These starting values can then be scaled up for profiling and
stress testing as needed.
***********************************************************************************************************************************/
#include <unistd.h>

#include "common/harnessConfig.h"

#include "command/backup/backup.h"
#include "command/stanza/create.h"
#include "postgres/interface.h"
#include "storage/posix/storage.h"

/***********************************************************************************************************************************
Test Run
***********************************************************************************************************************************/
void
testRun(void)
{
    FUNCTION_HARNESS_VOID();

    // *****************************************************************************************************************************
    if (testBegin("cmdBackup()"))
    {
        // One million files represents a fairly large cluster
        CHECK(testScale() <= 20000);
        unsigned int fileTotal = 1000 * (unsigned int)testScale();

        // !!! Create files in the pg path
        TEST_LOG_FMT("generate %u files in pg path", fileTotal);

        const String *pgPath = strNewFmt("%s/pg", testPath());
        const String *repoPath = strNewFmt("%s/repo", testPath());

        Storage *storagePg = storagePosixNewP(pgPath, .write = true);
        for (unsigned int fileIdx = 0; fileIdx < fileTotal; fileIdx++)
            storagePutP(storageNewWriteP(storagePg, strNewFmt("base/1/%u", 16384 + fileIdx)), BUFSTRDEF("TESTFILE"));
        storagePathCreateP(storagePg, STRDEF("global"));
        THROW_ON_SYS_ERROR(
            symlink(
                "../../../pg/global/pg_control", strZ(strNewFmt("%s/" PG_PATH_GLOBAL "/" PG_FILE_PGCONTROL, strZ(pgPath)))) == -1,
            FileOpenError, "unable to create symlink pg data");

        // THROW_ON_SYS_ERROR(
        //     symlink("../pg", strZ(pgPath)) == -1, FileOpenError, "unable to create symlink pg data");

        harnessLogLevelSet(logLevelError);

        // Create stanza
        // -------------------------------------------------------------------------------------------------------------------------
        StringList *argList = strLstNew();
        strLstAddZ(argList, "--" CFGOPT_STANZA "=test");
        strLstAdd(argList, strNewFmt("--" CFGOPT_PG1_PATH "=%s", strZ(pgPath)));
        strLstAdd(argList, strNewFmt("--" CFGOPT_REPO1_PATH "=%s", strZ(repoPath)));
        strLstAddZ(argList, "--no-" CFGOPT_ONLINE);
        harnessCfgLoad(cfgCmdStanzaCreate, argList);

        TEST_RESULT_VOID(cmdStanzaCreate(), "stanza create");

        // Perform backup
        // -------------------------------------------------------------------------------------------------------------------------
        strLstAddZ(argList, "--" CFGOPT_COMPRESS_TYPE "=none");
        strLstAddZ(argList, "--" CFGOPT_PROCESS_MAX "=8");
        strLstAddZ(argList, "--no-" CFGOPT_CHECKSUM_PAGE);
        harnessCfgLoad(cfgCmdBackup, argList);

        TEST_RESULT_VOID(cmdBackup(), "backup");
    }

    FUNCTION_HARNESS_RESULT_VOID();
}
