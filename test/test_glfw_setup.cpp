#include <catch2/catch_test_macros.hpp>
#include <glfw_setup.h>

TEST_CASE( "glfw_setup functions are accessed" ) {
    REQUIRE( sjd::printHelloTest(true) == "Hello to my tests." );
    REQUIRE( sjd::printHelloTest(false) == "Absolutely not" );
}
