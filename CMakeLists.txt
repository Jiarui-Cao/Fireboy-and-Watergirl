cmake_minimum_required(VERSION 3.3)

set(GAME_EXE fireboy_and_watergirl)
project(${GAME_EXE} CXX)
include(.cs211/cmake/CMakeLists.txt)

# Defines a variable to stand for the .cxx files that implement
# the model.
set(MODEL_SRC
        src/character.cxx
        src/game_config.cxx
        src/level_layout.cxx
        src/trap_door.cxx
        src/model.cxx)

# Adds a program named `reversi` built from the listed source
# files and the common model files.
add_program(${GAME_EXE}
        src/main.cxx
        src/controller.cxx
        src/view.cxx
        ${MODEL_SRC})
target_link_libraries(${GAME_EXE} ge211)

# Adds a test program named `model_test` built from the listed
# source file and the common model files.
add_test_program(model_test
        test/model_test.cxx
        ${MODEL_SRC})
target_link_libraries(model_test ge211)


# vim: ft=cmake
