#include <catch2/catch_test_macros.hpp>
#include <glfw_setup.h>
#include <shader.h>


struct DefaultWindowFixture {
    mutable GLFWwindow* window = sjd::createCursorLockedWindow(800, 600, "test_window");
    ~DefaultWindowFixture(){
        glfwTerminate();
    }
};

TEST_CASE_PERSISTENT_FIXTURE(DefaultWindowFixture,
                             "A Shader successfully compiles from valid shader files"){

    WHEN("I initialise a new shader with valid files"){
        sjd::Shader testShader{"./test_shader_data/valid.vert.glsl",
                               "./test_shader_data/valid.frag.glsl"};

        THEN("Shader compile status should be success"){
            int success {};
            glGetProgramiv(testShader.id(), GL_LINK_STATUS, &success);
            INFO( "Error Message: "<< testShader.errMsg() );
            CHECK( success == GL_TRUE );
            CHECK( testShader.isValid() == true );
        }
    }
    WHEN("I initialise a new shader with an INVALID vert shader"){
        sjd::Shader testShader{"./test_shader_data/invalid.vert.glsl",
                               "./test_shader_data/valid.frag.glsl"};
        THEN("Shader compile status should be failure"){
            int success {};
            glGetProgramiv(testShader.id(), GL_LINK_STATUS, &success);
            INFO( "Error Message: "<< testShader.errMsg() );
            CHECK( success == GL_FALSE );
            CHECK( testShader.isValid() == false );
        }

    }
    WHEN("I initialise a new shader with an INVALID frag shader"){
        sjd::Shader testShader{"./test_shader_data/valid.vert.glsl",
                               "./test_shader_data/invalid.frag.glsl"};
        THEN("Shader compile status should be failure"){
            int success {};
            glGetProgramiv(testShader.id(), GL_LINK_STATUS, &success);
            INFO( "Error Message: "<< testShader.errMsg() );
            CHECK( success == GL_FALSE );
            CHECK( testShader.isValid() == false );
        }

    }
}

