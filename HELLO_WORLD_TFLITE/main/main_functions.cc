#include "main_functions.h"

#include <cstdint>
#include <cstdio>

#include "constants.h"
#include "model.h"
#include "output_handler.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace {
constexpr int kTensorArenaSize = 20 * 1024;
alignas(16) uint8_t tensor_arena[kTensorArenaSize];

const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;
bool initialized = false;
}

void Setup() {
  printf("Hello World!\n");
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    printf("Model schema version %lu is not supported (expected %lu).\n",
           static_cast<unsigned long>(model->version()),
           static_cast<unsigned long>(TFLITE_SCHEMA_VERSION));
    return;
  }

  static tflite::MicroMutableOpResolver<1> resolver;
  if (resolver.AddFullyConnected() != kTfLiteOk) {
    printf("Failed to register FullyConnected operator.\n");
    return;
  }

  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, sizeof(tensor_arena));
  interpreter = &static_interpreter;
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    printf("AllocateTensors() failed.\n");
    return;
  }

  input = interpreter->input(0);
  output = interpreter->output(0);
  if (input->type != kTfLiteInt8 || output->type != kTfLiteInt8 ||
      input->params.scale <= 0.0f || output->params.scale <= 0.0f) {
    printf("The model must use quantized int8 input and output tensors.\n");
    return;
  }

  inference_count = 0;
  initialized = true;
  printf("TFLite Micro sine model ready.\n");
}

void Loop() {
  if (!initialized) {
    return;
  }

  const float position = static_cast<float>(inference_count) /
                         static_cast<float>(kInferencesPerCycle);
  const float x = position * kXrange;
  const int32_t quantized_input =
      static_cast<int32_t>(x / input->params.scale) + input->params.zero_point;
  input->data.int8[0] = static_cast<int8_t>(quantized_input);

  if (interpreter->Invoke() != kTfLiteOk) {
    printf("Invoke() failed for x=%.3f\n", static_cast<double>(x));
    return;
  }

  const float y = (static_cast<int32_t>(output->data.int8[0]) -
                   output->params.zero_point) *
                  output->params.scale;
  HandleOutput(x, y);
  inference_count = (inference_count + 1) % kInferencesPerCycle;
}