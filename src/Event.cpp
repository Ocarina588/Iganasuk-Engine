#include "Event.hpp"
#include "Context.hpp"

namespace ig = iganasuk;

ig::EventManager::EventManager(void)
{

}

ig::EventManager::~EventManager(void)
{

}

void ig::EventManager::process_key(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W) W = true;
        if (key == GLFW_KEY_A) A = true;
        if (key == GLFW_KEY_S) S = true;
        if (key == GLFW_KEY_D) D = true;
        if (key == GLFW_KEY_E) E = true;
        if (key == GLFW_KEY_Q) Q = true;
        if (key == GLFW_KEY_UP) u = true;
        if (key == GLFW_KEY_DOWN) d = true;
        if (key == GLFW_KEY_LEFT) l = true;
        if (key == GLFW_KEY_RIGHT) r = true;
    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W) W = false;
        if (key == GLFW_KEY_A) A = false;
        if (key == GLFW_KEY_S) S = false;
        if (key == GLFW_KEY_D) D = false;
        if (key == GLFW_KEY_E) E = false;
        if (key == GLFW_KEY_Q) Q = false;
        if (key == GLFW_KEY_UP) u = false;
        if (key == GLFW_KEY_DOWN) d = false;
        if (key == GLFW_KEY_LEFT) l = false;
        if (key == GLFW_KEY_RIGHT) r = false;
    }
}

void ig::EventManager::process_mouse(int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (mouse_clicked == false)
            glfwGetCursorPos(ig_window, &last_pos.x, &last_pos.y);

        glfwGetCursorPos(ig_window, &pos.x, &pos.y);

        mouse_offset = pos - last_pos;
        pos = last_pos;

        mouse_clicked = true;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouse_clicked = false;
        mouse_offset = { 0, 0 };
    }
}

void ig::EventManager::process_scroll(double xoffset, double yoffset)
{
    if (yoffset == 0) mouse_scroll = {};
    else mouse_scroll = { 0, yoffset };
}