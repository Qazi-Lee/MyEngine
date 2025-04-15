#Game
file(GLOB_RECURSE GAME_SOURCES
    "src/*.h"
    "src/*.cpp"
)

add_executable(Game ${GAME_SOURCES})

target_link_libraries(Game PUBLIC Engine)
target_link_libraries(Game PUBLIC box2d)
target_link_libraries(Game PUBLIC glad)
target_link_libraries(Game PUBLIC GLFW)
target_link_libraries(Game PUBLIC glm)
target_link_libraries(Game PUBLIC ImGui)
target_link_libraries(Game PUBLIC miniaudio)
target_link_libraries(Game PUBLIC stb_image)
target_link_libraries(Game PUBLIC yaml)

target_include_directories(Game PRIVATE ${THIRDPARTY_INCLUDE_DIR})
target_link_libraries(Game PRIVATE ${THIRDPARTY_LIB_DIR}/freetyped.lib)

target_include_directories(Game PRIVATE ${MONO_INCLUDE_DIR})
target_link_libraries(Game PRIVATE ${MONO_LIB_DIR}/mono-2.0-sgen.lib)

target_include_directories(Game PRIVATE ${VULKANSKD_INCLUDE_DIR})
target_link_libraries(Game PRIVATE ${VULKANSKD_LIB_DIR}/vulkan-1.lib)

target_include_directories(Game PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/src 
)

target_compile_definitions(Game PUBLIC 
    ENGINE_PLATFORM_WINDOWS
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/ScriptCore/bin/${CMAKE_BUILD_TYPE}/ScriptCore.dll
        $<TARGET_FILE_DIR:Game>
    COMMENT "Copying ScriptCore.dll to output directory"
)

