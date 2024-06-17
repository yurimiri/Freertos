#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vTaskFunction(void *pvParameters);
void vPrintString( const char *pcString );

static const char *pcTextForTask1 = "Yurim Son / Task 1 is running\r\n";
static const char *pcTextForTask2 = "Yurim Son / Task 2 is running\r\n";
static const char *pcTextForTask3 = "Yurim Son / Task 3 is running\r\n";
static const char *pcTextForTask4 = "Yurim Son / Task 4 is running\r\n";
static const char *pcTextForTask5 = "Yurim Son / Task 5 is running\r\n";
static const char *pcTextForTask6 = "Yurim Son / Task 6 is running\r\n";
static const char *pcTextForTask7 = "Yurim Son / Task 7 is running\r\n";
static const char *pcTextForTask8 = "Yurim Son / Task 8 is running\r\n";
static const char *pcTextForTask9 = "Yurim Son / Task 9 is running\r\n";
static const char *pcTextForTask10 = "Yurim Son / Task 10 is running\r\n";

int main(void)
{
    xTaskCreate(vTaskFunction, "Task 1", 1000, (void*)pcTextForTask1, 1, NULL);
    xTaskCreate(vTaskFunction, "Task 2", 1000, (void*)pcTextForTask2, 2, NULL);
    xTaskCreate(vTaskFunction, "Task 3", 1000, (void*)pcTextForTask3, 3, NULL);
    xTaskCreate(vTaskFunction, "Task 4", 1000, (void*)pcTextForTask4, 4, NULL);
    xTaskCreate(vTaskFunction, "Task 5", 1000, (void*)pcTextForTask5, 5, NULL);
    xTaskCreate(vTaskFunction, "Task 6", 1000, (void*)pcTextForTask6, 6, NULL);
    xTaskCreate(vTaskFunction, "Task 7", 1000, (void*)pcTextForTask7, 7, NULL);
    xTaskCreate(vTaskFunction, "Task 8", 1000, (void*)pcTextForTask8, 8, NULL);
    xTaskCreate(vTaskFunction, "Task 9", 1000, (void*)pcTextForTask9, 9, NULL);
    xTaskCreate(vTaskFunction, "Task 10", 1000, (void*)pcTextForTask10, 10, NULL);

    vTaskStartScheduler();

    return 0;
}

void vTaskFunction(void *pvParameters)
{
    char *pcTaskName;
    TickType_t xLastWakeTime;

    pcTaskName = (char *) pvParameters;

    xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        vPrintString(pcTaskName);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }
}

void vPrintString( const char *pcString )
{ 
    taskENTER_CRITICAL(); 
    { 
        printf("%s\n", pcString);
        fflush( stdout ); 
    } 
    taskEXIT_CRITICAL(); 
} 

