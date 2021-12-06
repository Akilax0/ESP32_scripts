#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"
#include "driver/i2c.h"

#define BLINK_GPIO 2

//I2C master configuration
#define I2C_MASTER_SDA_IO 21 
#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_FREQ_HZ 400000

#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */


#define I2C_MASTER_NUM 0 /*!< I2C port number for master dev */
#define ESP_SLAVE_ADDR 0x09 
#define WRITE_BIT I2C_MASTER_WRITE

#define ACK_CHECK_EN 0x1


static esp_err_t i2c_master_init(void){
	int i2c_master_port = I2C_MASTER_NUM;
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = I2C_MASTER_SDA_IO,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = I2C_MASTER_SCL_IO,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_MASTER_FREQ_HZ,
	};

    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t __attribute__((unused)) i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size){

/*I2C_MASTER_NUM, data_wr, RW_TEST_LENGTH*/

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);

	i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000/ portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	return ret;
}

void app_main(void)
{
	printf("Hello world!\n");

	/* Print chip information */
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);
	printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
			CONFIG_IDF_TARGET,
			chip_info.cores,
			(chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
			(chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

	printf("silicon revision %d, ", chip_info.revision);

	printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
			(chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

	printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

	printf("blinking LED \n");

	gpio_reset_pin(BLINK_GPIO);
	/*[> Set the GPIO as a push/pull output <]*/
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	/*while(1) {*/
		/*[> Blink off (output low) <]*/
		printf("Turning off the LED\n");
		gpio_set_level(BLINK_GPIO, 0);
		vTaskDelay(50 / portTICK_PERIOD_MS);
		/*[> Blink on (output high) <]*/
		printf("Turning on the LED\n");
		gpio_set_level(BLINK_GPIO, 1);
		vTaskDelay(50 / portTICK_PERIOD_MS);
	/*}*/


	
//I2C Communication

	ESP_ERROR_CHECK(i2c_master_init());
	/*ESP_ERROR_CHECK(send_msg());*/

	uint8_t *data_wr = (uint8_t *)malloc(DATA_LENGTH);

	int i;
        for (i = 0; i < DATA_LENGTH; i++) {
            data_wr[i] = i + 10;
        }
        esp_err_t ret = i2c_master_write_slave(I2C_MASTER_NUM, data_wr, RW_TEST_LENGTH);

	if(ret != ESP_OK){

		printf("\n\n******************************\n\n");
		printf("%s\n\n",esp_err_to_name(ret));
	}else{
		printf("\n\n*************************** I2C SENT *********************************** \n\n");
	}


	for (int i = 10; i >= 0; i--) {
		printf("Restarting in %d seconds...\n", i);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	printf("Restarting now.\n");
	fflush(stdout);
	esp_restart();
}
