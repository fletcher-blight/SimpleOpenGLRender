include(FetchContent)

FetchContent_Declare(
  GLEW
  GIT_REPOSITORY https://github.com/fletcher-blight/GLEWProxy
  GIT_TAG        main
  GIT_SHALLOW    ON
  OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
  glfw3
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG        3.3.8
  GIT_SHALLOW    ON
  OVERRIDE_FIND_PACKAGE
  CMAKE_ARGS -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=OFF
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG        0.9.9.8
    GIT_SHALLOW    ON
    OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        9.1.0
    GIT_SHALLOW    ON
    OVERRIDE_FIND_PACKAGE
)

