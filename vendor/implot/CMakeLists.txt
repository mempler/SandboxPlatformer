add_library(implot STATIC implot.cpp implot_demo.cpp implot_items.cpp)
    target_include_directories(implot PUBLIC .)
    target_link_libraries(implot PUBLIC imgui)
