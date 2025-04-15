#ScriptEngine

set(SCRIPTCORE_PROJECT "${CMAKE_CURRENT_SOURCE_DIR}/ScriptEngine.csproj")

if(CMAKE_CONFIGURATION_TYPES)
    set(CONFIG $<CONFIG>)
else()
    set(CONFIG ${CMAKE_BUILD_TYPE})
endif()

set(SCRIPTCORE_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CONFIG}/net8.0")
set(SCRIPTCORE_DLL "${SCRIPTCORE_OUTPUT_DIR}/ScriptEngine.dll")

add_custom_command(
    OUTPUT ${SCRIPTCORE_DLL}
    COMMAND dotnet build ${SCRIPTCORE_PROJECT} -c ${CONFIG}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building ScriptCore.dll in ${CONFIG} mode"
)

add_custom_target(ScriptEngineBuild ALL
    DEPENDS ${SCRIPTCORE_DLL}
)

