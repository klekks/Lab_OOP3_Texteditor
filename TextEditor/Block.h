#pragma once

#include "TUI.h"

#include "MyString.h"

extern class CursesWindow;

class ViewBlock
{
public:
	virtual void set_value(const MyString&) = 0;
	virtual void show() = 0;
	void moveto(int x, int y);
	void clear();
	int width();
	int height();
	void set_color(int color);
	~ViewBlock();
protected:
	CursesWindow* window_ = NULL;
	int color = 1;
};

class StringBlock : public ViewBlock
{
public:
	void show() override;
protected:
	MyString inner_text_;
};

class NamedBlock : public StringBlock
{
public:
	NamedBlock(const MyString&, int width);
	NamedBlock(const char*, int width);
	NamedBlock(int width);
	void set_value(const MyString&) override;
private:
	MyString header_;
};

class LineBlock : public StringBlock
{
public:
	LineBlock(int width);
	LineBlock(const MyString&, int width);
	LineBlock(const char*, int width);

	void set_value(const MyString&) override;
	void show() override;
	void centralize_value();
private:
};

class PageBlock : public StringBlock
{
public:
	PageBlock(int width, int height);
	void set_value(const MyString&) override;
	void update_pagination();
	void next_page();
	void prev_page();
	int get_page() const;

	void fit_cursor_by_global(int& row, int& column);
	void fit_cursor_by_page(int& row, int& column);
private:
	const MyString* value_;
	int page_;
};

