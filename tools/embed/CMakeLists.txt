add_executable(embed embed.cpp)

macro(embed_asset_library embed_target)
    set(sources)
    foreach(asset ${ARGN})
        get_filename_component(file_cpp ${asset} NAME_WE)
        get_filename_component(file_ext ${asset} EXT)
        string(REPLACE "." "" file_ext "${file_ext}")
        set(file_cpp "${CMAKE_CURRENT_BINARY_DIR}/embed_${file_cpp}_${file_ext}.cpp")
        add_custom_command(
            OUTPUT ${file_cpp}
            COMMAND embed ${asset} -o ${file_cpp}
            DEPENDS embed ${asset}
            COMMENT "GENERATING ${file_cpp}"
            VERBATIM)
        list(APPEND sources ${file_cpp})
    endforeach()
    add_library(${embed_target} ${sources})
endmacro()

