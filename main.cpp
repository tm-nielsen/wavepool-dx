#include "raylib.h"

#ifdef PLATFORM_WEB
#include "application_wrappers/web_application.cpp"
#else
#include "application_wrappers/desktop_application.cpp"
#endif

using namespace application_wrappers;

int main(void)
{
#ifdef PLATFORM_WEB
    WebApplication application = WebApplication();
    application.InitializeApplication();
#else
    DesktopApplication application = DesktopApplication();
    application.InitializeApplication();
#endif

    // Main Game Loop
    while(!WindowShouldClose())
    {
        application.Update();
        if (application.shouldExit) break;
        application.Draw();
    }
    // De-Initialization
    application.EndApplication();
    return 0;
}