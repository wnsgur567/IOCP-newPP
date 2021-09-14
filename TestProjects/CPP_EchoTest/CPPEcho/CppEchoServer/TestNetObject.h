#pragma once

#include "ISerializable.h"

class TestNetObject : public ISerializable
{
private:
	uint64_t sign_id;
	std::wstring id;
	std::wstring pw;
	std::vector<int32_t> integer_vec;
public:
	void SetInfo(uint64_t inSign_id, std::wstring inId, std::wstring inPw);
	void Print() const;

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(std::basic_ostringstream<Utils::byte>& out_stream) override;
	virtual int DeSerialize(std::basic_istringstream<Utils::byte>& out_stream) override;
};