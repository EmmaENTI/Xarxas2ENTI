#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include "Chat.h"
#include <SFML/Graphics.hpp>
#include "ConnectionAbstraction/SocketsManager.h"

//CHAT DEL ABRAHAM
void RunClient();
void RunServer();
void RunWindows();
unsigned short port = 3001;

enum PackagesIds: Packet::PacketKey { Message = 0 };

int main()
{
    std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;
    char mode = ' ';



    //RunWindows();

    do
    {
        //Se le pone el Wait por el bloqueante
        mode = ConsoleControl::WaitForReadNextChar();
    } while (mode != 'C' && mode != 'c' && mode != 'S' && mode != 's');

    switch (mode)
    {
    case 'C':

    case 'c':
    {
        RunClient();
        break;
        break;
    }
    case 'S':

    case 's':
    {
        RunServer();
        break;
    }

    default:
        break;
    }

    while (true)
    {

    }
}

void RunClient()
{
    std::cout << "Client";
    std::cout << std::endl << "Set Server IP --> ";
    std::string ip;
    std::getline(std::cin, ip);

    SocketsManager* SM = new SocketsManager([](TcpSocket* socket)
        {
            std::cout << std::endl << "Socket Connected: " << socket->getRemoteAddress().toString();

            socket->Subscribe(Message, [socket](Packet packet)
                {
                    std::string message;
                    packet >> message;
                    std::cout << std::endl << "New Message: " << message;
                });

            socket->SubscribeOnDisconnect([](TcpSocket* socket)
                {
                    std::cout << std::endl << "Socket disconnected: " << socket->getRemoteAddress().toString();
                });

            std::string message = "Hola soy el cliente";
            Packet packet;
            packet << message;

            socket->Send(Message, packet);

    });

    if (SM->ConnectToServer(ip, port))
    {
        SM->StartLoop();
    }

    //OLD
    /*sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("10.40.1.123", port);

    if (status != sf::Socket::Done)
    {
        std::cout << std::endl << "Error on connect to server";
        return;
    }

    while (true)
    {
        std::cout << std::endl << "Next Message: ";
        std::string message;
        std::getline(std::cin, message);

        char data[100];

        int stringSize = message.length();
        for (int i = 0; i < stringSize; i++)
        {
            char c = message[i];
            data[i] = c;
        }

        if (socket.send(data, 100) != sf::Socket::Done)
        {
            std::cout << std::endl << "Error Sending Message";
        }
    }*/
}

void RunServer()
{
    //Crear un listener para escuchar las conexiones.
    std::cout << "Server";
    Chat* chat = Chat::Server(port);

    SocketsManager* SM = new SocketsManager([](TcpSocket* socket)
        {
            std::cout << std::endl << "Socket Connected: " << socket->getRemoteAddress().toString();

            socket->Subscribe(Message, [socket](Packet packet)
                {
                    std::string message;
                    packet >> message;
                    std::cout << std::endl << "New Message: " << message;

                    std::string response = "Pues yo soy el server";
                    Packet responsePacket;
                    responsePacket << response;

                    socket->Send(Message, responsePacket);
                });

            socket->SubscribeOnDisconnect([](TcpSocket* socket)
                {
                    std::cout << std::endl << "Socket disconnected: " << socket->getRemoteAddress().toString();
                });
        });

    if (SM->StartListener(port))
    {
        sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
        std::cout << "Listening on IP: " << ipAddress.toString();
        SM->StartLoop();
    }

    /*sf::TcpListener listener;

    if (listener.listen(port) != sf::Socket::Done)
    {
        std::cout << std::endl << "Error on start listener";
        return;
    }
    sf::IpAddress ipAdress = sf::IpAddress::getLocalAddress();  //El client sha de conectar a tu, aixi que has de saber la ip previament
    //En aquest cas es red local

    std::cout << std::endl << "Listening on IP: " + ipAdress.toString();

    sf::TcpSocket client;

    if (listener.accept(client) != sf::Socket::Done)
    {
        std::cout << std::endl << "Error on accept client";
        return;
    }

    std::cout << std::endl << "Client Connected " << client.getRemoteAddress().toString();
    
    while (true)
    {
        char data[100];
        std::size_t received;

        std::string message;

        if (client.receive(data, 100, received) != sf::Socket::Done)
        {
            std::cout << std::endl << "Error recieve message";
        }
        else 
        {
            for (size_t i = 0; i < received; i++)
            {
                char c = data[i];
                message += c;


                std::cout << std::endl << message;
            }
        }
    }*/
}

void RunWindows()
{
    sf::RenderWindow windows;
    windows.create(sf::VideoMode(800, 600), "Chat");

    sf::Text label;

    sf::Font font;
    font.loadFromFile("Minecraft.ttf");

    label.setFont(font);
    label.setCharacterSize(16);
    label.setFillColor(sf::Color::White);
    label.setString("Hello World");
    label.setPosition(0, 0);

    while (windows.isOpen())
    {
        windows.clear(sf::Color::Black);
        windows.draw(label);
        windows.display();
    }

}