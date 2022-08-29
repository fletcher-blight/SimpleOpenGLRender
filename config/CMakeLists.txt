option(USE_CONAN "Use Conan for some dependencies" OFF)
if (${USE_CONAN})
    include(Conan.cmake)
endif()

include(FetchContent.cmake)
