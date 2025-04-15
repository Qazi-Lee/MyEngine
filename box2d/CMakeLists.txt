#box2d

file(GLOB_RECURSE BOX2D_SOURCES
    "include/box2d/*.h"
    "src/collision/*.cpp"
    "src/common/*.cpp"
    "src/dynamics/*.cpp"
    "src/roop/*.cpp"
)

#创建静态库
add_library(box2d STATIC ${BOX2D_SOURCES})

target_include_directories(box2d PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
