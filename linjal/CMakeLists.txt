find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(linjal
    drawing_area.cpp
    main.cpp
    main_window.cpp)

target_link_libraries(linjal ${GTKMM_LIBRARIES})
