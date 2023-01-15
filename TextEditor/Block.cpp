#include "Block.h"

void ViewBlock::set_color(int color)
{
    window_->set_color(COLOR_PAIR(color));
    this->color = color;
}

ViewBlock::~ViewBlock()
{
    if (window_)
        delete window_;
    window_ = NULL;
}

void ViewBlock::moveto(int x, int y)
{
    window_->moveto(x, y);
}

void ViewBlock::clear()
{
    window_->clear();
}

int ViewBlock::width()
{
    return window_->width();
}

int ViewBlock::height()
{
    return window_->height();
}

NamedBlock::NamedBlock(const MyString& data, int width) : NamedBlock(data.c_str(), width)
{}

NamedBlock::NamedBlock(const char* data, int width) : NamedBlock(width)
{
    inner_text_ = data;
    header_ = data;
}

NamedBlock::NamedBlock(int width)
{
    CursesWindow base_window;
    window_ = new CursesWindow(base_window, 0, 0, width, 1);
}

void NamedBlock::set_value(const MyString& value)
{
    inner_text_ = header_;
    inner_text_ += ": ";
    inner_text_ += value;
}

void StringBlock::show()
{
    window_->clear();
    window_->insert(inner_text_);
    window_->show();
}

LineBlock::LineBlock(int width)
{
    CursesWindow base_window;
    window_ = new CursesWindow(base_window, 0, 0, width, 1);
}

LineBlock::LineBlock(const MyString& data, int width) : LineBlock(data.c_str(), width)
{}

LineBlock::LineBlock(const char* data, int width) : LineBlock(width)
{
    inner_text_ = data;
}

void LineBlock::centralize_value()
{
    MyString spaces((width() - inner_text_.length()) / 2, ' ');
    inner_text_ = spaces + inner_text_ + spaces;
}

void LineBlock::set_value(const MyString& value)
{
    inner_text_ = value;
}

void LineBlock::show()
{
    centralize_value();
    StringBlock::show();
}

PageBlock::PageBlock(int width, int height)
    : page_(1), value_(nullptr)
{
    CursesWindow base_window;
    window_ = new CursesWindow(base_window, 0, 0, width, height);
}

void PageBlock::set_value(const MyString& value)
{
    value_ = &value;
    update_pagination();
}

// finds start and end of $page_
// puts text of $page_ into $inner_text_
void PageBlock::update_pagination()
{
    int N = page_;
    int l = 0, r = 0;
    int w = width(), h = height();
    int idx = 0;
    while (N--)
    {
        l = r;
        int hc = h;
        while (hc > 0)
        {
            idx = value_->find_offset('\n', r);
            if (idx == -1)
            {
                prev_page();
                return;
            }

            hc -= idx / w;
            hc -= idx % w ? 1 : 0;

            r += idx + 1;
        }

        if (hc < 0)
        {
            r += (hc + 1) * w;
            r -= idx % w;
        }
    }

    try
    {
        inner_text_ = value_->substr(l, r - l);
    }
    catch (const std::exception& ex)
    {
        prev_page();
    }
}

void PageBlock::next_page()
{
    page_++;
    update_pagination();
}

void PageBlock::prev_page()
{
    if (page_ > 1)
        page_--;
    update_pagination();
}

int PageBlock::get_page() const
{
    return page_;
}

void PageBlock::fit_cursor_by_global(int& row, int& column)
{
    fit_cursor_by_page(row, column);
}

void PageBlock::fit_cursor_by_page(int& row, int& column)
{
    int w = width();
    int row_ = row, column_ = column;
    int offset = 0;
    int idx = inner_text_.find("\n", offset);
    while (row_)
    {
        idx = inner_text_.find("\n", offset);

        offset += idx;

        for (; idx > w && row_; idx -= w, row_--);

        if (idx && row_) row_--, idx = 0;

    }
    column = std::min(column, idx);
}
