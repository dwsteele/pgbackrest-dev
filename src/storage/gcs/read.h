/***********************************************************************************************************************************
GCS Storage Read
***********************************************************************************************************************************/
#ifndef STORAGE_GCS_READ_H
#define STORAGE_GCS_READ_H

#include "storage/gcs/storage.intern.h"
#include "storage/read.h"

/***********************************************************************************************************************************
Constructors
***********************************************************************************************************************************/
StorageRead *storageReadGcsNew(StorageGcs *storage, const String *name, bool ignoreMissing);

#endif
