
//Use only core 1 for demo
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//char string
const char msg[] = "Tasks scheduling on RTOS";

//Task Handles
//So can be controlled within a third task
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//prints to sertial with lower priority
void startTask1(void *parameters){
	//counts number of char

	int msg_len = strlen(msg);

	while(1){
		Serial.println();
		for(int i=0;i<msg_len;i++){
			Serial.print(msg[i]);
		}
		Serial.println();
		vTaskDelay(1000/ portTICK_PERIOD_MS);
	}

}


//prints to sertial with higher priority
void startTask2(void *parameters){
	//prints asterix
	while(1){
		Serial.print('*');
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}

}

void setup() {
	// put your setup code here, to run once:

	//serial monito slow to see changes
	Serial.begin(300);


	//Delay so not to miss changes
	vTaskDelay((100) / portTICK_PERIOD_MS);
	Serial.println();
	Serial.println("-----FreeRTOS Task Demo---------");


	//print self priority

	Serial.print("Setup and loop task running in core : ");
	Serial.print(xPortGetCoreID());
	Serial.print(" with priority ");
	Serial.print(uxTaskPriorityGet(NULL));
	

	//Task to run forever
	xTaskCreatePinnedToCore(
			startTask1,
			"Task 1",
			1024,
			NULL,
			1,
			&task_1,
			app_cpu);

	//Task to run forever
	xTaskCreatePinnedToCore(
			startTask2,
			"Task 2",
			1024,
			NULL,
			2,
			&task_2,
			app_cpu);
}

void loop() {
	// put your main code here, to run repeatedly:
	//Suspend higher priority task for some intervals

	for(int i=0;i<3;i++){
		vTaskSuspend(task_2);
		vTaskDelay((2000) / portTICK_PERIOD_MS);

		vTaskResume(task_2);
		vTaskDelay((2000) / portTICK_PERIOD_MS);
	}
	//Delete lower prority task
	if(task_1 != NULL){
		vTaskDelete(task_1);
		task_1 = NULL;
	}
}
