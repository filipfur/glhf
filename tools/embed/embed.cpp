#include "embed.h"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <string_view>

namespace {
inline static constexpr size_t strlen(const char *path) {
    for (size_t i{0}; i < UINT64_MAX; ++i) {
        if (path[i] == '\0') {
            return i;
        }
    }
    return UINT64_MAX;
}

inline static constexpr const char *filename(const char *path) {
    auto len = strlen(path);
    if (len < 0) {
        return nullptr;
    }
    for (size_t i{len - 1}; i > 0; --i) {
        if (i == 0) {
            return nullptr;
        }
        if (path[i] == '/' || path[i] == '\\') {
            return path + i + 1;
        }
    }
    return nullptr;
}

inline static constexpr void replace(std::string &str, char a, char b) {
    for (size_t i{0}; i < str.length(); ++i) {
        if (str[i] == a) {
            str[i] = b;
        }
    }
}

inline static constexpr char to_hex(char c) { return c > 9 ? ('a' + c - 10) : (c + '0'); }

static char __membuf[1'000'000'000] = {};
static std::string_view load_file(const char *path) {
    FILE *file_ptr;
    size_t len;

    file_ptr = fopen(path, "r");
    if (file_ptr == NULL) {
        printf("file can't be opened (r): %s\n", path);
        return {};
    }
    len = fread(__membuf, 1, (sizeof __membuf) - 1, file_ptr);
    fclose(file_ptr);
    __membuf[len] = '\0';
    return {__membuf, len};
}

} // namespace

#define WRITE_TEXT(file, text)                                                                     \
    do {                                                                                           \
        static constexpr char data[] = text;                                                       \
        fwrite(data, sizeof(data) - 1, 1, file);                                                   \
    } while (0)

void embed_to_cpp(const char *in_file, const char *out_file, const ReplaceList &replaceList) {
    auto content = load_file(in_file);

    std::string str;
    if (!replaceList.empty()) {
        str.assign(content);
        for (const auto &replace : replaceList) {
            while (str.find(replace.first) != std::string::npos) {
                str.replace(str.find(replace.first), std::strlen(replace.first), replace.second);
            }
        }
        content = str;
    }

    in_file = ::filename(in_file);
    std::string symbol_name{in_file};
    ::replace(symbol_name, '.', '_');

    auto out_dir = std::filesystem::path(out_file).parent_path();
    if (!std::filesystem::exists(out_dir)) {
        std::filesystem::create_directories(out_dir);
    }

    FILE *file = fopen(out_file, "w");
    if (file == NULL) {
        printf("file can't be opened (w): %s\n", out_file);
    }

    WRITE_TEXT(file, "extern const unsigned char _embed_");
    fwrite(symbol_name.c_str(), symbol_name.length(), 1, file);
    WRITE_TEXT(file, "[];\n");
    WRITE_TEXT(file, "const unsigned char _embed_");
    fwrite(symbol_name.c_str(), symbol_name.length(), 1, file);
    WRITE_TEXT(file, "[] = {");

    static char arr[] = " 0x00,";
    for (size_t i{0}; i < content.length(); ++i) {
        arr[3] = ::to_hex(content[i] >> 4 & 0xF);
        arr[4] = ::to_hex(content[i] & 0xF);
        fwrite(arr, sizeof(arr) - 1, 1, file);
    }

    WRITE_TEXT(file, " 0x00};\n");

    fclose(file);
}

enum ParseMode { PARSE_NONE, PARSE_IN, PARSE_REPLACE_A, PARSE_REPLACE_B, PARSE_OUT };
int main(int argc, char *argv[]) {
    ParseMode mode = PARSE_NONE;
    const char *outfile = nullptr;
    const char *infile = nullptr;
    const char *replaceA = nullptr;
    const char *replaceB = nullptr;
    ReplaceList replaceList;
    for (int i{1}; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0) {
            mode = PARSE_IN;
        } else if (strcmp(argv[i], "-o") == 0) {
            mode = PARSE_OUT;
        } else if (strcmp(argv[i], "-r") == 0) {
            mode = PARSE_REPLACE_A;
        } else {
            switch (mode) {
            default:
            case PARSE_IN:
                infile = argv[i];
                mode = PARSE_NONE;
                break;
            case PARSE_OUT:
                outfile = argv[i];
                mode = PARSE_NONE;
                break;
            case PARSE_REPLACE_A:
                replaceA = argv[i];
                mode = PARSE_REPLACE_B;
                break;
            case PARSE_REPLACE_B:
                replaceB = argv[i];
                printf("replace %s with %s\n", replaceA, replaceB);
                replaceList.emplace_back(replaceA, replaceB);
                mode = PARSE_NONE;
                break;
            }
        }
    }
    if (infile == nullptr) {
        printf("Error: No input file.\n");
        return 1;
    }
    std::string str;
    if (outfile == nullptr) {
        str = std::string{infile} + ".hpp";
        outfile = str.c_str();
    }
    embed_to_cpp(infile, outfile, replaceList);
    return 0;
}