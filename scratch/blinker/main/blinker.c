#include <stdio.h>

//OS API
#include "freertos/FreeRTOS.h" 
//task handling API
#include "freertos/task.h"

#include "esp_log.h"

#include "driver/gpio.h"


#define BLINK_LED 2
int app_main(void)
{
    char *taskName = pcTaskGetName(NULL);

    ESP_LOGI(taskName,"Starting up application!!\n\n");

    gpio_reset_pin(BLINK_LED);
    gpio_set_direction(BLINK_LED, GPIO_MODE_OUTPUT);
    while(1){
        gpio_set_level(BLINK_LED,1);
        //yield task and priority set to other tasks
        // vTaskDelay(1000);   This is not 1 second but a 1000 sys ticks so have to convert

        vTaskDelay(500/ portTICK_PERIOD_MS);
        gpio_set_level(BLINK_LED,0);
        vTaskDelay(500/ portTICK_PERIOD_MS);
        
    
    }
    return 0;
}
