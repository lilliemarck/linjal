find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})

add_executable(linjal
    main.cpp)

target_link_libraries(linjal ${GTKMM_LIBRARIES})
