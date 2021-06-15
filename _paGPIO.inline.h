#include "_paGPIO.inline.inc.h"
// #include "paCoreHead/_paGPIO.types.h" //will be inc before include this.
/**
 * related article about gpio
 * https://blog.csdn.net/libin55/article/details/107091644/
 * check this article for detailed pin functions
*/
paErr init(N_paGPIO::Mode gpioMode, gpio_num_t pin);

inline void write(gpio_num_t pin, char status)
{
    gpio_set_level(pin, status);
}
inline char read(gpio_num_t pin)
{
    return gpio_get_level(pin);
}