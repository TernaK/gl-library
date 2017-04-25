option(BUILD_GL_LIB_APPS "Build applications" ON)

if(BUILD_GL_LIB_APPS)
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
  
  find_package(OpenCV REQUIRED)
  include_directories(${OpenCV_INCLUDE_DIRS})

  add_executable(Text Text.cpp)
  target_link_libraries(Text gl_library ${OpenCV_LIBS})

  add_executable(Shadow Shadow.cpp)
  target_link_libraries(Shadow gl_library)
  
  add_executable(Shadow+Camera Shadow+Camera.cpp)
  target_link_libraries(Shadow+Camera gl_library)

  add_executable(Flow Flow.cpp)
  target_link_libraries(Flow gl_library ${OpenCV_LIBS})
endif()
