#pragma once

#include "standard.h"
#include <vector>
#include <map>

class TestMemeberClass : public ISerializable
{
public:
	float_t x;
	float_t y;
	float_t z;
public:
	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(std::basic_ostream<byte>& out_stream) override;
	virtual int DeSerialize(std::basic_istream<byte>& in_stream) override;
	void Print() const;
};

class TestNetObject : public ISerializable
{
private:
	uint64_t m_int64;
	float_t m_float;
	std::wstring m_wstr;
	std::vector<int32_t> integer_vec;
	std::map<int, std::wstring> int_to_wstr_map;
	TestMemeberClass* class_ptr;
public:
	TestNetObject() : class_ptr(nullptr) {}
	void SetInfo();
	void Print() const;

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(std::basic_ostream<byte>& out_stream) override;
	virtual int DeSerialize(std::basic_istream<byte>& out_stream) override;
};

