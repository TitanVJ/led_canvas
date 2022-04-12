add_executable(Mic
    main.c
    micInput.c
    fileIO.c
)

target_include_directories(Mic
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(Mic
                        PROPERTIES
                        LINKER_LANGUAGE C
                        C_STANDARD 99
)

install(TARGETS Mic
    RUNTIME DESTINATION
)
