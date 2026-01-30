cmake_minimum_required(VERSION 3.16)

project(SParamAnalyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup()

qt_add_resources(QML_RESOURCES qml/qml.qrc)

qt_add_executable(appSParamAnalyzer
    main.cpp
    IVna.h
    tcp_vna.h tcp_vna.cpp
    debug_menu.h debug_menu.cpp
    debug_controller.h debug_controller.cpp
    ${QML_RESOURCES}
)

target_link_libraries(appSParamAnalyzer
    PRIVATE Qt6::Quick Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS appSParamAnalyzer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
