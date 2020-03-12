cmake_minimum_required(VERSION 3.5.1)

# CREDITS
set(COPYRIGHT "2019 AMADEUS. All rights reserved.")
set(AUTHOR "Paolo Iannino")

# PROJECT NAME/VERSION
set(PROJECT_NAME cPMML)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

# BUILD OPTIONS
if(NOT DEFINED BUILD_TYPE)
    set(BUILD_TYPE Release)
endif()
set(RELEASE_FLAGS "-std=c++11 -Wall -Ofast -flto")
set(DEBUG_FLAGS "-std=c++11 -Wall -g -Og")
set(VERBOSE OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE ${BUILD_TYPE})
set(CMAKE_CXX_FLAGS_RELEASE ${RELEASE_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS_RELEASE ${RELEASE_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS_DEBUG ${DEBUG_FLAGS})
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${RELEASE_FLAGS})
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG ${DEBUG_FLAGS})

if(BUILD_TYPE MATCHES "Debug")
    set(VERBOSE ON)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ${VERBOSE})
set(CMAKE_VERBOSE_MAKEFILE ${VERBOSE})
if (CMAKE_GENERATOR MATCHES "Makefiles")
    set_property(GLOBAL PROPERTY TARGET_MESSAGES ${VERBOSE})
endif ()

set(ADDITIONAL_LINK_LIBRARIES "")

# MULTITHREADING
if(NOT DEFINED THREADS)
    set(THREADS 4)
endif()
find_package(OpenMP 4.0)
if(THREADS AND THREADS GREATER 1)
    if (OPENMP_FOUND)# AND OPENMP_VERSION GREATER_EQUAL 4)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
        add_definitions(-DMULTITHREADING)
        add_definitions(-DNUM_THREADS=${THREADS})
    else()
        message("OpenMP is needed to enable multithreading.")
    endif()
endif ()

# REGEX SUPPORT
#set(REGEX_SUPPORT TRUE)
if(REGEX_SUPPORT)
    find_package(Boost COMPONENTS regex)
    if(Boost_FOUND)
        set(ADDITIONAL_LINK_LIBRARIES "${ADDITIONAL_LINK_LIBRARIES}" "${Boost_REGEX_LIBRARY}")
        add_definitions(-DREGEX_SUPPORT)
    else()
        message("boost_regex is needed to enable string modifiers.")
    endif()
endif()

# BUILD LIBRARY
include(GNUInstallDirs)

add_definitions(-DCPMML_VERSION=\"${PROJECT_VERSION}\")
add_library(
        ${PROJECT_NAME} SHARED
        src/core/header.h
        src/api/exceptions.cc
        src/api/model.cc
        src/api/prediction.cc
        src/api/version.cc
        src/options.h
        src/core/xmlnode.h
        src/utils/csvreader.h
        src/utils/utils.h
        src/core/property.h
        src/core/intervalbuilder.h
        src/core/miningschema.h
        src/core/fastpredicate.h
        src/core/miningfunction.h
        src/core/sample.h
        src/core/transformationdictionary.h
        src/core/datatype.h
        src/core/builtinfunctions.h
        src/core/invalidvaluetreatmentmethod.h
        src/core/outliertreatmentmethod.h
        src/core/derivedfield.h
        src/core/datafield.h
        src/core/closure.h
        src/core/value.h
        src/core/string_view.h
        src/core/internal_score.h
        src/core/fieldusagetype.h
        src/core/indexer.h
        src/core/predicateoptype.h
        src/core/dagbuilder.h
        src/core/predicatetype.h
        src/core/missingvaluetreatmentmethod.h
        src/core/internal_model.h
        src/core/datadictionary.h
        src/core/modelbuilder.h
        src/core/predicate.h
        src/core/optype.h
        src/core/internal_evaluator.h
        src/core/miningfield.h
        src/core/target.h
        src/treemodel/treescore.h
        src/core/predicatebuilder.h
        src/treemodel/scoredistribution.h
        src/treemodel/node.h
        src/treemodel/treemodel.h
        src/treemodel/treeevaluator.h
        src/regressionmodel/regressionscore.h
        src/regressionmodel/regressiontable.h
        src/regressionmodel/numericpredictor.h
        src/regressionmodel/regressionmodel.h
        src/regressionmodel/regressionevaluator.h
        src/regressionmodel/predictorterm.h
        src/regressionmodel/categoricalpredictor.h
        src/regressionmodel/normalizationmethodtype.h
        src/regressionmodel/normalizationmethodbuilder.h
        src/ensemblemodel/ensemblemodel.h
        src/ensemblemodel/segment.h
        src/ensemblemodel/multiplemodelmethod.h
        src/ensemblemodel/ensembleevaluator.h
        src/math/misc.h
        src/math/normalizationmethods.h
        src/treetable/treetablenode.h
        src/treetable/treetable.h
        src/expression/expression.h
        src/expression/expressiontype.h
        src/expression/discretize.h
        src/expression/mapvalues.h
        src/expression/normdiscrete.h
        src/expression/normcontinuous.h
        src/expression/constant.h
        src/expression/expressionbuilder.h
        src/expression/apply.h
        src/expression/fieldref.h
        src/output/outputexpression.h
        src/output/outputexpressiontype.h
        src/output/outputexpressionbuilder.h
        src/output/outputfield.h
        src/output/outputdictionary.h
        src/output/predictedValue.h
        src/output/probability.h
        src/output/transformedvalue.h
        third_party/rapidxml-1.13/rapidxml.hpp
        third_party/miniz/miniz.h
        third_party/miniz/miniz.cc
        third_party/boost/integral.h)

# INSTALL LIBRARY
set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION}
        LINKER_LANGUAGE CXX
        PUBLIC_HEADER include/cPMML.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE third_party)
target_link_libraries(${PROJECT_NAME} PUBLIC ${ADDITIONAL_LINK_LIBRARIES})

install(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# BENCHMARK
add_subdirectory(benchmark)

# TEST
enable_testing()
add_subdirectory(test)

# DOCUMENTATION
add_subdirectory(docsrc)

