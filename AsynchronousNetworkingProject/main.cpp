#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


std::vector<char> vBuffer(20 * 1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
	socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				for (int i = 0; i < length; i++)
				{
					std::cout << vBuffer[i];

				}

				GrabSomeData(socket);

			}
		}
	);
}

int main()
{

	asio::error_code ec;

	asio::io_context context;

	asio::io_context::work idleWork(context);

	std::thread thrContext = std::thread([&]() { context.run(); });

	std::string ipAddr = "93.184.216.34";
	int port = 80;

	asio::ip::tcp::endpoint endpoint(asio::ip::make_address(ipAddr, ec), port);

	asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, ec);

	if (!ec)
	{
		std::cout << "Connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}

	if (socket.is_open())
	{

		GrabSomeData(socket);

		std::string request = "GET /index.html HTTP/1.1\r\n";
		request += "Host: example.com\r\n";
		request += "Connection: close\r\n\r\n";



		socket.write_some(asio::buffer(request.data(), request.size()), ec);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20000ms);

	}

	system("pause");
	return 0;
}