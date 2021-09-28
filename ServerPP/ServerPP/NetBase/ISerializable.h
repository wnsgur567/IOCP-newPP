#pragma once
#include <cstdint>
#include "MemoryStream.h"

using byte = unsigned char;
// serialized size + serialized data
class ISerializable
{

public:
	virtual ~ISerializable()
	{

	}
	//virtual int Serialize(std::basic_ostream<byte>& out_stream) = 0;
	//virtual int DeSerialize(std::basic_istream<byte>& in_stream) = 0;
	virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) const = 0;
	virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) = 0;
};
using ISerializablePtr = std::shared_ptr<ISerializable>;