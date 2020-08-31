# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# set(EXTRA_COMPONENT_DIRS ${CMAKE_SOURCE_DIR}/components)
list(APPEND EXTRA_COMPONENT_DIRS "components/oled_display" "components/a_to_d_utils")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(i2c-display)
