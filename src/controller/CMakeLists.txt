add_library(controller
    controller.cpp
)

target_include_directories(controller
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(controller
                        PROPERTIES
                        LINKER_LANGUAGE CXX
                        CXX_STANDARD 17
)
