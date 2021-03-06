#include "Mouse.h"

namespace Altseed {

std::shared_ptr<Mouse> Mouse::instance = nullptr;

static double wheelTemp = 0;
static bool wheelCalled = false;
std::function<void(double x, double y)> wheelCallback;

static void GetWheelInternal(GLFWwindow* wHandle, double x, double y) {
    wheelTemp = y;
    wheelCalled = true;

    if (wheelCallback != nullptr) {
        wheelCallback(x, y);
    }
}

bool Mouse::Initialize(std::shared_ptr<Window>& window) {
    instance = MakeAsdShared<Mouse>();

    instance->window = window;

    instance->currentState.fill(false);
    instance->oldState.fill(false);

    glfwSetScrollCallback((GLFWwindow*)window->GetNativeWindow(), GetWheelInternal);

    return true;
}

std::shared_ptr<Mouse>& Mouse::GetInstance() { return instance; }

void Mouse::RefreshInputState() {
    auto w = (GLFWwindow*)window->GetNativeWindow();

    wheel = (wheelCalled) ? wheelTemp : 0;
    wheelCalled = false;

    oldState = currentState;

    for (int32_t i = 0; i < KEY_NUM; i++) {
        currentState[i] = (bool)glfwGetMouseButton(w, i);
    }
}

void Mouse::SetPosition(Vector2DF vec) {
    auto w = (GLFWwindow*)window->GetNativeWindow();
    glfwSetCursorPos(w, vec.X, vec.Y);
}

Vector2DF Mouse::GetPosition() {
    double x, y;
    auto w = (GLFWwindow*)window->GetNativeWindow();
    glfwGetCursorPos(w, &x, &y);
    Vector2DF* vec = new Vector2DF(x, y);
    return *vec;
}

void Mouse::SetWheelCallback(std::function<void(double x, double y)> func) { wheelCallback = func; }

float Mouse::GetWheel() const { return wheel; }

ButtonState Mouse::GetMouseButtonState(MouseButtons button) const {
    int index = (int32_t)button;
    return static_cast<ButtonState>((currentState[index] ? 1 : 0) | (oldState[index] ? 2 : 0));
}

void Mouse::SetCursorMode(CursorMode mode) {
    auto w = (GLFWwindow*)window->GetNativeWindow();
    int index = (int32_t)mode;
    glfwSetInputMode(w, GLFW_CURSOR, index);
}

CursorMode Mouse::GetCursorMode() const {
    auto w = (GLFWwindow*)window->GetNativeWindow();
    int index = (int32_t)glfwGetInputMode(w, GLFW_CURSOR);
    return static_cast<CursorMode>(index);
}

};  // namespace Altseed
