#pragma once

#include <string>
#include <inttypes.h>
#include <iostream>

class MyString
{
public:
	MyString();
	MyString(const std::initializer_list<char>);
	MyString(const char* str);
	MyString(const std::string);
	MyString(const char*, size_t);
	MyString(size_t, char);
	MyString(const MyString&);
	~MyString();

	friend MyString operator+(const MyString&, const MyString&);
	MyString operator+ (const char* str) const;
	MyString operator+ (const std::string) const;

	MyString operator+= (const MyString&);
	MyString operator+= (const char*);
	MyString operator+= (const std::string);

	MyString operator= (const MyString&);
	MyString operator= (char);
	MyString operator= (const char*);
	MyString operator= (const std::string);

	char operator[](int) const;
	bool operator>(const MyString&) const;
	bool operator>=(const MyString&) const;
	bool operator<(const MyString&) const;
	bool operator<=(const MyString&) const;
	bool operator!=(const MyString&) const;
	bool operator==(const MyString&) const;

	friend std::istream& operator>>(std::istream&, MyString&);
	friend std::ostream& operator<<(std::ostream&, const MyString&);

	void append(const MyString&);
	void append(size_t, char);
	void append(const char*);
	void append(const char*, size_t, size_t);
	void append(const std::string);
	void append(const std::string, size_t, size_t);

	const char* c_str() const;
	size_t		capacity() const;
	void		clear();
	const char* data() const;
	bool		empty() const;
	void		erase(size_t index, size_t count);

	size_t find(const char*) const;
	size_t find(const char*, size_t) const;
	size_t find(const std::string) const;
	size_t find(const std::string, size_t) const;

	size_t find_offset(const char symb, size_t offset) const;

	void insert(size_t index, const MyString&);
	void insert(size_t index, size_t, char);
	void insert(size_t index, const char*);
	void insert(size_t index, const char*, size_t);
	void insert(size_t index, const std::string);
	void insert(size_t index, const std::string, size_t);

	size_t length() const;

	void replace(size_t, size_t, const char*);
	void replace(size_t, size_t, const std::string&);

	void shrink_to_fit();

	size_t size()   const;

	MyString substr(size_t index) const;
	MyString substr(size_t index, size_t count) const;

private:
	char* raw_data_;
	size_t string_length_;
	size_t data_size_;
	friend int string_difference(const MyString&, const MyString&);
	void resize_to(size_t size);
};

