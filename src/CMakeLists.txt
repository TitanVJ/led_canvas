add_subdirectory(display)
add_subdirectory(frame)

add_executable(MatrixGaming
    main.cpp
)

target_link_libraries(MatrixGaming
    led_display
)

set_target_properties(MatrixGaming
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

install(TARGETS MatrixGaming
    RUNTIME DESTINATION
)

# any custom commands such as to copy over certain directories
