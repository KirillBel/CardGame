#pragma once
#define UDPIO_RECV_BUF_SIZE 65536
struct sIpAddr
{
	sIpAddr()
	{
		addr=0;
		port=0;
	};

	sIpAddr(sockaddr_in& addr)
	{
		*this=sIpAddr(addr.sin_addr.S_un.S_un_b.s_b1,
			addr.sin_addr.S_un.S_un_b.s_b2,
			addr.sin_addr.S_un.S_un_b.s_b3,
			addr.sin_addr.S_un.S_un_b.s_b4,
			ntohs(addr.sin_port));
	};

	sIpAddr(BYTE b1, BYTE b2, BYTE b3, BYTE b4, USHORT port)
	{
		IP(0)=b1;
		IP(1)=b2;
		IP(2)=b3;
		IP(3)=b4;

		this->port=port;
	};

	sIpAddr(const char* ip, USHORT port)
	{
		this->addr=inet_addr(ip);
		this->port=port;
	};

	sIpAddr(DWORD ip, USHORT port)
	{
		this->addr=ip;
		this->port=port;
	};

	void convert(sockaddr_in& addr)
	{
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = this->addr;
		addr.sin_port = htons(port);
	};

	BYTE& IP(BYTE num)
	{
		_ASSERT((num>=0) && (num<4));
		return ((BYTE*)&addr)[num];
	};

	std::string toString(bool bWithPort=false)
	{
		if(!bWithPort)
			return strkit::format("%u.%u.%u.%u",(int)IP(0),(int)IP(1),(int)IP(2),(int)IP(3));
		return strkit::format("%u.%u.%u.%u:%u",(int)IP(0),(int)IP(1),(int)IP(2),(int)IP(3),(int)port);
	};

	bool operator == (sIpAddr& other)
	{
		return ((addr==other.addr) && (port==other.port));
	};

	bool operator != (sIpAddr& other)
	{
		return !(this->operator==(other));
	};

	DWORD addr;
	USHORT port;
};

struct sInetPacket
{
	sInetPacket()
	{
		pData=0;
		dataSize=0;
	};

	sInetPacket(sIpAddr& Addr, void* pData,int dataSize) : addr(Addr)
	{
		this->pData=new BYTE[dataSize+1];
		memcpy(this->pData,pData,dataSize);
		((BYTE*)this->pData)[dataSize]=0;
		this->dataSize=dataSize;
	};

	~sInetPacket()
	{
	};

	void release()
	{
		if(pData) 
		{
			delete [] pData;
			pData=0;
		};
		dataSize=0;
	};

	sIpAddr addr;
	void* pData;
	int dataSize;
};

class UdpIO;
class UdpListenTh: public BaseThread
{
public:
	UdpListenTh() {};
	~UdpListenTh() {};

	void init(UdpIO* pParent, SOCKET sock)
	{
		this->pParent=pParent;
		this->sock=sock;
	};

	eThreadReturn run();

private:
	SOCKET sock;
	UdpIO* pParent;
};

class UdpIO
{
public:
	UdpIO(bool bAsyncSocket=false)
	{
		InitializeCriticalSectionAndSpinCount(&CS1, 0x00000400);
		InitializeCriticalSectionAndSpinCount(&CS2, 0x00000400);
		udpSock=0;
		bWSAStarted=false;
		Init(bAsyncSocket);
	};

	~UdpIO(void)
	{
		Release();
	};

	bool Init(bool bAsyncSocket);
	void Release();

	bool Bind(USHORT port);
	bool Connect(sIpAddr& addr);
	void StartListen();
	void StopListen();

	bool ReceiveAsync(sInetPacket& data, DWORD dwWaitTimeout=0xFFFFFFFF);
	bool Receive(sInetPacket* pData=0);

	int Send(void* pData, int size);
	int Send(const char* str);
	int SendTo(void* pData, int size,sIpAddr& addr);
	int SendTo(const char* str,sIpAddr& addr);

	SOCKET GetSocket() {return udpSock;};

	int getRecvMessageCount() {return packets.size();};
	bool getRecvMessage(sInetPacket& packet) {return pop_message(packet);};

protected:
	bool CreateSocket();
	void CloseSocket();
	void runListenThread();
	void stopListenThread();
	void restartSocket();

	friend class UdpListenTh;
	void push_message(sInetPacket& packet);
	bool pop_message(sInetPacket& packet);

private:
	bool bWSAStarted;
	WSADATA wsd;
	SOCKET udpSock;
	WSAEVENT evt;
	UdpListenTh listenTh;
	char recvBuf[UDPIO_RECV_BUF_SIZE];
	CRITICAL_SECTION CS1,CS2;
	std::list<sInetPacket> packets;

	bool bAsyncSocket;
};

