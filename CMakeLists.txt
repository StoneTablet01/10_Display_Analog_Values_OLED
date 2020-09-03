# CMakeLists for overall project

cmake_minimum_required(VERSION 3.5)

list(APPEND EXTRA_COMPONENT_DIRS
       "components/oled_display"
       "components/a_to_d_utils"
       "components/d_to_a_utils"
       )

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(digital_voltmeter)
