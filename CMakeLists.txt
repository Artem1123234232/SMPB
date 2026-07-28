cmake_minimum_required(VERSION 3.16)

project(smpb LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Устанавливаем стандарт C++20
set(CMAKE_CXX_STANDARD 23)
# Говорим, что это обязательное требование (если не потянет — вылетит с ошибкой)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Отключаем расширения компилятора (чтобы код был чистым стандартом)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(smpb
  main.cpp
  workconsole.h
  workconsole.cpp
  smpbcore.h
  smpbcore.cpp
  consoleapi.h
  consoleapi.cpp

  baseconsole.h
  baseconsole.cpp
  config.h
  debugconsole.h
  debugconsole.cpp
  baseclass.h
  baseclass.cpp

)
target_link_libraries(smpb PRIVATE Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS smpb
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
