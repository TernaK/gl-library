include_directories(/usr/local/include)
include_directories(${gl_library_SOURCE_DIR}/include)
link_directories(/usr/local/lib)

set(SOURCES 
	Texture.cpp
	Shader.cpp
	ResourceManager.cpp
	Material.cpp
	GLNode.cpp
	Light.cpp
        ParticleSystem.cpp
        MassSpringSystem.cpp
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
	)

add_library(gl_library SHARED ${SOURCES} ${HEADERS})#lgtest
target_link_libraries(gl_library glfw GLEW SOIL opencv_core)
target_link_libraries(gl_library "-framework CoreFoundation")
target_link_libraries(gl_library "-framework OPENGL")

install(TARGETS gl_library DESTINATION lib)
install(FILES ${gl_library_HEADERS} DESTINATION include)

add_subdirectory(apps)
add_subdirectory(unit_tests)
