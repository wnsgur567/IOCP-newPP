#pragma once

class Vector3 : public ISerializable
{
public:
	float x;
	float y;
	float z;
	Vector3() : x(), y(), z() {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};

