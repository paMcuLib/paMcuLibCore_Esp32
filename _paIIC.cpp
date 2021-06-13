#include "paCoreHead/paIIC.h"
#include "driver/i2c.h"

#define ACK_CHECK_EN 0x1  /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0 /*!< I2C master will not check ack from slave */

uint8_t paIIC::init(unsigned char iicId)
{
    switch (iicId)
    {
    case 0:
    {
        int i2c_master_port = 0;
        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = I2C_MASTER_SDA_IO, // select GPIO specific to your project
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_io_num = I2C_MASTER_SCL_IO, // select GPIO specific to your project
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = I2C_MASTER_FREQ_HZ, // select frequency specific to your project
            // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
        };
        esp_err_t err = i2c_param_config(i2c_master_port, &conf);
        if (err != ESP_OK)
        {
            return err;
        }
        return i2c_driver_install(i2c_master_port,
                                  conf.mode,
                                  I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    }
    break;

    default:
        return 0;
    }
    return 1;
}

uint8_t paIIC::writeLen(unsigned char iicId,
                        unsigned char addr,
                        unsigned char length, unsigned char *data_wr)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    switch (iicId)
    {
    case 0:
    {
        i2c_master_start(cmd);
        //addr
        i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
        //data
        i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
        // if (size > 1)
        // {
        //     i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
        // }

        // i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
        i2c_master_stop(cmd);
    }
    break;

    default:
        return 0;
    }
    return 1;
}
