#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <list>
#include <string>

class http_request
{
public:
    enum class method_e
    {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH
    };

    method_e get_method() const;
    void set_method(method_e method);

    void add_path_segment(std::string&& segment);

private:
    method_e m_method;
    std::list<std::string> m_path;
};

#endif
