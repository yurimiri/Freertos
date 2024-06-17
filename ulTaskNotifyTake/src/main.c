#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

void vSenderTask(void *pvParameters);
void vReceiverTask(void *pvParameters);
void vPrintString(const char *pcString);

TaskHandle_t xReceiverTaskHandle = NULL;

int main(void)
{
    xTaskCreate(vReceiverTask, "Receiver", 1000, NULL, 2, &xReceiverTaskHandle);
    xTaskCreate(vSenderTask, "Sender", 1000, NULL, 1, NULL);

    vTaskStartScheduler();

    return 0;
}

void vSenderTask(void *pvParameters)
{
    const TickType_t xDelay250ms = pdMS_TO_TICKS(250);
    for(;;)
    {
        vTaskDelay(xDelay250ms);

        xTaskNotifyGive(xReceiverTaskHandle);

        vPrintString("Sender Task: Notification sent.");
    }
}

void vReceiverTask(void *pvParameters)
{
    for(;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        vPrintString("Receiver Task: Notification received.");
    }
}

void vPrintString(const char *pcString)
{ 
    taskENTER_CRITICAL(); 
    { 
        printf("%s\n", pcString);
        fflush(stdout); 
    } 
    taskEXIT_CRITICAL(); 
}
