#include "acid.hpp"

int main() 
{
    acid::Ref<acid::Window> window = acid::Window::Create({});

    while (1) {
        window->OnUpdate();
    }
}
