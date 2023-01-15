#pragma once

#include "MyString.h"

#include <stdio.h>


class FileManager 
{
public:
	FileManager(const char* filename);
	FileManager(const MyString&);
	FileManager();

	~FileManager();

	bool open(const char* filename);
	bool open(const MyString&);

	void open(const char* name, const char* mode);

	void close();
	bool is_open();

	MyString& read_all();
	void update(const MyString&);
	MyString& value();
	void save();
private:
	FILE* fd;
	MyString filename_;
	MyString data;
};

