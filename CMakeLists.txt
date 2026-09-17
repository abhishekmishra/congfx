cmake_minimum_required(VERSION 3.16)

project(congfx LANGUAGES C)

add_subdirectory(examples)

find_package(Doxygen QUIET)
find_program(MKDOCS_EXECUTABLE NAMES mkdocs)

if(Doxygen_FOUND AND MKDOCS_EXECUTABLE)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        COMMAND ${MKDOCS_EXECUTABLE} build --clean
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API and MkDocs documentation"
        VERBATIM
    )
else()
    add_custom_target(docs
        COMMAND ${CMAKE_COMMAND} -E echo
            "Documentation requires Doxygen and MkDocs in PATH."
        COMMAND ${CMAKE_COMMAND} -E false
        VERBATIM
    )
endif()
