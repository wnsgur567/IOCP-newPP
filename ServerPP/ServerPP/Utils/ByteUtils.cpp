#include "ByteUtils.h"

namespace Utils
{
	bool BitConverter::m_isLittleEndian = false;
	BitConverter BitConverterStaticConstructor::bitconverter;

#ifdef __DEBUG
	void IntegerTest()
	{
		int16_t integer16 = 1215;
		int32_t integer32 = -34975745;
		int64_t integer64 = 985676612331453;
		Utils::BitConverter::byte bytes[512];

		printf("16 origin : ");
		printf("%d\n", integer16);
		memcpy(bytes, &integer16, sizeof(int16_t));
		Utils::BitConverter::DebugPrint(bytes, sizeof(int16_t));

		printf("32 origin : ");
		printf("%d\n", integer32);
		memcpy(bytes, &integer32, sizeof(int32_t));
		Utils::BitConverter::DebugPrint(bytes, sizeof(int32_t));

		printf("64 origin : ");
		printf("%lld\n", integer64);
		memcpy(bytes, &integer64, sizeof(int64_t));
		Utils::BitConverter::DebugPrint(bytes, sizeof(int64_t));


		Utils::BitConverter::GetBytes<int16_t>(integer16, bytes);
		printf("16 convert : ");
		Utils::BitConverter::DebugPrint(bytes, sizeof(int16_t));
		int16_t ret16 = Utils::BitConverter::BytesToVal<int16_t>(bytes);
		printf("16 revert : %d\n", ret16);

		Utils::BitConverter::GetBytes<int32_t>(integer32, bytes);
		printf("32 convert : ");
		Utils::BitConverter::DebugPrint(bytes, sizeof(int32_t));
		int32_t ret32 = Utils::BitConverter::BytesToVal<int32_t>(bytes);
		printf("32 revert : %d\n", ret32);

		Utils::BitConverter::GetBytes<int64_t>(integer64, bytes);
		printf("64 convert : ");
		Utils::BitConverter::DebugPrint(bytes, sizeof(int64_t));
		int64_t ret64 = Utils::BitConverter::BytesToVal<int64_t>(bytes);
		printf("64 revert : %lld\n", ret64);
	}

	void UIntegerTest()
	{
		uint16_t integer16 = 1215U;
		uint32_t integer32 = 34975745U;
		uint64_t integer64 = 2856766123312311453U;
		Utils::BitConverter::byte bytes[512];

		printf("16 origin : ");
		printf("%d\n", integer16);
		memcpy(bytes, &integer16, sizeof(uint16_t));
		Utils::BitConverter::DebugPrint(bytes, sizeof(uint16_t));

		printf("32 origin : ");
		printf("%d\n", integer32);
		memcpy(bytes, &integer32, sizeof(uint32_t));
		Utils::BitConverter::DebugPrint(bytes, sizeof(uint32_t));

		printf("64 origin : ");
		printf("%lld\n", integer64);
		memcpy(bytes, &integer64, sizeof(uint64_t));
		Utils::BitConverter::DebugPrint(bytes, sizeof(uint64_t));


		Utils::BitConverter::GetBytes<uint16_t>(integer16, bytes);
		printf("16 convert : ");
		Utils::BitConverter::DebugPrint(bytes, sizeof(uint16_t));
		uint16_t ret16 = Utils::BitConverter::BytesToVal<uint16_t>(bytes);
		printf("16 revert : %d\n", ret16);

		Utils::BitConverter::GetBytes<uint32_t>(integer32, bytes);
		printf("32 convert : ");
		Utils::BitConverter::DebugPrint(bytes, sizeof(uint32_t));
		uint32_t ret32 = Utils::BitConverter::BytesToVal<uint32_t>(bytes);
		printf("32 revert : %d\n", ret32);

		Utils::BitConverter::GetBytes<uint64_t>(integer64, bytes);
		printf("64 convert : ");
		Utils::BitConverter::DebugPrint(bytes, sizeof(uint64_t));
		uint64_t ret64 = Utils::BitConverter::BytesToVal<uint64_t>(bytes);
		printf("64 revert : %lld\n", ret64);
	}

	void FloatTest()
	{
		float f = 1.25f;
		double d = 10.2889465;

		Utils::BitConverter::byte bytes[512];

		memcpy(bytes, &f, sizeof(f));
		Utils::BitConverter::DebugPrint(bytes, sizeof(f));
		Utils::BitConverter::GetBytes(f, bytes);
		Utils::BitConverter::DebugPrint(bytes, sizeof(f));
		auto ff = Utils::BitConverter::BytesToFloat(bytes);
		printf("ff : %f\n", ff);

		memcpy(bytes, &d, sizeof(d));
		Utils::BitConverter::DebugPrint(bytes, sizeof(d));
		Utils::BitConverter::GetBytes(d, bytes);
		Utils::BitConverter::DebugPrint(bytes, sizeof(d));
		auto dd = Utils::BitConverter::BytesToDouble(bytes);
		printf("ff : %lf\n", dd);
	}
#endif
}