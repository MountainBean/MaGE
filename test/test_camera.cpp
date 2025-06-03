#include <catch2/catch_test_macros.hpp>
#include <camera.h>

TEST_CASE("camera is initialised with default state"){

    WHEN("I initialise a new camera"){
        sjd::Camera testCamera{};

        THEN("the camera position should be at [0,0,3]"){
            glm::vec3 pos = testCamera.getPos();
            CHECK( pos == glm::vec3{0.0f, 0.0f, 3.0f} );
        }
        THEN("the camera will be facing the origin"){
            // moving forward will only decrease pos.z
        }

    }
}

