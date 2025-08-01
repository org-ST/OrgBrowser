cmake_minimum_required(VERSION 3.10)
project(OrgBrowser)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt5 REQUIRED COMPONENTS
                        WebEngine WebEngineWidgets
                        Widgets Core Gui Network
            )

add_executable(Browser
    src/Main.cpp
)

target_link_libraries( Browser PRIVATE
        Qt5::WebEngine Qt5::WebEngineWidgets
        Qt5::Widgets Qt5::Core Qt5::Gui
        Qt5::Network
)