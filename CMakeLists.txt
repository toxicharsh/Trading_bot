cmake_minimum_required(VERSION 3.10)
project(trading_bot_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build executable
add_executable(bot main.cpp)

# Include all project folders
target_include_directories(bot PRIVATE
    core
    data
    indicators
    strategies
    execution
    portfolio
    analytics
    engine
)
