#include <cerrno>
#include <cstring>
#include <expected>
#include <gtest/gtest.h>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <system_error>
#include <unistd.h>

#include "http_connection.hpp"
#include "http_request.hpp"
#include "http_request_strings.hpp"

namespace it = insanetree;

class TestRequestParsing : public testing::Test
{
public:
    void SetUp() override
    {
        int ret;
        int fd_pair[2];
        ret = socketpair(AF_UNIX, SOCK_STREAM, 0, fd_pair);
        ASSERT_EQ(0, ret);
        sockaddr_in dummy{ .sin_family = AF_UNIX };
        m_fd_in = fd_pair[0];
        m_fd_out = fd_pair[1];
        m_http_connection =
          std::make_unique<it::http_connection>(m_fd_out, dummy);
        ASSERT_EQ(it::http_connection::connection_state_e::UNINITIALIZED,
                  m_http_connection->get_state());
    }

    void TearDown() override
    {
        int ret;
        ret = close(m_fd_in);
        ASSERT_EQ(0, ret);
        ret = close(m_fd_out);
        ASSERT_EQ(0, ret);
    }

    int m_fd_in;
    int m_fd_out;
    std::unique_ptr<it::http_connection> m_http_connection;
};

TEST_F(TestRequestParsing, TestBasicGet)
{
    int ret;
    std::expected<void, std::errc> result;
    m_http_connection->initialize();
    ASSERT_EQ(it::http_connection::connection_state_e::READY_TO_READ,
              m_http_connection->get_state());
    result = m_http_connection->read_socket();
    ASSERT_TRUE(!result);
    ASSERT_EQ(std::errc::resource_unavailable_try_again, result.error());
    ASSERT_EQ(it::http_connection::connection_state_e::READY_TO_READ,
              m_http_connection->get_state());
    ret = send(m_fd_in, basic_get_message, strlen(basic_get_message), 0);
    ASSERT_EQ(strlen(basic_get_message), ret);
    result = m_http_connection->read_socket();
    ASSERT_TRUE(result);
    ASSERT_EQ(it::http_connection::connection_state_e::READY_TO_PARSE,
              m_http_connection->get_state());
    result = m_http_connection->parse_buffer();
    ASSERT_TRUE(result);
    ASSERT_EQ(it::http_connection::connection_state_e::REQUEST_READY,
              m_http_connection->get_state());

    std::unique_ptr<http_request> request = m_http_connection->get_request();
    ASSERT_TRUE(request);
    ASSERT_EQ(http_request::method_e::GET, request->get_method());
    ASSERT_EQ(it::http_connection::connection_state_e::AWAITING_RESPONSE,
              m_http_connection->get_state());

    const std::list<std::string>& path = request->get_path();
    ASSERT_EQ(0ul, path.size());
}

TEST_F(TestRequestParsing, TestPartialRequest)
{
    int ret;
    m_http_connection->initialize();
    std::expected<void, std::errc> result;
    ASSERT_EQ(it::http_connection::connection_state_e::READY_TO_READ,
              m_http_connection->get_state());
    ret = send(m_fd_in, basic_get_message, 1, 0);
    ASSERT_EQ(1, ret);
    result = m_http_connection->read_socket();
    ASSERT_TRUE(!result);
    ASSERT_EQ(it::http_connection::connection_state_e::READY_TO_READ,
              m_http_connection->get_state());
    ret =
      ::send(m_fd_in, basic_get_message + 1, strlen(basic_get_message) - 1, 0);
    ASSERT_EQ(strlen(basic_get_message) - 1, ret);
    result = m_http_connection->read_socket();
    ASSERT_TRUE(result);
    ASSERT_EQ(it::http_connection::connection_state_e::READY_TO_PARSE,
              m_http_connection->get_state());
    result = m_http_connection->parse_buffer();
    ASSERT_TRUE(result);
    ASSERT_EQ(it::http_connection::connection_state_e::REQUEST_READY,
              m_http_connection->get_state());
    std::unique_ptr<http_request> request = m_http_connection->get_request();
    ASSERT_TRUE(request);
    ASSERT_EQ(http_request::method_e::GET, request->get_method());
    ASSERT_EQ(it::http_connection::connection_state_e::AWAITING_RESPONSE,
              m_http_connection->get_state());

    const std::list<std::string>& path = request->get_path();
    ASSERT_EQ(0ul, path.size());
}