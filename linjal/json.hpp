#pragma once

#include <json_spirit_value.h>

namespace linjal {

template <typename T>
struct json_converter
{
    static json_spirit::Value to_json(T const& value);
};

template <typename T>
json_spirit::Value to_json(T const& value)
{
    return json_converter<T>::to_json(value);
}

} // namespace linjal
