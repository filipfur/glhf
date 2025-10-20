#pragma once

#include "glm/glm.hpp"
#include <cstdint>
#include <string>

namespace glhf {
class Color {
  public:
    Color();
    Color(const std::string &hexstr);
    Color(uint32_t hex, float alpha = 1.0f);
    Color(uint8_t r, uint8_t g, uint8_t b);
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(const glm::vec3 &rgb);
    Color(const glm::vec4 &rgba);

    static Color opacity(float val);

    Color operator*(const Color &other) const;

    operator glm::vec4() const;

    glm::vec3 vec3() const;

    const glm::vec4 &vec4() const;
    glm::vec4 &vec4();

    glm::vec3 hsv() const;
    Color &setHSV(float h, float s, float v);

    void hueShift(float rad);
    void saturate(float S);
    void valueScale(float S);

    std::string hexString() const;

    static const Color white;
    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color yellow;
    static const Color magenta;
    static const Color cyan;
    static const Color black;

  private:
    glm::vec4 _rgba{1.0f, 1.0f, 1.0f, 1.0f};
};
} // namespace glhf

glhf::Color rgb(uint8_t r, uint8_t g, uint8_t b);
