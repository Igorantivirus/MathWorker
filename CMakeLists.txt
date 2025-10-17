cmake_minimum_required(VERSION 3.14)
project(MathWorker
    VERSION 1.0.0
    DESCRIPTION "Template for C++ library built with CMake"
    LANGUAGES CXX)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# MATH_WORKER_SHARED_LIBS option (undefined by default) can be used to force shared/static build
option(MATH_WORKER_BUILD_TESTS "Build MathWorker tests" OFF)
option(MATH_WORKER_BUILD_EXAMPLES "Build MathWorker examples" OFF)
option(MATH_WORKER_BUILD_DOCS "Build MathWorker documentation" OFF)
option(MATH_WORKER_INSTALL "Generate target for installing MathWorker" ${is_top_level})
set_if_undefined(MATH_WORKER_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/MathWorker" CACHE STRING
    "Install path for MathWorker package-related CMake files")

if(DEFINED MATH_WORKER_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${MATH_WORKER_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(MathWorker) # initialized below
add_library(MathWorker::MathWorker ALIAS MathWorker)

#----------------------------------------------------------------------------------------------------------------------
# MathWorker dependencies
#----------------------------------------------------------------------------------------------------------------------

# Search for your dependencies here

#----------------------------------------------------------------------------------------------------------------------
# MathWorker sources
#----------------------------------------------------------------------------------------------------------------------

include(GenerateExportHeader)
set(export_file_name "export_shared.hpp")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.hpp")
endif()

generate_export_header(MathWorker EXPORT_FILE_NAME include/MathWorker/${export_file_name})

set("SOURCE_CPP"
  "src/ComplexStringConverter.cpp"
  "src/ConvertStringException.cpp"
  "src/FunctionConnector.cpp"
  "src/LaTexStringConverter.cpp"
  "src/MathStringConverter.cpp"
  "src/ParseException.cpp"
  "src/Parser.cpp"
  "src/ParseUtils.cpp"
  "src/Signature.cpp"
  "src/SignatureGenerator.cpp"
  "src/SignatureNode.cpp"
  "src/SymbolNode.cpp"
  "src/Tokenizer.cpp"
  "src/ValueNode.cpp"
)
set("SOURCE_HPP"
  "include/MathWorker/MathWorker.hpp"
  "include/MathWorker/MathClasses/MathNode.hpp"
  "include/MathWorker/MathClasses/SignatureNode.hpp"
  "include/MathWorker/MathClasses/SymbolNode.hpp"
  "include/MathWorker/MathClasses/ValueNode.hpp"
  "include/MathWorker/MathUtils/MathDefines.hpp"
  "include/MathWorker/Parse/ParseException.hpp"
  "include/MathWorker/Parse/Parser.hpp"
  "include/MathWorker/Parse/ParseUtils.hpp"
  "include/MathWorker/Parse/Tokenizer.hpp"
  "include/MathWorker/Signature/FunctionConnector.hpp"
  "include/MathWorker/Signature/Signature.hpp"
  "include/MathWorker/Signature/SignatureGenerator.hpp"
  "include/MathWorker/StringConvert/ComplexStringConverter.hpp"
  "include/MathWorker/StringConvert/ConvertStringException.hpp"
  "include/MathWorker/StringConvert/LaTexStringConverter.hpp"
  "include/MathWorker/StringConvert/MathStringConverter.hpp"
)

set(sources
    ${SOURCE_HPP}
    ${SOURCE_CPP}
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# MathWorker target
#----------------------------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(MathWorker PRIVATE ${sources})
target_compile_definitions(MathWorker PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:MATH_WORKER_STATIC_DEFINE>")

target_include_directories(MathWorker
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(MathWorker PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

if(MATH_WORKER_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/MathWorker-config.cmake.in MathWorker-config.cmake
        INSTALL_DESTINATION "${MATH_WORKER_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(MathWorker-config-version.cmake
        COMPATIBILITY SameMajorVersion)

    install(TARGETS MathWorker EXPORT MathWorker_export
        RUNTIME COMPONENT MathWorker
        LIBRARY COMPONENT MathWorker NAMELINK_COMPONENT MathWorker-dev
        ARCHIVE COMPONENT MathWorker-dev
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
        TYPE INCLUDE
        COMPONENT MathWorker-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/MathWorker/${export_file_name}"
        COMPONENT MathWorker-dev
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/MathWorker")

    set(targets_file "MathWorker-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "MathWorker-static-targets.cmake")
    endif()

    install(EXPORT MathWorker_export
        COMPONENT MathWorker-dev
        FILE "${targets_file}"
        DESTINATION "${MATH_WORKER_INSTALL_CMAKEDIR}"
        NAMESPACE MathWorker::)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/MathWorker-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/MathWorker-config-version.cmake"
        COMPONENT MathWorker-dev
        DESTINATION "${MATH_WORKER_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:MathWorker>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:MathWorker>/$<TARGET_FILE_PREFIX:MathWorker>$<TARGET_FILE_BASE_NAME:MathWorker>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
            COMPONENT MathWorker-dev
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# other targets
#----------------------------------------------------------------------------------------------------------------------

if(MATH_WORKER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(MATH_WORKER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(MATH_WORKER_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()