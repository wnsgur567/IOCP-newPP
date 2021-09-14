#pragma once
#include <cstdint>
#include "standard.h"

// serialized size + serialized data
class ISerializable
{	
public:
	virtual ~ISerializable()
	{

	}
	virtual int Serialize(std::basic_ostringstream<Utils::byte>& out_stream) = 0;
	virtual int DeSerialize(std::basic_istringstream<Utils::byte>& in_stream) = 0;
};
