#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include "Chat.h"

//CHAT DEL ABRAHAM
void RunClient();
void RunServer();

unsigned short port = 3001;

int main()
{
    std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;
    char mode = ' ';

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

    std::cout << std::endl << "Set Server IP-> ";
    std::string ip;
    std::getline(std::cin, ip);

    Chat* chat = Chat::Client(ip, port);


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
