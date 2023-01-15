#pragma once

#include "MyString.h"
#include <vector>

#include <curses.h>

#include "Block.h"

enum COLOR
{
	WHITE = 1,
	BLACK = 2
};

enum SpecialKey
{
	ESC=27,
	ENTER=10,
	BACKSPACE=8,
	DELETE=74,
	KEY_PREV_PAGE=-61,
	KEY_NEXT_PAGE=-55,
	UP = 3,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 5
};


class CursesWindow
{
public:
	CursesWindow(const CursesWindow& parent, int x, int y, int width, int height);
	CursesWindow();

	void moveto(int x, int y);
	void show();
	void clear();
	void insert(const MyString& value);
	int width();
	int height();
	void set_color(int color);
private:
	WINDOW* win_ = NULL;
	int x_, y_, width_, height_;
	int parent_width_, parent_height_;
};

extern class TUI;

class TUIApplication
{
public:
	virtual bool process_key(char key) = 0;
	virtual void show() = 0;

	virtual void hide_cursor() = 0;
	virtual void show_cursor() = 0;
	virtual void move_cursor(int x, int y) = 0;
	virtual void get_cursor(int& x, int& y) = 0;

	void set_TUI(TUI* tui) { application = tui; };
protected:
	TUI* application;
};


class TUI
{
public:
	TUI();
	void set_application(TUIApplication* app);
	int width();
	int height();
	void keyboard_loop();

	void hide_cursor();
	void show_cursor();
	void set_cursor(int x, int y);
	void get_cursor(int& x, int& y);

	void close();

	~TUI();
private:
	TUIApplication* app;
	CursesWindow* base_window;
	bool close_message = false;
};

