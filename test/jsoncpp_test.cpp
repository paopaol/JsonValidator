#include <gtest/gtest.h>
#include <JsonValidator/JsonValidator.h>
#include <json.h>

static char *js = "{\"title\":\"s\",\"bool_v\":true,\"array_v\":[1,2,3],\"object\":\"xxx\",\"null_v\": null,\"school\":\"xxx\",\"obj\":{\"age\":18}, \"uint_v\":100}";
namespace {
    TEST(JsonValidator, default_value) {
        Json::Value v;
        Json::Reader r;

        r.parse(js, v);

        int v_int = JsonValidator::create<Json::intValue>(v, "not-exists-int").default(100).validate().asInt();
        EXPECT_EQ(v_int, 100);
        std::string v_string = JsonValidator::create<Json::stringValue>(v, "not-exists-string").default("string").validate().asString();
        EXPECT_EQ(v_string, "string");

        unsigned int v_uint = JsonValidator::create<Json::uintValue>(v, "not-exists-uint").default((unsigned int)100).validate().asUInt();
        EXPECT_EQ(v_uint, 100);
        bool v_bool = JsonValidator::create<Json::booleanValue>(v, "not-exists-bool").default(true).validate().asBool();
        EXPECT_EQ(v_bool, true);


        Json::Value array;
        array.append(1);
        array.append(2);
        Json::Value v_array = JsonValidator::create<Json::arrayValue>(v, "not-exists-array").default(array).validate();
        EXPECT_EQ(true, v_array.isArray());


       
        Json::Value null;
        Json::Value v_null = JsonValidator::create<Json::nullValue>(v, "not-exists-null").default(null).validate();
        EXPECT_EQ(true, v_null.isNull());

        Json::Value obj;
        obj["test"] = "obj";
        Json::Value v_obj = JsonValidator::create<Json::objectValue>(v, "not-exists-obj").default(obj).validate();
        EXPECT_EQ(true, v_obj.isObject());
    }


    TEST(JsonValidator, eq) {
        Json::Value v;
        Json::Reader r;

        r.parse(js, v);

        int v_int = JsonValidator::create<Json::intValue>(v, "obj->age").eq(18).validate().asInt();
        EXPECT_EQ(v_int, 18);
        std::string v_string = JsonValidator::create<Json::stringValue>(v, "title").eq("s").validate().asString();
        EXPECT_EQ(v_string, "s");

      
        bool v_bool = JsonValidator::create<Json::booleanValue>(v, "bool_v").eq(true).validate().asBool();
        EXPECT_EQ(v_bool, true);


        Json::Value array;
        array.append(1);
        array.append(2);
        array.append(3);
        Json::Value v_array = JsonValidator::create<Json::arrayValue>(v, "array_v").eq(array).validate();
        EXPECT_EQ(true, v_array == array);

        // test is null or not Useless
        //Json::Value null;
        //Json::Value v_null = JsonValidator::create<Json::nullValue>(v, "null_v").eq(null).validate();
        //EXPECT_EQ(true, v_null == null);

        Json::Value obj;
        obj["age"] = 18;
        Json::Value v_obj = JsonValidator::create<Json::objectValue>(v, "obj").eq(obj).validate();
        EXPECT_EQ(true, v_obj == obj);
    }

    TEST(JsonValidator, expect_type) {
        Json::Value v;
        Json::Reader r;

        r.parse(js, v);

        int v_int = JsonValidator::create<Json::intValue>(v, "obj->age").validate().asInt();
        EXPECT_EQ(v_int, 18);
        std::string v_string = JsonValidator::create<Json::stringValue>(v, "title").validate().asString();
        EXPECT_EQ(v_string, "s");


        bool v_bool = JsonValidator::create<Json::booleanValue>(v, "bool_v").validate().asBool();
        EXPECT_EQ(v_bool, true);


        Json::Value array;
        array.append(1);
        array.append(2);
        array.append(3);
        Json::Value v_array = JsonValidator::create<Json::arrayValue>(v, "array_v").validate();
        EXPECT_EQ(true, v_array == array);

        Json::Value obj;
        obj["age"] = 18;
        Json::Value v_obj = JsonValidator::create<Json::objectValue>(v, "obj").validate();
        EXPECT_EQ(true, v_obj == obj);
    }

    TEST(JsonValidator, minimum) {
        Json::Value v;
        Json::Reader r;

        r.parse(js, v);


        int v_int = JsonValidator::create<Json::intValue>(v, "obj->age").minimum(17).validate().asInt();
        EXPECT_EQ(v_int, 18);
        std::string v_string = JsonValidator::create<Json::stringValue>(v, "title").minimum("a").validate().asString();
        EXPECT_EQ(v_string, "s");


        //unuseless
        bool v_bool = JsonValidator::create<Json::booleanValue>(v, "bool_v").minimum(false).validate().asBool();
        EXPECT_EQ(v_bool, true);


        Json::Value array;
        array.append(1);
        array.append(2);
        array.append(3);
        Json::Value v_array = JsonValidator::create<Json::arrayValue>(v, "array_v").minimum(array).validate();
        EXPECT_EQ(true, v_array.size() == 3);
    }
}


GTEST_API_ int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}