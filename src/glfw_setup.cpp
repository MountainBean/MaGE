#include <glfw_setup.h>

namespace sjd {

auto createCursorLockedWindow(uint32_t windowWidth,
                              uint32_t windowHeight,
                              const std::string& windowName,
                              uint16_t msaaBuffers) -> GLFWwindow*{
    initializeGlfw(OPENGL_MAJOR_VERSION, OPENGL_MINOR_VERSION);
    setMultiSampling(msaaBuffers);

    e<GLFWwindow*> window = createCoreWindow(windowWidth,
                                             windowHeight,
                                             windowName);
    if (window.has_value()){
        configureViewPort(*window, windowWidth, windowHeight, msaaBuffers);
    }
    else {
        switch (window.error()){
        case GLFW_ERROR::WINDOW:
            std::cout << "Failed to create GLFW window.\n";
            break;
        case GLFW_ERROR::GLAD:
            std::cout << "Failed to initialised GLAD.\n";
            break;
        default:
            std::cout << "Unknown GFLW Error.\n";
            break;
        }
    }
    return *window;
}

auto createCoreWindow(uint32_t windowWidth,
                      uint32_t windowHeight,
                      const std::string& windowName) -> e<GLFWwindow*> {

    GLFWwindow* window = glfwCreateWindow(windowWidth, 
                            windowHeight, 
                            windowName.c_str(), 
                            NULL, 
                            NULL
    );
    if (!window) {
        glfwTerminate();
        return std::unexpected(GLFW_ERROR::WINDOW);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        return std::unexpected(GLFW_ERROR::GLAD);
    }
    return window;
}

void configureViewPort(GLFWwindow* window,
                       uint32_t windowWidth,
                       uint32_t windowHeight,
                       uint16_t msaaBuffers) {
    glViewport(0, 0, windowWidth, windowHeight);

    if (msaaBuffers > 1) {
        glEnable(GL_MULTISAMPLE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window,
                                   []([[maybe_unused]] GLFWwindow* window,
                                      int width,
                                      int height){
                                          glViewport(0, 0, width, height);
                                      }
    );
}

void initializeGlfw(uint16_t MajorOpenGLVersion, uint16_t MinorOpenGLVersion) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorOpenGLVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorOpenGLVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

void setMultiSampling(uint16_t buffers){
    if (buffers > 1 && buffers <= 4) {
        glfwWindowHint(GLFW_SAMPLES, buffers);
    }
}

}
