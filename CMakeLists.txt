cmake_minimum_required(VERSION 3.15)

project(RGS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(HEADERS

 "src/RGS/Application.h" "src/RGS/Base.h" "src/RGS/Window.h" "src/RGS/WindowsWindow.h" "src/RGS/InputCode.h" "src/RGS/Framebuffer.h" "src/RGS/Maths.h" "src/RGS/Shader/ShaderBase.h"
 
 "src/RGS/Render.h" "src/RGS/Shader/BlinnShader.h" "src/RGS/Texture.h")

set(SOURCES
	src/RGS/Main.cpp;
 "src/RGS/Application.cpp" "src/RGS/Window.cpp" "src/RGS/WindowsWindow.cpp" "src/RGS/Maths.cpp" "src/RGS/Framebuffer.cpp"
 "src/RGS/Shader/BlinnShader.cpp" "src/RGS/Render.cpp" "src/RGS/stb/std_image.cpp" "src/RGS/Texture.cpp")

set(INCLUDE_PATH
"src"
)
set(TARGET RGS)
add_executable(${TARGET} ${HEADERS} ${SOURCES})
target_include_directories(${TARGET} PRIVATE ${INCLUDE_PATH})

set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DST "$<TARGET_FILE_DIR:${TARGET}>/assets")

set(ASSETS_CMD copy_directory)
set(ASSETS_MSG "Copy Assets")

add_custom_command(
	TARGET ${TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo
	"${ASSETS_MSG}: ${ASSETS_SRC} to ${ASSETS_DST}"
)
add_custom_command(
	TARGET ${TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E ${ASSETS_CMD}
	"${ASSETS_SRC}" "${ASSETS_DST}"
)