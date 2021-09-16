#pragma once

namespace Sign
{

	class SignInfo;
	using SignInfoPtr = std::shared_ptr<SignInfo>;

	// ID �ߺ� ��� x
	// ID ���� ������ ����
	class SignInfo : public ISerializable
	{
	public:
		static constexpr size_t MAX_IDSIZE = 30;
		static constexpr size_t MAX_PWSIZE = 40;
		using signid_t = uint64_t;
	public:
		signid_t sign_id;
		std::wstring ID;
		std::wstring PW;
		std::wstring JoinDate;
		bool IsActivated;
		void Flush();
	public:
		SignInfo() : sign_id(0), ID(), PW(), JoinDate(), IsActivated(false){}
		SignInfo(signid_t inSignID, const wchar_t* inID, const wchar_t* inPW);
		SignInfo(signid_t inSignID, std::wstring inID, std::wstring inPW);
		bool operator == (const SignInfo& other);
		bool operator != (const SignInfo& other);
		bool operator > (const SignInfo& other);
		bool operator < (const SignInfo& other);
		

		// ISerializable��(��) ���� ��ӵ�
		virtual int Serialize(NetBase::OutputMemoryStreamPtr out_stream) override;
		virtual int DeSerialize(NetBase::InputMemoryStreamPtr in_stream) override;
	};
}