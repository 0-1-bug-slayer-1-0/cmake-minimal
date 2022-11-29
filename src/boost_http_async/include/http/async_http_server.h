//
// Created by feres on 11/26/22.
//

#ifndef MULTIPLYTEST_ASYNC_HTTP_SERVER_H
#define MULTIPLYTEST_ASYNC_HTTP_SERVER_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <glog/logging.h>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

void fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << '\n';
}

class session : public std::enable_shared_from_this<session>
{
public:
    explicit session(net::io_context& ioc)
        : resolver_(net::make_strand(ioc))
        , stream_(net::make_strand(ioc))
        {
            LOG(INFO) << __FUNCTION__ << "Created Session object";
        }
    //start the async operation
    void run(char const* host, char const* port, char const* target, int version)
    {
        req_.version(version);
        req_.method(http::verb::get);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        resolver_.async_resolve(host, port,
                                beast::bind_front_handler(&session::on_resolve, shared_from_this())
                                );
    }
    void on_resolve(beast::error_code ec, tcp::resolver::results_type results)
    {
        LOG(INFO) << __FUNCTION__;
        if(ec)
        {
            return fail(ec, "resolve");
        }
        //set a timeout
        stream_.expires_after(std::chrono::seconds(30));
        //make ip connection
        stream_.async_connect(results,
                              beast::bind_front_handler(&session::on_connect, shared_from_this())
                              );
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        LOG(INFO) << __FUNCTION__;
        if(ec)
        {
            return fail(ec, "connect");
        }
        //set a timeout
        stream_.expires_after(std::chrono::seconds(30));
        //send http to the remote host
        http::async_write(stream_, req_,
                          beast::bind_front_handler(&session::on_write, shared_from_this())
                          );

    }
    void on_write(beast::error_code ec, std::size_t bytes_transfered)
    {
        LOG(INFO) << __FUNCTION__;
        boost::ignore_unused(bytes_transfered);
        if(ec)
        {
            return fail(ec, "write");
        }
        //receive http response
        http::async_read(stream_, buffer_, res_,
                         beast::bind_front_handler(&session::on_read, shared_from_this())
                         );
    }

    void on_read(beast::error_code ec, std::size_t bytes_transfered)
    {
        LOG(INFO) << __FUNCTION__;
        boost::ignore_unused(bytes_transfered);
        if(ec)
        {
            return fail(ec, "read");
        }
        std::cout << res_ << "\n";
        stream_.socket().shutdown(tcp::socket::shutdown_both, ec);
        if(ec && ec != beast::errc::not_connected)
            return fail(ec, "shutdown");
    }
private:
    tcp::resolver resolver_;
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    http::request<http::empty_body> req_;
    http::response<http::string_body> res_;
};

#endif //MULTIPLYTEST_ASYNC_HTTP_SERVER_H
