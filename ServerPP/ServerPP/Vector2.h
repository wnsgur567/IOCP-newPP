#pragma once
class Vector2 : public ISerializable
{
public:
	float x;
	float y;
public:
	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;

	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;

};
using Vector2Ptr = std::shared_ptr<Vector2>;

class Vector2Int : public ISerializable
{
public:
	int32_t x;
	int32_t y;

	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
using VectorInt2Ptr = std::shared_ptr<Vector2>;

