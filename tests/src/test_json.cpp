#include <gtest/gtest.h>

#include "glhf/json.h"
#include <iostream>

const char *input = R"({
    "string": "value",
    "integer": 123,
    "real": 13.37,
    "object": {
        "a": 1,
        "aaa": 2,
        "aa": 3,
        "b": {
            "c": {"d":12}
        }
    },
    "object_2": {
        "k": 333
        "array": []
    },
    "array": [20, 21, 22, 23, 24, 25],
    "floats": [2.14, 666.06],
    "labels": ["Dog", "Cat", "Deer"],
    "zero": 0,
    "empty": {
    }
})";

TEST(JsonTest, BasicAssertions) {
    ASSERT_EQ('[', ']' - 2);
    ASSERT_EQ('{', '}' - 2);
    glhf::Json json;
    json.loadString(input);
    ASSERT_EQ(json.children.size(), 10u);
    ASSERT_EQ(json.get<std::string_view>("string").compare("value"), 0);
    ASSERT_EQ(json.get<int>("integer"), 123);
    ASSERT_EQ(json.get<int>("real"), 13);
    ASSERT_EQ(json.get<float>("integer"), 123.0f);
    ASSERT_EQ(json.get<float>("real"), 13.37f);
    const auto &object = json[3];
    ASSERT_EQ(object.children.size(), 4u);
    ASSERT_EQ((long long)&object, (long long)&json["object"]);
    ASSERT_EQ((int)object["a"], 1);
    ASSERT_EQ((int)object["aaa"], 2);
    ASSERT_EQ((int)object["aa"], 3);
    ASSERT_EQ((int)object["b"]["c"]["d"], 12);
    ASSERT_EQ((int)json["object_2"]["k"], 333);
    ASSERT_TRUE(json["object_2"]["array"].children.empty());
    ASSERT_EQ((long long)&json["empty"], (long long)&json.children.back());
    ASSERT_TRUE(json["empty"].children.empty());
    ASSERT_EQ((int)json["array"][0], 20);
    ASSERT_EQ((int)json["array"][1], 21);
    ASSERT_EQ((int)json["array"][2], 22);
    ASSERT_EQ((int)json["array"][5], 25);
    ASSERT_EQ((float)json["floats"][0], 2.14f);
    float total = 0;
    for (float f : json["floats"]) {
        total += f;
    }
    ASSERT_EQ(total, 666.06f + 2.14f);
    int itotal = 0;
    for (int i : json["array"]) {
        itotal += i;
    }
    ASSERT_EQ(itotal, 20 + 21 + 22 + 23 + 24 + 25);
    ASSERT_EQ(json["labels"][0].value.compare("Dog"), 0);
    ASSERT_EQ(json["labels"][1].value.compare("Cat"), 0);
    ASSERT_EQ(json["labels"][2].value.compare("Deer"), 0);
    ASSERT_EQ((int)json["zero"], 0);
    ASSERT_TRUE(json["zero"]);
    ASSERT_FALSE(json["missing_key"]);
    std::cout << json << std::endl;
}