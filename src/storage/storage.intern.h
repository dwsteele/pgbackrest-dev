/***********************************************************************************************************************************
Storage Interface Internal
***********************************************************************************************************************************/
#ifndef STORAGE_STORAGE_INTERN_H
#define STORAGE_STORAGE_INTERN_H

#include "common/type/param.h"
#include "storage/read.intern.h"
#include "storage/storage.h"
#include "storage/write.intern.h"

/***********************************************************************************************************************************
Default file and path modes
***********************************************************************************************************************************/
#define STORAGE_MODE_FILE_DEFAULT                                   0640
#define STORAGE_MODE_PATH_DEFAULT                                   0750

/***********************************************************************************************************************************
Error messages
***********************************************************************************************************************************/
#define STORAGE_ERROR_READ_CLOSE                                    "unable to close file '%s' after read"
#define STORAGE_ERROR_READ_OPEN                                     "unable to open file '%s' for read"
#define STORAGE_ERROR_READ_MISSING                                  "unable to open missing file '%s' for read"

#define STORAGE_ERROR_INFO                                          "unable to get info for path/file '%s'"
#define STORAGE_ERROR_INFO_MISSING                                  "unable to get info for missing path/file '%s'"

#define STORAGE_ERROR_LIST                                          "unable to list files for path '%s'"
#define STORAGE_ERROR_LIST_MISSING                                  "unable to list files for missing path '%s'"

#define STORAGE_ERROR_LIST_INFO                                     "unable to list file info for path '%s'"
#define STORAGE_ERROR_LIST_INFO_MISSING                             "unable to list file info for missing path '%s'"

#define STORAGE_ERROR_PATH_REMOVE                                   "unable to remove path '%s'"
#define STORAGE_ERROR_PATH_REMOVE_FILE                              "unable to remove file '%s'"
#define STORAGE_ERROR_PATH_REMOVE_MISSING                           "unable to remove missing path '%s'"

#define STORAGE_ERROR_PATH_SYNC                                     "unable to sync path '%s'"
#define STORAGE_ERROR_PATH_SYNC_CLOSE                               "unable to close path '%s' after sync"
#define STORAGE_ERROR_PATH_SYNC_OPEN                                "unable to open path '%s' for sync"
#define STORAGE_ERROR_PATH_SYNC_MISSING                             "unable to sync missing path '%s'"

#define STORAGE_ERROR_WRITE_CLOSE                                   "unable to close file '%s' after write"
#define STORAGE_ERROR_WRITE_OPEN                                    "unable to open file '%s' for write"
#define STORAGE_ERROR_WRITE_MISSING                                 "unable to open file '%s' for write in missing path"
#define STORAGE_ERROR_WRITE_SYNC                                    "unable to sync file '%s' after write"

/***********************************************************************************************************************************
Path expression callback function type - used to modify paths based on expressions enclosed in <>
***********************************************************************************************************************************/
typedef String *StoragePathExpressionCallback(const String *expression, const String *path);

/***********************************************************************************************************************************
Required interface functions
***********************************************************************************************************************************/
// Does a file exist? This function is only for files, not paths.
typedef struct StorageInterfaceExistsParam
{
    VAR_PARAM_HEADER;
} StorageInterfaceExistsParam;

typedef bool StorageInterfaceExists(void *thisVoid, const String *file, StorageInterfaceExistsParam param);

#define storageInterfaceExistsP(thisVoid, file, ...)                                                                               \
    STORAGE_COMMON_INTERFACE(thisVoid).exists(thisVoid, file, (StorageInterfaceExistsParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Get information about a file
typedef struct StorageInterfaceInfoParam
{
    VAR_PARAM_HEADER;

    // Should symlinks be followed?  Only required on storage that supports symlinks.
    bool followLink;
} StorageInterfaceInfoParam;

typedef StorageInfo StorageInterfaceInfo(void *thisVoid, const String *file, StorageInterfaceInfoParam param);

#define storageInterfaceInfoP(thisVoid, file, ...)                                                                                 \
    STORAGE_COMMON_INTERFACE(thisVoid).info(thisVoid, file, (StorageInterfaceInfoParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Get a list of files
typedef struct StorageInterfaceListParam
{
    VAR_PARAM_HEADER;

    // Regular expression used to filter the results
    const String *expression;
} StorageInterfaceListParam;

typedef StringList *StorageInterfaceList(void *thisVoid, const String *path, StorageInterfaceListParam param);

#define storageInterfaceListP(thisVoid, path, ...)                                                                                 \
    STORAGE_COMMON_INTERFACE(thisVoid).list(thisVoid, path, (StorageInterfaceListParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Create a file read object.  The file should not be opened immediately -- open() will be called on the IoRead interface when the
// file needs to be opened.
typedef struct StorageInterfaceNewReadParam
{
    VAR_PARAM_HEADER;

    // Is the file compressible?  This is useful when the file must be moved across a network and some temporary compression is
    // helpful.
    bool compressible;

    // Limit bytes read from the file
    const Variant *limit;
} StorageInterfaceNewReadParam;

typedef StorageRead *StorageInterfaceNewRead(
    void *thisVoid, const String *file, bool ignoreMissing, StorageInterfaceNewReadParam param);

#define storageInterfaceNewReadP(thisVoid, file, ignoreMissing, ...)                                                               \
    STORAGE_COMMON_INTERFACE(thisVoid).newRead(                                                                                    \
        thisVoid, file, ignoreMissing, (StorageInterfaceNewReadParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Create a file write object.  The file should not be opened immediately -- open() will be called on the IoWrite interface when the
// file needs to be opened.
typedef struct StorageInterfaceNewWriteParam
{
    VAR_PARAM_HEADER;

    // File/path mode for storage that supports Posix-style permissions.  modePath is only used in conjunction with createPath.
    mode_t modeFile;
    mode_t modePath;

    // User/group name
    const String *user;
    const String *group;

    // Modified time
    time_t timeModified;

    // Will paths be created as needed?
    bool createPath;

    // Sync file/path when required by the storage
    bool syncFile;
    bool syncPath;

    // Ensure the file written atomically.  If this is false it's OK to write atomically if that's all the storage supperts
    // (e.g. S3).  Non-atomic writes are used in some places where there is a performance advantage and atomicity is not needed.
    bool atomic;

    // Is the file compressible?  This is useful when the file must be moved across a network and some temporary compression is
    // helpful.
    bool compressible;
} StorageInterfaceNewWriteParam;

typedef StorageWrite *StorageInterfaceNewWrite(void *thisVoid, const String *file, StorageInterfaceNewWriteParam param);

#define storageInterfaceNewWriteP(thisVoid, file, ...)                                                                             \
    STORAGE_COMMON_INTERFACE(thisVoid).newWrite(thisVoid, file, (StorageInterfaceNewWriteParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Get info for a path and all paths/files in the path (does not recurse)
typedef struct StorageInterfaceInfoListParam
{
    VAR_PARAM_HEADER;
} StorageInterfaceInfoListParam;

typedef bool StorageInterfaceInfoList(
    void *thisVoid, const String *path, StorageInfoListCallback callback, void *callbackData, StorageInterfaceInfoListParam param);

#define storageInterfaceInfoListP(thisVoid, path, callback, callbackData, ...)                                                     \
    STORAGE_COMMON_INTERFACE(thisVoid).infoList(                                                                                   \
        thisVoid, path, callback, callbackData, (StorageInterfaceInfoListParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Remove a path (and optionally recurse)
typedef struct StorageInterfacePathRemoveParam
{
    VAR_PARAM_HEADER;
} StorageInterfacePathRemoveParam;

typedef bool StorageInterfacePathRemove(void *thisVoid, const String *path, bool recurse, StorageInterfacePathRemoveParam param);

#define storageInterfacePathRemoveP(thisVoid, path, recurse, ...)                                                                  \
    STORAGE_COMMON_INTERFACE(thisVoid).pathRemove(                                                                                 \
        thisVoid, path, recurse, (StorageInterfacePathRemoveParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Remove a file
typedef struct StorageInterfaceRemoveParam
{
    VAR_PARAM_HEADER;

    // Error when the file to delete is missing
    bool errorOnMissing;
} StorageInterfaceRemoveParam;

typedef void StorageInterfaceRemove(void *thisVoid, const String *file, StorageInterfaceRemoveParam param);

#define storageInterfaceRemoveP(thisVoid, file, ...)                                                                               \
    STORAGE_COMMON_INTERFACE(thisVoid).remove(thisVoid, file, (StorageInterfaceRemoveParam){VAR_PARAM_INIT, __VA_ARGS__})

/***********************************************************************************************************************************
Optional interface functions
***********************************************************************************************************************************/
// Move a file atomically
typedef struct StorageInterfaceMoveParam
{
    VAR_PARAM_HEADER;
} StorageInterfaceMoveParam;

typedef bool StorageInterfaceMove(void *thisVoid, StorageRead *source, StorageWrite *destination, StorageInterfaceMoveParam param);

#define storageInterfaceMoveP(thisVoid, source, destination, ...)                                                                  \
    STORAGE_COMMON_INTERFACE(thisVoid).move(                                                                                       \
        thisVoid, source, destination, (StorageInterfaceMoveParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Create a path
typedef struct StorageInterfacePathCreateParam
{
    VAR_PARAM_HEADER;
} StorageInterfacePathCreateParam;

typedef void StorageInterfacePathCreate(
    void *thisVoid, const String *path, bool errorOnExists, bool noParentCreate, mode_t mode,
    StorageInterfacePathCreateParam param);

#define storageInterfacePathCreateP(thisVoid, path, errorOnExists, noParentCreate, mode, ...)                                      \
    STORAGE_COMMON_INTERFACE(thisVoid).pathCreate(                                                                                 \
        thisVoid, path, errorOnExists, noParentCreate, mode, (StorageInterfacePathCreateParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Does a path exist?
typedef struct StorageInterfacePathExistsParam
{
    VAR_PARAM_HEADER;
} StorageInterfacePathExistsParam;

typedef bool StorageInterfacePathExists(void *thisVoid, const String *path, StorageInterfacePathExistsParam param);

#define storageInterfacePathExistsP(thisVoid, path, ...)                                                                           \
    STORAGE_COMMON_INTERFACE(thisVoid).pathExists(thisVoid, path, (StorageInterfacePathExistsParam){VAR_PARAM_INIT, __VA_ARGS__})

// ---------------------------------------------------------------------------------------------------------------------------------
// Sync a path
typedef struct StorageInterfacePathSyncParam
{
    VAR_PARAM_HEADER;
} StorageInterfacePathSyncParam;

typedef void StorageInterfacePathSync(void *thisVoid, const String *path, StorageInterfacePathSyncParam param);

#define storageInterfacePathSyncP(thisVoid, path, ...)                                                                             \
    STORAGE_COMMON_INTERFACE(thisVoid).pathSync(thisVoid, path, (StorageInterfacePathSyncParam){VAR_PARAM_INIT, __VA_ARGS__})

/***********************************************************************************************************************************
Constructor
***********************************************************************************************************************************/
typedef struct StorageInterface
{
    // Features implemented by the storage driver
    uint64_t feature;

    // Required functions
    StorageInterfaceExists *exists;
    StorageInterfaceInfo *info;
    StorageInterfaceInfoList *infoList;
    StorageInterfaceList *list;
    StorageInterfaceNewRead *newRead;
    StorageInterfaceNewWrite *newWrite;
    StorageInterfacePathRemove *pathRemove;
    StorageInterfaceRemove *remove;

    // Optional functions
    StorageInterfaceMove *move;
    StorageInterfacePathCreate *pathCreate;
    StorageInterfacePathExists *pathExists;
    StorageInterfacePathSync *pathSync;
} StorageInterface;

#define storageNewP(type, path, modeFile, modePath, write, pathExpressionFunction, driver, ...)                                    \
    storageNew(type, path, modeFile, modePath, write, pathExpressionFunction, driver, (StorageInterface){__VA_ARGS__})

Storage *storageNew(
    const String *type, const String *path, mode_t modeFile, mode_t modePath, bool write,
    StoragePathExpressionCallback pathExpressionFunction, void *driver, StorageInterface interface);

/***********************************************************************************************************************************
Common members to include in every storage driver and macros to extract the common elements
***********************************************************************************************************************************/
#define STORAGE_COMMON_MEMBER                                                                                                      \
    StorageInterface interface                                      /* Storage interface */

typedef struct StorageCommon
{
    STORAGE_COMMON_MEMBER;
} StorageCommon;

#define STORAGE_COMMON(thisVoid)                                                                                                   \
    ((const StorageCommon *)thisVoid)
#define STORAGE_COMMON_INTERFACE(thisVoid)                                                                                         \
    (STORAGE_COMMON(thisVoid)->interface)

/***********************************************************************************************************************************
Getters/Setters
***********************************************************************************************************************************/
void *storageDriver(const Storage *this);
StorageInterface storageInterface(const Storage *this);

/***********************************************************************************************************************************
Macros for function logging
***********************************************************************************************************************************/
#define FUNCTION_LOG_STORAGE_INTERFACE_TYPE                                                                                        \
    StorageInterface
#define FUNCTION_LOG_STORAGE_INTERFACE_FORMAT(value, buffer, bufferSize)                                                           \
    objToLog(&value, "StorageInterface", buffer, bufferSize)

#endif
