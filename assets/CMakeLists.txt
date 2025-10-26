embed_assets(glhf_images
    ${CMAKE_CURRENT_SOURCE_DIR}/images/sheep.png
)

embed_assets(glhf_objects
    ${CMAKE_CURRENT_SOURCE_DIR}/objects/cube.glb
    ${CMAKE_CURRENT_SOURCE_DIR}/objects/joyful_vampire.glb
)

if(EMSCRIPTEN)
    set(EMBED_FLAGS -r "#version 330 core" "#version 300 es")
endif()
embed_assets(glhf_shaders ${CMAKE_CURRENT_SOURCE_DIR}/shaders/object.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/anim.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/inst.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/anim_tbn.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/screen.vert
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/object.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/object_normal.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/texture.frag
)
