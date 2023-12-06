# CMakeList.txt : CMake project for comp345a1, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

# Setting the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source to this project's executable.
add_executable(comp345a1 "Map.h" "Map.cpp" "MapDriver.cpp" Player.cpp Player.h
        PlayerDriver.cpp
        Main.cpp
        Orders.cpp
        OrdersDriver.cpp
        GameEngine.cpp
        PlayerDriver.h
        GameEngineDriver.h
        MapDriver.h
        GameEngineDriver.cpp
        Cards.h
        Cards.cpp
        CardsDriver.cpp
        CardsDriver.h
        CommandProcessing.h
        CommandProcessing.cpp
        CommandProcessingDriver.cpp
        CommandProcessingDriver.h
        GameLoop.cpp
        GameLoopDriver.cpp
        GameLoopDriver.h
        GameLoop.cpp
        PlayerStrategies.cpp PlayerStrategies.h PlayerStrategiesDriver.cpp PlayerStrategiesDriver.h
        LoggingObserver.cpp
        LoggingObserver.h
        LoggingObserver.cpp
        TournamentDriver.h
        TournamentDriver.cpp
        TournamentDriver.cpp
)


# Copy the maps directory to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/maps DESTINATION ${CMAKE_BINARY_DIR}/comp345a1)
file(COPY ${CMAKE_SOURCE_DIR}/command_files DESTINATION ${CMAKE_BINARY_DIR}/comp345a1)

# TODO: Add tests and install targets if needed.
