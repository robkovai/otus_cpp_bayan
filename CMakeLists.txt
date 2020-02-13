cmake_minimum_required(VERSION 3.2)

project(otus_cpp_bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

add_executable(bayan "main.cpp" "options.h" "options.cpp" "hashlib.h" "hashlib.cpp" "duplicates.h" "duplicates.cpp")

find_package(Boost COMPONENTS program_options filesystem REQUIRED)
target_include_directories(bayan PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(bayan ${Boost_LIBRARIES})

set_target_properties(bayan PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_NAME "bayan")
set(CPACK_PACKAGE_CONTACT "dedi4@mail.ru")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
