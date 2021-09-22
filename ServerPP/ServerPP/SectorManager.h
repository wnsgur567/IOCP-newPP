#pragma once
class SectorManager : public MyBase::Singleton<SectorManager>
{
	friend class Singleton;
	using EDirection = Sector::EDirection;
public:
	// 20 �� flag ���� ���
	enum class EProtocol : ProtocolSize_t
	{
		None,

		FirstInit = 1 << 20,
		PlayerAction = 1 << 21,							// �̵����� �ʴ� ��� ������
		PlayerMove = 1 << 22,							// �̵��ϴ� ������
		PlayerMoveAndAction = (1 << 21) | (1 << 22),	// action �� move �� ���� �Ͼ �� ����
	};
	enum class  EResult :ResultSize_t
	{
		None = 0,
	};
protected:
	SectorManager() {}
public:
	~SectorManager()
	{

	}
	bool Initialize(LPVOID) noexcept override;
	void Finalize() noexcept override;
public:
	void GetProtocol(ProtocolSize_t inOrigin, EProtocol& outProtocol);

	EResult ActionProcess(NetBase::InputMemoryStreamPtr inpStream, SectorPtr inCurrentSector, PlayerInfoPtr inpPlayer);
	EResult MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer);
	EResult MoveAndActionProcess(std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer);
};

