cmake_minimum_required(VERSION 2.6)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Locate OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})

# Link
add_executable(test tests.cpp)
# add_executable(test fulltests.cpp)
target_link_libraries(test ${GTEST_LIBRARIES} ${OPENSSL_LIBRARIES} pthread)
