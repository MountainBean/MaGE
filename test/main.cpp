#include <glfw_setup.h>

namespace globals {
    constexpr uint32_t windowWidth {1200};
    constexpr uint32_t windowHeight {900};
}

int main(void) {
    // INIT WINDOW
    GLFWwindow* window {sjd::createCursorLockedWindow(globals::windowWidth,
                                                      globals::windowHeight,
                                                      "MaGE window")};
    while(!glfwWindowShouldClose(window)) {
        // End Frame Processing
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
