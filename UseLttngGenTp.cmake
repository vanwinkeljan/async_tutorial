cmake_minimum_required(VERSION 2.8)

find_program(LTTNG_GEN_TP NAMES lttng-gen-tp DOC "lttng-gen-tp executable")

if(NOT LTTNG_GEN_TP)
  message(FATAL_ERROR "Excutable lttng-gen-top not found")
endif()

include_directories( "${CMAKE_CURRENT_BINARY_DIR}")

function(add_lttng_gen_tp)
  set(_one_value NAME)
  cmake_parse_arguments (arg "" "${_one_value}" "" ${ARGN})
  add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/tp_${arg_NAME}.h" "${CMAKE_CURRENT_BINARY_DIR}/tp_${arg_NAME}.c"
    COMMAND "${LTTNG_GEN_TP}"
               -o "tp_${arg_NAME}.h"
               -o "tp_${arg_NAME}.c"
               "${PROJECT_SOURCE_DIR}/tracepoints/${arg_NAME}.tp"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS "tracepoints/${arg_NAME}.tp"
  )
endfunction(add_lttng_gen_tp)

