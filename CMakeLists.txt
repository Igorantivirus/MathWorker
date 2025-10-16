cmake_minimum_required(VERSION 3.16)

# === Project ===
project(MathWorker VERSION 0.1.0 LANGUAGES CXX)

# === Language / toolchain defaults ===
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Экспорт команд компиляции (clangd/clang-tidy и т.д.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Options ===
option(MATHWORKER_BUILD_STATIC "Build static library (.lib/.a)" ON)
option(MATHWORKER_BUILD_SHARED "Build shared library (.dll/.so + import lib)" OFF)

# Если используешь vcpkg как внешнюю toolchain, её лучше задавать в Presets/CLI,
# а не здесь, чтобы не мешать локальной среде разработчика.

# === Layout ===
# Складываем артефакты в build/lib и build/bin (с подкаталогом конфигурации для Multi-Config)
function(_mw_set_output_dirs tgt)
  set_target_properties(${tgt} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<IF:$<CONFIG:>,$<CONFIG>,.>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:>,$<CONFIG>,.>"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<IF:$<CONFIG:>,$<CONFIG>,.>"
  )
endfunction()

# === Sources / includes ===
set(MATHWORKER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Используем GLOB для простоты (CONFIGURE_DEPENDS — чтобы не забывать про регенерацию)
file(GLOB_RECURSE MATHWORKER_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if(NOT MATHWORKER_SRC)
  message(FATAL_ERROR
    "MathWorker: не найдено исходников в ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp. "
    "Добавь .cpp файлы или поправь путь в CMakeLists.txt")
endif()

# === Targets ===
set(_mw_at_least_one OFF)

if(MATHWORKER_BUILD_STATIC)
  add_library(MathWorker_static STATIC ${MATHWORKER_SRC})
  set(EXECUTABLE_NAME MathWorker_static)
  target_include_directories(MathWorker_static PUBLIC "${MATHWORKER_INCLUDE_DIR}")

  # На *nix часто полезно PIC даже для статической, если статическая войдёт в shared наверху
  set_target_properties(MathWorker_static PROPERTIES POSITION_INDEPENDENT_CODE ON)

  _mw_set_output_dirs(MathWorker_static)

  add_library(MathWorker::static ALIAS MathWorker_static)
  set(_mw_at_least_one ON)
endif()

if(MATHWORKER_BUILD_SHARED)
  add_library(MathWorker_shared SHARED ${MATHWORKER_SRC})
  set(EXECUTABLE_NAME MathWorker_shared)
  target_include_directories(MathWorker_shared PUBLIC "${MATHWORKER_INCLUDE_DIR}")

  # На Windows упростим экспорт символов, если нет экспортных макросов/DEF
  if(MSVC)
    set_target_properties(MathWorker_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(MathWorker_shared PRIVATE MATHWORKER_EXPORTS)
  endif()

  _mw_set_output_dirs(MathWorker_shared)

  add_library(MathWorker::shared ALIAS MathWorker_shared)
  set(_mw_at_least_one ON)
endif()

target_include_directories(${EXECUTABLE_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if(NOT _mw_at_least_one)
  message(FATAL_ERROR
    "MathWorker: ни один вариант библиотеки не включён. "
    "Включи MATHWORKER_BUILD_STATIC и/или MATHWORKER_BUILD_SHARED.")
endif()

# Удобный универсальный алиас 'MathWorker', если собран ровно один вариант
if(MATHWORKER_BUILD_STATIC AND NOT MATHWORKER_BUILD_SHARED)
  add_library(MathWorker ALIAS MathWorker_static)
elseif(MATHWORKER_BUILD_SHARED AND NOT MATHWORKER_BUILD_STATIC)
  add_library(MathWorker ALIAS MathWorker_shared)
endif()

# === Install (базовый) ===
# Позволит установить артефакты локально и потом использовать из другого проекта через find_library()/manual include.
include(GNUInstallDirs)

if(TARGET MathWorker_static)
  install(TARGETS MathWorker_static
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(TARGET MathWorker_shared)
  install(TARGETS MathWorker_shared
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

# Публичные заголовки
if(EXISTS "${MATHWORKER_INCLUDE_DIR}")
  install(DIRECTORY "${MATHWORKER_INCLUDE_DIR}/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

# === User hints ===
message(STATUS "MathWorker version: ${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build static: ${MATHWORKER_BUILD_STATIC}")
message(STATUS "Build shared: ${MATHWORKER_BUILD_SHARED}")
message(STATUS "Binary dir:   ${CMAKE_BINARY_DIR}")
