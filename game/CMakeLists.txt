set(GAME_LIBS
    ${PROJECT_NAME}Engine
    
    Kokoro::Memory
    Kokoro::Utility
    Kokoro::FileSystem
    Kokoro::Exception
    Kokoro::Config

    GameNetworkingSockets
)
set(GAME_SERVER_LIBS
)


file(GLOB_RECURSE GAME_SOURCES
   src/Game/*.cc
)
file(GLOB_RECURSE GAME_SERVER_SOURCES
   src/Server/*.cc
)

if (ANDROID)
    add_library(${PROJECT_NAME} MODULE ${GAME_SOURCES} src/main.cc)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIBS})
        target_include_directories(${PROJECT_NAME} PRIVATE src/)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${BACKWARD_ENABLE})
            add_backward(${PROJECT_NAME})
    endif()
else()
    # Game
    add_executable(${PROJECT_NAME} ${GAME_SOURCES} src/main.cc)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIBS})
        target_include_directories(${PROJECT_NAME} PRIVATE src/)

    # Server
    add_executable(${PROJECT_NAME}Server ${GAME_SOURCES} ${GAME_SERVER_SOURCES} src/main.cc)
        target_link_libraries(${PROJECT_NAME}Server PRIVATE ${GAME_LIBS} ${GAME_SERVER_LIBS})
        target_include_directories(${PROJECT_NAME}Server PRIVATE src/)
        target_compile_definitions(${PROJECT_NAME}Server PRIVATE GAME_SERVER=1)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${BACKWARD_ENABLE})
            add_backward(${PROJECT_NAME})

        target_link_libraries(${PROJECT_NAME}Server PRIVATE ${BACKWARD_ENABLE})
            add_backward(${PROJECT_NAME}Server)
    endif()
endif()
