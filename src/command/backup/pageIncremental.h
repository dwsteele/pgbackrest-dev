/***********************************************************************************************************************************
Page Incremental Backup Filter

Copy only pages that have changed since the last backup.
***********************************************************************************************************************************/
#ifndef COMMAND_BACKUP_PAGE_INCREMENTAL_H
#define COMMAND_BACKUP_PAGE_INCREMENTAL_H

#include "common/io/filter/filter.h"

/***********************************************************************************************************************************
Filter type constant
***********************************************************************************************************************************/
#define BACKUP_PAGE_INCREMENTAL_FILTER_TYPE                         "pageIncrementalBackup"
    STRING_DECLARE(PAGE_INCREMENTAL_BACKUP_FILTER_TYPE_STR);

/***********************************************************************************************************************************
Constructor
***********************************************************************************************************************************/
IoFilter *pageIncrementalNew(uint64_t lsnCopyMin);
IoFilter *pageIncrementalNewVar(const VariantList *paramList);

#endif
