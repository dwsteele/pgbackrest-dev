/***********************************************************************************************************************************
Posix Extended Attribute Test Harness
***********************************************************************************************************************************/
#include "common/assert.h"
#include "common/type/keyValue.h"

#include "common/harnessDebug.h"
#include "common/harnessXattr.h"

/***********************************************************************************************************************************
Local variables
***********************************************************************************************************************************/
static struct HarnessXAttrLocal
{
    MemContext *memContext;                                         // Mem context for storage helper
    KeyValue *xAttr;                                                // Key value store extended attributes
} harnessXAttrLocal;

/**********************************************************************************************************************************/
String *
storagePosixInfoXAttr(const String *path, const String *name)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRING, path);
        FUNCTION_HARNESS_PARAM(STRING, name);
    FUNCTION_HARNESS_END();

    ASSERT(path != NULL);
    ASSERT(name != NULL);

    String *result = strDup(varStr(kvGet(harnessXAttrLocal.xAttr, VARSTR(strNewFmt("%s:%s", strPtr(path), strPtr(name))))));

    FUNCTION_HARNESS_RESULT(STRING, result);
}

/**********************************************************************************************************************************/
void
storagePosixInfoXAttrSet(const String *path, const String *name, const Buffer *value)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(STRING, path);
        FUNCTION_HARNESS_PARAM(STRING, name);
        FUNCTION_HARNESS_PARAM(BUFFER, value);
    FUNCTION_HARNESS_END();

    ASSERT(path != NULL);
    ASSERT(name != NULL);
    ASSERT(value != NULL);

    if (harnessXAttrLocal.memContext == NULL)
    {
        harnessXAttrLocal.memContext = memContextNew("HarnessXAttrShim");

        MEM_CONTEXT_BEGIN(harnessXAttrLocal.memContext)
        {
            harnessXAttrLocal.xAttr = kvNew();
        }
        MEM_CONTEXT_END();
    }

    kvPut(harnessXAttrLocal.xAttr, VARSTR(strNewFmt("%s:%s", strPtr(path), strPtr(name))), VARSTR(strNewBuf(value)));

    FUNCTION_HARNESS_RESULT_VOID();
}
