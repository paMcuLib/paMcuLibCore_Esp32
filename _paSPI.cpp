#include "paCoreHead/paSPI.h"
#include "paBase/_ForCore.h"
#include "driver/spi_master.h"
#include "_paSPI.PLATFORM.h"
#include "esp_system.h"
#include "string.h"
#include "esp_log.h"

// spi_device_handle_t spi_handle;
namespace N_paSPI
{
    spi_device_handle_t spi_handle;
    void callback(spi_transaction_t *trans)
    {
    }
}
using namespace N_paSPI;

paErr paSPI::init(char spiId)
{
    spi_bus_config_t buscfg;
    {
        memset(&buscfg, 0, sizeof(spi_bus_config_t));
        buscfg.miso_io_num = PIN_NUM_MISO;
        buscfg.mosi_io_num = PIN_NUM_MOSI;
        buscfg.sclk_io_num = PIN_NUM_CLK;
        buscfg.quadwp_io_num = -1;
        buscfg.quadhd_io_num = -1;
        buscfg.max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE;
    }

    spi_device_interface_config_t devcfg;
    {
        memset(&devcfg, 0, sizeof(spi_device_interface_config_t));
#ifdef CONFIG_SPI_OVERCLOCK
        devcfg.clock_speed_hz = 26 * 1000 * 1000; //Clock out at 26 MHz
#else
        devcfg.clock_speed_hz = 10 * 1000 * 1000; //Clock out at 10 MHz
#endif
        devcfg.mode = 0;          //SPI mode 0
        devcfg.spics_io_num = -1; //CS pin
        devcfg.queue_size = 7;    //We want to be able to queue 7 transactions at a time
        devcfg.pre_cb = callback; //Specify pre-transfer callback to handle D/C line
    };

    esp_err_t ret;

    ret = spi_bus_initialize(PASPI_HOST, &buscfg, 0);
    ESP_ERROR_CHECK(ret);
    if (ret != ESP_OK)
    {
        ESP_LOGE("App", "spi Fail");
        return E_Err;
    }
    else
    {
        ESP_LOGI("App", "spi succ");
        return E_Succ;
    }
    ret = spi_bus_add_device(PASPI_HOST, &devcfg, &spi_handle);
    ESP_ERROR_CHECK(ret);

    if (ret != ESP_OK)
    {
        ESP_LOGE("App", "spi Fail");
        return E_Err;
    }
    else
    {
        ESP_LOGI("App", "spi succ");
        return E_Succ;
    }
}

paErr paSPI::transmit(char spiId, unsigned char *data, unsigned int len)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    t.length = len;
    t.tx_buffer = data;
    // t.tx_buffer = buffer;

    esp_err_t ret = spi_device_transmit(spi_handle, &t);

    // free(buffer);

    if (ret != ESP_OK)
    {
        ESP_LOGE("App", "spi send Fail");
        return E_Err;
    }
    else
    {
        ESP_LOGI("App", "spi send succ");
        return E_Succ;
    }
}
