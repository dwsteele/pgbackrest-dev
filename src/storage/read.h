/***********************************************************************************************************************************
Storage Read Interface
***********************************************************************************************************************************/
#ifndef STORAGE_READ_H
#define STORAGE_READ_H

/***********************************************************************************************************************************
Object type
***********************************************************************************************************************************/
#define STORAGE_READ_TYPE                                           StorageRead
#define STORAGE_READ_PREFIX                                         storageRead

typedef struct StorageRead StorageRead;

#include "common/io/read.h"

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
StorageRead *storageReadMove(StorageRead *this, MemContext *parentNew);

/***********************************************************************************************************************************
Getters
***********************************************************************************************************************************/
IoRead *storageReadIo(const StorageRead *this);

// Is there a read limit?
const Variant *storageReadLimit(const StorageRead *this);

bool storageReadIgnoreMissing(const StorageRead *this);
const String *storageReadName(const StorageRead *this);
const String *storageReadType(const StorageRead *this);

/***********************************************************************************************************************************
Destructor
***********************************************************************************************************************************/
void storageReadFree(StorageRead *this);

/***********************************************************************************************************************************
Macros for function logging
***********************************************************************************************************************************/
String *storageReadToLog(const StorageRead *this);

#define FUNCTION_LOG_STORAGE_READ_TYPE                                                                                             \
    StorageRead *
#define FUNCTION_LOG_STORAGE_READ_FORMAT(value, buffer, bufferSize)                                                                \
    FUNCTION_LOG_STRING_OBJECT_FORMAT(value, storageReadToLog, buffer, bufferSize)

#endif
