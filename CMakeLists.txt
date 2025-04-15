cmake_minimum_required(VERSION 3.10)
project(GameEngine LANGUAGES CXX  C CSharp)

#C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/Third)
set(MONO_DIR ${CMAKE_SOURCE_DIR}/Mono)
set(VULKANSKD_DIR ${CMAKE_SOURCE_DIR}/VulkanSDK)

set(THIRDPARTY_INCLUDE_DIR ${THIRDPARTY_DIR}/include)
set(THIRDPARTY_LIB_DIR ${THIRDPARTY_DIR}/lib)

set(MONO_INCLUDE_DIR ${MONO_DIR}/include)
set(MONO_LIB_DIR ${MONO_DIR}/lib)

set(VULKANSKD_INCLUDE_DIR ${VULKANSKD_DIR}/include)
set(VULKANSKD_LIB_DIR ${VULKANSKD_DIR}/lib)

#包含第三方头文件
include_directories(
    ${THIRDPARTY_INCLUDE_DIR}
    ${MONO_INCLUDE_DIR}
    ${VULKANSKD_INCLUDE_DIR}
)
#链接第三方库路径
link_directories(
    ${THIRDPARTY_LIB_DIR}
    ${MONO_LIB_DIR}
    ${VULKANSKD_LIB_DIR}
)

#添加子项目
add_subdirectory(box2d)
add_subdirectory(Engine)
add_subdirectory(Game)
add_subdirectory(glad)
add_subdirectory(GLFW)
add_subdirectory(glm)
add_subdirectory(ImGui)
add_subdirectory(miniaudio)
add_subdirectory(ScriptEngine)
add_subdirectory(stb_image)
add_subdirectory(yaml)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")



