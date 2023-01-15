#pragma once

#include "MyString.h"
#include "TUI.h"


extern class ModeController;


class ControllableApplication : public TUIApplication
{
public:
	virtual void set_column(int n) = 0;
	virtual void set_row(int n) = 0;
	virtual ModeController* set_active_mode(ModeController*) = 0;
	virtual ModeController* set_active_mode(char key) = 0;
	virtual ModeController* set_active_mode(const char* key) = 0;
	virtual void set_input_block(const MyString&) = 0;

	virtual void add_mode_controller(ModeController*) = 0;

	virtual void exit() = 0;

	virtual void open_file(const MyString& filename) = 0;
	virtual void save_to_file(const MyString& filename) = 0;
};

class ModeController
{
public:
	virtual bool process_key(char key) = 0;
	virtual void grab_control() = 0;
	virtual void escape() = 0;
	virtual void enter() = 0;
	virtual void backspace() = 0;
	virtual bool has_control_key(char key) = 0;
	virtual bool has_control_key(const char* key) = 0;
	virtual const MyString& name() { return name_; };
	void set_additional_mode(MyString& value) { mode = value; }
protected:
	ControllableApplication* app_;
	MyString name_;
	MyString mode;
};

class NavigationMode : public ModeController
{
public:
	NavigationMode(ControllableApplication*);
	bool process_key(char key) override;
	void grab_control() override;
	void escape() override;
	void enter() override;
	void backspace() override;

	bool has_control_key(char key) override;
	bool has_control_key(const char* key) override;
private:
	MyString command;
};

class FindMode : public ModeController
{
public:
	FindMode(ControllableApplication*);
	bool process_key(char key);
	void grab_control();
	void escape() override;
	void enter() override;
	void backspace() override;

	bool has_control_key(char key) override;
	bool has_control_key(const char* key) override;
private:
	MyString command;
};

class WriteMode : public ModeController
{
public:
	WriteMode(ControllableApplication*);
	bool process_key(char key);
	void grab_control();
	void escape() override;
	void enter() override;
	void backspace() override;

	bool has_control_key(char key) override;
	bool has_control_key(const char* key) override;
};

class CommandMode : public ModeController
{
public:
	CommandMode(ControllableApplication*);
	bool process_key(char key);
	void grab_control();
	void escape() override;
	void enter() override;
	void backspace() override;

	bool has_control_key(char key) override;
	bool has_control_key(const char* key) override;
private:
	MyString command;
};
