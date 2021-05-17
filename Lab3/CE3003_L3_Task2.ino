#include <Arduino_FreeRTOS.h>
#include <semphr.h>
char s1[5];
int ready = 0;
SemaphoreHandle_t xMutex;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  //set baud rate
  xMutex = xSemaphoreCreateMutex();
  if (xMutex ==NULL)
    Serial.println("Mutex creation failed");
  xTaskCreate(Task1, "Read from PC",100, NULL ,1,NULL);
  xTaskCreate(Task2, "Print from PC",100, NULL ,1,NULL);
  vTaskStartScheduler(); 
}


static void Task1(void *pvParameters) 
{
  while(1) {
  xSemaphoreTake(xMutex,portMAX_DELAY);
  char t;
  
  int i ;
  i = 0;
  

  Serial.println("'''''''''''''''''''''Task 1 started'''''''''''''''''''''''''");
  while (Serial.available() ==0){}
  while (Serial.available()>0) {
      
      t = Serial.read(); 
      vTaskDelay(pdMS_TO_TICKS(100));
      //Serial.print("You typed: " );
      //Serial.print(t);  
      if (i<4) {   
        s1[i] = t;
        i++;
        //Serial.println();
      }
      else{
        //Serial.println(" - discarded");
      }
  }
  

  Serial.println("'''''''''''''''''''''End of Task 1'''''''''''''''''''''");
  xSemaphoreGive(xMutex);
  
  delay(1000);
  }
}

static void Task2(void* pvParameters) {
  while(1){
  xSemaphoreTake(xMutex,portMAX_DELAY);
  Serial.println("'''''''''''''''''''''Start of Task 2'''''''''''''''''''''");
  s1[4] = '\0'; // terminating the String
  Serial.print("This is what it comes in: ");
  Serial.println(s1);
  Serial.println('\n');
  Serial.println("'''''''''''''''''''''End of Task 2'''''''''''''''''''''");
  
  xSemaphoreGive(xMutex);
  delay(1000);
  
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
