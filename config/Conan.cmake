set(CONAN_DIR "${CMAKE_CURRENT_BINARY_DIR}/conan")

if(NOT EXISTS "${CONAN_DIR}/conan.cmake")
    file(DOWNLOAD
            "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CONAN_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()
include("${CONAN_DIR}/conan.cmake")

set(ENV{CONAN_USER_HOME} "${CONAN_DIR}")
conan_cmake_run(
        CONANFILE conanfile.txt
        BUILD missing
        GENERATORS CMakeDeps
        INSTALL_FOLDER "${CONAN_DIR}")
list(PREPEND CMAKE_MODULE_PATH "${CONAN_DIR}")
list(PREPEND CMAKE_PREFIX_PATH "${CONAN_DIR}")