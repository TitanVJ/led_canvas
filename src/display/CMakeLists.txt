add_library(led_display
    display.cpp
)

target_include_directories(led_display
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(led_display
    led_frame
)

set_target_properties(led_display
                        PROPERTIES
                        LINKER_LANGUAGE CXX
                        CXX_STANDARD 17
)
