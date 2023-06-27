# CMakeList.txt : CMake project for Face-Controlled Arkanoid, include source and define
# project specific logic here.
#
# 
#        "toolchainFile": "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"


# Add source to this project's executable.
add_executable(CMakeTarget "Face-Controlled Arkanoid.cpp" "Face-Controlled Arkanoid.h"    "game-objects/Ball.h" "game-objects/Ball.cpp" "Game.cpp" "Game.h" "game-objects/Paddle.h" "game-objects/Paddle.cpp"   "game-objects/Block.cpp" "game-objects/Block.h"    "CollisionHandler.h" "CollisionHandler.cpp" "GameObjectsParameters.h"       "game-states/RunningState.h" "game-states/MenuState.h" "game-states/PauseState.h" "game-states/MenuState.cpp" "game-states/PauseState.cpp" "game-states/RunningState.cpp"   "UI/Button.h" "UI/Button.cpp"   "game-states/GameResultState.cpp" "game-states/GameResultState.h" "UI/TextUtils.h" "UI/TextUtils.cpp")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/UI/ARIAL.TTF ${CMAKE_CURRENT_BINARY_DIR}/UI/ARIAL.TTF COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/face_detection_yunet_2022mar.onnx ${CMAKE_CURRENT_BINARY_DIR}/face_detection_yunet_2022mar.onnx COPYONLY)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.


find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package( OpenCV REQUIRED )

target_link_libraries(CMakeTarget PRIVATE sfml-system sfml-network sfml-graphics sfml-window)
target_link_libraries(CMakeTarget PRIVATE ${OpenCV_LIBS})

# If you want SFML to provide an implementation of main():
target_link_libraries(CMakeTarget PRIVATE sfml-main)