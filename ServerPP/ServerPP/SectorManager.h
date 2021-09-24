#pragma once
class SectorManager : public MyBase::Singleton<SectorManager>
{
	friend class Singleton;
	using EDirection = Sector::EDirection;
public:
	// 20 번 flag 부터 사용
	enum class EProtocol : ProtocolSize_t
	{
		None,

		FirstInit = 1 << 20,				// 최초 입장시 현재 클라에게
		FirstInit_Others = 1 << 21,			// 최초 입장시 입장 위치에서 근처섹터의 오브젝트들

		PlayerAction = 1 << 22,							// 이동하지 않는 모든 움직임
		PlayerMove = 1 << 23,							// 이동하는 움직임
		PlayerMoveAndAction = (PlayerAction) | (PlayerMove),	// action 과 move 가 같이 일어날 수 있음

		EnterInView = 1 << 24,			// 시야 범위 내로 들어온 새로운 오브젝트
		LeaveInView = 1 << 25,			// 시야 범위 밖으로 나간 기존 오브젝트
		EnterSection = 1 << 26,			// 새로운 섹션에 들어갈 경우 새로운 섹션에 존재하는 player들을 그려야 함
		LeaveSection = 1 << 27          // 새로운 섹션에 들어갈 경우 기존에 그릴 필요 없는 놈들은 없에야됨
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

