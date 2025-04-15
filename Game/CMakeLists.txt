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

${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/src 

)

target_compile_definitions(Game PUBLIC 
    ENGINE_PLATFORM_WINDOWS
)

add_dependencies(Game ScriptEngineBuild)

if(CMAKE_CONFIGURATION_TYPES)
    set(CONFIG $<CONFIG>)
else()
    set(CONFIG ${CMAKE_BUILD_TYPE})
endif()

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/ScriptEngine/bin/${CONFIG}/net8.0/ScriptEngine.dll"
    "$<TARGET_FILE_DIR:Game>"
    COMMENT "Copying ScriptEngine.dll to output directory"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/ScriptEngine/bin/${CONFIG}/net8.0/ScriptEngine.dll"
    "${CMAKE_BINARY_DIR}/Game"
    COMMENT "Copying ScriptEngine.dll to output directory"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Game>/assets"
    COMMENT "Copying entire assets folder"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Icons"
        "$<TARGET_FILE_DIR:Game>/Icons"
    COMMENT "Copying entire Icons folder"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_BINARY_DIR}/Game/assets"
    COMMENT "Copying entire assets folder"
)

add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Icons"
        "${CMAKE_BINARY_DIR}/Game/Icons"
    COMMENT "Copying entire Icons folder"
)