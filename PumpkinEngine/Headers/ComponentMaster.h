#ifndef _COMPONENTMASTER_H_
#define _COMPONENTMASTER_H_

#include "Base.h"
#include "Component.h"

NAMESPACE_BEGIN(Engine)

class CComponent;
class ENGINE_API CComponentMaster : public CBase
{
	SINGLETON(CComponentMaster)

private:
	typedef std::unordered_map<std::string, CComponent*> COMPONENT_MAP;
	COMPONENT_MAP				m_mapComponent;

	typedef std::unordered_map<std::string, std::string> MESH_MAP;
	MESH_MAP					m_mapMesh;


private:
	explicit CComponentMaster();
	virtual ~CComponentMaster();
private:
	void Destroy();

public:
	RESULT AddNewComponent(std::string tag, CComponent* pComponent);
	RESULT AddNewMeshInfo(std::string tag, std::string initSize);
	CComponent* FindComponent(std::string tag);
	COMPONENT_MAP* GetComponentMap()			{ return &m_mapComponent; }
	MESH_MAP* GetMeshMap()						{ return &m_mapMesh; }
};

template <class T>
T CloneComponent(std::string tag)
{
	CComponent* pComponent = CComponentMaster::GetInstance()->FindComponent(tag);
	if (nullptr != pComponent)
	{
		CComponent* clone = pComponent->Clone();
		return dynamic_cast<T>(clone);
	}
	return nullptr;
}

NAMESPACE_END

#endif //_COMPONENTMASTER_H_