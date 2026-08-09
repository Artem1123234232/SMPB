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
  VirtualBuffer/VirtualBuffer.h VirtualBuffer/VirtualBuffer.cpp
  config.h
  Core/Core.h Core/Core.cpp
  Core/Config.h Core/Config.cpp
  Core/Test3D.h Core/Test3D.cpp
  third_party/cgltf.h
)

add_custom_command(
  TARGET smpb POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
          "${CMAKE_CURRENT_SOURCE_DIR}/model.glb"
          "${CMAKE_CURRENT_BINARY_DIR}/model.glb"
  COMMENT "Copying jess.glb to build directory..."
)

include(GNUInstallDirs)
install(TARGETS smpb
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)