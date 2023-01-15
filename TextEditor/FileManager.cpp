#include "FileManager.h"

void FileManager::open(const char* filename, const char* mode)
{
	if (is_open())
		close();

	filename_ = filename;
	fd = fopen(filename, mode);
}

bool FileManager::open(const char* filename)
{
	open(filename, "r");
	return is_open();
}

bool FileManager::open(const MyString& filename)
{
	open(filename.c_str(), "r");
	return is_open();
}

void FileManager::close()
{
	if (is_open())
		fclose(fd);
	fd = nullptr;
}

bool FileManager::is_open()
{
	return fd != nullptr;
}

FileManager::FileManager(const char* filename)
{
	open(filename);
}

FileManager::FileManager(const MyString& filename) : FileManager(filename.c_str())
{}

FileManager::FileManager()
	: fd(nullptr)
{}

FileManager::~FileManager()
{
	close();
}

MyString& FileManager::read_all()
{
	char buff[513];
	buff[512] = 0;
	while (fread(buff, 1, 512, fd) == 512)
	{
		data += buff;
	}

	data += buff;
	close();
	return data;
}

MyString& FileManager::value()
{
	return data;
}

void FileManager::update(const MyString& new_value)
{
	data = new_value;
	save();
}

void FileManager::save()
{
	open(filename_.c_str(), "w");

	fwrite(data.data(), 1, data.size(), fd);

	close();
}
