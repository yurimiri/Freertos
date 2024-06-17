#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

void vTaskFunction(void *pvParameters);
void vDemoFunction(void);
void vTriggerISR(void);
void vPrintString(const char *pcString);


int main(void)
{
    xTaskCreate(vTaskFunction, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}

void vDemoFunction(void)
{
    UBaseType_t uxSavedInterruptStatus;

    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    vPrintString("vDemoFunction: Inside critical section\r\n");

    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

void vDemoISR(void)
{
    UBaseType_t uxSavedInterruptStatus;

    uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

    vPrintString("vDemoISR: Inside critical section\r\n");

    vDemoFunction();

    taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}

void vTriggerISR(void)
{
    vDemoISR();
}

void vTaskFunction(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        vTriggerISR();
    }
}

void vPrintString(const char *pcString)
{
    taskENTER_CRITICAL();
    {
        printf("%s", pcString);
        fflush(stdout); 
    }
    taskEXIT_CRITICAL();
}


