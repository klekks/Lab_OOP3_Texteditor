#include "ModeController.h"

NavigationMode::NavigationMode(ControllableApplication* app)
{
	app_ = app;
	name_ = "--NAVIGATION--";
	app_->add_mode_controller(this);
}

void NavigationMode::escape()
{}

void NavigationMode::enter()
{
	ModeController *next_mode = app_->set_active_mode(command.c_str());

	if (next_mode)
		next_mode->set_additional_mode(command);

	command.clear();
	app_->set_input_block(command.c_str());
}

void NavigationMode::backspace()
{
	if (command.empty()) return;

	command.erase(command.size() - 1, 1);
	app_->set_input_block(command);
}

void NavigationMode::grab_control()
{
	app_->hide_cursor();
	app_->set_input_block("");
	command.clear();
}

bool NavigationMode::process_key(char key)
{
	command += MyString(1, key);
	app_->set_input_block(command);
	enter();
	return false;
}

bool NavigationMode::has_control_key(char key)
{
	return key == SpecialKey::ESC;
}

bool NavigationMode::has_control_key(const char* key)
{
	return has_control_key(key[0]);
}

FindMode::FindMode(ControllableApplication* app)
{
	app_ = app;
	name_ = "--FIND--";
	app_->add_mode_controller(this);
}

void FindMode::grab_control()
{
	app_->hide_cursor();
	app_->set_input_block("");
	command.clear();
}

void FindMode::backspace()
{
	if (command.empty()) return;

	command.erase(command.size() - 1, 1);
	app_->set_input_block(command);
}

void FindMode::enter()
{

}

void FindMode::escape()
{
	app_->set_active_mode(SpecialKey::ESC);
}

bool FindMode::has_control_key(char key)
{
	return key == '/' || key == '?';
}

bool FindMode::has_control_key(const char* key)
{
	return !strcmp("/", key) || !strcmp("?", key);
}

bool FindMode::process_key(char key)
{
	command += MyString(1, key);
	app_->set_input_block(command);

	return false;
}

WriteMode::WriteMode(ControllableApplication* app)
{
	app_ = app;
	name_ = "--WRITE--";
	app_->add_mode_controller(this);
}

bool WriteMode::process_key(char key)
{
	return false;
}

void WriteMode::grab_control()
{
	app_->show_cursor();
	app_->move_cursor(0, 0);
}

void WriteMode::escape()
{
	app_->set_active_mode(SpecialKey::ESC);
}

void WriteMode::enter()
{
	process_key('\n');
}

void WriteMode::backspace()
{

}

bool WriteMode::has_control_key(char key)
{
	return strchr("iISAr", key);
}

bool WriteMode::has_control_key(const char* key)
{
	return !key[1] && has_control_key(key[0]);
}

CommandMode::CommandMode(ControllableApplication* app)
{
	app_ = app;
	name_ = "--COMMAND--";
	app_->add_mode_controller(this);
}

bool CommandMode::process_key(char key)
{
	command += MyString(1, key);
	app_->set_input_block(command);
	return false;
}

void CommandMode::grab_control()
{
	app_->hide_cursor();
	app_->set_input_block("");
	command.clear();
}

void CommandMode::escape()
{
	app_->set_active_mode(SpecialKey::ESC);
}

void CommandMode::enter()
{
	if (command == "q" || command == "!q")
		app_->exit();

	if (command.find("o ") == 0)
		app_->open_file(command.substr(2));

	command.clear();
	app_->set_input_block(command.c_str());
}

void CommandMode::backspace()
{
	if (command.empty()) return;

	command.erase(command.size() - 1, 1);
	app_->set_input_block(command);
}

bool CommandMode::has_control_key(char key)
{
	return key == ':';
}

bool CommandMode::has_control_key(const char* key)
{
	return !strcmp(":", key);
}