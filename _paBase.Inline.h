#include "_paBase.Inline_inc.h"
inline void delayMs(uint32_t ms)
{
    vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
}