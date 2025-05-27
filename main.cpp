#include "App.h"


int main()
{
    using namespace gs;

    App &app = App::getInstance();

    app.run();

    return 0;
}
