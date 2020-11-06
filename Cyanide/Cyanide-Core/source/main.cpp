// Cyanide.cpp

#include <iostream>

#include "core/application.h"
#include <stdlib.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#ifdef _DEBUG
struct after_main_check
{
    ~after_main_check()
    {
        _CrtDumpMemoryLeaks();
    }
};
#endif

int main(int argc, char* argv[])
{
#ifdef _DEBUG
    after_main_check memory;
#endif
    application* app = application::implement();
    if(app == nullptr)
        return -1;
    app->enter();
    app->run();
    app->exit();
    delete app;

    return 0;
}
