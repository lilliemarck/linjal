#include "json.hpp"
#include <json_spirit_utils.h>

namespace linjal {

json_spirit::Array const &get_array(json_spirit::Object const& object, std::string const& name)
{
    return find_value(object, name).get_array();
}

json_spirit::Array const *get_array_ptr(json_spirit::Object const& object, std::string const& name)
{
    json_spirit::Value const& value = find_value(object, name);

    if (value.type() != json_spirit::array_type)
    {
        return nullptr;
    }

    return &value.get_array();
}

std::uint8_t get_uint8_t(json_spirit::Object const& object, std::string const& name)
{
    return static_cast<std::uint8_t>(find_value(object, name).get_int());
}

std::string const& get_string(json_spirit::Object const& object, std::string const& name)
{
    return find_value(object, name).get_str();
}

} // namespace linjal
