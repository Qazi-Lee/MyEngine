#Engine
file(GLOB_RECURSE ENGINE_SOURCES
    "vender/entt.hpp"
    "src/*.h"
    "src/*.cpp"
)

add_library(Engine STATIC ${ENGINE_SOURCES})

target_link_libraries(Engine PUBLIC box2d)
target_link_libraries(Engine PUBLIC glad)
target_link_libraries(Engine PUBLIC GLFW)
target_link_libraries(Engine PUBLIC glm)
target_link_libraries(Engine PUBLIC ImGui)
target_link_libraries(Engine PUBLIC miniaudio)
target_link_libraries(Engine PUBLIC stb_image)
target_link_libraries(Engine PUBLIC yaml)

target_include_directories(Engine PRIVATE ${THIRDPARTY_INCLUDE_DIR})
target_link_libraries(Engine PRIVATE ${THIRDPARTY_LIB_DIR}/freetyped.lib)

target_include_directories(Engine PRIVATE ${MONO_INCLUDE_DIR})
target_link_libraries(Engine PRIVATE ${MONO_LIB_DIR}/mono-2.0-sgen.lib)

target_include_directories(Engine PRIVATE ${VULKANSKD_INCLUDE_DIR})
target_link_libraries(Engine PRIVATE ${VULKANSKD_LIB_DIR}/vulkan-1.lib)

target_include_directories(Engine PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/src 
${CMAKE_CURRENT_SOURCE_DIR}/vender 
)

target_compile_definitions(Engine PUBLIC 
    ENGINE_PLATFORM_WINDOWS
)

set(MY_FILE_PATH "${CMAKE_SOURCE_DIR}")

target_compile_definitions(Engine PUBLIC 
    MY_FILE_PATH="${MY_FILE_PATH}"
)
