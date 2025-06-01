#ifndef GLFW_SETUP_H
#define GLFW_SETUP_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <expected>

namespace sjd {

const uint16_t OPENGL_MAJOR_VERSION = 3;
const uint16_t OPENGL_MINOR_VERSION = 3;

enum class GLFW_ERROR {
    WINDOW,
    GLAD
};

template <typename T>
using e = std::expected<T, GLFW_ERROR>;

auto createCursorLockedWindow(uint32_t windowWidth,
                      uint32_t windowHeight,
                      const std::string& windowName,
                      uint16_t msaaBuffers = 1) -> GLFWwindow*;

std::string printHelloTest(bool todo);

auto createCoreWindow(uint32_t windowWidth,
                             uint32_t windowHeight,
                             const std::string& windowName) -> e<GLFWwindow*>;

void initializeGlfw(uint16_t MajorOpenGLVersion, uint16_t MinorOpenGLVersion);
void setMultiSampling(uint16_t buffers);
void configureViewPort(GLFWwindow* window,
                       uint32_t windowWidth,
                       uint32_t windowHeight,
                       uint16_t msaaBuffers);

}
#endif
