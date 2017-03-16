add_executable(SineSurface SineSurface.cpp)
target_link_libraries(SineSurface gl_library)

add_executable(GravityParticles GravityParticles.cpp)
target_link_libraries(GravityParticles gl_library)

add_executable(Particles+Shader Particles+Shader.cpp)
target_link_libraries(Particles+Shader gl_library)

add_executable(ControlPoints ControlPoints.cpp)
target_link_libraries(ControlPoints gl_library)