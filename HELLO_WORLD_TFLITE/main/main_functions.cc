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

}  // namespace


void Setup() {
  // ==========================================
  // Carrega o modelo TensorFlow Lite
  // ==========================================
  model = tflite::GetModel(g_model);

  if (model->version() != TFLITE_SCHEMA_VERSION) {
    printf(
        "Model schema version %lu is not supported (expected %lu).\n",
        static_cast<unsigned long>(model->version()),
        static_cast<unsigned long>(TFLITE_SCHEMA_VERSION)
    );
    return;
  }

  // ==========================================
  // Registra as operacoes utilizadas
  // ==========================================
  static tflite::MicroMutableOpResolver<1> resolver;

  if (resolver.AddFullyConnected() != kTfLiteOk) {
    printf("Failed to register FullyConnected operator.\n");
    return;
  }

  // ==========================================
  // Cria o interpretador
  // ==========================================
  static tflite::MicroInterpreter static_interpreter(
      model,
      resolver,
      tensor_arena,
      sizeof(tensor_arena)
  );

  interpreter = &static_interpreter;

  // ==========================================
  // Aloca os tensores
  // ==========================================
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    printf("AllocateTensors() failed.\n");
    return;
  }

  // ==========================================
  // Obtem os tensores de entrada e saida
  // ==========================================
  input = interpreter->input(0);
  output = interpreter->output(0);

  // ==========================================
  // Verifica se o modelo utiliza INT8
  // ==========================================
  if (input->type != kTfLiteInt8 ||
      output->type != kTfLiteInt8 ||
      input->params.scale <= 0.0f ||
      output->params.scale <= 0.0f) {

    printf(
        "The model must use quantized int8 input and output tensors.\n"
    );

    return;
  }

  inference_count = 0;
  initialized = true;

  //printf("TFLite Micro sine model ready.\n");
}


void Loop() {

  // ==========================================
  // Verifica se o TensorFlow foi inicializado
  // ==========================================
  if (!initialized) {
    return;
  }

  // ==========================================
  // Calcula o valor de entrada X
  // ==========================================
  const float position =
      static_cast<float>(inference_count) /
      static_cast<float>(kInferencesPerCycle);

  const float x = position * kXrange;

  // ==========================================
  // Quantizacao da entrada
  // ==========================================
  const int32_t quantized_input =
      static_cast<int32_t>(
          x / input->params.scale
      ) + input->params.zero_point;

  input->data.int8[0] =
      static_cast<int8_t>(quantized_input);

  // ==========================================
  // Executa a inferencia
  // ==========================================
  if (interpreter->Invoke() != kTfLiteOk) {

    printf(
        "Invoke() failed for x=%.3f\n",
        static_cast<double>(x)
    );

    return;
  }

  // ==========================================
  // Desquantizacao da saida
  // ==========================================
  const float y =
      (static_cast<int32_t>(output->data.int8[0]) -
       output->params.zero_point) *
      output->params.scale;

  // ==========================================
  // Mostra X e Y
  // ==========================================
   
  //printf(
  //    "x = %.3f | y = %.3f\n",
  //    static_cast<double>(x),
  //    static_cast<double>(y)
  //);

  // Mantem o output handler original
  //HandleOutput(x, y);

  // ==========================================
  // Proxima inferencia
  // ==========================================
  inference_count =
      (inference_count + 1) %
      kInferencesPerCycle;
}
