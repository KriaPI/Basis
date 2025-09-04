include(helpers)

# Enable formatting of source files by using clang-format.
# Console command:
# cmake --build name_of_build_folder --target=format
macro(project_enable_clang_format)
    if (NOT DEFINED PROJECT_FILES)
        project_get_files_of_subdirectories(STORE_AS PROJECT_FILES)
    endif()
    
    find_program(PROJECT_CLANG_FORMAT clang-format REQUIRED)

    add_custom_target(format
        COMMAND ${PROJECT_CLANG_FORMAT} -i ${PROJECT_FILES}
    )
endmacro()

# Enables static analysis of source files by using clang-tidy.
# Console command:
# cmake --build name_of_build_folder --target=check
macro(project_enable_clang_tidy)
    if (NOT DEFINED PROJECT_FILES)
        project_get_files_of_subdirectories(STORE_AS PROJECT_FILES)
    endif()

    find_program(PROJECT_CLANG_TIDY clang-tidy REQUIRED)

    add_custom_target(check
        COMMAND ${PROJECT_CLANG_TIDY} -p ${CMAKE_BINARY_DIR} ${PROJECT_FILES}
    )
endmacro()