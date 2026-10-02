idf_component_register(
    SRCS
        "main.cc"
        "main_functions.cc"
        "constants.cc"
        "model.cc"
        "output_handler.cc"
    INCLUDE_DIRS "."
    REQUIRES esp-tflite-micro esp-nn
)
