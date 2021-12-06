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
#define I2C_MASTER_FREQ_HZ

#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */

static esp_err_t i2c_master_init(void){
	int i2c_master_port = 0;
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


	for (int i = 10; i >= 0; i--) {
		printf("Restarting in %d seconds...\n", i);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	printf("Restarting now.\n");
	fflush(stdout);
	esp_restart();
}
