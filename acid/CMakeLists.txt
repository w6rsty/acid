set(ACID_INCLUDE_LIST ./include)
aux_source_directory(./src ACID_SRC_LIST)
aux_source_directory(./src/core ACID_SRC_LIST)
aux_source_directory(./src/window ACID_SRC_LIST)
aux_source_directory(./src/renderer ACID_SRC_LIST)
aux_source_directory(./src/platform/OpenGL ACID_SRC_LIST)

aux_source_directory(./src/platform/Windows ACID_SRC_LIST)

add_executable(acid ${ACID_SRC_LIST})
target_include_directories(acid PUBLIC ${ACID_INCLUDE_LIST})
target_compile_features(acid PRIVATE cxx_std_17)

if (ACID_PLATFORM_WINDOWS)
    target_compile_definitions(acid PRIVATE ACID_PLATFORM_WINDOWS)
endif()
if (ACID_DEBUG)
    target_compile_definitions(acid PRIVATE ACID_DEBUG)
endif()

set(ACID_LIBS
    glfw
    glad
    debugbreak
    )

target_link_libraries(acid ${ACID_LIBS})