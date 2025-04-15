#ImGui
file(GLOB IMGUI_SOURCES
    "*.h"
    "*.cpp" 
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC 

${CMAKE_CURRENT_SOURCE_DIR}
)