cmake_minimum_required(VERSION 3.16)

project(SParamAnalyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appSParamAnalyzer
    main.cpp
)

qt_add_qml_module(appSParamAnalyzer
    URI SParamAnalyzer
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES IVna.h
        SOURCES vna.h vna.cpp
)

target_link_libraries(appSParamAnalyzer
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appSParamAnalyzer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
