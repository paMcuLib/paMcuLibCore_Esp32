#include "paCoreHead/paSPI.h"
#include "paBase/_ForCore.h"
#include "driver/spi_master.h"
#include "_paSPI.PLATFORM.h"
#include "esp_system.h"

spi_device_handle_t spi_handle;

paErr paSPI::init(char spiId)
{
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = PARALLEL_LINES * 320 * 2 + 8};
    spi_device_interface_config_t devcfg = {
#ifdef CONFIG_SPI_OVERCLOCK
        .clock_speed_hz = 26 * 1000 * 1000, //Clock out at 26 MHz
#else
        .clock_speed_hz = 10 * 1000 * 1000, //Clock out at 10 MHz
#endif
        .mode = 0,                               //SPI mode 0
        .spics_io_num = -1,                      //CS pin
        .queue_size = 7,                         //We want to be able to queue 7 transactions at a time
        .pre_cb = lcd_spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
    };

    esp_err_t ret;

    ret = spi_bus_initialize(SPI_HOST, &buscfg, 0);
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(SPI_HOST, &devcfg, &spi_handle);
    ESP_ERROR_CHECK(ret);

    if (ret != ESP_OK)
    {
        return E_Err;
    }
    else
    {
        return E_Succ;
    }
}

paErr paSPI::transmit(char spiId, unsigned char *data, unsigned int len)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = len;
    // t.tx_buffer = buffer;

    esp_err_t ret = spi_device_transmit(rc522_spi, &t);

    // free(buffer);

    if (ret != ESP_OK)
    {
        return E_Err;
    }
    else
    {
        return E_Succ;
    }
}
