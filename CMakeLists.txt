cmake_minimum_required(VERSION 3.20)
project(PricingProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========================================
# GoogleTest (FetchContent)
# ========================================
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)


# Activer CTest
enable_testing()

# Empêche GoogleTest d’utiliser des tests internes
set(gtest_disable_pthreads ON)
FetchContent_MakeAvailable(googletest)

# ========================================
# Sous-dossiers
# ========================================
add_subdirectory(src)
add_subdirectory(tests)
