cmake_minimum_required(VERSION 3.16)

project(smpb LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Устанавливаем стандарт C++23
set(CMAKE_CXX_STANDARD 23)
# Говорим, что это обязательное требование (если не потянет — вылетит с ошибкой)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Отключаем расширения компилятора (чтобы код был чистым стандартом)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)

add_executable(smpb
  main.cpp

  Core/Core.h Core/Core.cpp
  Core/BaseTask.h Core/BaseTask.cpp
  Core/BaseComponent.h Core/BaseComponent.cpp
  Core/tools.h Core/tools.cpp
  Core/Config.h Core/Config.cpp

)

if(SMPB_ANDROID)
  target_sources(smpb PRIVATE AndroidFix.cpp)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_ok)
if(ipo_ok)
  set_property(TARGET smpb PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
endif()

include(GNUInstallDirs)
install(TARGETS smpb
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
