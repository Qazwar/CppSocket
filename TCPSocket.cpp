#include "TCPSocket.h"

TCPSocket::TCPSocket()
    : Socket(Socket::TCP,Socket::GetSocketProtocol("TCP")),
    m_Connected(false),
    m_TCPMode(TCPSocket::NotSet)
{ }

TCPSocket::TCPSocket(TCPSocket& TCPSocketObject)
    : Socket(TCPSocketObject),
    m_Connected(TCPSocketObject.m_Connected),
    m_PortNumber(TCPSocketObject.m_PortNumber),
    m_TCPMode(TCPSocketObject.m_TCPMode)
{ }

TCPSocket::TCPSocket(Socket& SocketObject)
    : Socket(SocketObject),
    m_Connected(false),
    m_PortNumber(0),
    m_TCPMode(TCPSocket::NotSet)
{ }

TCPSocket::TCPSocket(unsigned short PortNumber)
    : Socket(Socket::TCP, Socket::GetSocketProtocol("TCP")),
    m_Connected(false),
    m_TCPMode(TCPSocket::Server)
{
    this->Create();
    this->Listen(PortNumber);
}

TCPSocket::TCPSocket(const char* IPAddress, unsigned short PortNumber)
    : Socket(Socket::TCP, Socket::GetSocketProtocol("TCP")),
    m_Connected(true),
    m_TCPMode(TCPSocket::Client)
{
    this->Create();
    try
    {
        this->Connect(IPAddress, PortNumber);
    }
    catch(SocketException& e)
    {
        m_Connected = false;
        throw SocketException(e.what());
    }
}

TCPSocket::TCPSocket(std::string& IPAddress, unsigned short PortNumber)
    : Socket(Socket::TCP, Socket::GetSocketProtocol("TCP")),
    m_Connected(true),
    m_TCPMode(TCPSocket::Client)
{
    this->Create();
    try
    {
        this->Connect(IPAddress, PortNumber);
    }
    catch(SocketException& e)
    {
        m_Connected = false;
        throw SocketException(e.what());
    }
}

TCPSocket::~TCPSocket()
{
    if(m_Connected)
        this->Disconnect();
}

int TCPSocket::Connect(const char* IPAddress, unsigned short PortNumber)
{
    return Socket::Connect(IPAddress, PortNumber);
}

int TCPSocket::Connect(std::string& IPAddress, unsigned short PortNumber)
{
    return Socket::Connect(IPAddress, PortNumber);
}

int TCPSocket::Disconnect()
{
    if(!m_Connected)
    {
        throw SocketException("TCPSocket is not connected!");
        return -1;
    }
    this->m_Connected = false;
    return Socket::Close();
}

TCPSocket* TCPSocket::Accept()
{
    Socket *pointer = Socket::Accept();
    TCPSocket* sock = new TCPSocket(*pointer);
    sock->m_Connected = true;
    sock->m_TCPMode = TCPSocket::Client;
    sock->m_PortNumber = this->m_PortNumber;

    delete pointer;
    return sock;
}

int TCPSocket::Send(const void* Buffer, int Size)
{
    if(m_Connected || m_TCPMode == Server)
        return Socket::Write(Buffer, Size);

    throw SocketException("TCPSocket not connected.");
    return -1;
}

int TCPSocket::Receive(const void* Buffer, int Size)
{
    return Socket::Read(Buffer, Size);
}

bool TCPSocket::Close()
{
    this->m_Connected = false;
    return Socket::Close();
}
