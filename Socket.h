#ifndef __SOCKET__H__
#define __SOCKET__H__

#include <iostream>
#include <string>

#include "SocketException.h"

#ifdef __unix__
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <sys/un.h>
#   include <netinet/in.h>
#   include <netdb.h>
#   include <arpa/inet.h>
#else
#   include <winsock2.h>
#   include <Ws2tcpip.h>
#   include <windows.h>
#   pragma comment( lib, "Ws2_32.lib")
#endif

typedef int SocketProtocol;
typedef int SocketMode;
typedef int IPVersion;
class Socket
{
public:
    enum SocketModes
    {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM,
        RAW = SOCK_RAW
    };
    enum SocketDomains
    {
        IPv4 = AF_INET,
        IPv6 = AF_INET6
    };
protected:
#ifdef __unix__
    int             m_SocketDescriptor;
#else
    SOCKET          m_SocketDescriptor;
#endif
    SocketProtocol  m_SocketProtocol;
    int             m_Domain;
    SocketMode      m_Mode;

    unsigned short  m_ConnectionQueueSize;

    unsigned int    m_IPAddress;
    unsigned short  m_PortNumber;
    int             m_ErrorFlag;

    std::string     m_ErrorString;

    virtual void    ProvideErrorString();

private:
    bool            SocketInit();

public:
    Socket();
    Socket(SocketMode Mode, SocketProtocol Protocol = 0, int Domain = IPv4, unsigned short MaxConnection = SOMAXCONN );
    virtual ~Socket();

    virtual int             SetProtocol(SocketProtocol Protocol);
    virtual int             SetProtocol(const char* ProtocolName);
    virtual int             SetProtocol(std::string ProtocolName);

    virtual int             SetDomain(int Domain);
#ifdef __unix__
    virtual int             Create();
#elif _WIN32
    virtual SOCKET          Create();
#endif
    virtual int             Bind(unsigned short PortNumber);
    virtual int             Bind(unsigned short PortNumber, const char* IPAddress);
    virtual int             Bind(unsigned short PortNumber, std::string& IPAddress);

    virtual int             Listen();
    virtual int             Listen(unsigned short PortNumber);

    virtual int             Connect(const char* IPAddress, unsigned short PortNumber);
    virtual int             Connect(std::string& IPAddress, unsigned short PortNumber);

    virtual Socket*         Accept();

    virtual int             Read(const void* Buffer, int Size);
    virtual int             Write(const void* Buffer, int Size);

    virtual int             ReadFrom(void* Buffer, int Size);
    virtual int             WriteTo(void* Buffer, int Size, const char* DestinationHost, unsigned short PortNumber);

    virtual bool            Close();

    virtual SocketMode      GetMode();
    virtual SocketProtocol  GetProtocol();

    virtual int             GetError() const;
    virtual std::string&    GetErrorString();

    static SocketProtocol GetSocketProtocol(const char* ProtocolName);

};

#endif//__SOCKET__H__
