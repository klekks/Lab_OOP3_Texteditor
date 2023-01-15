#include "TUI.h"

#include <curses.h>
#include <panel.h>

#include <algorithm>

TUI::TUI() : app(nullptr)
{
    initscr();
    noecho();
    refresh();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(COLOR::BLACK, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR::WHITE, COLOR_WHITE, COLOR_BLACK);

    //this->base_window.resize();
    base_window = new CursesWindow();
}

void TUI::keyboard_loop()
{
    close_message = false;
    do
    {
        app->show();
    }
    while (!app->process_key(getch()) && !close_message);
}

void TUI::close()
{
    close_message = true;
}

void TUI::set_application(TUIApplication* app)
{
    this->app = app;
}

int TUI::width()
{
    return base_window->width();
}

int TUI::height()
{
    return base_window->height();
}

void TUI::hide_cursor()
{
    curs_set(0);
}

void TUI::show_cursor()
{
    curs_set(1);
}

void TUI::set_cursor(int x, int y)
{
    move(y, x);
    refresh();
}

void TUI::get_cursor(int& x, int& y)
{
    getsyx(y, x);
}

TUI::~TUI()
{
    delete base_window;
    endwin();
}

CursesWindow::CursesWindow() : win_(stdscr), x_(0), y_(0), parent_height_(0xFFFFFFFF), parent_width_(0xFFFFFFFF)
{
    getmaxyx(win_, height_, width_);
}

CursesWindow::CursesWindow(const CursesWindow& parent, int x, int y, int width, int height)
    : x_(x), y_(y), width_(width), height_(height), parent_width_(parent.width_), parent_height_(parent.height_)
{
    if (x_ + width > parent_width_)
        width_ = std::max(0, parent_width_ - x_);

    if (y_ + height > parent_height_)
        height_ = std::max(0, parent_height_ - y_);

    win_ = subwin(parent.win_, height_, width_, y_, x_);
}

void CursesWindow::moveto(int x, int y)
{
    while (y < 0)
        y += parent_height_;
    while (x < 0)
        x += parent_width_;


    int res = mvwin(win_, y, x);
    x_ = x;
    y_ = y;
}

void CursesWindow::show()
{
    wrefresh(win_);
}

void CursesWindow::clear()
{
    werase(win_);
}

int CursesWindow::width()
{
    if (width_ > 0)
        return width_;
    return 0;
}

int CursesWindow::height()
{
    if (height_ > 0)
        return height_;
    return 0;
}

void CursesWindow::insert(const MyString& value)
{
    waddstr(win_, value.c_str());
}

void CursesWindow::set_color(int color)
{
    wbkgd(win_, color);
}
