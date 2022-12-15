#ifndef _LAYER_H_
#define _LAYER_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

class CGameObject;
class CObjectManager;
class CScene;

class ENGINE_API CLayer : public CBase
{
protected:
	_uint							m_tag;
	_bool							m_bEnable;

	std::list<CGameObject*>			m_listGameObjects;

protected:
	explicit CLayer();
	virtual ~CLayer();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
	void Enable();
	void Disable();
private:
	virtual void Destroy();

public:
	const _uint GetTag()						{ return m_tag; }
	const _bool GetEnable()						{ return m_bEnable; }
	std::list<CGameObject*>* GetObjectList()	{ return &m_listGameObjects; }
	void SetTag(_uint layerTag);
	RESULT AddGameObject(CGameObject* obj);
	RESULT RemoveGameObject(CGameObject* obj);
	void RemoveAllGameObject();
	CGameObject* FindGameObjectByObjName(std::string objName);

private:
	RESULT Ready(_uint tag);
public:
	static CLayer* Create(_uint tag);
};

NAMESPACE_END

#endif //_LAYER_H_