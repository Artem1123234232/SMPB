cmake_minimum_required(VERSION 3.21)

project(smpb VERSION 1.0.0 LANGUAGES CXX)

# Настройки сборки — только если smpb собирается сам, а не внутри другого проекта
if(PROJECT_IS_TOP_LEVEL)
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
  endif()

  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_ok)
  if(ipo_ok)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
  endif()
endif()

# Библиотека ядра
add_library(smpb_core STATIC
  src/smpb/Core.cpp
  src/smpb/BaseTask.cpp
  src/smpb/BaseComponent.cpp
  src/smpb/tools.cpp
  src/smpb/Config.cpp
)
add_library(smpb::core ALIAS smpb_core)

target_include_directories(smpb_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(smpb_core PUBLIC cxx_std_23)
set_target_properties(smpb_core PROPERTIES CXX_EXTENSIONS OFF)

# Программа
option(SMPB_BUILD_APP "Build smpb executable" ${PROJECT_IS_TOP_LEVEL})
if(SMPB_BUILD_APP)
  add_executable(smpb main.cpp)
  target_link_libraries(smpb PRIVATE smpb::core)
  set_target_properties(smpb PROPERTIES CXX_EXTENSIONS OFF)

  if(SMPB_ANDROID)
    target_sources(smpb PRIVATE AndroidFix.cpp)
  endif()

  include(GNUInstallDirs)
  install(TARGETS smpb RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
