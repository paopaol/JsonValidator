
#include <stdio.h>

#include <cassert>
#include <filesystem>

#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <functional>





#if defined(USE_JSON_CPP)
    #include <json.h>
#elif defined(USE_QT_JSON)
#else
    #error "must define USE_JSON_CPP (support jsoncpp) or USE_QT_JSON (support qt json)"
#endif

namespace JsonValidator {
    namespace detail {
        std::vector<std::string> &Split(std::vector<std::string> &vstr,
            const std::string &str, const std::string &pattern)
        {
            vstr.resize(0);

            if (str == "" || pattern == "") {
                return vstr;
            }

            std::string s = str + pattern;
            size_t size = s.size();

            for (size_t pos = s.find(pattern);
                pos != std::string::npos;) {
                std::string x(s.substr(0, pos));
                vstr.push_back(x);
                s = s.substr(pos + pattern.size(), size);
                pos = s.find(pattern);
            }
            return vstr;
        }

        static const std::string value_error_message = "invalid value";
        static const std::string type_error_message = "invalid type";
        static const std::string key_error_message = "key not exists or is null";

    }


    class ValidatorExcecption : public std::runtime_error {
    public:
        ValidatorExcecption(const std::string &message)
            : std::runtime_error(message) {}
        virtual ~ValidatorExcecption() throw() {}
    };




    template<class JsonValueClass,
        class JsonTypeClass, JsonTypeClass type>
        class Validator {
        public:

            /**
             * \fn  Validator::Validator(JsonValueClass &v, const char *section)
             *
             * \brief   Constructor
             *
             * \author  Jinzh
             * \date    2018/4/12
             *
             * \param [in,out]  v       the JsonValueClass to process.
             * \param           section The section format  "key->subkey->subsubkey"
             */

            Validator(JsonValueClass &v, const char *section)
                :vt_(type)
                ,required_(true)
                ,validate_equal_(false)
                ,validate_max_(false)
                ,validate_minimum_(false)
                ,use_default_(false)
                ,regex_replace_(false)
                ,v_(v)
            {
                assert(section != nullptr);
                prepareSection(section);
            }

            /**
             * \fn  Validator Validator::&default(const JsonValueClass &v)
             *
             * \brief   Defaults the given v, if the expected key not exist
             *
             * \author  Jinzh
             * \date    2018/4/12
             *
             * \param   v   expected default value.
             *
             * \return  A reference to a Validator.
             */

            Validator &default(const JsonValueClass &v)
            {
                use_default_ = true;
                v_default_ = v;
                return *this;
            }

            /**
             * \fn  Validator Validator::&eq(const JsonValueClass &v)
             *
             * \brief   Eqs the given v
             *
             * \author  Jinzh
             * \date    2018/4/12
             *
             * \param   v   expect equal value
             *
             * \return  A reference to a Validator.
             */

            Validator &eq(const JsonValueClass &v)
            {
                validate_equal_ = true;
                v_equal_ = v;
                return *this;
            }

            /**
             * \fn  Validator Validator::&minimum(const JsonValueClass &v)
             *
             * \brief   Determines the minimum of the given parameters
             *
             * \author  Jinzh
             * \date    2018/4/12
             *
             * \param   v    expect minimum value
             *
             * \return  The minimum value.
             */

            Validator &minimum(const JsonValueClass &v)
            {
                validate_minimum_ = true;
                v_minimum_ = v;
                return *this;
            }

            /**
             * \fn  Validator Validator::&maxmum(const JsonValueClass &v)
             *
             * \brief   Maxmums the given v
             *
             * \author  Jinzh
             * \date    2018/4/12
             *
             * \param   v    expect maxmum value
             *
             * \return  A reference to a Validator.
             */

            Validator &maxmum(const JsonValueClass &v)
            {
                v_max_ = v;
                validate_max_ = true;
                return *this;
            }

            /**
             * \fn  Validator Validator::&nonezero()
             *
             * \brief   expect the key is nonezero, only for object/array
             *
             * \author  Jinzh
             * \date    2018/4/12
             *
             * \return  A reference to a Validator.
             */

            Validator &nonezero() {
                nonezero_ = true;
                return *this;
            }

            /**
             * \fn  Validator Validator::&regex_search(const std::string &pattern)
             *
             * \brief   RegEx search, only for string value
             *
             * \author  Jinzh
             * \date    2018/4/12
             *
             * \param   pattern Specifies the pattern.
             *
             * \return  A reference to a Validator.
             */

            Validator &regex_search(const std::string &pattern) {
                regex_pattern_ = pattern;
                return *this;
            }

            /*
            Validator &is_email() {
                is_email_ = true;
                return *this;
            }

            //check if the string contains ASCII chars only.
            Validator &is_ascii() {
                is_email_ = true;
                return *this;
            }

            //check if a string is base64 encoded.
            Validator &is_base64() {

            }
            //check if the string is an URL.

             //   options is an object which defaults to
             //   { protocols: ['http','https','ftp'], require_tld : true, require_protocol : false, 
             //   require_host : true, require_valid_protocol : true, allow_underscores : false,
             //    host_whitelist : false, host_blacklist : false, allow_trailing_dot : false, allow_protocol_relative_urls : false }.
            Validator & isURL(str[, options]) {

            }
           
            //check if the string is a valid port number.
            Validator &isPort(str) {

            }

            //check if the string matches to a valid MIME type format
            Validator & &isMimeType() {

            }
            //check if the string is lowercase
            Validator &isLowercase(str) {

            }
            Validator &isUppercase(str) {

            }

            Validator &trim(input[, chars]) {

            }
            //check if the string is an IP (version 4 or 6).
            Validator &isIP(str[, version]) {

            }

            */

            JsonValueClass validate()
            {
                JsonValueClass v = v_;
              
                unsigned int size = key_list_.size();
                std::string last_key = size > 0 ?
                    key_list_[size - 1] : "";

                for (unsigned int index = 0; index < size - 1; index++) {
                    std::string key = key_list_[index];
                    v = v_[key];
                    if (!v.isObject()) {
                        throw ValidatorExcecption(key + ":" +
                            detail::type_error_message);
                    }
                }
                v = v[last_key];

                validate_required(v, last_key);
               
                validate_type(v, last_key);
                if (v.isNull()) {
                    return v;
                }
                validate_nonezero(v, last_key);
                validate_eq(v, last_key);
                validate_min(v, last_key);
                validate_max(v, last_key);
                validate_regex(v, last_key);
                return v;
            }



        private:

            void prepareSection(const std::string &section)
            {
                detail::Split(key_list_, section, "->");
            }

            void validate_required(JsonValueClass &v, const std::string &key)
            {
                if (!required_) {
                    return;
                }
                if (!v.isNull()) {
                    return;
                }
                if (use_default_) {
                    use_default(v);
                    return;
                }
                throw ValidatorExcecption(key + ":"
                    + detail::key_error_message);

            }

            void use_default(JsonValueClass &v)
            {
                v = v_default_;
            }

            void validate_type(const JsonValueClass &v, const std::string &key)
            {
                bool expect_type = (vt_ == v.type());

                if (!expect_type) {
                    throw ValidatorExcecption(key + ":"
                        + detail::type_error_message);
                }
            }


            void throw_if_type_neq(const JsonValueClass &v1, 
                const JsonValueClass &v2, const std::string &description, const std::string &key)
            {
                if (v1.type() != v2.type()) {
                    throw ValidatorExcecption(key +  " " + description + " type error:"
                        + detail::value_error_message);
                }
            }

            void throw_if_not_expect_type(bool is_expected, 
                const std::string &description, const std::string &key)
            {
                if (!is_expected) {
                    throw ValidatorExcecption(key + " " + description + " type error:"
                        + detail::value_error_message);
                }
            }



            void validate_eq(const JsonValueClass &v, const std::string &key)
            {
                if (!validate_equal_) {
                    return;
                }
                throw_if_type_neq(v, v_equal_, "eq", key);

                if (v == v_equal_) {
                    return;
                }
              
                throw ValidatorExcecption(key + ":"
                    + detail::value_error_message);
            }

            void validate_min(const JsonValueClass &v, const std::string &key)
            {
                if (!validate_minimum_) {
                    return;
                }
                throw_if_type_neq(v, v_minimum_, "minimum", key);

                if (v < v_minimum_) {
                    throw ValidatorExcecption(key + ":"
                        + detail::value_error_message);
                    return;
                }

                return;
            }

            void validate_max(const JsonValueClass &v, const std::string &key)
            {
                if (!validate_max_) {
                    return;
                }
                throw_if_type_neq(v, v_max_, "maxmum", key);

                if (v > v_max_) {
                    throw ValidatorExcecption(key + ":"
                        + detail::value_error_message);
                }
                return;
            }



            void validate_nonezero(const JsonValueClass &v, const std::string &key)
            {
                if (!nonezero_) {
                    return;
                }
                if (!v.empty()) {
                    return;
                }
                throw ValidatorExcecption(key + ":"
                    + detail::value_error_message);
            }


            void validate_regex(const JsonValueClass &v, const std::string &key)
            {
                if (regex_pattern_.empty()) {
                    return;
                }
                throw_if_not_expect_type(v.isString(), "regex", key);

                std::regex pattern(regex_pattern_);

                bool matched = std::regex_search(v.asString(), pattern);
                if (matched) {
                    return;
                }

                throw ValidatorExcecption(key + ":"
                    + detail::value_error_message);
            }


            void validate_regex(const JsonValueClass &v, 
                const std::string &pattern, const std::string &key)
            {
                if (pattern.empty()) {
                    return;
                }
                throw_if_not_expect_type(v.isString(), "regex", key);
               
                std::regex pattern(pattern);

                bool matched = std::regex_search(v.asString(), pattern);
                if (matched) {
                    return;
                }

                throw ValidatorExcecption(key + ":"
                    + detail::value_error_message);
            }


            JsonTypeClass vt_;
            std::vector<std::string> key_list_;
            JsonValueClass &v_;

            JsonValueClass v_equal_;
            bool validate_equal_;

            JsonValueClass v_minimum_;
            bool validate_minimum_;

            JsonValueClass v_max_;
            bool validate_max_;

            bool required_;

            bool use_default_;
            JsonValueClass v_default_;
           



            bool nonezero_;
            bool is_email_;

            std::string regex_pattern_;
            bool regex_replace_;
    };


    template<class JsonValueClass,
        class JsonTypeClass,
        JsonTypeClass type>
        static Validator<JsonValueClass,
        JsonTypeClass, type> create(JsonValueClass &v, const char *section)
    {
        return Validator<JsonValueClass,
            JsonTypeClass, type>(v, section);
    }

    template<Json::ValueType type>
    static Validator<Json::Value,
        Json::ValueType, type> create(Json::Value &v, const char *section)
    {
        return Validator<Json::Value,
            Json::ValueType, type>(v, section);
    }

}



