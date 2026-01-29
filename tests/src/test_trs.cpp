#include <gtest/gtest.h>

#include "glhf/trs.h"

TEST(TestTrs, Basic) {
    glm::mat4 model;
    glhf::TRS trs;
    ASSERT_TRUE(trs.valid());
    trs.translation() += glm::vec3{1.0f, 1.0f, 1.0f};
    ASSERT_FALSE(trs.valid());
    model = trs.transformation();
    ASSERT_TRUE(trs.valid());
    const glhf::TRS &c_t = trs;
    ASSERT_FLOAT_EQ(c_t.translation().y, 1.0f);
    ASSERT_TRUE(trs.valid());
}