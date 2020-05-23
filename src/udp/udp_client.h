#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
boost::shared_mutex io_mutex;

namespace
{
int p2p_connect = 0;
}

using boost::asio::ip::udp;
class udp_client
{
public:
    udp_client(boost::asio::io_service& io_service, const char* host, const char* port)
        : _sock(io_service)
    {
        udp::resolver _resolver(io_service);
        udp::resolver::query _query(udp::v4(), host, port);
        _server_endpoint = *_resolver.resolve(_query);
        _sock.open(udp::v4());
        start_send(); //**Must send first**
    }

    void start_send();
    void session_send();
    void handle_send(const boost::system::error_code& ec, std::size_t len);
    void session_receive();
    void handle_recevie(const boost::system::error_code& ec, std::size_t len);
    [[noreturn]] void p2p_receive(udp::socket& sock, udp::endpoint& peer_endpoint);
    void p2p_send(udp::socket* sock, udp::endpoint* peer_endpoint);

private:
    udp::socket _sock;
    udp::endpoint _server_endpoint;
    boost::array<char, 512> _recv_buffer;
    std::string _write_message;
};
#endif // UDP_CLIENT_H
