file(GLOB TEST_SOURCES *.cpp)

add_executable(pricing_tests ${TEST_SOURCES})

# Ajouter l'include des headers de src/
target_include_directories(pricing_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(pricing_tests
    PRIVATE
        pricing_lib
        gtest
        gtest_main
)

# Activer la découverte des tests pour CTest
include(GoogleTest)
gtest_discover_tests(pricing_tests)
