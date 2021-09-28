#include "IOCPNet_RootHeader.h"

int Vector3::Serialize(NetBase::OutputMemoryStreamPtr out_stream) const
{
	int size = 0;
	size += NetBase::WriteToBinStream(out_stream, x);
	size += NetBase::WriteToBinStream(out_stream, y);
	size += NetBase::WriteToBinStream(out_stream, z);	
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
