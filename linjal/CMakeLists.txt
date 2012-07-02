find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

set(CML_INCLUDE_DIR "" CACHE PATH "Path to CML headers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=extra-semi -Wno-error=mismatched-tags")

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CML_INCLUDE_DIR})

add_executable(linjal
    camera.cpp
    color_cell_renderer.cpp
    color_palette_model.cpp
    color_palette_widget.cpp
    color_palette_window.cpp
    drawing_area.cpp
    main.cpp
    main_window.cpp
    math.cpp
    model.cpp
    pen_tool.cpp
    point_ref.cpp
    select_tool.cpp
    shape.cpp
    tool.cpp
    utils.cpp)

target_link_libraries(linjal ${GTKMM_LIBRARIES})
