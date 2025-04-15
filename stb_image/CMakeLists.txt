#stb_image
file(GLOB_RECURSE STB_IMAGE_SOURCES
    "stb_image.h"
    "stb_image.cpp"
)

add_library(stb_image STATIC ${STB_IMAGE_SOURCES})

target_include_directories(stb_image PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)