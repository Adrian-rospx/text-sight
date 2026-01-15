#include "app/app_controller.hpp"
#include "app/app_state.hpp"

using namespace ftxui;

int main(int argc, char* argv[]) {
    AppState state {};

    AppController controller(state);
    controller.run();
}