#pragma once

#include <string_view>

template <typename T> struct name_equals {
    name_equals() {}
    bool operator()(const T &t0, const T &t1) { return t0.name == t1.name; }
    bool operator()(T *t0, T *t1) { return t0->name == t1->name; }
};

template <typename T> struct name_is_literal {
    name_is_literal(std::string_view name) : _name{name} {}
    bool operator()(const T &t) { return t.name == _name; }
    bool operator()(T *t) { return t->name == _name; }
    std::string_view _name;
};