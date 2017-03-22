add_executable(SineSurface SineSurface.cpp)
target_link_libraries(SineSurface gl_library)

#add_executable(GravityParticles GravityParticles.cpp)
#target_link_libraries(GravityParticles gl_library)

#add_executable(Particles+Shader Particles+Shader.cpp)
#target_link_libraries(Particles+Shader gl_library)

add_executable(ControlPoints ControlPoints.cpp)
target_link_libraries(ControlPoints gl_library)

add_executable(Shapes Shapes.cpp)
target_link_libraries(Shapes gl_library)

add_executable(Drone Drone.cpp)
target_link_libraries(Drone gl_library)

add_executable(MassSpring MassSpring.cpp)
target_link_libraries(MassSpring gl_library)
