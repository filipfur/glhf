#include "glhf/json.h"

#include <cassert>

// TODO: Might be better to store "text" instead of text in json.value for strings.

static constexpr size_t LOOP_MAX = SIZE_MAX;

const glhf::Json glhf::Json::empty{};

inline static std::string_view _parseString(const char *sub, size_t &i) {
    for (size_t j{0}; j < LOOP_MAX; ++j) {
        ++i;
        if (sub[j] == '"') {
            return {sub, j};
        }
    }
    return {};
}

inline static bool _isNumericToken(char c) {
    return (c >= '0' && c <= '9') || c == '-' || c == '.';
}

inline static std::string_view _parseNumber(const char *sub, size_t &i) {
    static const char *True = "1";
    static const char *False = "0";
    if (sub[0] == 't') {
        assert(strncmp(sub, "true", 4) == 0);
        i += 3;
        return {True, 1};
    } else if (sub[0] == 'f') {
        assert(strncmp(sub, "false", 5) == 0);
        i += 4;
        return {False, 1};
    } else {
        for (size_t j{0}; j < LOOP_MAX; ++j) {
            if (!_isNumericToken(sub[j + 1])) {
                return {sub, j + 1};
            }
            ++i;
        }
    }
    return {};
}

void glhf::Json::loadString(const char *str) {
    std::list<Json *> jsons;
    for (size_t i{0}; i < LOOP_MAX; ++i) {
        switch (str[i]) {
        case '{':
        case '[':
            jsons.push_back(jsons.empty() ? this : &jsons.back()->children.emplace_back());
            jsons.back()->value = {str + i, 1};
            break;
        case '"':
            switch (jsons.back()->value[0]) {
            case '{':
                jsons.back()->children.emplace_back().key = _parseString(str + i + 1, i);
                ++i;
                // std::cout << "key: " << jsons.back()->key << std::endl;
                for (; i < LOOP_MAX; ++i) {
                    if (str[i] != ' ' && str[i] != ':') {
                        break;
                    }
                }
                switch (str[i]) {
                case '"':
                    jsons.back()->children.back().value = _parseString(str + i + 1, i);
                    break;
                case '{':
                case '[':
                    jsons.push_back(&jsons.back()->children.back());
                    jsons.back()->value = {str + i, 1};
                    break;
                default:
                    jsons.back()->children.back().value = _parseNumber(str + i, i);
                    break;
                }
                break;
            case '[':
                jsons.back()->children.emplace_back().value = _parseString(str + i + 1, i);
                break;
            default:
                assert(false);
                break;
            }
            break;
        case '}':
        case ']':
            assert(str[i] - 2 == jsons.back()->value[0]);
            jsons.pop_back();
            if (jsons.empty()) {
                return; // Done!
            }
            break;
        default:
            if (jsons.back()->value[0] == '[') {
                if (_isNumericToken(str[i])) {
                    jsons.back()->children.emplace_back().value = _parseNumber(str + i, i);
                }
            }
            break;
        }
    }
}
