#include "main_functions.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" void app_main(void) {
  Setup();
  while (true) {
    Loop();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}