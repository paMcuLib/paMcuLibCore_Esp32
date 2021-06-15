#include "_paBase.Inline.inc.h"
inline void delayMs(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}