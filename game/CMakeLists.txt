set(GAME_LIBS
    ${PROJECT_NAME}Engine
    
    Kokoro::Memory
    Kokoro::Utility
    Kokoro::FileSystem
    Kokoro::Exception
    Kokoro::Config
)

#file(GLOB_RECURSE GAME_SOURCES
#    src/*.cc
#)
set(GAME_SOURCES
        src/main.cc)

if (ANDROID)
    add_library(${PROJECT_NAME} MODULE ${GAME_SOURCES})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIBS})
        target_include_directories(${PROJECT_NAME} PRIVATE src/)
else()
    add_executable(${PROJECT_NAME} ${GAME_SOURCES})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIBS})
        target_include_directories(${PROJECT_NAME} PRIVATE src/)
endif()
