#include <iostream>
#include "NetCommon.h"
#include "NetMessage.h"
#include "NetClient.h"

enum class CustomMsgTypes :uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomClient : public basti::net::client_interface<CustomMsgTypes>
{
public:
	void PingServer()
	{
		basti::net::Message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		msg << timeNow;
		Send(msg);
	}

};

int main()
{
	int count = 0;

	CustomClient c;
	bool bConnectOk = c.Connect("localhost", 60000);

	if (bConnectOk) std::cout << "Client Successfully connected to the server...\n";
	else std::cout << "Client failed to connect to the server...\n";


	bool key[3] = { false, false, false };
	bool old_key[3] = { false, false, false };

	bool bQuit = false;

	while (!bQuit)
	{

		bool isConnectedStart = c.IsConnected();

		
		
		if (GetForegroundWindow() == GetConsoleWindow())
		{
			key[0] = GetAsyncKeyState('1') & 0x8000;
			key[1] = GetAsyncKeyState('2') & 0x8000;
			key[2] = GetAsyncKeyState('3') & 0x8000;
		}

		bool isConnectedEnd = c.IsConnected();

		if (isConnectedStart && !isConnectedEnd)
		{
			std::cout << "Count: " << count << " Connection broke...\n";
		}

		count++;

		if (key[0] && !old_key[0]) c.PingServer();
		if (key[2] && !old_key[2]) bQuit = true;

		for (int i = 0; i < 3; i++)
		{
			old_key[i] = key[i];
		}
		

		bool bIsConnected = c.IsConnected();

		if (bIsConnected)
		{
			
			

			if (!c.Incoming().empty())
			{
				auto msg = c.Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerPing:
				{
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
				}
				break;
				}
			}

			
		}
		else
		{
			

			std::cout << "Sorry, Server Down\n";
			bQuit = true;
		}
	}

	system("pause");

	return 0;
}