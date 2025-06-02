#include <catch2/catch_test_macros.hpp>
#include <glfw_setup.h>

struct DefaultWindowFixture {
    mutable GLFWwindow* window = sjd::createCursorLockedWindow(800, 600, "test_window");
    ~DefaultWindowFixture(){
        glfwTerminate();
    }
};

TEST_CASE_PERSISTENT_FIXTURE( DefaultWindowFixture, "glfw accepts default args"){

    WHEN("I ask for a 800x600 window with no msaa"){

        THEN("I should not get a null pointer"){
             REQUIRE( window != nullptr );
        }
        THEN("The window framebuffer must be 800x600"){
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            REQUIRE( width == 800 ); 
            REQUIRE( height == 600 ); 
        }
        THEN("The number of MSAA samples should be 0") {
            GLint samples;
            glGetIntegerv(GL_SAMPLES, &samples);
            REQUIRE( samples == 0);
        }
    }

    WHEN("I change the window size"){
        int width {1600};
        int height {1200};
        glfwSetWindowSize(window, width, height);
        THEN("The window framebuffer will also change"){
            int fbWidth, fbHeight;
            glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
            REQUIRE( fbWidth == width );
            REQUIRE( fbHeight == height );
        }
    }
}


struct MsaaWindowFixture {
    mutable GLFWwindow* window = sjd::createCursorLockedWindow(1920, 1080, "test_window", 4);
    ~MsaaWindowFixture(){
        glfwTerminate();
    }
};

TEST_CASE_PERSISTENT_FIXTURE(MsaaWindowFixture, "glfw accepts default args"){

    WHEN("I ask for a 1920x1080 window with 4x msaa"){

        THEN("I should not get a null pointer"){
             REQUIRE( window != nullptr );
        }
        THEN("The window framebuffer must be 1920x1080"){
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            REQUIRE( width == 1920 ); 
            REQUIRE( height == 1080 ); 
        }
        THEN("The number of MSAA samples should be 4") {
            GLboolean msaaEnabled;
            glGetBooleanv(GL_MULTISAMPLE, &msaaEnabled);
            REQUIRE( msaaEnabled );

            GLint samples;
            glGetIntegerv(GL_SAMPLES, &samples);
            REQUIRE( samples == 4);
        }
    }
}

TEST_CASE("Window creation returns error codes") {
    WHEN("I ask for a 0x0 window"){
        auto windowError = sjd::createCoreWindow(0, 0, "test_window");
        THEN("I should get GFLW_ERROR::WINDOW"){
            REQUIRE( windowError.error() == sjd::GLFW_ERROR::WINDOW );
        }
    }
}
