#include "glhf/shader.h"

#include "glhf/log.h"
#include "glhf/opengl.h"
#include <list>

static std::unordered_map<std::string_view, std::string_view> _sharedSources;

void glhf::createSharedSource(std::string_view key, std::string_view src) {
    _sharedSources.emplace(key, src);
}

glhf::Shader::Shader(uint32_t type, const char *src,
                     const std::unordered_set<std::string> &defines) {

    id = glCreateShader(type);

    bool useShaderExtension{!defines.empty()};
    for (size_t i{0}; i < 100'000 && !useShaderExtension; ++i) {
        if (src[i] == '#') {
            if (strncmp(src + i, "#include", sizeof("#include") - 1) == 0) {
                useShaderExtension = true;
                break;
            }
        } else if (src[i] == '\0') {
            break;
        }
    }
    std::string str;
    if (useShaderExtension) {
        str.assign(src);

        if (!defines.empty()) {
            size_t i = str.find("\n");
            const std::string version = str.substr(0, i + 1);
            str = str.substr(i);
            for (const auto &define : defines) {
                str = "#define " + define + "\n" + str;
            }
            str = version + str;
        }

        size_t a = str.find("#include");
        while (a != std::string::npos) {
            size_t b0 = str.find('"', a);
            size_t b1 = str.find('"', b0 + 1);
            std::string name = str.substr(b0 + 1, (b1 - b0 - 1));
            auto it = _sharedSources.find(name.c_str());
            if (it == _sharedSources.end()) {
                LOG_WARN("Failed to find SharedSource: %s\n", name.c_str());
            }
            str.replace(a, b1 - a + 1, it != _sharedSources.end() ? it->second : "");
            a = str.find("#include", a + 1);
        }
        src = str.c_str();
        // printf("\n%s\n", src);
    }

    compile(src);
}

glhf::Shader::~Shader() { glDeleteShader(id); }

bool glhf::Shader::compile(const char *src) {
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    GLint status{0};
    GLint length{0};
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        const int buf_len = 1024;
        GLchar buf[buf_len] = {};
        GLint n = length < buf_len ? length : buf_len;
        glGetShaderInfoLog(id, length, &n, buf);
        printf("%s\n %.*s\n", src, n, buf);
        return false;
    }
    return true;
}

glhf::ShaderProgram::ShaderProgram(std::unordered_map<std::string_view, Uniform> &&uniforms_)
    : id{glCreateProgram()}, uniforms{std::move(uniforms_)} {}

glhf::ShaderProgram::ShaderProgram(
    std::unordered_map<std::string_view, Uniform> &&uniforms_,
    const std::unordered_map<std::string_view, uint32_t> &uniformBuffers,
    const glhf::Shader &vertex, const glhf::Shader &fragment)
    : ShaderProgram{std::move(uniforms_)} {
    link(vertex, fragment);
    for (const auto &entry : uniformBuffers) {
        auto index = glGetUniformBlockIndex(id, entry.first.data());
        glUniformBlockBinding(id, index, entry.second);
    }
}

glhf::ShaderProgram::~ShaderProgram() { glDeleteProgram(id); }

glhf::Uniform *glhf::ShaderProgram::uniform(const char *key) {
    auto it = uniforms.find(key);
    if (it == uniforms.end()) {
        return nullptr;
    }
    return &it->second;
}

void glhf::ShaderProgram::use() { glUseProgram(id); }

void glhf::ShaderProgram::link(const Shader &vertex, const Shader &fragment) {
    glAttachShader(id, vertex.id);
    glAttachShader(id, fragment.id);
    glLinkProgram(id);
    GLint res = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &res);
    if (!res) {
        const int buf_len = 1024;
        GLchar buf[buf_len] = {};
        glGetProgramInfoLog(id, buf_len, NULL, buf);
        printf("%s\n", buf);
        return;
    }
    use();
    for (auto it = uniforms.begin(); it != uniforms.end(); ++it) {
        it->second.location = glGetUniformLocation(id, it->first.data());
        it->second.update();
    }
}
