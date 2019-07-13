cmake_minimum_required(VERSION 2.8)
project(G52CPP)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${G52CPP_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})


set(SOURCE_FILES src/mainfunction.cpp 
src/BaseEngine.cpp
src/BouncingBallMain.cpp
src/BouncingBall.cpp
src/DisplayableObject.cpp
src/DraggingDemo.cpp
src/DrawingSurface.cpp
src/FontManager.cpp
src/ImageObjectDemo.cpp
src/Images.cpp
src/JasonsDemoA.cpp
src/JasonsObjectA.cpp
src/JasonsTileManager.cpp
src/MazeDemoMain.cpp
src/MazeDemoObject.cpp
src/MazeDemoTileManager.cpp
src/PlayingCardsDemo.cpp
src/RawImage.cpp
src/SimpleDemo.cpp
src/TileManager.cpp
src/ZoomingDemo.cpp
)

add_executable(SDL2_App ${SOURCE_FILES})
target_link_libraries(SDL2_App ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY})

add_custom_command(TARGET SDL2_App PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${G52CPP_SOURCE_DIR}/src/resources ${CMAKE_CURRENT_BINARY_DIR})
