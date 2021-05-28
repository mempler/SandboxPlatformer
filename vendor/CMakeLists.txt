###############################################################################################

add_definitions(-DGLM_FORCE_PURE) # FIXME: glm crashes using SSE because of some alignment issues
add_definitions(-DGLM_ENABLE_EXPERIMENTAL) # Quaternions

add_subdirectory(glm)

###############################################################################################

add_subdirectory(Kokoro)

###############################################################################################

add_library(imgui STATIC imgui/imgui_demo.cpp imgui/imgui_draw.cpp
                         imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
                         imgui/imgui.cpp)
    target_include_directories(imgui PUBLIC imgui/ imgui/backends/)

###############################################################################################

add_subdirectory(tracy)

###############################################################################################

option(SPDLOG_FMT_EXTERNAL_HO "Use external fmt header-only library instead of bundled" ON)
add_subdirectory(spdlog)

###############################################################################################

add_subdirectory(entt)

###############################################################################################
