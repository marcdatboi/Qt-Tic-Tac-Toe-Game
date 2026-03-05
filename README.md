# PROJECT OVERVIEW
* This project is a desktop app that was built using Qt6 with C++. 
* The project features an interactive tic-tac-toe game designed for 2 players.

# FEATURES (WIP)
1. A Modernized board game powered by Qt6 and C++
2. 2-player gameplay
3. Custom player icons (not limited to 'X' and 'O')
4. Win's and Tie's visuals

# SETUP
## Before You Start
1. C++ 17 or later
2. Ninja Generator
3. CMake version >=3.16 & AutoMOC enabled
4. Qt6 version 6.10.2 -- Core, Gui, Widgets
5. Bash

## Installation
1. Clone the repo -- https://github.com/marcdatboi/Qt-Tic-Tac-Toe-Game.git

2. Initialize CMake (if you need to):
    * Make a build folder -- cmake -S . -B build
    * Set "CMAKE_PREFIX_PATH" -- path/to/your/qt/folder/bin
    * Find the Qt6 package -- "Qt6 COMPONENTS REQUIRED Gui Core Widgets"
    * Link all necessary libraries -- tictactoe PRIVATE Qt6::Gui Qt6::Core Qt6::Widgets

# NOTES
1. This project was developed using CLion!
2. If you use VSCode, make sure to install the **Qt Extension Pack** from the extension store
3. If you're looking for a fast setup, use the **Qt Creator** IDE, copy this repo to your local machine and select the CMakeLists.txt as your project for everything to load properly.