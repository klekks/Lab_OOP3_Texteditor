#pragma once

#include "TUI.h"
#include "ModeController.h"
#include "FileManager.h"

class Editor : public ControllableApplication
{
public:
	Editor();
	void init(int width, int height);
	void show() override;
	bool process_key(char key) override;

	void set_column(int n) override;
	void set_row(int n) override;
	ModeController* set_active_mode(char key) override;
	ModeController* set_active_mode(const char* key) override;
	ModeController* set_active_mode(ModeController*) override;

	void set_input_block(const MyString&);

	void hide_cursor() override;
	void show_cursor() override;
	void move_cursor(int x, int y) override;
	void get_cursor(int& x, int& y) override;

	void add_mode_controller(ModeController*) override;

	void exit() override;

	void open_file(const MyString& filename);
	void save_to_file(const MyString& filename);

	~Editor();
private:
	void update_cursor_title();

	LineBlock* mode_block, * filename_block;
	NamedBlock* columns_block, * rows_block, *input_block;
	PageBlock* body_area;

	std::vector<ViewBlock*> blocks;

	MyString body;
	FileManager file_manager;

	ModeController* active_mode = nullptr;
	std::vector<ModeController*> mods;

	int cursor_x = 0, cursor_y = 0;
};

