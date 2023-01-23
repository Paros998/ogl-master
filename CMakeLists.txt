# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Tutorials)

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)



include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# Tutorial 1
add_executable(tutorial01_first_window 
	tutorial01_first_window/tutorial01.cpp
)
target_link_libraries(tutorial01_first_window
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial01_first_window PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial01_first_window/")
create_target_launcher(tutorial01_first_window WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial01_first_window/")

# Tutorial 2
add_executable(tutorial02_red_triangle 
	tutorial02_red_triangle/tutorial02.cpp
	common/shader.cpp
	common/shader.hpp
	
	tutorial02_red_triangle/SimpleFragmentShader.fragmentshader
	tutorial02_red_triangle/SimpleVertexShader.vertexshader
)
target_link_libraries(tutorial02_red_triangle
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial02_red_triangle PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/")
create_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/")
create_default_target_launcher(tutorial02_red_triangle WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/") # tut 1 is not the default or people would complain that tut 2 doesn't work

# Tutorial 3
add_executable(tutorial03_matrices 
	tutorial03_matrices/tutorial03.cpp
	common/shader.cpp
	common/shader.hpp

	tutorial03_matrices/SimpleTransform.vertexshader
	tutorial03_matrices/SingleColor.fragmentshader
)
#set_target_properties(tutorial03_matrices PROPERTIES RUNTIME_OUTPUT_DIRECTORY /test1)
target_link_libraries(tutorial03_matrices
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial03_matrices PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial03_matrices/")
create_target_launcher(tutorial03_matrices WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial03_matrices/") # Visual

# Tutorial 4
add_executable(tutorial04_colored_cube
	tutorial04_colored_cube/tutorial04.cpp
	common/shader.cpp
	common/shader.hpp
	
	tutorial04_colored_cube/TransformVertexShader.vertexshader
	tutorial04_colored_cube/ColorFragmentShader.fragmentshader
)
target_link_libraries(tutorial04_colored_cube
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial04_colored_cube PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial04_colored_cube/")
create_target_launcher(tutorial04_colored_cube WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial04_colored_cube/")

# Tutorial 5
add_executable(tutorial05_textured_cube
	tutorial05_textured_cube/tutorial05.cpp
	common/shader.cpp
	common/shader.hpp
	common/texture.cpp
	common/texture.hpp
	
	tutorial05_textured_cube/TransformVertexShader.vertexshader
	tutorial05_textured_cube/TextureFragmentShader.fragmentshader
)
target_link_libraries(tutorial05_textured_cube
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial05_textured_cube PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial05_textured_cube/")
create_target_launcher(tutorial05_textured_cube WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial05_textured_cube/")

# Tutorial 6
add_executable(tutorial06_keyboard_and_mouse
	tutorial06_keyboard_and_mouse/tutorial06.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	
	tutorial06_keyboard_and_mouse/TransformVertexShader.vertexshader
	tutorial06_keyboard_and_mouse/TextureFragmentShader.fragmentshader
)
target_link_libraries(tutorial06_keyboard_and_mouse
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial06_keyboard_and_mouse PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial06_keyboard_and_mouse/")
create_target_launcher(tutorial06_keyboard_and_mouse WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial06_keyboard_and_mouse/")

# Tutorial 7
add_executable(tutorial07_model_loading
	tutorial07_model_loading/tutorial07.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp

	tutorial07_model_loading/TransformVertexShader.vertexshader
	tutorial07_model_loading/TextureFragmentShader.fragmentshader
)
target_link_libraries(tutorial07_model_loading
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial07_model_loading PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial07_model_loading/")
create_target_launcher(tutorial07_model_loading WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial07_model_loading/")

# Tutorial 8
add_executable(tutorial08_basic_shading
	tutorial08_basic_shading/tutorial08.cpp
	common/shader.cpp
	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	
	tutorial08_basic_shading/StandardShading.vertexshader
	tutorial08_basic_shading/StandardShading.fragmentshader
)
target_link_libraries(tutorial08_basic_shading
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(tutorial08_basic_shading PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tutorial08_basic_shading/")
create_target_launcher(tutorial08_basic_shading WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tutorial08_basic_shading/")


# lab1
add_executable(lab1
	pg_dg/lab1/lab1.cpp
	pg_dg/common/Shader.cpp
	pg_dg/common/Shader.h


	pg_dg/lab1/shaders/Basic.vert
	pg_dg/lab1/shaders/Color.frag
)
target_link_libraries(lab1
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(lab1 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab1/")
create_target_launcher(lab1 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab1/")


# lab2
add_executable(lab2
	pg_dg/lab2/lab2.cpp

	pg_dg/common/Shader.h
	pg_dg/common/GlobalData.h
	pg_dg/common/Vector2f.h
	pg_dg/common/Vector2i.h
	pg_dg/common/Roller.h

	pg_dg/common/Shader.cpp
	pg_dg/common/GlobalData.cpp
	pg_dg/common/Vector2f.cpp
	pg_dg/common/Vector2i.cpp
	pg_dg/common/Roller.cpp

	pg_dg/lab2/shaders/Basic.vert
	pg_dg/lab2/shaders/Color.frag

	pg_dg/lab2/shaders/Texture.frag
	pg_dg/lab2/shaders/Texture.frag
)
target_link_libraries(lab2
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(lab2 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab2/")
create_target_launcher(lab2 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab2/")

# lab3
add_executable(lab3
	pg_dg/lab3/lab3.cpp

	pg_dg/common/Shader.h
	pg_dg/common/GlobalData.h
	pg_dg/common/Vector2f.h
	pg_dg/common/Vector2i.h
	pg_dg/common/ParticleSystem.h

	pg_dg/common/Shader.cpp
	pg_dg/common/GlobalData.cpp
	pg_dg/common/Vector2f.cpp
	pg_dg/common/Vector2i.cpp
	pg_dg/common/ParticleSystem.cpp

	pg_dg/lab3/shaders/Basic.vert
	pg_dg/lab3/shaders/Color.frag

	pg_dg/lab3/shaders/Particle.vert
	pg_dg/lab3/shaders/Particle.frag
)
target_link_libraries(lab3
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(lab3 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab3/")
create_target_launcher(lab3 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab3/")

# lab4
add_executable(lab4
	pg_dg/lab4/lab4.cpp

	pg_dg/common/Shader.h
	pg_dg/common/GlobalData.h
	pg_dg/common/Vector2f.h
	pg_dg/common/Vector2i.h
	pg_dg/common/Texture.h
	pg_dg/common/stb_image.h

	pg_dg/common/Shader.cpp
	pg_dg/common/GlobalData.cpp
	pg_dg/common/Vector2f.cpp
	pg_dg/common/Vector2i.cpp
	pg_dg/common/Texture.cpp
	pg_dg/common/Sphere.cpp

	pg_dg/lab4/shaders/multiTexture/vertex.shader
	pg_dg/lab4/shaders/multiTexture/frag.shader

	pg_dg/lab4/shaders/simpleTexture/vertex.shader
	pg_dg/lab4/shaders/simpleTexture/frag.shader

	pg_dg/lab4/assets/stars.jpg
	pg_dg/lab4/assets/texture.png
)
target_link_libraries(lab4
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(lab4 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab4/")
create_target_launcher(lab4 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab4/")

# lab5
add_executable(lab5
	pg_dg/lab5/lab5.cpp

	pg_dg/common/Shader.h
	pg_dg/common/GlobalData.h
	pg_dg/common/Vector2f.h
	pg_dg/common/Vector2i.h
	pg_dg/common/stb_image.h
	pg_dg/common/Roller.h

	pg_dg/common/Shader.cpp
	pg_dg/common/GlobalData.cpp
	pg_dg/common/Vector2f.cpp
	pg_dg/common/Vector2i.cpp
	pg_dg/common/Roller.cpp

	pg_dg/lab5/shaders/Basic.vert
	pg_dg/lab5/shaders/Color.frag
)
target_link_libraries(lab5
	${ALL_LIBS}
)
# Xcode and Visual working directories
set_target_properties(lab5 PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab5/")
create_target_launcher(lab5 WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab5/")

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )
SOURCE_GROUP(assets REGULAR_EXPRESSION ".*/.*assets" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
add_custom_command(
   TARGET tutorial01_first_window POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial01_first_window${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial01_first_window/"
)
add_custom_command(
   TARGET tutorial02_red_triangle POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial02_red_triangle${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/"
)
add_custom_command(
   TARGET tutorial03_matrices POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial03_matrices${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial03_matrices/"
)
add_custom_command(
   TARGET tutorial04_colored_cube POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial04_colored_cube${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial04_colored_cube/"
)
add_custom_command(
   TARGET tutorial05_textured_cube POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial05_textured_cube${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial05_textured_cube/"
)
add_custom_command(
   TARGET tutorial06_keyboard_and_mouse POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial06_keyboard_and_mouse${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial06_keyboard_and_mouse/"
)
add_custom_command(
   TARGET tutorial07_model_loading POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial07_model_loading${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial07_model_loading/"
)
add_custom_command(
   TARGET tutorial08_basic_shading POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial08_basic_shading${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial08_basic_shading/"
)
add_custom_command(
   TARGET lab1 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lab1${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab1/"
)
add_custom_command(
   TARGET lab2 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lab2${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab2/"
)
add_custom_command(
   TARGET lab4 POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/lab4${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/pg_dg/lab4/"
)

elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

