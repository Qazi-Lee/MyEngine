#miniaudio
file(GLOB_RECURSE MINIAUDIO_SOURCES
    "*.h"
    "*.c"
)

add_library(miniaudio STATIC ${MINIAUDIO_SOURCES})

target_include_directories(miniaudio PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)