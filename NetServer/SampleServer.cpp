#include <iostream>
#include "BastiNet.h"

enum class CustomMsgTypes :uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomServer : public basti::net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : basti::net::server_interface<CustomMsgTypes>(nPort)
	{
		
	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<basti::net::connection<CustomMsgTypes>> client)
	{
		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<basti::net::connection<CustomMsgTypes>> client)
	{
		
	}

	virtual void OnMessage(std::shared_ptr<basti::net::connection<CustomMsgTypes>> client, basti::net::Message<CustomMsgTypes>& msg)
	{
		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "] Server Ping\n";

			client->Send(msg);
		}
		break;
		}
	
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update();
	}
	return 0;
}