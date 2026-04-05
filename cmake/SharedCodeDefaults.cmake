if (MSVC)
    # Debug
    target_compile_options(SharedCode INTERFACE
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Debug>:/RTC1>
    )

    # Warnings
    target_compile_options(SharedCode INTERFACE
        /W4
        /WX
    )

    # Release
    target_compile_options(SharedCode INTERFACE $<$<CONFIG:Release>:/fp:fast>)
    target_compile_options(SharedCode INTERFACE $<$<CONFIG:Release>:/Ox>)
else ()
    # Debug
    target_compile_options(SharedCode INTERFACE
        $<$<CONFIG:Debug>:-O0>
        $<$<CONFIG:Debug>:-g>
    )

    # Warnings
    target_compile_options(SharedCode INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        #-Werror
    )

    # Release
    target_compile_options(SharedCode INTERFACE $<$<CONFIG:Release>:-Ofast>)
    target_compile_options(SharedCode INTERFACE $<$<CONFIG:RelWithDebInfo>:-Ofast>)
endif ()

if (MSVC)
    target_compile_options(SharedCode INTERFACE /Zc:__cplusplus)
endif ()

target_compile_features(SharedCode INTERFACE cxx_std_23)
