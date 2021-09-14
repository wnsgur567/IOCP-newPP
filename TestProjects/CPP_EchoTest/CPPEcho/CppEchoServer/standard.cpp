#include "standard.h"


namespace Utils
{

	class MyClass
	{
	public:
		int32_t a = 10;
		float_t b = 20.265465f;
	};

	void characterTest()
	{
		std::basic_ostringstream<byte> output_buffer;
		std::basic_istringstream<byte> input_buffer;

		char a = 'd';
		int write_size = Utils::WriteToBinStream(output_buffer, a);
		input_buffer.str(output_buffer.str());
		char b;
		int read_size = Utils::ReadFromBinStream(input_buffer, b);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		std::cout << a << ',' << b << std::endl;

		wchar_t c = L'b';
		output_buffer.str(byte_string());
		write_size = Utils::WriteToBinStream(output_buffer, c);
		wchar_t d;
		input_buffer.str(output_buffer.str());
		read_size = Utils::ReadFromBinStream(input_buffer, d);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		std::wcout << c << ',' << d << std::endl;
	}

	void SerializationIntegralTest()
	{
		std::basic_ostringstream<byte> output_buffer;
		std::basic_istringstream<byte> input_buffer;

		MyClass myclass;

		int write_size = Utils::WriteToBinStreamImpl(output_buffer, myclass.a);
		input_buffer.str(output_buffer.str());
		int32_t a;
		int read_size = Utils::ReadFromBinStreamImpl(input_buffer, a);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("%d\n", a);

		output_buffer.str(byte_string());
		uint32_t uinteger32 = 162354365;
		write_size = Utils::WriteToBinStreamImpl(output_buffer, uinteger32);
		input_buffer.str(output_buffer.str());
		uint32_t b;
		read_size = Utils::ReadFromBinStreamImpl(input_buffer, b);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("%u\n", b);

		uint64_t uinteger64 = 643484879435598954U;
		write_size = Utils::WriteToBinStreamImpl(output_buffer, uinteger64);
		input_buffer.str(output_buffer.str());
		uint64_t c;
		read_size = Utils::ReadFromBinStreamImpl(input_buffer, c);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("%llu\n", c);
	}

	void SerializationFloatingTest()
	{
		std::basic_ostringstream<byte> output_buffer;
		std::basic_istringstream<byte> input_buffer;

		MyClass myclass;

		int write_size = Utils::WriteToBinStreamImpl(output_buffer, myclass.b);
		input_buffer.str(output_buffer.str());
		float_t a;
		int read_size = Utils::ReadFromBinStreamImpl(input_buffer, a);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("%f\n", a);

		output_buffer.str(byte_string());
		double_t double32 = 586.4843443;
		write_size = Utils::WriteToBinStreamImpl(output_buffer, double32);
		input_buffer.str(output_buffer.str());
		double_t b;
		read_size = Utils::ReadFromBinStreamImpl(input_buffer, b);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("%lf\n", b);
	}

	void StringTest()
	{
		std::basic_ostringstream<byte> output_buffer;
		std::basic_istringstream<byte> input_buffer;
		// string
		std::string str = "asdfasdfafefawefasdf\nnasdfasdfa\tyasdfsdafzxdfzxcf\n";
		output_buffer.str(byte_string());	//flush
		int write_size = Utils::WriteToBinStreamImpl(output_buffer, str);

		input_buffer.str(byte_string()); // flush
		input_buffer.str(output_buffer.str());
		std::string input_str;
		int read_size = Utils::ReadFromBinStreamImpl(input_buffer, input_str);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		std::cout << "output_str : " << str;
		std::cout << "input_str : " << input_str;

		// wstring
		std::wstring wstr = L"9879879876545fdsfdfsdf_dsfsadfasdfdsf\nasdfpfe\tkdfj99\n";
		output_buffer.str(byte_string());	//flush
		write_size = Utils::WriteToBinStreamImpl(output_buffer, wstr);

		input_buffer.str(byte_string()); // flush
		input_buffer.str(output_buffer.str());
		std::wstring winput_str;
		read_size = Utils::ReadFromBinStreamImpl(input_buffer, winput_str);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		std::wcout << L"output_str : " << wstr;
		std::wcout << L"input_str : " << winput_str;

		const char* c_str = "this is error";
		output_buffer.str(byte_string());
		//Utils::ReadFromBinStreamImpl(output_buffer, c_str);
	}

	void VectorTest()
	{
		int write_size;
		int read_size;

		// Int Test
		std::vector<int> integer_vec = { 1,2,3,4,5,6,7,8 };
		std::basic_ostringstream<byte> u_buffer;
		Utils::WriteToBinStreamImpl<int, byte>(u_buffer, integer_vec);
		std::vector<int> integer_vec2 = { 10,20,30,40,50,60,70,80 };
		Utils::WriteToBinStreamImpl(u_buffer, integer_vec2);
		u_buffer.str(std::basic_string<byte>());


		write_size = Utils::WriteToBinStreamImpl(u_buffer, integer_vec2);
		std::basic_istringstream<byte> u_iBuffer;
		std::vector<int> input_vec;
		u_iBuffer.str(u_buffer.str()); // ostream -> istream
		read_size = Utils::ReadFromBinStreamImpl<int, byte>(u_iBuffer, input_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("input vec : ");
		for (auto& item : input_vec)
		{
			printf("%d ", item);
		}
		printf("\n");

		write_size = Utils::WriteToBinStreamImpl(u_buffer, integer_vec);
		u_iBuffer.str(u_buffer.str());
		read_size = Utils::ReadFromBinStreamImpl<int, byte>(u_iBuffer, input_vec);
		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("input vec : ");
		for (auto& item : input_vec)
		{
			printf("%d ", item);
		}
		printf("\n");


		// uint test
		std::vector<uint64_t> uinteger_vec = { 3213546386216354U,5465666U,44878787U,11U,0U,9876543210987654321U };
		u_buffer.str(byte_string());	// flush output buffer		
		write_size = Utils::WriteToBinStreamImpl(u_buffer, uinteger_vec);
		//u_iBuffer.str(byte_string());	// flush input buffer
		u_iBuffer.str(u_buffer.str());
		std::vector<uint64_t > uinput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, uinput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("uinput vec : ");
		for (auto& item : uinput_vec)
		{
			printf("%llu ", item);
		}
		printf("\n");

		// float Test
		std::vector<float> float_vec = { 123.86f,999.87f,-879987987.1f,-0.0002f ,0.0f };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, float_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::vector<float> finput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, finput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("float vec : ");
		for (auto& item : finput_vec)
		{
			printf("%f ", item);
		}
		printf("\n");

		// double test
		std::vector<double> double_vec = { 98764.1654321354,0.000000216546,0.0000000000121,-0.0004542145 };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, double_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::vector<double> dinput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, dinput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("double vec : ");
		for (auto& item : dinput_vec)
		{
			printf("%lf ", item);
		}
		printf("\n");

		// string test
		std::vector<std::string> string_vec = { "asdfaewsf2er2er4\n","aa","","ffkfkf\n" };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, string_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::vector<std::string> stringInput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, stringInput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("string vec : ");
		for (auto& item : stringInput_vec)
		{
			printf("%s ", item.c_str());
		}
		printf("\n");
	}

	void ListTest()
	{
		int write_size;
		int read_size;

		// Int Test
		std::list<int> integer_vec = { 1,2,3,4,5,6,7,8 };
		std::basic_ostringstream<byte> u_buffer;
		Utils::WriteToBinStreamImpl<int, byte>(u_buffer, integer_vec);
		std::list<int> integer_vec2 = { 10,20,30,40,50,60,70,80 };
		Utils::WriteToBinStreamImpl(u_buffer, integer_vec2);
		u_buffer.str(std::basic_string<byte>());
		Utils::WriteToBinStreamImpl(u_buffer, integer_vec2);
		Utils::WriteToBinStreamImpl(u_buffer, integer_vec);


		std::basic_istringstream<byte> u_iBuffer;
		std::list<int> input_vec;
		u_iBuffer.str(u_buffer.str()); // ostream -> istream
		Utils::ReadFromBinStreamImpl<int, byte>(u_iBuffer, input_vec);

		printf("input vec : ");
		for (auto& item : input_vec)
		{
			printf("%d ", item);
		}
		printf("\n");

		Utils::ReadFromBinStreamImpl<int, byte>(u_iBuffer, input_vec);

		printf("input vec : ");
		for (auto& item : input_vec)
		{
			printf("%d ", item);
		}
		printf("\n");


		// uint test
		std::list<uint64_t> uinteger_vec = { 3213546386216354U,5465666U,44878787U,11U,0U,9876543210987654321U };
		u_buffer.str(byte_string());	// flush output buffer		
		write_size = Utils::WriteToBinStreamImpl(u_buffer, uinteger_vec);
		//u_iBuffer.str(byte_string());	// flush input buffer
		u_iBuffer.str(u_buffer.str());
		std::list<uint64_t > uinput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, uinput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("uinput vec : ");
		for (auto& item : uinput_vec)
		{
			printf("%llu ", item);
		}
		printf("\n");

		// float Test
		std::list<float> float_vec = { 123.86f,999.87f,-879987987.1f,-0.0002f ,0.0f };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, float_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::list<float> finput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, finput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("float vec : ");
		for (auto& item : finput_vec)
		{
			printf("%f ", item);
		}
		printf("\n");

		// double test
		std::list<double> double_vec = { 98764.1654321354,0.000000216546,0.0000000000121,-0.0004542145 };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, double_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::list<double> dinput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, dinput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("double vec : ");
		for (auto& item : dinput_vec)
		{
			printf("%lf ", item);
		}
		printf("\n");

		// string test
		std::list<std::string> string_vec = { "asdfaewsf2er2er4\n","aa","","ffkfkf\n" };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, string_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::list<std::string> stringInput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, stringInput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("string vec : ");
		for (auto& item : stringInput_vec)
		{
			printf("%s ", item.c_str());
		}
		printf("\n");
	}

	void SetTest()
	{
		int write_size;
		int read_size;

		// Int Test
		std::set<int> integer_vec = { 1,2,3,4,5,6,7,8 };
		std::basic_ostringstream<byte> u_buffer;
		std::set<int> integer_vec2 = { 10,20,30,40,50,60,70,80 };

		u_buffer.str(std::basic_string<byte>());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, integer_vec2);

		std::basic_istringstream<byte> u_iBuffer;
		std::set<int> input_vec;
		u_iBuffer.str(u_buffer.str()); // ostream -> istream
		read_size = Utils::ReadFromBinStreamImpl<int, byte>(u_iBuffer, input_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("input vec : ");
		for (auto& item : input_vec)
		{
			printf("%d ", item);
		}
		printf("\n");

		write_size = Utils::WriteToBinStreamImpl(u_buffer, integer_vec);
		u_iBuffer.str(u_buffer.str());
		read_size = Utils::ReadFromBinStreamImpl<int, byte>(u_iBuffer, input_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("input vec : ");
		for (auto& item : input_vec)
		{
			printf("%d ", item);
		}
		printf("\n");


		// uint test
		std::set<uint64_t> uinteger_vec = { 3213546386216354U,5465666U,44878787U,11U,0U,9876543210987654321U };
		u_buffer.str(byte_string());	// flush output buffer		
		write_size = Utils::WriteToBinStreamImpl(u_buffer, uinteger_vec);
		//u_iBuffer.str(byte_string());	// flush input buffer
		u_iBuffer.str(u_buffer.str());
		std::set<uint64_t > uinput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, uinput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("uinput vec : ");
		for (auto& item : uinput_vec)
		{
			printf("%llu ", item);
		}
		printf("\n");

		// float Test
		std::set<float> float_vec = { 123.86f,999.87f,-879987987.1f,-0.0002f ,0.0f };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, float_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::set<float> finput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, finput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("float vec : ");
		for (auto& item : finput_vec)
		{
			printf("%f ", item);
		}
		printf("\n");

		// double test
		std::set<double> double_vec = { 98764.1654321354,0.000000216546,0.0000000000121,-0.0004542145 };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, double_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::set<double> dinput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, dinput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("double vec : ");
		for (auto& item : dinput_vec)
		{
			printf("%lf ", item);
		}
		printf("\n");

		// string test
		std::set<std::string> string_vec = { "asdfaewsf2er2er4\n","aa","","ffkfkf\n" };
		u_buffer.str(byte_string());
		write_size = Utils::WriteToBinStreamImpl(u_buffer, string_vec);
		//u_iBuffer.str(byte_string());
		u_iBuffer.str(u_buffer.str());
		std::set<std::string> stringInput_vec;
		read_size = Utils::ReadFromBinStreamImpl(u_iBuffer, stringInput_vec);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		printf("string vec : ");
		for (auto& item : stringInput_vec)
		{
			printf("%s ", item.c_str());
		}
		printf("\n");
	}

	void MapTest()
	{
		int write_size;
		int read_size;

		std::map <int, std::string> integer_to_string_map;
		integer_to_string_map.emplace(1, "aaaa");
		integer_to_string_map.emplace(5, "eeee");
		integer_to_string_map.emplace(3, "cccc");
		integer_to_string_map.emplace(2, "bbbb");
		integer_to_string_map.emplace(9, "iiii");
		integer_to_string_map.emplace(7, "gggg");

		std::basic_ostringstream<byte> out_buffer;
		std::basic_istringstream<byte> in_buffer;

		out_buffer.str(byte_string());	// flush
		write_size = Utils::WriteToBinStreamImpl(out_buffer, integer_to_string_map);

		in_buffer.str(out_buffer.str());
		std::map<int, std::string> integer_to_string_inputmap;
		read_size = Utils::ReadFromBinStreamImpl(in_buffer, integer_to_string_inputmap);

		printf("write_size : %d\n", write_size);
		printf("read_size : %d\n", read_size);
		for (const auto& item : integer_to_string_inputmap)
		{
			printf("( %d , %s )  ", item.first, item.second.c_str());
		}
		printf("\n");
	}
}