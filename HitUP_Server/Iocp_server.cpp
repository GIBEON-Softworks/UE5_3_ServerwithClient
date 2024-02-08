#include "Iocp_server.h"
#include <iostream>
#include "packet_handler.h"
#include "tcp_session.h"

#pragma comment(lib, "Ws2_32.lib")

IocpServer::IocpServer()
	:iocp_(nullptr),
	packet_handler_(nullptr),
	thread_pool_(nullptr),
	listen_socket_(0),
	thread_count_(0),
	port_(0)

{
}

IocpServer::~IocpServer()
{
	if (packet_handler_)
		delete[] packet_handler_;

	if (thread_pool_ != nullptr)
		delete thread_pool_;

	while (true)
	{
		TcpSession* session;
		bool bpop = session_list_.TryPop(session);
		if (bpop == false) break;
		delete session;
	}
}

TcpSession* IocpServer::CreateSession()
{
	return new TcpSession;
}


void IocpServer::Worker(uint16_t thread_index) {
	srand((unsigned int)time(nullptr));

	DWORD buffer_len;
	TcpSession* session = nullptr;
	OVERLAPPED* overlapped = nullptr;
	std::thread::id worker_number = std::this_thread::get_id();
	PacketHandler* packet_handler = &packet_handler_[thread_index];

	while (true) {
		// GetQueuedCompletionStatus �Լ����� �����ִٰ� �̺�Ʈ�� �߻��Ǹ� return��
		BOOL success = GetQueuedCompletionStatus(iocp_, &buffer_len, (PULONG_PTR)&session, &overlapped, INFINITE);
		IO_DATA* ioData = (IO_DATA*)overlapped;

		if (ioData->ioType == IO_TYPE::RECV)
		{
			if (buffer_len == 0)
			{
				CloseSession(session);
				continue;
			}

			// recv �Ϸ� ó��
			size_t enq_size = session->Recv(buffer_len);

			// ���۰� Ǯ����Ȳ
			if (enq_size != buffer_len)
			{
				CloseSession(session);
				continue;
			}

			// ��Ŷ ó��
			bool invalid = packet_handler->ProcessPacket(session);
			if (invalid == false)
			{
				CloseSession(session);
				continue;
			}

			// ���� ��ȣ��
			session->CallRecv();
		}
		else
		{
			session->Send();
		}
	}
}

void IocpServer::Start(uint16_t thread_count, uint16_t port)
{
	if (thread_pool_ != nullptr)
		return;

	std::cout << "서버 시작" << std::endl;
	port_ = port;
	thread_count_ = thread_count;
	packet_handler_ = new PacketHandler[thread_count];

	Init();
	InitSocket();
	InitIocp();

	thread_pool_ = new ThreadPool(THREAD_FUNC_C(IocpServer::Worker), thread_count);

	for (int i = 0; i < 100; i++)
		session_list_.Push(CreateSession());

	Run();
}

void IocpServer::Run()
{
	while (true)
	{
		SOCKET client_socket = accept(listen_socket_, nullptr, nullptr);
		std::cout << "accept" << std::endl;
		if (client_socket == INVALID_SOCKET)
		{
			closesocket(listen_socket_);
			WSACleanup();
			return;
		}


		// Ŭ���̾�Ʈ ����
		TcpSession* session;
		bool bpop = session_list_.TryPop(session);

		if (bpop == false)
		{
			closesocket(client_socket);
		}

		session->Init(client_socket);

		// �����ڿ� �߰�
		session_lock_.lock();
		session_map_.insert({ client_socket, session });
		session_lock_.unlock();

		// Ŭ���̾�Ʈ ���Ͽ� IOCP ����
		CreateIoCompletionPort((HANDLE)client_socket, iocp_, (ULONG_PTR)session, 0);

		// ���� recv
		int last_error_code = session->CallRecv();
	}

	closesocket(listen_socket_);
	WSACleanup();
}

int IocpServer::InitSocket()
{
	//	WSAStartup	
	WSADATA wsa;
	int		result;

	result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (result != 0)
	{
		return -1;
	}

	//	�񵿱�� ��Ĺ ���� .ssm
	listen_socket_ = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (listen_socket_ == INVALID_SOCKET)
	{
		WSACleanup();

		return -1;
	}

	// �������� �ּ� ����
	sockaddr_in     receiverAddr = {};
	receiverAddr.sin_family = AF_INET;					//	���� ��������
	receiverAddr.sin_port = htons(port_);				//	�������� ��Ʈ ��ȣ
	receiverAddr.sin_addr.s_addr = INADDR_ANY;			//	������ IP

	//	bind
	// ���� ���ε�
	result = bind(listen_socket_, (sockaddr*)&receiverAddr, sizeof(receiverAddr));

	if (result != 0)
	{
		closesocket(listen_socket_);
		WSACleanup();
		return -1;
	}

	//	listen
	if (listen(listen_socket_, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(listen_socket_);
		WSACleanup();
		return -1;
	}

	return 0;
}

void IocpServer::InitIocp()
{
	// IOCP ����
	iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

}

void IocpServer::CloseSession(TcpSession* session)
{
	ExitSession(session);

	session->Close();

	session_lock_.lock();
	session_map_.erase(session->GetSocket());
	session_list_.Push(session);
	session_lock_.unlock();
}

void IocpServer::Broadcast(BasePacket* packet)
{
	session_lock_.lock();

	for (auto iter = session_map_.begin(); iter != session_map_.end(); iter++)
	{
		auto other = reinterpret_cast<TcpSession*>(iter->second);

		other->CallSend(packet, packet->packet_len);
	}

	session_lock_.unlock();
}