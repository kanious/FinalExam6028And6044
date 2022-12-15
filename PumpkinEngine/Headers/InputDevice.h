#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include "Base.h"
#include "OpenGLDefines.h"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CInputDevice : public CBase
{
	SINGLETON(CInputDevice)

private:
	class KeyInfo
	{
	public:
		_int scancode;
		_int action;
		_int preAction;
		_int mods;
	};

private:
	GLFWwindow*								m_pWindow;
	std::unordered_map<_int, KeyInfo*>		m_mapKeyInfos;
	std::unordered_map<_int, KeyInfo*>		m_mapMouseButtonInfos;
	glm::vec2								m_vecMousePos;
	glm::vec2								m_vecMousePosPrevious;
	glm::vec2								m_vecScroll;
	_float									m_fMouseSensitivity;
	_bool									m_bEntered;
	_bool									m_bPosFixed;

private:
	explicit CInputDevice();
	virtual ~CInputDevice();
	void Destroy();

public:
	RESULT SetupInputSystem(GLFWwindow* pWindow, _int mouseMode);
	void ClearInputSystem();
	void SetMouseCursorMode(_int mouseMode);
	static void KeyCallBack(GLFWwindow* window, _int key, _int scancode, _int action, _int mods);
	static void MouseButtonCallBack(GLFWwindow* window, _int button, _int action, _int mods);
	static void MousePosCallBack(GLFWwindow* window, _double x, _double y);
	static void MouseScrollCallBack(GLFWwindow* window, _double xOffset, _double yOffset);
	_bool IsKeyDown(_int key);
	_bool IsAnyKeyDown(_int key);
	_bool IsMousePressed(_int key);
	void InitMousePos();
	glm::vec2 GetMousePos() { return m_vecMousePos; }
	glm::vec2 GetMouseMovedDistance();
	glm::vec2 GetMouseScrollDistance();
	glm::vec3 GetMouseWorldCoord();
	glm::vec3 GetCenterMouseWorldCoord();
	void SetMouseSensitivity(_float value) { m_fMouseSensitivity = value; }
	void SetCustomCrosshair();
	void RemoveCustomCrosshair();
	void SetMousePosFixed(_bool value) { m_bPosFixed = value; }

private:
	void SetInputKey(_int key, _int scancode, _int action, _int mods);
	void SetInputMouseButton(_int button, _int action, _int mods);
	void SetMousePos(glm::vec2 vPos);
	void SetMouseScroll(glm::vec2 vPos);
};

NAMESPACE_END

#endif //_INPUTMANAGER_H_