set(ENGINE_LIBS
    Kokoro::Memory
    Kokoro::Utility
    Kokoro::FileSystem
    Kokoro::Exception
    Kokoro::Config
)

set(ENGINE_SOURCES
    src/Engine.cc
)

add_library(${PROJECT_NAME}Engine STATIC ${ENGINE_SOURCES})
    target_link_libraries(${PROJECT_NAME}Engine PUBLIC ${ENGINE_LIBS})
    target_include_directories(${PROJECT_NAME}Engine PUBLIC src/)
