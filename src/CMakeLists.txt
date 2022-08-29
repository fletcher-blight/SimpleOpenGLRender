find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)

add_executable(renderer renderer/main.cpp)
target_link_libraries(renderer OpenGL::GL GLEW::GLEW glfw glm::glm fmt::fmt)