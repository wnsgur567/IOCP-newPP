#pragma once
class SectorManager : public MyBase::Singleton<SectorManager>
{
	friend class Singleton;
	using EDirection = Sector::EDirection;
public:
	// 20 �� flag ���� ~ 29�� ���� ���
	enum class EProtocol : ProtocolSize_t
	{
		None,

		FirstInit = 1ULL << 20,									// ���� ����� ���� Ŭ�󿡰�
		FirstInit_Others = 1ULL << 21,							// ���� ����� ���� ��ġ���� ��ó������ ������Ʈ��

		PlayerAction = 1ULL << 22,								// �̵����� �ʴ� ��� ������
		PlayerMove = 1ULL << 23,								// �̵��ϴ� ������
		PlayerMoveAndAction = (PlayerAction) | (PlayerMove),	// action �� move �� ���� �Ͼ �� ����

		EnterInView = 1ULL << 24,			// �þ� ���� ���� ���� ���ο� ������Ʈ
		LeaveInView = 1ULL << 25,			// �þ� ���� ������ ���� ���� ������Ʈ
		EnterSection = 1ULL << 26,			// ���ο� ���ǿ� �� ��� ���ο� ���ǿ� �����ϴ� player���� �׷��� ��
		LeaveSection = 1ULL << 27           // ���ο� ���ǿ� �� ��� ������ �׸� �ʿ� ���� ����� �����ߵ�
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

	EResult FirstEnterProcess(SectorPtr inCurrentSector, Vector2 inlocal_position, PlayerInfoPtr inpPlayer, IOCP_Base::IOCPSessionBasePtr inpSession);
	EResult ActionProcess(NetBase::InputMemoryStreamPtr inpStream, SectorPtr inCurrentSector, PlayerInfoPtr inpPlayer);
	EResult MoveProcess(NetBase::InputMemoryStreamPtr inpStream, std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer);
	EResult MoveAndActionProcess(std::vector<std::vector<SectorPtr>>& inSectors, SectorPtr inCurrent_sector, PlayerInfoPtr inpPlayer);
};