#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "hello_world.h"
#include "clock.h"

void app_main(void)
{
    while (true)
    {
        // print_hello_world();
        clock();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    } 
}