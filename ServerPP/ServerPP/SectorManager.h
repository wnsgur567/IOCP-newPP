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

		FirstInit = 1 << 20,
		PlayerAction = 1 << 21,							// 이동하지 않는 모든 움직임
		PlayerMove = 1 << 22,							// 이동하는 움직임
		PlayerMoveAndAction = (1 << 21) | (1 << 22),	// action 과 move 가 같이 일어날 수 있음
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

