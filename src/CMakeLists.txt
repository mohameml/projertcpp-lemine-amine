# src/CMakeLists.txt

# Récupérer tous les fichiers source
file(GLOB SRC_FILES *.cpp)

# ========================================
#   Bibliothèque pricing_lib
# ========================================
add_library(pricing_lib ${SRC_FILES})

target_include_directories(pricing_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(pricing_lib PUBLIC cxx_std_20)

# ========================================
#   Executable principal
# ========================================
add_executable(pricing_app main.cpp)

# Enlever main.cpp de la library
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_link_libraries(pricing_app PRIVATE pricing_lib)
