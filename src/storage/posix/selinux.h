/***********************************************************************************************************************************
SELinux Functions
***********************************************************************************************************************************/
#ifndef STORAGE_POSIX_SELINUX_H
#define STORAGE_POSIX_SELINUX_H

#include "common/type/variant.h"

/***********************************************************************************************************************************
SELinux constants
***********************************************************************************************************************************/
// MLS extension keys
#define STORAGE_POSIX_MLS_KEY                                       "mls"
    VARIANT_DECLARE(STORAGE_POSIX_MLS_KEY_VAR);
#define STORAGE_POSIX_MLS_SELINUX_CONTEXT_RAW_KEY                   "scr"
    VARIANT_DECLARE(STORAGE_POSIX_MLS_SELINUX_CONTEXT_RAW_KEY_VAR);
#define STORAGE_POSIX_MLS_SELINUX_CONTEXT_TRANSLATED_KEY            "sct"
    VARIANT_DECLARE(STORAGE_POSIX_MLS_SELINUX_CONTEXT_TRANSLATED_KEY_VAR);

// Extended attribute
#define STORAGE_POSIX_SELINUX_XATTR_CONTEXT                         "security.selinux"
    STRING_DECLARE(STORAGE_POSIX_SELINUX_XATTR_CONTEXT_STR);

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
#ifdef HAVE_LIBSELINUX

// Convert an SELinux context to a raw extended attribute
String *storagePosixSelContextTransToRaw(const String *context);

// Convert a raw extended attribute to an SELinux context
String *storagePosixSelContextRawToTrans(const String *raw);

#endif // HAVE_LIBSELINUX

#endif
