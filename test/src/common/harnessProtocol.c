/***********************************************************************************************************************************
Harness for Protocol Testing
***********************************************************************************************************************************/
#include "common/assert.h"
#include "common/type/convert.h"
#include "common/type/pack.h"
#include "common/type/stringz.h"

#include "common/harnessDebug.h"
#include "common/harnessPack.h"

/**********************************************************************************************************************************/
void
hrnProtocolLocalHander(const Buffer *buffer)
{
    FUNCTION_HARNESS_BEGIN();
        FUNCTION_HARNESS_PARAM(BUFFER, buffer);
    FUNCTION_HARNESS_END();

    FUNCTION_HARNESS_RETURN(STRING, hrnPackToStr(pckReadNewBuf(buffer)));
}
