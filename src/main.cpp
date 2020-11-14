#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include <Arduino_FreeRTOS.h>
#include <queue.h>


struct Boton {
  int id;
  //char nombre[15]; //btn + Alta/Baja, bomba, On/Off
};

QueueHandle_t cola_Mensaje;


void TaskBlink( void *pvParameters );

void TaskRead( void *pvParameters );


void on_off(void *pvParameter);
void alta_baja(void *pvParameter);
void bomba(void *pvParameter);

void Actuador(void *pvParameter);


void setup() {

   cola_Mensaje = xQueueCreate(10, // Queue length
                              sizeof(struct Boton) // Queue item size
                              );
                              
  Serial.begin(9600);
  
  while (!Serial) {
  }

  xTaskCreate(
    TaskBlink
    ,  "Blink"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );


    xTaskCreate(
    Actuador
    ,  "Actuador"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskRead
    ,  "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

}

void loop()
{
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/



void on_off( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  struct Boton boton = { 0 };
  
  uint8_t pushButton = 2;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {
      
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );

    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}


void alta_baja( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  struct Boton boton = { 1 };
  
  uint8_t pushButton = 4;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {
      
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );

    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}


void bomba( void *pvParameters __attribute__((unused)) )  // This is a Task.
{

  struct Boton boton = { 2 };
  
  uint8_t pushButton = 3;

  pinMode(pushButton, INPUT);

  for (;;) // A Task shall never return or exit.
  {
    int buttonState = digitalRead(pushButton);

    if ( buttonState == 1 )
    {
      
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );

    }

    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}



void Actuador(void * pvParameters) {
  
  for (;;) 
  {

    struct Boton boton; 
   
    if (xQueueReceive(cola_Mensaje, &boton, portMAX_DELAY) == pdPASS) {
      Serial.print("Id: ");
      Serial.println(boton.id);   

         if(boton.id == 0)
         {
            Serial.println("Se apaga o prende el Aire");
         }

         if(boton.id == 1)
         {
            Serial.println("Se cambia la velocidad del Aire");
         }
         
         if(boton.id == 2)
         {
            Serial.println("Se prende o apaga la Bomba");
         }
    }
  }
}


void TaskBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
  }
}


void TaskRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  struct Boton boton;
  
  for (;;)
  {
    
    if (Serial.available()) 
    {
      
      //Serial.print(Serial.readString()); 
      String b = Serial.readString();

      boton.id = 0;
      
      if(b == "b0")      
          boton.id = 0;


      if(b == "b1")      
          boton.id = 1;


      if(b == "b2")      
          boton.id = 2;
      
      
      
      if (xQueueSend(cola_Mensaje, &boton, 2000 / portTICK_PERIOD_MS) != pdTRUE)
      {
        Serial.println("error");  
      }
          
      vTaskDelay( 500 / portTICK_PERIOD_MS );     
   }    
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
  
}