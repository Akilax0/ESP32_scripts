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
    //checks if msg in queue
    //args(queue handler, address of local variable queue item gets copied to, time parameter(no of ticks))
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
  Serial.println("---FreeRTOS Queue Demo---");

  //Create Queue
  //args(no of items, size of each)
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  // Start Serial receive task
  xTaskCreatePinnedToCore(printMessages,
                          "print messages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);


}

void loop() {
  // Execution should never get here
  static int num =0;

  //Try to add item for 10 tick else fail
  if(xQueueSend(msg_queue, (void*)&num, 10)!=pdTRUE){
    Serial.println("Queue Full");
    }

    //Generally seraial input output handled by one task
    num ++;

    //No issue while 1s delay read write happen simultanously
    //vTaskDelay(1000/ portTICK_PERIOD_MS);
    //500ms would disrupt this queue would fill up quickly
    vTaskDelay(500/ portTICK_PERIOD_MS);
}
