include("FindPythonModule.cmake")

option(BUILD_DOC "Build documentation" ON)


find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_INPUT_DIR "${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src/core ${PROJECT_SOURCE_DIR}/src/expression ${PROJECT_SOURCE_DIR}/src/ensemblemodel ${PROJECT_SOURCE_DIR}/src/math ${PROJECT_SOURCE_DIR}/src/treemodel ${PROJECT_SOURCE_DIR}/src/regressionmodel ${PROJECT_SOURCE_DIR}/src/output ${PROJECT_SOURCE_DIR}/src/treetable ${PROJECT_SOURCE_DIR}/src/utils")
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docsrc/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(DOXYGEN_WARNINGS NO)
    if(VERBOSE)
        set(DOXYGEN_WARNINGS "NO")
    endif()

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen ..."
        VERBATIM )
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation.")
endif (DOXYGEN_FOUND)

find_python_module(sphinx)
find_python_module(sphinx_rtd_theme)
find_python_module(breathe)
find_python_module(m2r)
if(DOXYGEN_FOUND AND SPHINX_FOUND AND SPHINX_RTD_THEME_FOUND AND BREATHE_FOUND)
    set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
    set(SPHINX_BUILD ${PROJECT_SOURCE_DIR}/docs)
    set(SPHINX_EXECUTABLE sphinx-build)
    set(SPHINX_FLAGS -Q)
    if(VERBOSE)
        set(SPHINX_FLAGS -q)
    endif()

    add_custom_target(Sphinx ALL
            COMMAND
            ${SPHINX_EXECUTABLE} ${SPHINX_FLAGS} -b html
            # Tell Breathe where to find the Doxygen output
            -Dproject=${PROJECT_NAME}
            -Dauthor=${AUHTOR}
            -Dcopyright=${COPYRIGHT}
            -Dversion=${PROJECT_VERSION}
            -Dbreathe_default_project=${PROJECT_NAME}
            -Dbreathe_projects.${PROJECT_NAME}=${DOXYGEN_OUTPUT_DIR}/xml
            ${SPHINX_SOURCE} ${SPHINX_BUILD}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Sphinx ...")
else(DOXYGEN_FOUND AND SPHINX_FOUND AND SPHINX_RTD_THEME_FOUND AND BREATHE_FOUND)
    message("Python 3 and sphinx, breathe, sphinx_rtd_theme python modules are needed to generate sphinx documentation")
endif(DOXYGEN_FOUND AND SPHINX_FOUND AND SPHINX_RTD_THEME_FOUND AND BREATHE_FOUND)