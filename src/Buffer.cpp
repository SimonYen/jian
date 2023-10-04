#include "Buffer.h"
#include <iostream>
#include <string>
#include <sys/types.h>

jian::Buffer::Buffer() { }

jian::Buffer::~Buffer() { }

void jian::Buffer::append(const char* _str, int _size)
{
    for (int i = 0; i < _size; i++) {
        if (_str[i] == '\0')
            break;
        buf.push_back(_str[i]);
    }
}

ssize_t jian::Buffer::size() { return buf.size(); }

const char* jian::Buffer::c_str() { return buf.c_str(); }

void jian::Buffer::clear() { buf.clear(); }

void jian::Buffer::getline()
{
    buf.clear();
    std::getline(std::cin, buf);
}