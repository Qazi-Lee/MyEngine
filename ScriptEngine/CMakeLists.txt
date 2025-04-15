#ScriptEngine

file(GLOB_RECURSE SCRIPT_ENGINE_SOURCES
    "ScriptEngine.csproj"
    "*.cs"
)

add_library(ScriptEngine SHARED ${SCRIPT_ENGINE_SOURCES})

set_target_properties(ScriptEngine PROPERTIES
    VS_DOTNET_TARGET_FRAMEWORK_VERSION "net8.0"
    VS_DOTNET_REFERENCES "System;System.Core"
)