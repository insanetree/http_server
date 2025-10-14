#include "http_request.hpp"

void
http_request::set_method(http_request::method_e method)
{
    m_method = method;
}

http_request::method_e
http_request::get_method() const
{
    return m_method;
}

const std::list<std::string>&
http_request::get_path() const
{
    return m_path;
}

void
http_request::add_path_segment(std::string&& segment)
{
    m_path.emplace_back(segment);
}
