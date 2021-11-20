/*
The array is 400 bytes
we have allocated 1KB - 1024bytes as stack
but 768bytes are overhead 
Array takes up more stack memeory than left

Keeps rebooting good indication of overflowed stack
Stack canary watchpoint 
  OS keeps know bytes at the end of stack and periodically checks them 
  if differnet error is triggered
  ' Stack canary watchpoint triggered (test Task) '


Change stack size to aroumd 1500
accounting for 768 + local variables + some extra
*/


//Use only 1 core for demo
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


//Task: to perform mundane task
void testTask(void *parameters){
  while(1){
    int a = 1;
    int b[100];

    //Do something with array so not optimised by compiler
    for(int i=0;i<100;i++){
      b[i] = a+i;
    }
    Serial.println(b[0]);   

    //Print out remaining stack memory (words)
    Serial.print("High water mark (words):");
    Serial.println(uxTaskGetStackHighWaterMark(NULL));

    //Print out number of free memory in heap in bytes before malloc
    Serial.print("Heap before malloc(bytes): ");
    Serial.println(xPortGetFreeHeapSize());

    //if not using heap3 scheme have to use pvPortMalloc in vanilla (regular malloc not thread safe )
    //In Esp can use malloc 
    int * ptr = (int*)pvPortMalloc(1024*sizeof(int));


    if(ptr==NULL){
      Serial.println("Not enough memory");
     }else{
      
      //Do something so compiler doesnt optimize this out
      for(int i=0;i<1024;i++){
          ptr[i] = 3;
        
      }
    }
     //Print out number of free memory in heap in bytes before malloc
    Serial.print("Heap after malloc(bytes): ");
    Serial.println(xPortGetFreeHeapSize());


    //Freeing malloc mem
    vPortFree(ptr);
    /*
    Now only uses memeory in the loop 
    Hence doesnt run out of heap
    */
    vTaskDelay((100) / portTICK_PERIOD_MS);
    }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //Delay so not to miss changes
  vTaskDelay((1000) / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("-----FreeRTOS Memory Demo---------");

    //Task to run forever
  xTaskCreatePinnedToCore(
      testTask,
      "test Task",
      1500,
      NULL,
      1,
      NULL,
      app_cpu);

//Delete setup and loop
vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
