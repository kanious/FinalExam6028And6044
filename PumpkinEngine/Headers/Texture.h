#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Component.h"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CTexture : public CComponent
{
private:
	_uint				m_iTextureID;
	_int				m_iWidth;
	_int				m_iHeight;

private:
	explicit CTexture();
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

private:
	virtual void Destroy();

public:
	_uint GetTextureID()		{ return m_iTextureID; }
	_int GetWidth()				{ return m_iWidth; }
	_int GetHeight()			{ return m_iHeight; }

private:
	RESULT Ready(std::string ID, std::string filePath);
public:
	virtual CComponent* Clone();
	static CTexture* Create(std::string ID, std::string filePath);
};

NAMESPACE_END

#endif //_TEXTURE_H_