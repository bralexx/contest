#ifndef STRING_STRING_H
#define STRING_STRING_H

#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>

class String {
    size_t size = 0;
    size_t array_size = 1;
    char* str = nullptr;

    void resize(size_t new_size) {
        String copy(new_size);
        copy.size = std::min(size, copy.array_size);
        memcpy(copy.str, str, copy.size);
        swap(copy);
    }

    void swap(String& str2) {
        std::swap(size, str2.size);
        std::swap(array_size, str2.array_size);
        std::swap(str, str2.str);
    }
public:
    String() = default;

    explicit String(size_t sz): size(sz),
                                array_size(pow(2,ceil(std::log(size)/std::log(2)))),
                                str(new char[array_size]) {}

    explicit String(const size_t& sz, const char& c) : String(sz) {
        std::memset(str, c, sz);
    }

    String(const char& c) : String(1, c) {}

    String(const char* chars) : String(strlen(chars)) {
        memcpy(str, chars, size);
    }

    String(const String& str2) : String(str2.size) {
        std::memcpy(str, str2.str, size);
    }

    ~String(){
        delete[] str;
    }

    String& operator= (String str2) {
        swap(str2);
        return *this;
    }

    bool operator==(const String& str2) const {
        return ((size == str2.size)&&(!memcmp(str, str2.str, size)));
    }

    const char& operator[](const size_t& index) const {
        return str[index];
    }

    char& operator[](const size_t& index) {
        return str[index];
    }

    size_t length() const {
        return size;
    }

    void push_back(char c) {
        *this += c;
    }

    void pop_back() {
        if(--size < array_size/4) {
            resize(size);
        }
    }

    const char& front() const {
        return str[0];
    }

    char& front() {
        return str[0];
    }

    const char& back() const {
        return str[size - 1];
    }

    char& back() {
        return str[size - 1];
    }

    String& operator+=(const String& str2) {
        if(size + str2.size >= array_size) {
            resize(size + str2.size);
        }
        memcpy(str + size, str2.str, str2.size);
        size += str2.size;
        return *this;
    }

    size_t find(const String& substr) const {
        for(size_t i = 0; i < size; ++i) {
            if(!memcmp(str + i, substr.str, substr.size)) return i;
        }
        return size;
    }

    size_t rfind(const String& substr) const {
        for(size_t i = substr.size; i <= size; ++i) {
            if(!memcmp(str + size - i, substr.str, substr.size)) return size - i;
        }
        return size;
    }

    String substr(size_t start, size_t count) const {
        String str2(count);
        memcpy(str2.str, str + start, count);
        return str2;
    }

    bool empty() const {
        return size == 0;
    }

    void clear() {
        resize(1);
        size = 0;
    }
};

std::ostream& operator<< (std::ostream& out, const String& str) {
    for(size_t i = 0; i < str.length(); ++i) {
        out << str[i];
    }
    return out;
}

String& operator>> (std::istream& in, String& str) {
    char* c = new char();
    in >> c;
    str = c;
    return str;
}

String operator+ (const String& s1, const String& s2) {
    String copy = s1;
    copy += s2;
    return copy;
}

#endif //STRING_STRING_H
