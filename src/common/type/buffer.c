/***********************************************************************************************************************************
Buffer Handler
***********************************************************************************************************************************/
#include "build.auto.h"

#include <stdio.h>
#include <string.h>

#include "common/debug.h"
#include "common/type/buffer.h"
#include "common/type/object.h"

/***********************************************************************************************************************************
Constant buffers that are generally useful
***********************************************************************************************************************************/
BUFFER_STRDEF_EXTERN(BRACEL_BUF,                                    BRACEL_Z);
BUFFER_STRDEF_EXTERN(BRACER_BUF,                                    BRACER_Z);
BUFFER_STRDEF_EXTERN(BRACKETL_BUF,                                  BRACKETL_Z);
BUFFER_STRDEF_EXTERN(BRACKETR_BUF,                                  BRACKETR_Z);
BUFFER_STRDEF_EXTERN(COMMA_BUF,                                     COMMA_Z);
BUFFER_STRDEF_EXTERN(CR_BUF,                                        CR_Z);
BUFFER_STRDEF_EXTERN(DOT_BUF,                                       DOT_Z);
BUFFER_STRDEF_EXTERN(EQ_BUF,                                        EQ_Z);
BUFFER_STRDEF_EXTERN(LF_BUF,                                        LF_Z);
BUFFER_STRDEF_EXTERN(QUOTED_BUF,                                    QUOTED_Z);

/***********************************************************************************************************************************
Contains information about the buffer
***********************************************************************************************************************************/
struct Buffer
{
    BufferPub pub;                                                  // Publicly accessible variables
    MemContext *memContext;                                         // Mem context for dynamic buffers
};

OBJECT_DEFINE_MOVE(BUFFER);
OBJECT_DEFINE_FREE(BUFFER);

/**********************************************************************************************************************************/
Buffer *
bufNew(size_t size)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(SIZE, size);
    FUNCTION_TEST_END();

    Buffer *this = NULL;

    MEM_CONTEXT_NEW_BEGIN("Buffer")
    {
        // Create object
        this = memNew(sizeof(Buffer));

        *this = (Buffer)
        {
            .pub =
            {
                .sizeAlloc = size,
                .size = size,
            },
            .memContext = MEM_CONTEXT_NEW(),
        };

        // Allocate buffer
        if (size > 0)
            this->pub.buffer = memNew(this->pub.sizeAlloc);
    }
    MEM_CONTEXT_NEW_END();

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
Buffer *
bufNewC(const void *buffer, size_t size)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM_P(VOID, buffer);
        FUNCTION_TEST_PARAM(SIZE, size);
    FUNCTION_TEST_END();

    ASSERT(buffer != NULL);

    // Create object and copy data
    Buffer *this = bufNew(size);
    memcpy(this->pub.buffer, buffer, this->pub.size);
    this->pub.used = this->pub.size;

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
Buffer *
bufNewDecode(EncodeType type, const String *string)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(ENUM, type);
        FUNCTION_TEST_PARAM(STRING, string);
    FUNCTION_TEST_END();

    Buffer *this = bufNew(decodeToBinSize(type, strZ(string)));

    decodeToBin(type, strZ(string), bufPtr(this));
    bufUsedSet(this, bufSize(this));

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
Buffer *
bufDup(const Buffer *buffer)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, buffer);
    FUNCTION_TEST_END();

    ASSERT(buffer != NULL);

    // Create object and copy data
    Buffer *this = bufNew(buffer->pub.used);
    memcpy(this->pub.buffer, buffer->pub.buffer, this->pub.size);
    this->pub.used = this->pub.size;

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
Buffer *
bufCat(Buffer *this, const Buffer *cat)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM(BUFFER, cat);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);

    if (cat != NULL)
        bufCatC(this, cat->pub.buffer, 0, cat->pub.used);

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
Buffer *
bufCatC(Buffer *this, const unsigned char *cat, size_t catOffset, size_t catSize)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM_P(UCHARDATA, cat);
        FUNCTION_TEST_PARAM(SIZE, catOffset);
        FUNCTION_TEST_PARAM(SIZE, catSize);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);
    ASSERT(catSize == 0 || cat != NULL);

    if (catSize > 0)
    {
        if (this->pub.used + catSize > bufSize(this))
            bufResize(this, this->pub.used + catSize);

        // Just here to silence nonnull warnings from clang static analyzer
        ASSERT(this->pub.buffer != NULL);

        memcpy(this->pub.buffer + this->pub.used, cat + catOffset, catSize);
        this->pub.used += catSize;
    }

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
Buffer *
bufCatSub(Buffer *this, const Buffer *cat, size_t catOffset, size_t catSize)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM(BUFFER, cat);
        FUNCTION_TEST_PARAM(SIZE, catOffset);
        FUNCTION_TEST_PARAM(SIZE, catSize);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);

    if (cat != NULL)
    {
        ASSERT(catOffset <= cat->pub.used);
        ASSERT(catSize <= cat->pub.used - catOffset);

        bufCatC(this, cat->pub.buffer, catOffset, catSize);
    }

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
bool
bufEq(const Buffer *this, const Buffer *compare)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM(BUFFER, compare);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);
    ASSERT(compare != NULL);

    bool result = false;

    if (this->pub.used == compare->pub.used)
        result = memcmp(this->pub.buffer, compare->pub.buffer, compare->pub.used) == 0;

    FUNCTION_TEST_RETURN(result);
}

/**********************************************************************************************************************************/
String *
bufHex(const Buffer *this)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);

    String *result = strNew("");

    for (unsigned int bufferIdx = 0; bufferIdx < bufUsed(this); bufferIdx++)
        strCatFmt(result, "%02x", this->pub.buffer[bufferIdx]);

    FUNCTION_TEST_RETURN(result);
}

/**********************************************************************************************************************************/
Buffer *
bufResize(Buffer *this, size_t size)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM(SIZE, size);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);

    // Only resize if it the new size is different
    if (this->pub.sizeAlloc != size)
    {
        // If new size is zero then free memory if allocated
        if (size == 0)
        {
            // When setting size down to 0 the buffer should always be allocated
            ASSERT(this->pub.buffer != NULL);

            MEM_CONTEXT_BEGIN(this->memContext)
            {
                memFree(this->pub.buffer);
            }
            MEM_CONTEXT_END();

            this->pub.buffer = NULL;
            this->pub.sizeAlloc = 0;
        }
        // Else allocate or resize
        else
        {
            MEM_CONTEXT_BEGIN(this->memContext)
            {
                if (this->pub.buffer == NULL)
                    this->pub.buffer = memNew(size);
                else
                    this->pub.buffer = memResize(this->pub.buffer, size);
            }
            MEM_CONTEXT_END();

            this->pub.sizeAlloc = size;
        }

        if (this->pub.used > this->pub.sizeAlloc)
            this->pub.used = this->pub.sizeAlloc;

        if (!this->pub.sizeLimit)
            this->pub.size = this->pub.sizeAlloc;
        else if (this->pub.size > this->pub.sizeAlloc)
            this->pub.size = this->pub.sizeAlloc;
    }

    FUNCTION_TEST_RETURN(this);
}

/**********************************************************************************************************************************/
void
bufLimitClear(Buffer *this)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);

    this->pub.sizeLimit = false;
    this->pub.size = this->pub.sizeAlloc;

    FUNCTION_TEST_RETURN_VOID();
}

void
bufLimitSet(Buffer *this, size_t limit)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM(SIZE, limit);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);
    ASSERT(limit <= this->pub.sizeAlloc);
    ASSERT(limit >= this->pub.used);

    this->pub.size = limit;
    this->pub.sizeLimit = true;

    FUNCTION_TEST_RETURN_VOID();
}

/**********************************************************************************************************************************/
void
bufUsedInc(Buffer *this, size_t inc)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM(SIZE, inc);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);
    ASSERT(this->pub.used + inc <= bufSize(this));

    this->pub.used += inc;

    FUNCTION_TEST_RETURN_VOID();
}

void
bufUsedSet(Buffer *this, size_t used)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
        FUNCTION_TEST_PARAM(SIZE, used);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);
    ASSERT(used <= bufSize(this));

    this->pub.used = used;

    FUNCTION_TEST_RETURN_VOID();
}

void
bufUsedZero(Buffer *this)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(BUFFER, this);
    FUNCTION_TEST_END();

    ASSERT(this != NULL);

    this->pub.used = 0;

    FUNCTION_TEST_RETURN_VOID();
}

/**********************************************************************************************************************************/
String *
bufToLog(const Buffer *this)
{
    String *result = strNewFmt(
        "{used: %zu, size: %zu%s", this->pub.used, this->pub.size,
        this->pub.sizeLimit ? strZ(strNewFmt(", sizeAlloc: %zu}", this->pub.sizeAlloc)) : "}");

    return result;
}
