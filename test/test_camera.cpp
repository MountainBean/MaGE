#include "glm/geometric.hpp"
#include "glm_to_string.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <camera.h>
#include <glm/gtc/epsilon.hpp>

const float FLOAT_EPSILON {0.0001};

TEST_CASE("camera is initialised with default state"){

    WHEN("I initialise a new camera"){
        sjd::Camera testCamera{};

        THEN("the camera position should be at [0,0,3]"){
            glm::vec3 pos = testCamera.getPos();
            CHECK( pos == glm::vec3{0.0f, 0.0f, 3.0f} );
        }
        THEN("the camera will be facing the origin"){
            glm::vec3 dirOfOrigin {0.0f, 0.0f, -1.0f};
            INFO("Checking if " << testCamera.getFace() << " == " << dirOfOrigin );
            CHECK( glm::all(glm::epsilonEqual(testCamera.getFace(),
                                              dirOfOrigin,
                                              FLOAT_EPSILON)));
        }

    }
}

TEST_CASE("camera is initialised to look at its own position"){
    sjd::Camera testCamera{glm::vec3(0.0f)};
    INFO("getFace(): " << testCamera.getFace() << 
         "\ndefault: " << glm::vec3(0.0f, 0.0f, -1.0f));
    CHECK( glm::all(glm::epsilonEqual(testCamera.getFace(),
                                      glm::vec3(0.0f, 0.0f, -1.0f),
                                      FLOAT_EPSILON)));
}

TEST_CASE("Camera is initialised with a given state"){

    WHEN("I initialise a new camera at a location, facing a direction"){
        glm::vec3 startingPos = glm::vec3{0.0f, 0.0f, 0.0f};
        glm::vec3 startingFace = glm::vec3{1.0f, 0.0f, 0.0f};
        sjd::Camera testCamera{
            startingPos,
            startingPos + startingFace
        };

        THEN("the camera position should be at the give starting point"){
            glm::vec3 pos = testCamera.getPos();
            CHECK( pos == startingPos );
        }
        THEN("the camera will be facing the given starting face direction"){
            CHECK( testCamera.getFace() == startingFace );
        }

    }
}

TEST_CASE("A camera moves with respect to it's facing direction (front)"){
    auto facing = GENERATE(as<glm::vec3>{},
                           glm::vec3{0.0f, 0.0f, -1.0f},
                           glm::vec3{0.0f, 0.0f, 1.0f},
                           glm::vec3{-1.0f, 0.0f, 0.0f},
                           glm::vec3{1.0f, 0.0f, 0.0f});
    // 3.0f is arbitrary here: a camera at some coordinate:
    auto posX = GENERATE( take(1, random(-3.0f, 3.0f)) );
    auto posY = GENERATE( take(1, random(-3.0f, 3.0f)) );
    auto posZ = GENERATE( take(1, random(-3.0f, 3.0f)) );
    glm::vec3 position {posX, posY, posZ};

    sjd::Camera testCamera{position, position + facing};

    GIVEN ("The camera correctly reports its front"){

        INFO("getFace(): " << testCamera.getFace() << 
             "\nfacing: " << facing);
        CHECK( glm::all(glm::epsilonEqual(testCamera.getFace(),
                                          facing,
                                          FLOAT_EPSILON)));

        WHEN("The Camera moves forward"){
            testCamera.processMovement(sjd::Camera::Movement::FORWARD,
                                       1.0f);
            THEN("The Camera moved towards the facing direction"){
                glm::vec3 newPos = testCamera.getPos();
                glm::vec3 dirMoved = glm::normalize(newPos - position);

                INFO("dirMoved: " << dirMoved << 
                     "\nfacing: " << facing);
                CHECK(glm::all(glm::epsilonEqual(dirMoved,
                                                 facing,
                                                 FLOAT_EPSILON)));
            }
        }

        WHEN("The Camera moves backward"){
            testCamera.processMovement(sjd::Camera::Movement::BACKWARD,
                                       1.0f);
            THEN("The Camera moved away from the facing direction"){
                glm::vec3 newPos = testCamera.getPos();
                glm::vec3 dirMoved = glm::normalize(newPos - position);

                INFO("dirMoved: " << dirMoved << 
                     "\nbackwards-facing: " << -facing);
                CHECK(glm::all(glm::epsilonEqual(dirMoved,
                                                 -facing,
                                                 FLOAT_EPSILON)));
            }
        }

        WHEN("The Camera moves to its right"){
            testCamera.processMovement(sjd::Camera::Movement::RIGHT,
                                       1.0f);
            THEN("The Camera moved to the right of the facing direction"){
                glm::vec3 newPos = testCamera.getPos();
                glm::vec3 dirMoved = glm::normalize(newPos - position);
                glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f};
                glm::vec3 right = glm::cross(facing, up);

                INFO("dirMoved: " << dirMoved << 
                     "\nright-facing: " << right);
                CHECK(glm::all(glm::epsilonEqual(dirMoved,
                                                 right,
                                                 FLOAT_EPSILON)));
            }
        }

        WHEN("The Camera moves to its left"){
            testCamera.processMovement(sjd::Camera::Movement::LEFT,
                                       1.0f);
            THEN("The Camera moved to the right of the facing direction"){
                glm::vec3 newPos = testCamera.getPos();
                glm::vec3 dirMoved = glm::normalize(newPos - position);
                glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f};
                glm::vec3 left = -glm::cross(facing, up);

                INFO("dirMoved: " << dirMoved << 
                     "\nleft-facing: " << left);
                CHECK(glm::all(glm::epsilonEqual(dirMoved,
                                                 left,
                                                 FLOAT_EPSILON)));
            }
        }
    }
}

TEST_CASE("A camera moves directly up and down regardless of its direction"){
    auto facing = GENERATE(as<glm::vec3>{},
                           glm::vec3{0.0f, 0.0f, -1.0f},
                           glm::vec3{0.0f, 0.0f, 1.0f},
                           glm::vec3{-1.0f, 0.0f, 0.0f},
                           glm::vec3{1.0f, 0.0f, 0.0f});
    // 3.0f is arbitrary here: a camera at some coordinate:
    glm::vec3 position {0.0f};
    sjd::Camera testCamera{position, position + facing};
    WHEN("A camera facing any direction moves up"){
        testCamera.processMovement(sjd::Camera::Movement::UP, 1.0f);
        THEN("The camera will move directly up the y-axis"){
            glm::vec3 newPos = testCamera.getPos();
            glm::vec3 dirMoved = glm::normalize(newPos - position);
            glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f};
            INFO("dirMoved: " << dirMoved << 
                 "\nup: " << up);
            CHECK(glm::all(glm::epsilonEqual(dirMoved,
                                             up,
                                             FLOAT_EPSILON)));

        }
    }

    WHEN("A camera facing any direction moves down"){
        testCamera.processMovement(sjd::Camera::Movement::DOWN, 1.0f);
        THEN("The camera will move directly up the y-axis"){
            glm::vec3 newPos = testCamera.getPos();
            glm::vec3 dirMoved = glm::normalize(newPos - position);
            glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f};
            INFO("dirMoved: " << dirMoved << 
                 "\ndown: " << -up);
            CHECK(glm::all(glm::epsilonEqual(dirMoved,
                                             -up,
                                             FLOAT_EPSILON)));

        }
    }
}

TEST_CASE("A Camera will turn to face a given direction"){
    sjd::Camera testCamera{};
    GIVEN("A default-state camera at [0,0,3] facing the origin"){
        WHEN("I turn the camera to look at a point"){
            auto rands = GENERATE( take(4, chunk(3, random(-50.0f, 50.0f))));
            glm::vec3 point {rands[0], rands[1], rands[2]};
            glm::vec3 oldFace {testCamera.getFace()};
            testCamera.turnTo(point);
            CHECKED_IF(glm::all(glm::epsilonNotEqual(point,
                                                     testCamera.getPos(),
                                                     FLOAT_EPSILON))){
                THEN("The camera will face that point"){
                    glm::vec3 dirToPoint {glm::normalize(point - testCamera.getPos())};
                    INFO("dirToPoint: " << dirToPoint <<
                         "\nfacing: " << testCamera.getFace());
                    CHECK(glm::all(glm::epsilonEqual(dirToPoint,
                                                     testCamera.getFace(),
                                                     FLOAT_EPSILON)));
                }
            } CHECKED_ELSE(glm::all(glm::epsilonNotEqual(point,
                                                     testCamera.getPos(),
                                                     FLOAT_EPSILON))){
                THEN("The camera will not change direction"){
                    INFO("oldFace: " << oldFace <<
                         "\nfacing: " << testCamera.getFace());
                    CHECK(glm::all(glm::epsilonEqual(oldFace,
                                                     testCamera.getFace(),
                                                     FLOAT_EPSILON)));
                }

            }

        }
    }
}

