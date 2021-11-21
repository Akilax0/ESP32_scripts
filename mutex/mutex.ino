// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Globals
static int shared_var = 0;

//=========================================
//Task
//two seperate tasks will call this function  
void incTask(void* parameters){
  int local_var;
  while(1){
      //Roundabout way to do "shared_var++" reandomly and poorly
      local_var = shared_var;
      local_var++;
      vTaskDelay(random(100,500) / portTICK_PERIOD_MS);
      shared_var = local_var;

      //Print out new shared variable
      Serial.println(shared_var);
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
