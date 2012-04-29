find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

set(CML_INCLUDE_DIR "" CACHE PATH "Path to CML headers")

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CML_INCLUDE_DIR})

add_executable(linjal
    drawing_area.cpp
    main.cpp
    main_window.cpp
    math.cpp
    pen_tool.cpp
    point_ref.cpp
    select_tool.cpp
    shape.cpp
    tool.cpp
    utils.cpp)

target_link_libraries(linjal ${GTKMM_LIBRARIES})
