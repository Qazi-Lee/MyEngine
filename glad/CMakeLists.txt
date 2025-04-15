#glad

file(GLOB_RECURSE GLAD_SOURCES
    "glad/*.h"
    "glad.c"
)


add_library(glad STATIC ${GLAD_SOURCES})

target_include_directories(glad PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)