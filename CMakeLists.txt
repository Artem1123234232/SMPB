cmake_minimum_required(VERSION 3.16)

project(smpb LANGUAGES CXX)

# Устанавливаем стандарт C++20
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
  Core/Config.h Core/Config.cpp

)

include(GNUInstallDirs)
install(TARGETS smpb
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)