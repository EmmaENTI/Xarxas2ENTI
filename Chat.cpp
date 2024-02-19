#include "Chat.h"

void Chat::ShowMessage(std::string message)
{
    _messageMutex.lock();
    _messages.push_back(message);
    _messageMutex.unlock();

    ConsoleControl::LockMutex();
    std::cout << std::endl << message;
    ConsoleControl::UnlockMutex();
}

void Chat::ShowAlert(std::string message)
{
    ConsoleControl::LockMutex();
    ConsoleControl::SetColor(ConsoleControl::YELLOW);
    std::cout << std::endl << message;
    ConsoleControl::SetColor(ConsoleControl::WHITE);
    ConsoleControl::UnlockMutex();
}

void Chat::ShowError(std::string message)
{
    ConsoleControl::LockMutex();
    ConsoleControl::SetColor(ConsoleControl::RED);
    std::cout << std::endl << message;
    ConsoleControl::SetColor(ConsoleControl::WHITE);
    ConsoleControl::UnlockMutex();
}

void Chat::ListenClientsConnections(unsigned short port)
{
    sf::TcpListener listener;

    if (listener.listen(port) != sf::Socket::Done)
    {
        ShowError("Error on Start Listener");
        return;
    }
    sf::IpAddress ipAdress = sf::IpAddress::getLocalAddress();  //El client sha de conectar a tu, aixi que has de saber la ip previament
    //En aquest cas es red local
    ShowAlert("Listening on IP: " + ipAdress.toString());

    while (true)
    {
        sf::TcpSocket* client = new sf::TcpSocket();
        sf::Socket::Status status = listener.accept(*client);

        switch (status)
        {
        case sf::Socket::Done: {
            //Client Done
            std::thread clientThread = std::thread(&Chat::OnClientEnter, this, client);
            clientThread.detach();
        }
            break;
        case sf::Socket::NotReady:
            //Client Not ready
            break;
        case sf::Socket::Partial:
            //Client Partial
            break;
        case sf::Socket::Disconnected:
            //Client Disconnected
            break;
        case sf::Socket::Error:
            //Client Error
            break;
        default:
            break;
        }
    }
}

void Chat::OnConnectToServer(std::string ip, unsigned short port)
{
    sf::TcpSocket* socket = new sf::TcpSocket();
    sf::Socket::Status status = socket->connect(ip, port);

    if (status != sf::Socket::Done)
    {
        ShowError("Error on connect to server");
        return;
    }

    _socketsMutex.lock();
    _sockets.push_back(socket);
    _socketsMutex.unlock();

    ShowAlert("Connected to Server");

    //Listen Keyboard
    std::thread keyboardThread = std::thread(&Chat::ListenKeyboardToSendMessage, this);
    keyboardThread.detach();

    std::thread listenMessageThread = std::thread(&Chat::ListenMessages, this, socket);
    listenMessageThread.detach();
}

void Chat::OnClientEnter(sf::TcpSocket* client)
{
    _socketsMutex.lock();
    _sockets.push_back(client);
    _socketsMutex.unlock();

    ShowAlert("Client Accepted IP:" + client->getRemoteAddress().toString());

    ListenMessages(client);
}

void Chat::ListenMessages(sf::TcpSocket* socket)
{
    while (true)
    {
        sf::Packet packet;

        std::string message;

        if (socket->receive(packet) != sf::Socket::Done) 
        {
            ShowError("Error recieve message");
        }
        else
        {
            //Si poses un msg y despres nom, has de treure per ordre, primer nom  y despres msg.
            packet >> message;
            ShowMessage(message);

            _isServerMutex.lock();
            bool isServer = _isServer;
            _isServerMutex.unlock();

            if (isServer)
            {
                SendMessage(message);
            }
        }
    }
}

void Chat::ListenKeyboardToSendMessage()
{
    std::string message = "";
    while (true)
    {
        char c = ConsoleControl::WaitForReadNextChar();

        if (c == KB_Enter)
        {
            SendMessage(message);

            //Si som server ens mostrem el msg tambe
            _isServerMutex.lock();
            bool isServer = _isServer;
            _isServerMutex.unlock();

            if (isServer)
            {
                ShowMessage(message);
            }

            message = "";
        }
        else
        {
            message += c;
        }
    }
}

void Chat::SendMessage(std::string message)
{
    sf::Packet packet;
    packet << message;

    _socketsMutex.lock();
    for (sf::TcpSocket* socket : _sockets)
    {
        if (socket->send(packet) != sf::Socket::Done)
        {
            ShowError("Error Sending Message");
        }
    }
    _socketsMutex.unlock();
}

Chat* Chat::Server(unsigned short port)
{
    Chat* chat = new Chat();

    chat->_serverAddress = sf::IpAddress::getLocalAddress();

    chat->_isServer = true;
    //Listen Clients
    std::thread listenerThread = std::thread(&Chat::ListenClientsConnections, chat, port);
    listenerThread.detach();

    //Listen Keyboard
    std::thread keyboardThread = std::thread(&Chat::ListenKeyboardToSendMessage, chat);
    keyboardThread.detach();

    return chat;
}

Chat* Chat::Client(std::string ip, unsigned short port)
{
    Chat* chat = new Chat();
    chat->_serverAddress = sf::IpAddress(ip);
    chat->OnConnectToServer(ip, port);

    return chat;
}
