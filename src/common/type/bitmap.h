/***********************************************************************************************************************************
Bitmap Handler
***********************************************************************************************************************************/
#ifndef COMMON_TYPE_BITMAP_H
#define COMMON_TYPE_BITMAP_H

/***********************************************************************************************************************************
Bitmap object
***********************************************************************************************************************************/
#define BITMAP_TYPE                                                 Bitmap
#define BITMAP_PREFIX                                               bm

typedef struct Bitmap Bitmap;

/***********************************************************************************************************************************
Constructor
***********************************************************************************************************************************/
Bitmap *bmNew(size_t entryTotal, size_t entryBits);

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/

/***********************************************************************************************************************************
Destructor
***********************************************************************************************************************************/
void bmFree(Bitmap *this);

/***********************************************************************************************************************************
Macros for function logging
***********************************************************************************************************************************/
#define FUNCTION_LOG_BITMAP_TYPE                                                                                                   \
    Bitmap *
#define FUNCTION_LOG_MOST_COMMON_VALUE_FORMAT(value, buffer, bufferSize)                                                           \
    objToLog(value, "Bitmap", buffer, bufferSize)

#endif
