#include "FileManager.h"

#include <curses.h>
#include <panel.h>
#include "TUI.h"
#include "Editor.h"

#include <cstdio>

int main()
{
    TUI curses_interface;
    Editor app;

    app.set_TUI(&curses_interface);
    curses_interface.set_application(&app);

    app.init(curses_interface.width(), curses_interface.height() - 2);

    NavigationMode navigationmode(&app);
    FindMode findmode(&app);
    WriteMode writemode(&app);
    CommandMode commandmode(&app);

    curses_interface.keyboard_loop();

    app.show();

    refresh();
    
}