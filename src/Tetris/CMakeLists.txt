add_executable(Tetris
    tetris.cpp
    game.cpp
)

target_include_directories(Tetris
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(Tetris
    led_display
    led_frame
    controller
)

set_target_properties(Tetris
                        PROPERTIES
                        LINKER_LANGUAGE CXX
                        CXX_STANDARD 17
)

install(TARGETS Tetris
    RUNTIME DESTINATION
)
