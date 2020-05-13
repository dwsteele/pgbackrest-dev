/***********************************************************************************************************************************
SELinux Functions
***********************************************************************************************************************************/
#ifndef STORAGE_POSIX_SELINUX_H
#define STORAGE_POSIX_SELINUX_H

#include "common/type/variant.h"

/***********************************************************************************************************************************
SELinux constants
***********************************************************************************************************************************/
#define STORAGE_POSIX_SELINUX_KEY                                   "sel"
    VARIANT_DECLARE(STORAGE_POSIX_SELINUX_KEY_VAR);

#ifdef HAVE_LIBSELINUX

#define STORAGE_POSIX_SELINUX_KEY_CONTEXT                           "ctx"
    VARIANT_DECLARE(STORAGE_POSIX_SELINUX_KEY_CONTEXT_VAR);

// Extended attribute
VARIANT_DECLARE(STORAGE_POSIX_SELINUX_XATTR_CONTEXT_VAR);

#endif // HAVE_LIBSELINUX

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
// Was the binary compiled with SELinux?
void storagePosixSelCheck(void);

#ifdef HAVE_LIBSELINUX

// Convert an SELinux context to a raw extended attribute
String *storagePosixSelContextTransToRaw(const String *context);

// Convert a raw extended attribute to an SELinux context
String *storagePosixSelContextRawToTrans(const String *raw);

#endif // HAVE_LIBSELINUX

#endif
