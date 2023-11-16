cmake_minimum_required(VERSION 3.10)
project(lvgl)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD 17)#C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE INCLUDES "./*.h" )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_CXX_FLAGS "-O3")
SET(CMAKE_C_FLAGS "-O0 -g")

find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(lvgl)
add_executable(main main.c mouse_cursor_icon.c ${SOURCES} ${INCLUDES} test_img_cogwheel_a8.c test_img_cogwheel_argb8888.c test_img_cogwheel_i4.c test_img_cogwheel_rgb565.c test_img_cogwheel_rgb565a8.c test_img_cogwheel_xrgb8888.c)
add_compile_definitions(LV_CONF_INCLUDE_SIMPLE)
target_link_libraries(main lvgl lvgl::examples lvgl::demos lvgl::thorvg ${SDL2_LIBRARIES} m pthread)
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/main DEPENDS main)

target_compile_options(lvgl PRIVATE -Wall -Werror -Werror=float-conversion)

# Options lvgl and examples are compiled with.
set(COMPILE_OPTIONS
    -pedantic-errors
    -Wall
    -Wclobbered
    -Wdeprecated
    -Wdouble-promotion
    -Wempty-body
    -Werror
    -Wextra
    -Wformat-security
    -Wmaybe-uninitialized
    -Wmissing-prototypes
    -Wpointer-arith
    -Wmultichar
    -Wpedantic
    -Wreturn-type
    -Wshadow
    -Wshift-negative-value
    -Wsizeof-pointer-memaccess
    -Wstack-usage=5000
    -Wtype-limits
    -Wundef
    -Wuninitialized
    -Wunreachable-code
    -Werror=float-conversion
    -Werror=strict-aliasing
    -Werror=maybe-uninitialized
)
