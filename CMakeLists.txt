cmake_minimum_required(VERSION 3.16)

project(SParamAnalyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets Charts Concurrent)

qt_standard_project_setup()

qt_add_resources(QML_RESOURCES qml/qml.qrc)

qt_add_executable(appSParamAnalyzer
    ${QML_RESOURCES}
    main.cpp
    IVna.h
    tcp_vna.h tcp_vna.cpp
    debug/debug_menu.h debug/debug_menu.cpp
    debug/debug_controller.h debug/debug_controller.cpp
    s_param_measurement.h s_param_measurement.cpp
    curve.h curve.cpp
    curve_model.h curve_model.cpp
    measurement_controller.h measurement_controller.cpp
    common.h
    app.h app.cpp
)

target_link_libraries(appSParamAnalyzer
    PRIVATE Qt6::Quick Qt6::Widgets Qt6::Charts Qt6::Concurrent
)

include(GNUInstallDirs)
install(TARGETS appSParamAnalyzer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
