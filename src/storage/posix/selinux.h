/***********************************************************************************************************************************
SELinux Functions
***********************************************************************************************************************************/
#ifndef STORAGE_POSIX_SELINUX_H
#define STORAGE_POSIX_SELINUX_H

#ifdef HAVE_LIBSELINUX

#include "common/type/string.h"

/***********************************************************************************************************************************
SELinux context extended attribute
***********************************************************************************************************************************/
STRING_DECLARE(STORAGE_POSIX_SELINUX_CONTEXT_XATTR_STR);

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
// Convert an SELinux context to a raw extended attribute
String *storagePosixSelContextTransToRaw(const String *context);

// Convert a raw extended attribute to an SELinux context
String *storagePosixSelContextRawToTrans(const String *raw);

#endif // HAVE_LIBSELINUX

#endif
