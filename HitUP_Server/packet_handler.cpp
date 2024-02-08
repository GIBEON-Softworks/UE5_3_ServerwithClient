#include "packet_handler.h"
#include "tcp_session.h"
#include "stream_buffer.h"
#include "worker_thread.h"

PacketHandler::PacketHandler()
{
}

PacketHandler::~PacketHandler()
{
	Release();
}

bool PacketHandler::ProcessPacket(TcpSession* tcp_session)
{
	// ���ǿ��� ���� ��������
	StreamBuffer* stream = tcp_session->GetStreamBuffer();

	// ���� ������Ȯ��
	size_t buffer_size = stream->GetSize();
	while (buffer_size > PACKET_HEADER_SIZE)
	{
		// ����� �Ϻ� �������� (��Ʈ������ ������ ���� ����)
		BasePacket header;
		header = stream->PeekBuffer<BasePacket>();

		// ��� ũ�� üũ
		if (header.packet_len > buffer_size)
		{
			break;
		}

		// ��ϵ� ��Ŷ ã��
		auto item = packet_pool_.find(header.packet_id);
		auto callback_item = callback_list_.find(header.packet_id);

		// ���� ��Ŷ
		if (item == packet_pool_.end())
			return false;

		if (callback_item == callback_list_.end())
			return false;

		auto queue = item->second;
		auto callback = callback_item->second;

		// ��Ŷ ������ ���� (��Ʈ�� ���� ������ ����)
		BasePacket* packet;
		queue->WaitAndPop(packet);
		stream->Dequeue((char*)packet, header.packet_len);

		Packet warpper(this, tcp_session, packet);
		callback(warpper);

		// ���� ��Ȯ��
		buffer_size = stream->GetSize();
	}

	return true;
}


void PacketHandler::RestorePacket(BasePacket* packet)
{
	auto iter = packet_pool_.find(packet->packet_id);

	if (iter == packet_pool_.end())
		return;

	auto queue = iter->second;

	queue->Push(packet);
}

void PacketHandler::Release()
{
	for (auto iter = packet_pool_.begin(); iter != packet_pool_.end(); iter++)
	{
		auto queue = iter->second;
		std::vector<BasePacket*> packet_list;
		queue->DrainQueue(packet_list);

		for (auto packet : packet_list)
			delete packet;

		delete queue;
	}
}