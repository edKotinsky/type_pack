project(type_pack_doc)

find_package(Doxygen REQUIRED)

set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

set(DOC_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(MAINPAGE_MDFILE ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md)
set(DOTFILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dot)

find_program(CLANG clang)

if(CLANG)
    set(DOC_CLANG_ASSISTED_PARSING YES)
    set(DOC_CLANG_DATABASE_PATH ${CMAKE_BINARY_DIR})
    set(DOC_CLANG_OPTIONS -Wdocumentation)
else()
    set(CLANG_ASSISTED_PARSING NO)
endif()

set(REPLACE_TARGET_FILE ${CMAKE_SOURCE_DIR}/include/type_pack.hpp)
set(REPLACE_OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/type_pack.hpp)

configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

add_custom_target(replace_comments ALL
    COMMAND python3 ${REPLACE_SCRIPT}
        -i ${REPLACE_TARGET_FILE} -o ${REPLACE_OUTPUT_FILE} -r "//*" -t "//!"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
    USES_TERMINAL
)

add_custom_target( doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    DEPENDS replace_comments
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    VERBATIM
    USES_TERMINAL
)
