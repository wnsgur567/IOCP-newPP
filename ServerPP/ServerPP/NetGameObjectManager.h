#pragma once

class NetGameObjectManager : public MyBase::Singleton<NetGameObjectManager>
{
	friend class Singleton;
private:
	uint64_t m_newObj_id;

protected:
	NetGameObjectManager() : m_newObj_id(), m_netobject_map() {}
	std::unordered_map<uint64_t, NetGameObjectPtr> m_netobject_map;
public:
	~NetGameObjectManager() {}
	bool Initialize(LPVOID) noexcept override;
	void Finalize() noexcept override;
	NetGameObjectPtr GetNetObject(uint64_t inNetobj_id);
	template<typename _Derived>
	inline std::shared_ptr<_Derived> Create();
	void Destroy(uint64_t inNetobj_id);
};

template<typename _Derived>
inline std::shared_ptr<_Derived> NetGameObjectManager::Create()
{
	_Derived* created = new typename std::enable_if<std::is_base_of<NetGameObject, _Derived>::value, _Derived>::type();
	std::shared_ptr<_Derived> retPtr;
	retPtr.reset(created);
	retPtr->m_net_id = m_newObj_id;
	++m_newObj_id;
	retPtr->OnCreated();
	return retPtr;
}
