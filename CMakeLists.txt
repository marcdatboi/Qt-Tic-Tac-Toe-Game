



cmake_minimum_required(VERSION 3.16)
project(tictactoe LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure sum stuff for Qt6
set(CMAKE_PREFIX_PATH "C:/DevFolder/tools/Frameworks/Qt/6.10.2/mingw_64/bin")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(HEADERS
        include/MainWindow.hpp
        include/Board.hpp

)

set(SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/Board.cpp
)

# Find Qt
find_package(Qt6 COMPONENTS REQUIRED Gui Core Widgets)

add_executable(tictactoe
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(tictactoe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tictactoe PRIVATE
        Qt6::Gui
        Qt6::Core
        Qt6::Widgets
)

