#include "IOCPNet_RootHeader.h"

int Vector2::Serialize(NetBase::OutputMemoryStreamPtr out_stream)
{
	int size = 0;
	WriteToStream(out_stream,x);
	WriteToStream(out_stream, y);
	return size;
}

int Vector2::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
	int size = 0;
	ReadFromStream(in_stream, x);
	ReadFromStream(in_stream, y);
	return size;
}

int Vector2Int::Serialize(NetBase::OutputMemoryStreamPtr out_stream)
{
	int size = 0;
	WriteToStream(out_stream, x);
	WriteToStream(out_stream, y);
	return size;
}

int Vector2Int::DeSerialize(NetBase::InputMemoryStreamPtr in_stream)
{
	int size = 0;
	ReadFromStream(in_stream, x);
	ReadFromStream(in_stream, y);
	return size;
}
