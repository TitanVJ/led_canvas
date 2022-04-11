add_library(led_frame
    frame.cpp
)

target_include_directories(led_frame
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(led_frame
    led_display
)

set_target_properties(led_frame
                        PROPERTIES
                        LINKER_LANGUAGE CXX
                        CXX_STANDARD 17
)
