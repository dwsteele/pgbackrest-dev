/***********************************************************************************************************************************
Page Incremental Backup Filter

!!! Notes:
1) Store page bitmap in a separate file or in the backup file?
2) Store all backups in the page bitmap, or only the current backup?
3) Right now we read beyond the size of the file as originally reported, but we could/should stop doing that
***********************************************************************************************************************************/
#include "build.auto.h"

#include "common/debug.h"
#include "common/io/filter/filter.intern.h"
#include "command/backup/pageIncremental.h"
#include "common/log.h"
#include "common/memContext.h"
#include "common/object.h"
#include "postgres/interface/static.auto.h"

/***********************************************************************************************************************************
Filter type constant
***********************************************************************************************************************************/
STRING_EXTERN(BACKUP_PAGE_INCREMENTAL_FILTER_TYPE_STR,              BACKUP_PAGE_INCREMENTAL_FILTER_TYPE);

/***********************************************************************************************************************************
Object type
***********************************************************************************************************************************/
typedef struct PageIncremental
{
    MemContext *memContext;                                         // Mem context of filter

    uint64_t lsnCopyMin;                                            // Minimum LSN that needs to be copied
} PageIncremental;

/***********************************************************************************************************************************
Macros for function logging
***********************************************************************************************************************************/
String *
pageIncrementalToLog(const PageIncremental *this)
{
    return strNewFmt("{lsnCopyMin: %" PRIu64 "}", this->lsnCopyMin);
}

#define FUNCTION_LOG_PAGE_INCREMENTAL_TYPE                                                                                         \
    PageIncremental *
#define FUNCTION_LOG_PAGE_INCREMENTAL_FORMAT(value, buffer, bufferSize)                                                            \
    FUNCTION_LOG_STRING_OBJECT_FORMAT(value, pageIncrementalToLog, buffer, bufferSize)

/***********************************************************************************************************************************
Count bytes in the input
***********************************************************************************************************************************/
static void
pageIncrementalProcess(THIS_VOID, const Buffer *input)
{
    THIS(PageIncremental);

    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(PAGE_INCREMENTAL, this);
        FUNCTION_LOG_PARAM(BUFFER, input);
    FUNCTION_LOG_END();

    ASSERT(this != NULL);
    ASSERT(input != NULL);

    (void)this; // !!! REMOVE
    (void)input; // !!! REMOVE

    FUNCTION_LOG_RETURN_VOID();
}

/***********************************************************************************************************************************
Return filter result
***********************************************************************************************************************************/
static Variant *
pageIncrementalResult(THIS_VOID)
{
    THIS(PageIncremental);

    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(PAGE_INCREMENTAL, this);
    FUNCTION_LOG_END();

    ASSERT(this != NULL);

    (void)this; // !!! REMOVE

    FUNCTION_LOG_RETURN(VARIANT, NULL);
}

/***********************************************************************************************************************************
New object
***********************************************************************************************************************************/
IoFilter *
pageIncrementalNew(uint64_t lsnCopyMin)
{
    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(UINT64, lsnCopyMin);
    FUNCTION_LOG_END();

    IoFilter *this = NULL;

    MEM_CONTEXT_NEW_BEGIN("PageIncremental")
    {
        PageIncremental *driver = memNew(sizeof(PageIncremental));

        *driver = (PageIncremental)
        {
            .memContext = memContextCurrent(),
            .lsnCopyMin = lsnCopyMin,
        };

        // Create param list
        VariantList *paramList = varLstNew();
        varLstAdd(paramList, varNewUInt64(lsnCopyMin));

        this = ioFilterNewP(
            BACKUP_PAGE_INCREMENTAL_FILTER_TYPE_STR, driver, paramList, .in = pageIncrementalProcess,
            .result = pageIncrementalResult);
    }
    MEM_CONTEXT_NEW_END();

    FUNCTION_LOG_RETURN(IO_FILTER, this);
}

IoFilter *
pageIncrementalNewVar(const VariantList *paramList)
{
    return pageIncrementalNew(varUInt64(varLstGet(paramList, 0)));
}
