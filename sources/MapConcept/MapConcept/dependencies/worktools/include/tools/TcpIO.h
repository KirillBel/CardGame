#pragma once

#define TCPIO_RECV_BUF_SIZE 65536

class TcpIO;
class TcpAcceptTh: public BaseThread
{
public:
	TcpAcceptTh() {};
	~TcpAcceptTh() {};

	void init(TcpIO* pParent, SOCKET sock)
	{
		this->pParent=pParent;
		this->sock=sock;
	};

	eThreadReturn run();

private:
	SOCKET sock;
	TcpIO* pParent;
};

class TcpClient;
class TcpListenTh: public BaseThread
{
public:
	TcpListenTh() {};
	~TcpListenTh() {};

	void init(TcpClient* pParent, SOCKET sock)
	{
		this->pParent=pParent;
		this->sock=sock;
	};

	eThreadReturn run();

private:
	SOCKET sock;
	TcpClient* pParent;
};

class TcpClient
{
public:
	TcpClient(SOCKET sock, sIpAddr addr)
	{
		InitializeCriticalSectionAndSpinCount(&CS1, 0x00000400);
		InitializeCriticalSectionAndSpinCount(&CS2, 0x00000400);
		this->sock=sock;
		this->addr=addr;
	};

	virtual ~TcpClient(void) 
	{
		Release();
	};

	void Release();
	void setAsync();

	void StartListen();
	void StopListen();
	bool IsListening();

	int Send(void* pData, int size);
	int Send(const char* str);
	int SendTo(void* pData, int size,sIpAddr& addr);
	int SendTo(const char* str,sIpAddr& addr);

	bool ReceiveAsync(sInetPacket* pData, DWORD dwWaitTimeout=0xFFFFFFFF);
	bool Receive(sInetPacket* pData=0);

	int getRecvMessageCount() {return packets.size();};
	bool getRecvMessage(sInetPacket& packet) {return pop_message(packet);};

	sIpAddr getAddr() {return addr;};
	SOCKET getSocket() {return sock;};

protected:
	friend class TcpListenTh;
	void push_message(sInetPacket& packet);
	bool pop_message(sInetPacket& packet);
protected:
	sIpAddr addr;
	SOCKET sock;
	TcpListenTh listenTh;
	WSAEVENT receiveEvt;
	CRITICAL_SECTION CS1,CS2;
	std::list<sInetPacket> packets;
	char recvBuf[TCPIO_RECV_BUF_SIZE];
};

class TcpIO
{
public:
	TcpIO(bool bAsync=false);
	~TcpIO(void);

	bool Init(bool bAsync=false);
	void Release();

	bool Bind(USHORT port);
	TcpClient* Connect(sIpAddr& addr);
	void StartAccept();
	void StopAccept();

	TcpClient* WaitAccept();
	TcpClient* WaitAcceptAsync(DWORD time=0xFFFFFFFF);

	int findClient(sIpAddr& addr);
	int findClient(SOCKET sock);

	bool processNewClient(TcpClient** client);
	int getNewClientCount();

	TcpClient* getAcceptedClient(int index);
	int getAcceptedClientCount();
protected:
	bool CreateSocket();
	void CloseSocket();
	void RestartSocket();

	TcpClient* push_client(SOCKET s, sIpAddr& addr);
	bool pop_client(TcpClient** client=0);

	TcpClient* OnClientAccepted(SOCKET s, sIpAddr& addr);

protected:
	bool bWSAStarted;
	WSADATA wsd;
	SOCKET tcpSock;
	bool bAsyncSocket;

	WSAEVENT acceptEvt;

	std::list<TcpClient*> added_clients;
	std::vector<TcpClient*> clients;

	TcpAcceptTh acceptTh;
	CRITICAL_SECTION CS1;
};

