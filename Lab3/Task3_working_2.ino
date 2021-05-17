#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#define SEAT_NUM 2

int LED1 = 1;
int LED2 = 12;

int id = 1;
int seats[SEAT_NUM] = {LED1,LED2};

SemaphoreHandle_t barberReady;
SemaphoreHandle_t getTicket;
SemaphoreHandle_t custReady;
int avail_seats = SEAT_NUM;
int assign = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  randomSeed(analogRead(0));
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  custReady = xSemaphoreCreateCounting(SEAT_NUM,0);
  getTicket = xSemaphoreCreateMutex();
  barberReady = xSemaphoreCreateMutex();
  //Creating Customer Threads
  xTaskCreate(CustomerTask, "WA",100, (void *) '1' ,1,NULL);
  xTaskCreate(CustomerTask, "RU",100, (void *) '1' ,1,NULL);
  //xTaskCreate(CustomerTask, "DO",100, (void *) '1' ,1,NULL);
  //xTaskCreate(CustomerTask, "DA",100, (void *) '1' ,1,NULL);

  xTaskCreate(BarberTask,"Barber",100, NULL,1,NULL);
  vTaskStartScheduler(); 
}

static void CustomerTask (void* pvParameters) {
    int n;
    int seat_no;
    while(1) {
      
      xSemaphoreTake(getTicket,portMAX_DELAY);
      Serial.print("This is customer "); 
      Serial.println(id);
      n =id;
      id++;
      if (avail_seats > 0){
        avail_seats--;
        seat_no = assignSeat();
        printID(n);   Serial.print("I will take this seat #");  Serial.println(seat_no);
        digitalWrite(seats[seat_no],HIGH);
        Serial.print("{LED @ PIN ");Serial.print(seats[seat_no]);Serial.println(" TURNING ON."); 
        xSemaphoreGive(custReady); 
        xSemaphoreGive(getTicket);
        xSemaphoreTake(barberReady,portMAX_DELAY); //After this the barber will be tending to him alr
        digitalWrite(seats[seat_no],LOW);
        Serial.print("{LED @ PIN ");Serial.print(seats[seat_no]);Serial.println(" TURNING OFF."); 
        
        
      }
      else {
        printID(n);
        Serial.println("No more empty seats. I 'll leave");
        xSemaphoreGive(getTicket);
      }
      //vTaskDelay(pdMS_TO_TICKS(10000));
      vTaskDelay(pdMS_TO_TICKS(random(1000,15000)));   //Customer entry rate
    }
}

static void BarberTask(void *pvParameters) {
  int sleep_print_flag = 0;
  while(1) {
    if (avail_seats == 0) {
      printID(0);   Serial.println("Wakkey, time to cut sum hair.");
      xSemaphoreTake(getTicket,portMAX_DELAY); //racing condition between barber take one out and cus coming in
      avail_seats++; //take in one customer
      printID(0); Serial.println("Let's give you a BOOMZ haircut.");
      xSemaphoreGive(barberReady);
      xSemaphoreGive(getTicket);
      vTaskDelay(pdMS_TO_TICKS(random(5000,7000))); //cutting ya damn hair
      printID(0); Serial.println("Hope you are happy with your hair, see you again");
      sleep_print_flag = 0;
      }
    else if (sleep_print_flag == 0) {
      printID(0);   Serial.println("Sleepy Sleepy gonna do a nappy....Zzz....Zzz");
      vTaskDelay(pdMS_TO_TICKS(random(1000,10000)));  //Sleep rate
    }
    else {
        //do nothing
    }
  }  
}

void printID(int id){
  if (id > 0) {
    Serial.print("[Customer ");
    Serial.print(id);
    Serial.print(" ]: ");
  }
  else //is the barber 
  {
    Serial.print("[Barber]: ");
  }
  
}
int assignSeat() {
  int k = assign;
  assign = (assign+1)%SEAT_NUM;
  return k; 
}
void loop() {
  // put your main code here, to run repeatedly:

}
