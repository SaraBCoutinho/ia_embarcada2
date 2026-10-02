#include "main_functions.h"

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main(void) {

    printf("\n============================\n");
    printf("Hello World!\n");
    printf("============================\n");
    fflush(stdout);

    Setup();

    while (true) {
        Loop();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}