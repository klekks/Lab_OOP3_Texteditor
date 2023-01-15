#include "Editor.h"

Editor::Editor()
{
	mode_block = new LineBlock("--NORMAL--", 30);
	filename_block = new LineBlock("--NOFILE--", 30);

	columns_block = new NamedBlock("Col", 15);
	rows_block = new NamedBlock("Row", 15);
	input_block = new NamedBlock("~#", 150);

	body_area = NULL;

	mode_block->moveto(0, -1);
	filename_block->moveto(mode_block->width(), -1);
	columns_block->moveto(mode_block->width() + filename_block->width(), -1);
	rows_block->moveto(mode_block->width() + filename_block->width() + columns_block->width(), -1);
	input_block->moveto(0, -2);

	input_block->set_color(COLOR::BLACK);
}

void Editor::add_mode_controller(ModeController* controller)
{
	mods.push_back(controller);

	if (active_mode == nullptr)
		set_active_mode(controller);
}

void Editor::init(int width, int height)
{
	body_area = new PageBlock(width, height);
	blocks.push_back(mode_block);
	blocks.push_back(filename_block);
	blocks.push_back(columns_block);
	blocks.push_back(rows_block);
	blocks.push_back(input_block);
	blocks.push_back(body_area);
}

void Editor::show()
{
	for (const auto& block : blocks)
		block->show();
}

Editor::~Editor()
{
	for (const auto& block : blocks)
		delete block;
}

bool Editor::process_key(char key)
{
	//std::cout << "!!!process_key!!!" << " " << int(key) << std::endl;
	switch (key)
	{
	case SpecialKey::ESC:
		active_mode->escape();
		break;
	case SpecialKey::ENTER:
		active_mode->enter();
		break;
	case SpecialKey::BACKSPACE:
		active_mode->backspace();
		break;
	case SpecialKey::KEY_PREV_PAGE: // -55
		body_area->prev_page();
		break;
	case SpecialKey::KEY_NEXT_PAGE: // - 61
		body_area->next_page();
		break;
	case SpecialKey::UP:
		move_cursor(cursor_x, cursor_y - 1);
		break;
	case SpecialKey::DOWN:
		move_cursor(cursor_x, cursor_y + 1);
		break;
	case SpecialKey::LEFT:
		move_cursor(cursor_x - 1, cursor_y);
		break;
	case SpecialKey::RIGHT:
		move_cursor(cursor_x + 1, cursor_y);
		break;
	default:
		return active_mode->process_key(key);
	}
	return false;
}

ModeController* Editor::set_active_mode(ModeController* controller)
{
	active_mode = controller;
	active_mode->grab_control();
	mode_block->set_value(active_mode->name());

	return controller;
}


ModeController* Editor::set_active_mode(char key)
{
	for (const auto& mode : mods)
	{
		if (mode->has_control_key(key))
		{
			set_active_mode(mode);
			return mode;
		}
	}
	return nullptr;
}

ModeController* Editor::set_active_mode(const char *key)
{
	for (const auto& mode : mods)
	{
		if (mode->has_control_key(key))
		{
			set_active_mode(mode);
			return mode;
		}
	}
	return nullptr;
}

void Editor::set_column(int column)
{
	char buff[16];
	itoa(column, buff, 10);
	columns_block->set_value(buff);
}

void Editor::set_row(int column)
{
	char buff[16];
	itoa(column, buff, 10);
	rows_block->set_value(buff);
}

void Editor::set_input_block(const MyString& value)
{
	input_block->set_value(value);
}

void Editor::hide_cursor()
{
	application->hide_cursor();
}

void Editor::show_cursor()
{
	application->show_cursor();
}

void Editor::move_cursor(int x, int y)
{
	while (x < 0)
	{
		y--;
		x += body_area->width();
	}

	while (x >= body_area->width())
	{
		x -= body_area->width();
		y++;
	}

	while (y < 0)
	{
		body_area->prev_page();
		y += body_area->height();
	}

	while (y >= body_area->height())
	{
		y -= body_area->height();
		body_area->next_page();
	}
	
	body_area->fit_cursor_by_page(y, x);

	cursor_x = x;
	cursor_y = y;

	application->set_cursor(cursor_x, cursor_y);
	update_cursor_title();
}

void Editor::update_cursor_title()
{
	char name[16];
	itoa(cursor_x, name, 10);
	rows_block->set_value(name);
	itoa(cursor_y, name, 10);
	columns_block->set_value(name);
}


void Editor::get_cursor(int& x, int& y)
{
	application->get_cursor(x, y);
}

void Editor::exit()
{
	application->close();
}

void Editor::open_file(const MyString& filename)
{
	file_manager.open(filename);
	body = file_manager.read_all();
	body_area->set_value(body);
}

void Editor::save_to_file(const MyString& filename)
{

}
