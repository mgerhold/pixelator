add_executable(c2k_pixelator_sandbox
        main.cpp
        vertex_buffer.hpp
        vertex_buffer.cpp
        gl_data_usage_pattern.hpp
        gl_utils.hpp
        vertex_attribute_definition.hpp
        window.cpp
        window.hpp
        window_size.hpp
        opengl_version.hpp
        input.cpp
        input.hpp
        application.cpp
        application.hpp
        scoped_timer.cpp
        scoped_timer.hpp
        time.hpp
        random.cpp
        random.hpp
        application_context.cpp
        application_context.hpp
        shader_program.cpp
        shader_program.hpp
        include_glm.hpp
        color.hpp
        rect.hpp
        hash/hash.cpp
        hash/hash.hpp
        renderer.cpp
        renderer.hpp
        texture.cpp
        texture.hpp
        guid.hpp
        image.cpp
        image.hpp
)

target_compile_definitions(c2k_pixelator_sandbox PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
)

target_link_libraries(c2k_pixelator_sandbox
        PRIVATE
        c2k_pixelator_project_options
)

target_compile_definitions(c2k_pixelator_sandbox PRIVATE "GLFW_INCLUDE_NONE")

target_link_system_libraries(c2k_pixelator_sandbox
        PRIVATE
        spdlog::spdlog
        glfw
        glad
        GSL
        imgui
        range-v3
        glm::glm-header-only
        tl::expected
        stbi_image
)
