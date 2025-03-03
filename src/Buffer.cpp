#include "Buffer.h"
#include <string.h>
#include <iostream>

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::append(const char *_str, int _size) {
  for (int i = 0; i < _size; ++i) {
    if (_str[i] == '\0') break;
    buf.push_back(_str[i]);
  }
}

ssize_t Buffer::size() { return buf.size(); }

const char *Buffer::c_str() { return buf.c_str(); }

// c_str() 返回一个 const char* 类型的指针，指向 std::string
// 内部存储的字符串数据。
//  返回的字符串是以 \0 结尾的，符合 C 风格字符串的格式。
//  返回的指针是只读的（const），不能通过该指针修改字符串内容。
void Buffer::clear() { buf.clear(); }

void Buffer::getline() {
  buf.clear();
  std::getline(std::cin, buf);
}

void Buffer::setBuf(const char *_buf) {
  buf.clear();
  buf.append(_buf);
}
