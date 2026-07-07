cmake_minimum_required(VERSION 3.15)
project(upcpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} 
    main.cpp
    Entities/Entity.cpp
    Entities/Data/TransformComponent.cpp
    Entities/Data/HealthComponent.cpp
    Logic/Scene.cpp
    Logic/Systems/MovementSystem.cpp
)