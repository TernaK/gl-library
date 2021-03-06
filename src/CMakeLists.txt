include_directories(/usr/local/include)
include_directories(${gl_library_SOURCE_DIR}/include)
link_directories(/usr/local/lib)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCES 
	Texture.cpp
	Shader.cpp
	ResourceManager.cpp
	Material.cpp
	GLNode.cpp
	Light.cpp
  	ParticleSystem.cpp
  	MassSpringSystem.cpp
	GLTextNode.cpp
	)

set(HEADERS 
	${gl_library_SOURCE_DIR}/include/Texture.hpp
	${gl_library_SOURCE_DIR}/include/Shader.hpp
	${gl_library_SOURCE_DIR}/include/ResourceManager.hpp
	${gl_library_SOURCE_DIR}/include/Material.hpp
	${gl_library_SOURCE_DIR}/include/GLNode.hpp
	${gl_library_SOURCE_DIR}/include/Light.hpp
	${gl_library_SOURCE_DIR}/include/ParticleSystem.hpp
	${gl_library_SOURCE_DIR}/include/MassSpringSystem.hpp
	${gl_library_SOURCE_DIR}/include/AppTestUtility.hpp
	${gl_library_SOURCE_DIR}/include/GLTextNode.hpp
	)


find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_library(gl_library SHARED ${SOURCES} ${HEADERS})#lgtest
target_link_libraries(gl_library ${GLEW_LIBRARIES} glfw SOIL ${OpenCV_LIBS})
target_link_libraries(gl_library "-framework CoreFoundation")
target_link_libraries(gl_library "-framework OPENGL")

# install(TARGETS gl_library DESTINATION lib)
# install(FILES ${gl_library_HEADERS} DESTINATION include)

add_subdirectory(apps)
add_subdirectory(unit_tests)
