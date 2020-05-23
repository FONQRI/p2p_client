#include "udp_client.h"

void udp_client::start_send()
{
    _sock.send_to(boost::asio::buffer("help"), _server_endpoint);
    session_receive();
}

void udp_client::session_send()
{
    std::getline(std::cin, _write_message);
    _sock.async_send_to(boost::asio::buffer(_write_message), _server_endpoint,
                        boost::bind(&udp_client::handle_send, this, boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
}

void udp_client::handle_send(const boost::system::error_code&, std::size_t)
{
    //if(p2p_connect)
    //    return;
    //else
    //    session_send();
}

void udp_client::session_receive()
{
    _sock.async_receive_from(boost::asio::buffer(_recv_buffer), _server_endpoint,
                             boost::bind(&udp_client::handle_recevie, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

void udp_client::handle_recevie(const boost::system::error_code&, std::size_t len)
{
    std::string receive_message(_recv_buffer.data(), len);
    if (strncmp(receive_message.c_str(), "PUNCH_SUCCESS", 13) == 0)
    {
        //punch finished
        //start a p2p session to remote peer
        std::cout << receive_message << std::endl;
        p2p_connect = 1;
        char str_endpoint[127];
        strcpy(str_endpoint, receive_message.c_str() + 14);
        char* peer_ip = strtok(str_endpoint, ":");
        char* peer_port = strtok(nullptr, ":");
        udp::endpoint request_peer(boost::asio::ip::address::from_string(peer_ip),
                                   static_cast<unsigned short>(std::atoi(peer_port)));
        _sock.send_to(boost::asio::buffer("Sender peer connection complete."), request_peer);
        boost::thread(boost::bind(&udp_client::p2p_send, this, &_sock, &request_peer));
        p2p_receive(_sock, request_peer);
    }
    else if (strncmp(receive_message.c_str(), "PUNCH_REQUEST", 13) == 0)
    {
        //send something to request remote peer
        //and start a p2p session
        std::cout << receive_message << std::endl;
        p2p_connect = 1;
        char str_endpoint[127];
        strcpy(str_endpoint, receive_message.c_str() + 14);
        char* peer_ip = strtok(str_endpoint, ":");
        char* peer_port = strtok(nullptr, ":");
        udp::endpoint request_peer(boost::asio::ip::address::from_string(peer_ip),
                                   static_cast<unsigned short>(std::atoi(peer_port)));

        std::cin.clear(std::cin.rdstate() & std::cin.eofbit);
        _sock.send_to(boost::asio::buffer("Receiver peer connection complete."), request_peer);
        boost::thread(boost::bind(&udp_client::p2p_send, this, &_sock, &request_peer));
        p2p_receive(_sock, request_peer);
    }
    else
    {
        std::cout << receive_message << std::endl;
    }
    session_receive();
    if (p2p_connect)
        return;
    else
        session_send();
}

void udp_client::p2p_receive(udp::socket& sock, udp::endpoint& peer_endpoint)
{
    while (true)
    {
        boost::system::error_code error;
        //blocked until successfully received
        size_t len = sock.receive_from(boost::asio::buffer(_recv_buffer), peer_endpoint, 0, error);
        std::string receive_message(_recv_buffer.data(), len);
        std::cout << receive_message << std::endl;
    }
}

void udp_client::p2p_send(udp::socket* sock, udp::endpoint* peer_endpoint)
{
    while (std::getline(std::cin, _write_message))
    {
        sock->send_to(boost::asio::buffer(_write_message), *peer_endpoint); //blocked
    }
}
