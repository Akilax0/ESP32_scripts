/**
 * FreeRTOS Queue Demo
 * 
 * 
 */

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif





// Settings
static const uint8_t msg_queue_len = 5;

// Globals
static QueueHandle_t msg_queue;


//*****************************************************************************
// Tasks

//Task: wait for item  on queue and print it
void printMessages(void *parameters){
  int item;

  while(1){
    if(xQueueReceive(msg_queue, (void *)&item, 0)==pdTRUE){
      Serial.println(item);
      }
      vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
    
 }
//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Heap Demo---");
  Serial.println("Enter a string");

  // Start Serial receive task
  xTaskCreatePinnedToCore(readSerial,
                          "Read Serial",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Start Serial print task
  xTaskCreatePinnedToCore(printMessage,
                          "Print Message",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);
  
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // Execution should never get here
}
