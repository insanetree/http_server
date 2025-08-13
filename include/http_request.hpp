#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <string>

class http_request
{
    enum class method_e {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH};

    method_e get_method() const;
    void set_method(method_e method);

private:
    method_e m_method;

};

#endif
