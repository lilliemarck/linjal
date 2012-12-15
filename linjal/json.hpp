#pragma once

#include <json_spirit_value.h>

namespace linjal {

json_spirit::Array const &get_array(json_spirit::Object const& object, std::string const& name);
json_spirit::Array const *get_array_ptr(json_spirit::Object const& object, std::string const& name);
std::uint8_t get_uint8_t(json_spirit::Object const& object, std::string const& name);
std::string const& get_string(json_spirit::Object const& object, std::string const& name);

template <typename T>
struct json_converter
{
    static json_spirit::Value to_json(T const& value);
    static T from_json(json_spirit::Value const& value);
};

template <typename T>
json_spirit::Value to_json(T const& value)
{
    return json_converter<T>::to_json(value);
}

template <typename T>
T from_json(json_spirit::Value const& value)
{
    return json_converter<T>::from_json(value);
}

} // namespace linjal
