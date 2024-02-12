#pragma once
#include <string>
#include <SFML/Network.hpp>
#include <vector>
#include <thread>
#include <list>
#include <mutex>
#include <iostream>
#include "ConsoleControl.h"

class Chat
{
private:
	bool _isServer = false;
	std::list<sf::TcpSocket*> _sockets;
	std::mutex _isServerMutex;

	std::vector<std::string> _messages;
	std::mutex _messageMutex; //La mayoria de mutex nos interesan por puntero

	sf::IpAddress _serverAddress;

	Chat() {};

	void ShowMessage(std::string message);
	void ShowAlert(std::string message);
	void ShowError(std::string message);

	void ListenClientsConnections(unsigned short port);
	void OnConnectToServer(std::string ip, unsigned short port);
	void OnClientEnter(sf::TcpSocket* client);
	void ListenMessages(sf::TcpSocket* socket);
	void ListenKeyboardToSendMessage();
	void SendMessage(std::string message);

public:
	static Chat* Server(unsigned short port);
	static Chat* Client(std::string ip, unsigned short port);
};

