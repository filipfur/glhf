#pragma once

#include <iostream>
#include <list>
#include <string_view>

namespace glhf {
struct Json {
    void loadString(const char *str);

    template <typename T> T get(const char *str, T defaultValue = {}) const {
        const Json &json = this->operator[](str);
        return json ? static_cast<T>(json) : defaultValue;
    }

    const Json &operator[](int index) const {
        auto it = children.begin();
        std::advance(it, index);
        return *it;
    }

    const Json &operator[](const char *str) const {
        for (const auto &child : children) {
            if (child.key.compare(str) == 0) {
                return child;
            }
        }
        return empty;
    }

    operator bool() const { return this != &empty; }
    operator short() const { return std::atoi(value.data()); }
    operator unsigned short() const { return std::atoi(value.data()); }
    operator int() const { return std::atoi(value.data()); }
    operator unsigned int() const { return std::atoi(value.data()); }
    operator long() const { return std::atol(value.data()); }
    operator size_t() const { return operator long(); }
    operator float() const { return std::atof(value.data()); }
    operator std::string_view() const { return value; }

    std::list<Json>::const_iterator begin() const { return children.begin(); }
    std::list<Json>::const_iterator end() const { return children.end(); }

    friend std::ostream &operator<<(std::ostream &os, const Json &json) {
        if (json.key.length() > 0) {
            os << "\"" << json.key << "\": ";
        }
        if ((json.value[0] >= '0' && json.value[0] <= '9') || json.value[0] == '-' ||
            json.value[0] == '{' || json.value[0] == '[') {
            os << json.value;
        } else {
            os << "\"" << json.value << "\"";
        }
        std::string delim{""};
        for (const Json &child : json.children) {
            os << delim << child;
            delim = ",";
        }
        if (json.value[0] == '{' || json.value[0] == '[') {
            os << (char)(json.value[0] + 2);
        }
        return os;
    }

    size_t size() const { return children.size(); }

    std::string_view key;
    std::string_view value;
    std::list<Json> children;
    Json *parent;

    const static Json empty;
};
}; // namespace glhf