#include "endpoint.h"
#include <boost/asio.hpp>
#include <iostream>

int client_end_point()
{
	std::string raw_ip_address = "127.4.8.1";
	unsigned short port_num = 3333;

	boost::system::error_code ec;
	boost::asio::ip::address ip_address = boost::asio::ip::address::from_string(raw_ip_address, ec);
	if (ec.value() != 0)
	{
		std::cout << "Failed to parse the IP address. Error code = "
			<< ec.value() << ". Message: " << ec.message() << std::endl;
		return ec.value();
	}

	boost::asio::ip::tcp::endpoint ep(ip_address, port_num);
	return 0;
}

int server_end_point()
{
	unsigned short port_num = 3333;

	boost::asio::ip::address ip_address = boost::asio::ip::address_v4::any();

	boost::asio::ip::tcp::endpoint ep(ip_address, port_num);
	return 0;
}

int create_tcp_socket()
{
	boost::asio::io_context ioc;
	boost::asio::ip::tcp::socket sock(ioc);

	boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
	boost::system::error_code ec;
	sock.open(protocol, ec);
	if (ec.value() != 0)
	{
		std::cout << "Failed to open the socket! Error code = "
			<< ec.value() << ". Message: " << ec.message() << std::endl;
		return ec.value();
	}
	return 0;
}

int create_acceptor_socket()
{
	boost::asio::io_context ioc;
	/*
	boost::asio::ip::tcp::acceptor acceptor(ioc);

	boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
	boost::system::error_code ec;
	acceptor.open(protocol, ec);
	if (ec.value() != 0)
	{
		std::cout << "Failed to open the acceptor socket! Error code = "
			<< ec.value() << ". Message: " << ec.message() << std::endl;
		return ec.value();
	}
	*/

	boost::asio::ip::tcp::acceptor a(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3333));
	return 0;
}

int bind_accptor_socket()
{
	unsigned short port_num = 3333;
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), port_num);

	boost::asio::io_context ioc;
	boost::asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());
	boost::system::error_code ec;
	acceptor.bind(ep, ec);
	if (ec.value() != 0)
	{
		std::cout << "Failed to bind the acceptor socket! Error code = "
			<< ec.value() << ". Message: " << ec.message() << std::endl;
		return ec.value();
	}
	return 0;
}

int connect_to_end()
{
	std::string raw_ip_address = "192.168.1.124";
	unsigned short port_num = 3333;
	try
	{
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(raw_ip_address), port_num);
		boost::asio::io_context ioc;
		boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
		sock.connect(ep);
	}
	catch (boost::system::error_code& e)
	{
		std::cout << "Error occured! Error code = " 
			<< e.value() << ". Message: " << e.value() << std::endl;
		return e.value();
	}
	return 0;
}
