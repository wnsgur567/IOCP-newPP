#pragma once

#include "ISerializable.h"

#include <string>


class TestSignInfo : public ISerializable
{
private:
	uint64_t sign_id;
	std::wstring id;
	std::wstring pw;

public:
	void SetInfo(uint64_t inSign_id, std::wstring inId, std::wstring inPw);
	void Print() const;

	// ISerializable을(를) 통해 상속됨
	virtual int Serialize(std::basic_ostringstream<Utils::byte>& out_stream) override;
	virtual int DeSerialize(std::basic_istringstream<Utils::byte>& out_stream) override;
	
};