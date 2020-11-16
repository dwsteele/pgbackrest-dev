/***********************************************************************************************************************************
Posix Extended Attributes
***********************************************************************************************************************************/
#ifndef STORAGE_POSIX_XATTR_H
#define STORAGE_POSIX_XATTR_H

#include "common/type/variant.h"

/***********************************************************************************************************************************
Extended attribute constants
***********************************************************************************************************************************/
#define STORAGE_POSIX_XATTR_KEY                                     "xtr"
    VARIANT_DECLARE(STORAGE_POSIX_XATTR_KEY_VAR);

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
#ifdef HAVE_XATTR

String *storagePosixInfoXAttr(const String *path, bool followLink, const String *name);
void storagePosixInfoXAttrSet(const String *path, bool followLink, const String *name, const Buffer *value);

#endif // HAVE_XATTR

#endif
