#include "MyString.h"


#include <cstring>
#include <stdexcept>

MyString::MyString() : raw_data_(new char[1]), string_length_(0), data_size_(1)
{
	raw_data_[0] = 0;
}

MyString::MyString(const char* base_str) : MyString(base_str, strlen(base_str))
{}

MyString::MyString(const std::string std_str) : MyString(std_str.c_str(), std_str.size())
{}

MyString::MyString(const char* base_str, size_t count)
{
	string_length_ = count;
	data_size_ = string_length_ + 1;
	raw_data_ = new char[count + 1];
	std::copy(base_str, base_str + count, raw_data_);
	raw_data_[string_length_] = 0;
}

MyString::MyString(size_t repeat_count, char c)
	: string_length_(repeat_count),
	data_size_(string_length_ + 1)
{
	raw_data_ = new char[data_size_];
	std::fill(raw_data_, raw_data_ + string_length_, c);
	raw_data_[string_length_] = 0;
}

MyString::MyString(const MyString& str) : MyString(str.c_str(), str.size())
{}

MyString::MyString(const std::initializer_list<char> str)
	: data_size_(str.size() + 1), string_length_(str.size())
{
	raw_data_ = new char[data_size_];
	std::copy(str.begin(), str.end(), raw_data_);
	raw_data_[string_length_] = 0;
}

MyString::~MyString()
{
	if (data_size_ && raw_data_)
		delete[] raw_data_;
	raw_data_ = nullptr;
}

MyString operator+(const MyString& first, const MyString& second)
{
	MyString result;
	result += first;
	return result += second;
}

MyString MyString::operator+ (const char* second) const
{
	return this->operator+ (second);;
}

MyString MyString::operator+ (const std::string second) const
{
	return this->operator+ (second.c_str());
}

MyString MyString::operator+=(const MyString& second)
{
	return this->operator+=(second.c_str());
}

MyString MyString::operator+=(const char* second)
{
	append(second);
	return *this;
}

MyString MyString::operator+=(const std::string second)
{
	return this->operator+=(second.c_str());
}

MyString MyString::operator=(const MyString& src)
{
	return this->operator=(src.c_str());
}

MyString MyString::operator=(const char* str)
{
	clear();
	append(str);
	return *this;
}

MyString MyString::operator=(const std::string str)
{
	return this->operator=(str.c_str());
}

MyString MyString::operator=(char str)
{
	char c[] = { str, 0 };
	return this->operator=(c);
}

char MyString::operator[](int index) const
{
	if (index < 0 || index > size())
		throw std::out_of_range("String index out of range.");
	return raw_data_[index];
}

int string_difference(const MyString& first, const MyString& second)
{
	if (second.size() != first.size())
		return static_cast<int>(first.size()) - static_cast<int>(second.size());

	for (int i = 0; i < second.string_length_; ++i)
		if (second.raw_data_[i] != first.raw_data_[i])
			return first.raw_data_[i] - second.raw_data_[i];

	return 0;
}

bool MyString::operator>(const MyString& second) const
{
	int difference = string_difference(*this, second);
	return difference > 0;
}

bool MyString::operator<=(const MyString& second) const
{
	return !this->operator>(second);
}

bool MyString::operator<(const MyString& second) const
{
	int difference = string_difference(*this, second);
	return difference < 0;
}

bool MyString::operator>=(const MyString& second) const
{
	return !this->operator<(second);
}

bool MyString::operator!=(const MyString& second) const
{
	return !this->operator==(second);
}

bool MyString::operator==(const MyString& second) const
{
	int difference = string_difference(*this, second);
	return difference == 0;
}

const char* MyString::c_str() const
{
	return raw_data_;
}

const char* MyString::data() const
{
	return c_str();
}

bool MyString::empty() const
{
	return string_length_ == 0;
}

size_t MyString::size() const
{
	return string_length_;
}

size_t MyString::length() const
{
	return size();
}

size_t MyString::capacity() const
{
	return data_size_;
}

void MyString::clear()
{
	string_length_ = 0;
	if (data_size_)
		raw_data_[0] = 0;
}

void MyString::shrink_to_fit()
{
	data_size_ = string_length_ + 1;
	char* new_data = new char[data_size_];
	std::copy(raw_data_, raw_data_ + data_size_, new_data);
	delete[] raw_data_;
	raw_data_ = new_data;
}

std::istream& operator>>(std::istream& input, MyString& str)
{
	char* buffer = new char[1025];
	char c = input.get();
	int i = 0;

	while (c != '\n' && c != EOF)
	{
		buffer[i++] = c;
		if (i == 1024)
		{
			buffer[i] = 0;
			str += buffer;
			i = 0;
		}
		c = input.get();
	}

	if (i != 0)
	{
		buffer[i] = 0;
		str += buffer;
	}
	delete[] buffer;
	return input;
}

std::ostream& operator<<(std::ostream& output, const MyString& str)
{
	output << str.c_str();
	return output;
}

void MyString::insert(size_t index, const MyString& str)
{
	this->insert(index, str.c_str(), str.size());
}

void MyString::insert(size_t index, size_t count, char c)
{
	this->insert(index, MyString(count, c));
}

void MyString::insert(size_t index, const char* str)
{
	this->insert(index, str, strlen(str));
}

void MyString::insert(size_t index, const char* str, size_t count)
{
	if (index > string_length_)
		throw std::out_of_range("String index out of range.");

	resize_to(count + string_length_ + 1);

	std::copy(raw_data_ + index, raw_data_ + size(), raw_data_ + size());
	std::copy(str, str + count, raw_data_ + index);

	string_length_ += count;
	raw_data_[string_length_] = 0;
}

void MyString::insert(size_t index, const std::string str)
{
	this->insert(index, str.c_str(), str.size());
}

void MyString::insert(size_t index, const std::string str, size_t count)
{
	this->insert(index, str.c_str(), count);
}

void MyString::erase(size_t index, size_t count)
{
	if (index > string_length_)
		throw std::out_of_range("String index out of range.");

	size_t l = index, r = index + count;

	while (r < string_length_)
		raw_data_[l++] = raw_data_[r++];

	raw_data_[l] = 0;
	string_length_ = l;
}

void MyString::append(const MyString& str)
{
	this->append(str.c_str(), 0, str.size());
}

void MyString::append(size_t count, char c)
{
	this->append(MyString(count, c));
}

void MyString::append(const char* str)
{
	this->append(str, 0, strlen(str));
}

void MyString::append(const char* _str, size_t index, size_t count)
{
	this->insert(size(), _str + index, count);
}

void MyString::append(const std::string str)
{
	this->append(str.c_str(), 0, str.size());
}

void MyString::append(const std::string str, size_t index, size_t count)
{
	this->append(str.c_str(), index, count);
}

void MyString::replace(size_t index, size_t count, const char* str)
{
	this->erase(index, count);
	this->insert(index, str);
}

void MyString::replace(size_t index, size_t count, const std::string& str)
{
	this->replace(index, count, str.c_str());
}

MyString MyString::substr(size_t index) const
{
	return this->substr(index, size() - index);
}

MyString MyString::substr(size_t index, size_t count) const
{
	if (index + count > string_length_)
		throw std::out_of_range("String index out of range.");

	char tmp = raw_data_[index + count];
	raw_data_[index + count] = 0;
	MyString res(raw_data_ + index, count);
	raw_data_[index + count] = tmp;
	return res;
}

size_t MyString::find(const char* str) const
{
	return find(str, 0);
}

size_t MyString::find(const char* str, size_t index) const
{
	if (index > string_length_)
		return -1;
	char* pos = strstr(raw_data_ + index, str);

	if (pos == nullptr)
		return -1;

	return pos - raw_data_;
}

size_t MyString::find(const std::string str) const
{
	return find(str.c_str());
}

size_t MyString::find(const std::string str, size_t index) const
{
	return find(str.c_str(), index);
}

void MyString::resize_to(size_t size)
{
	if (data_size_ >= size) return;
	data_size_ = size;
	char* new_buff = new char[data_size_];
	std::copy(raw_data_, raw_data_ + string_length_ + 1, new_buff);
	delete[] raw_data_;
	raw_data_ = new_buff;
}

size_t MyString::find_offset(const char symb, size_t offset) const
{
	char substr[] = { symb, 0 };

	size_t index = find(substr, offset);
	return index - offset;
}
