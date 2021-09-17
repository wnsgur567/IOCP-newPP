#include "IOCPNet_RootHeader.h"

int Vector3::Serialize(NetBase::OutputMemoryStreamPtr out_stream)
{
	int size = 0;
	WriteToStream(out_stream, x);
	WriteToStream(out_stream, y);
	WriteToStream(out_stream, z);
	return size;
}

int Vector3::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
	int size = 0;
	ReadFromStream(in_stream, x);
	ReadFromStream(in_stream, y);
	ReadFromStream(in_stream, z);
	return size;
}
