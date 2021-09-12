#include "standard.h"

#include <sstream>

namespace Utils
{

	class MyClass
	{
	public:
		int a = 10;
		int b = 20;
	};

	void StringTest()
	{

	}

	void VectorTest()
	{


		MyClass* c_test = new MyClass();

		std::vector<int> integer_vec = { 1,2,3,4,5,6,7,8 };
		std::vector<char*> pChar_vec = { nullptr,nullptr ,nullptr ,nullptr ,nullptr };
		std::vector<MyClass*> pClass_vec = { nullptr ,nullptr ,nullptr ,nullptr ,nullptr };


		std::basic_ostringstream<unsigned char> u_buffer;

		// u_buffer << "abdsfd" << "12312312";

		std::basic_string<unsigned char> contents = u_buffer.str();
		std::cout << contents.c_str() << std::endl;

		std::stringstream buffer;

		//Utils::WriteToBinStreamImpl<int, char>(buffer, integer_vec);

		Utils::WriteToBinStreamImpl<int, unsigned char>(u_buffer, integer_vec);

		std::basic_istringstream<unsigned char> u_iBuffer;
		std::vector<int> input_vec;
		u_iBuffer.str(u_buffer.str());
		Utils::ReadFromBinStreamImpl<int, unsigned char>(u_iBuffer, input_vec);

		printf("input vec : ");
		for (auto& item : input_vec)
		{
			printf("%d ", item);
		}
		printf("\n");
	}

	void ListTest()
	{

	}

	void SetTest()
	{

	}

	void MapTest()
	{

	}
}