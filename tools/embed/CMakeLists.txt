cmake_minimum_required(VERSION 3.14)

if(EMSCRIPTEN)
    find_program(EMBED_EXE
        NAMES embed
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../bin
        REQUIRED)
else()
    add_executable(embed embed.cpp)
    target_compile_features(embed PUBLIC cxx_std_20)
    install(TARGETS embed RUNTIME DESTINATION bin)
endif()


macro(embed_assets embed_target)
    set(sources)
    foreach(asset ${ARGN})
        get_filename_component(file_cpp ${asset} NAME_WE)
        get_filename_component(file_ext ${asset} EXT)
        string(REPLACE "." "" file_ext "${file_ext}")
        set(file_cpp "${CMAKE_CURRENT_BINARY_DIR}/embed_${file_cpp}_${file_ext}.cpp")
        if(EMSCRIPTEN)
            add_custom_command(
                OUTPUT ${file_cpp}
                COMMAND ${EMBED_EXE} ${asset} ${EMBED_FLAGS} -o ${file_cpp}
                DEPENDS ${EMBED_EXE} ${asset}
                COMMENT "GENERATING ${file_cpp}"
                VERBATIM)
        else()
            add_custom_command(
                OUTPUT ${file_cpp}
                COMMAND embed ${asset} ${EMBED_FLAGS} -o ${file_cpp}
                DEPENDS embed ${asset}
                COMMENT "GENERATING ${file_cpp}"
                VERBATIM)
        endif()
        list(APPEND sources ${file_cpp})
    endforeach()
    add_library(${embed_target} ${sources})
endmacro()
