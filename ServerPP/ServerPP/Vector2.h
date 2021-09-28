#pragma once
class Vector2 : public ISerializable
{
public:
	float x;
	float y;
public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
using Vector2Ptr = std::shared_ptr<Vector2>;

class Vector2Int : public ISerializable
{
public:
	int32_t x;
	int32_t y;
public:
	Vector2Int() : x(0), y(0) {}
	Vector2Int(int _x, int _y) :x(_x), y(_y) {}
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const override;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
};
using VectorInt2Ptr = std::shared_ptr<Vector2>;

