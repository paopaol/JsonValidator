#include <JsonValidator/JsonValidator.h>




int main(int argc, char *argv[])
{
    char *js = "{\"title\":\"s\",\"subject\":\"xxx\",\"grade\":\"xxx\",\"object\":\"xxx\",\"teacher\":\"xxx\",\"school\":\"xxx\",\"obj\":{\"age\":18}}";

    Json::Value v;
    Json::Reader r;

    r.parse(js, v);

    std::string title;
    int age;

    try {
       title = JsonValidator::create<Json::stringValue>(v, "title").default("hello").regex_search("^.{1,}$").validate().asString();
       age = JsonValidator::create<Json::intValue>(v, "obj->age").default(100).minimum(18).maxmum(100).validate().asInt();
    }
    catch (const JsonValidator::ValidatorExcecption &e) {
        std::string err = e.what();
    }

    printf("title:%s\n", title.c_str());
}                                                                                                                   