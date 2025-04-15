#GLFW

file(GLOB_RECURSE GLFW_SOURCES
    "include/GLFW/*.h"
    "src/internal.h"
    "src/mappings.h"
    "src/platform.h"
    "src/null_joystick.h"
    "src/null_platform.h"
    "src/win32_joystick.h"
    "src/win32_platform.h"
    "src/win32_thread.h"
    "src/win32_time.h"
    "src/context.c"
    "src/init.c"
    "src/input.c"
    "src/monitor.c"
    "src/platform.c"
    "src/vulkan.c"
    "src/window.c"
    "src/egl_context.c"
    "src/osmesa_context.c"
    "src/null_init.c"
    "src/null_monitor.c"
    "src/null_window.c"
    "src/null_joystick.c"
    "src/win32_module.c"
    "src/win32_time.c"
    "src/win32_thread.c"
    "src/win32_init.c"
    "src/win32_joystick.c"
    "src/win32_monitor.c"
    "src/win32_window.c"
    "src/wgl_context.c"
)

add_library(GLFW STATIC ${GLFW_SOURCES})

target_compile_definitions(GLFW PUBLIC 
    _CRT_SECURE_NO_WARNINGS
    _GLFW_WIN32
)
target_include_directories(GLFW PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include
)