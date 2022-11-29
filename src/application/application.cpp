//
// Created by feres on 11/26/22.
//
#include <iostream>
#include "glog/logging.h"
#include "http/async_http_server.h"

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    LOG(INFO) << "started application";
    if(argc != 4 && argc != 5)
    {
        std::cerr <<
                  "Usage: http-client-async <host> <port> <target> [<HTTP version: 1.0 or 1.1(default)>]\n" <<
                  "Example:\n" <<
                  "    http-client-async www.example.com 80 /\n" <<
                  "    http-client-async www.example.com 80 / 1.0\n";
        return EXIT_FAILURE;
    }
    auto const host = argv[1];
    auto const port = argv[2];
    auto const target = argv[3];
    int version = argc == 5 && !std::strcmp("1.0", argv[4]) ? 10 : 11;

    net::io_context ioc;

    std::make_shared<session>(ioc)->run(host, port, target, version);

    ioc.run();
//    std::cout << "hello there!\n";
}