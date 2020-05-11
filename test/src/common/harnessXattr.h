/***********************************************************************************************************************************
Posix Extended Attribute Test Harness

Shim functions for getting and setting extended attributes that cannot normally be set.
***********************************************************************************************************************************/
#ifndef TEST_COMMON_HARNESS_XATTR_H
#define TEST_COMMON_HARNESS_XATTR_H

#include "common/type/string.h"

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
void storagePosixInfoXAttrSet(const String *path, const String *name, const Buffer *value);

#endif
