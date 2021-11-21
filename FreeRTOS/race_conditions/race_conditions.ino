/*

Race conditions 

*/

//likely might have to include for vanilla FreeRTOS
//#include<semphr.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Globals
static int shared_var = 0;

//semaphores and mutex implemented in the same way in RTOS 
//FreeRTOS makes this efficient by implementing the same way
static SemaphoreHandle_t mutex;

//=========================================
//Task
//two seperate tasks will call this function  
void incTask(void* parameters){
  int local_var;
  while(1){

      //Take mutex prior to critical section
      if(xSemaphoreTake(mutex,0) == pdTRUE){
    
        //Roundabout way to do "shared_var++" reandomly and poorly
        local_var = shared_var;
        local_var++;
        vTaskDelay(random(100,500) / portTICK_PERIOD_MS);
        shared_var = local_var;

        //Give Mutex after critical section
        xSemaphoreGive(mutex);
  
        //Print out new shared variable
        Serial.println(shared_var);
      }else{
        
          //Do something
        }
   }
  
 }
 
void setup() {
  // put your setup code here, to run once:

//Hack to kinda get randomness
  randomSeed(analogRead(0));

  Serial.begin(115200);

  //Wait a moment to start 
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("-------FreeRTOS Race Condition Demo------");


  //Create mutex before starting tasks
  mutex = xSemaphoreCreateMutex();

  //start task1
  xTaskCreatePinnedToCore(incTask,
                          "Increment Task 1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);       

    //start Task 2
    xTaskCreatePinnedToCore(incTask,
                          "Increment Task 2",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

    //Delete Setup and Loop tasks
    vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
