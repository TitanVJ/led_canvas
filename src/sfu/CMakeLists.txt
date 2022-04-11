add_executable(SFU
    sfu.cpp
)

target_link_libraries(SFU
    led_display
    led_frame
)

set_target_properties(SFU
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

install(TARGETS SFU
    RUNTIME DESTINATION
)
