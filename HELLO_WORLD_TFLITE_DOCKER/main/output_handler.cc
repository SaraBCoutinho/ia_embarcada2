#include "output_handler.h"

#include <cstdio>

void HandleOutput(float x_value, float y_value) {
  printf("x: %.3f, sin(x): %.3f\n", static_cast<double>(x_value),
         static_cast<double>(y_value));
}