#pragma once

class Vector3 : public ISerializable
{
public:
	float x;
	float y;
	float z;

	// ISerializable��(��) ���� ��ӵ�
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};

