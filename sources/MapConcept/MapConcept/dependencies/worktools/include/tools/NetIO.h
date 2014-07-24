#pragma once

#define NETIO_BASE_RECV_BUFFER_SIZE 65536

enum eSocketType
{
	SOCK_TCP,
	SOCK_UDP
};

enum eReceiveCode
{
	RECV_CLOSED=0,
	RECV_DATA=1,
	RECV_NONE,
	RECV_ERROR,
	RECV_TIMEOUT,
	
};

void NetUseAutomaticInit(bool bVal);
bool NetInitSockets();
void NetShutdownSockets();

class NetIO
{
public:
	NetIO(eSocketType type);
	NetIO(SOCKET sock);
	NetIO();
	~NetIO(void);

	bool Create(eSocketType type);
	bool Create(SOCKET sock);

	bool Open(USHORT port);
	bool Connect(sIpAddr& addr);
	void Close();

	sIpAddr getConnectAddr() {return connectAddr;};

	bool Accept(NetIO& io, DWORD timeout=0xFFFFFFFF);

	bool SetBlockingMode(bool bVal);
	void EnableAccumulatingPacket(bool bVal);
	void SetReceiveBufferSize(UINT size);

	int Send(void* pData, USHORT size);
	int Send(const char* str);
	int SendTo(void* pData, USHORT size, sIpAddr& addr);
	int SendTo(const char* str,sIpAddr& addr);

	eReceiveCode Receive(sInetPacket* pPacket, DWORD timeout=0xFFFFFFFF);

	int GetRecvPacketCount();
	bool PopPacket(sInetPacket& packet);

private:
	bool WaitSocket(DWORD timeout);
	void InitNetIO();
	void ReleaseNetIO();

	bool CreateSocket(eSocketType type);
	void CloseSocket();

	void push_packet(sInetPacket& packet);
	bool pop_packet(sInetPacket& packet);
private:
	CRITICAL_SECTION CS1,CS2;
	char* RecvBuffer;
	UINT recvBufferSize;
	bool bAccumulatePackets;
	std::list<sInetPacket> packets;

	fd_set readset;

	SOCKET handle;
	eSocketType socketType;
	sIpAddr connectAddr;
};

