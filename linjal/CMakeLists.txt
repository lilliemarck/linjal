find_package(Qt4 REQUIRED QtCore QtGui QtOpenGl)
find_package(PkgConfig)

pkg_check_modules(SIGC sigc++-2.0)
pkg_check_modules(CAIROMM cairomm-1.0)

include(${QT_USE_FILE})
add_definitions(-DQT_NO_KEYWORDS)

include_directories(${CMAKE_SOURCE_DIR}/math/include)
include_directories(${CAIROMM_INCLUDE_DIRS})
include_directories(${SIGC_INCLUDE_DIRS})
include_directories(${QT_DEFINITIONS})

qt4_wrap_cpp(MOC_SOURCES
    drawing_area.hpp
    main_window.hpp)

add_executable(linjal
    camera.cpp
    drawing_area.cpp
    drawing_context.cpp
    json.cpp
    main.cpp
    main_window.cpp
    model.cpp
    pen_tool.cpp
    point_ref.cpp
    select_tool.cpp
    shape.cpp
    tool.cpp
    ${MOC_SOURCES})

target_link_libraries(linjal json_spirit ${CAIROMM_LIBRARIES} ${SIGC_LIBRARIES} ${QT_LIBRARIES})
