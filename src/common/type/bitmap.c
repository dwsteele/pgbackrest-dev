/***********************************************************************************************************************************
Bitmap Handler
***********************************************************************************************************************************/
#include "build.auto.h"

#include <stdio.h>
#include <string.h>

#include "common/debug.h"
#include "common/type/bitmap.h"
#include "common/object.h"

/***********************************************************************************************************************************
Contains information about the buffer
***********************************************************************************************************************************/
struct Bitmap
{
    MemContext *memContext;                                         // Mem context
    unsigned char *buffer;                                          // Internal buffer
};

OBJECT_DEFINE_MOVE(BITMAP);
OBJECT_DEFINE_FREE(BITMAP);

/***********************************************************************************************************************************
Create a new buffer
***********************************************************************************************************************************/
Bitmap *
bmNew(size_t entryTotal, size_t entryBits)
{
    FUNCTION_TEST_BEGIN();
        FUNCTION_TEST_PARAM(SIZE, entryTotal);
        FUNCTION_TEST_PARAM(SIZE, entryBits);
    FUNCTION_TEST_END();

    Bitmap *this = NULL;

    MEM_CONTEXT_NEW_BEGIN("Bitmap")
    {
        // Create object
        this = memNew(sizeof(Bitmap));

        *this = (Bitmap)
        {
            .memContext = MEM_CONTEXT_NEW(),
            .size = size,
        };

        // Allocate buffer
        if (size > 0)
            this->buffer = memNew(this->size);
    }
    MEM_CONTEXT_NEW_END();

    FUNCTION_TEST_RETURN(this);
}
