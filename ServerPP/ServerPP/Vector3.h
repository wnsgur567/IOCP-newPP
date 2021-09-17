#pragma once

class Vector3 : public ISerializable
{
public:
	float x;
	float y;
	float z;

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};

