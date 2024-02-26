#include "TcpSocket.h"

bool TcpSocket::Connect(std::string ip, unsigned short port)
{
	return connect(ip, port) == sf::Socket::Done;
}

void TcpSocket::Receive()
{
	//Crear paket
	Packet packet;
	sf::Socket::Status status = receive(packet);

	switch (status)
	{
	case sf::Socket::Done:
		ProcessPacket(packet);
		break;
	case sf::Socket::Disconnected:
		_onSocketDisconnectMutex.lock();
		for (OnSocketDisconnect disconnect : _onSocketDisconnectList)
		{
			disconnect(this);
		}
		_onSocketDisconnectList.clear();
		_onSocketDisconnectMutex.unlock();
		break;
	case sf::Socket::Partial:
	case sf::Socket::NotReady:
	case sf::Socket::Error:
	default:


		break;
	}
}

bool TcpSocket::Send(Packet::PacketKey key)
{
	Packet signedPacket;
	signedPacket << key;


	return send(signedPacket) == sf::Socket::Done;
}

bool TcpSocket::Send(Packet::PacketKey key, Packet packet)
{
	Packet signedPacket;
	signedPacket << key;
	
	signedPacket.append(packet.getData(), packet.getDataSize());
	return send(signedPacket) == sf::Socket::Done;;
}

void TcpSocket::Subscribe(Packet::PacketKey key, OnRecievePacket onReceivePacket)
{
	_subscriptionsMutex.lock();
	_subscriptions[key] = onReceivePacket; //TODO hacer multiples subscripciones
	_subscriptionsMutex.unlock();
}

void TcpSocket::SubscribeAsync(Packet::PacketKey key, OnRecievePacket onReceivePacket)
{
	std::thread* subscriveThread = new std::thread(&TcpSocket::Subscribe, this, key, onReceivePacket);
	subscriveThread->detach();
}

void TcpSocket::SubscribeOnDisconnect(OnSocketDisconnect onSocketDisconnect)
{
	_onSocketDisconnectMutex.lock();
	_onSocketDisconnectList.push_back(onSocketDisconnect);
	_onSocketDisconnectMutex.unlock();
}

void TcpSocket::ProcessPacket(Packet packet)
{
	Packet::PacketKey key;
	packet >> key;

	std::map<Packet::PacketKey, OnRecievePacket>::iterator it;

	_subscriptionsMutex.lock();
	it = _subscriptions.find(key);
	if (it != _subscriptions.end())
	{
		it->second(packet);
	}

	_subscriptionsMutex.unlock();
}
