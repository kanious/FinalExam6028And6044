#ifndef _MESH_H_
#define _MESH_H_

#include "Component.h"
#include "EngineStruct.h"

NAMESPACE_BEGIN(Engine)

class CVIBuffer;
class CBoundingBox;
class CTexture;
class CShader;
class CTransform;
class COpenGLDevice;
class CQuadTree;
class COctree;

class ENGINE_API CMesh : public CComponent
{
private:
	COpenGLDevice*				m_pOpenGLDevice;
	CVIBuffer*					m_pVIBuffer;
	CBoundingBox*				m_pBoundingBox;
	CTexture*					m_pDiffTexture;
	CShader*					m_pShader;
	CTransform*					m_pParentTransform;
	_bool						m_bWireFrame;
	_bool						m_bSelected;
	_bool						m_bDebug;
	_bool						m_bTransparency;
	_bool						m_bBiilboard;
	_uint						m_iTriNum;
	TRIANGLE*					m_pTriangles;
	CQuadTree*					m_pQuadTree;
	COctree*					m_pOctree;

private:
	explicit CMesh();
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh();

public:
	virtual void Render();
private:
	virtual void Destroy();

public:
	CBoundingBox* GetBoundingBox()					{ return m_pBoundingBox; }
	TRIANGLE* GetTriangleArray()					{ return m_pTriangles; }
	CQuadTree* GetQuadTree()						{ return m_pQuadTree; }
	COctree* GetOctree()							{ return m_pOctree; }
	_uint GetTriangleNumber()						{ return m_iTriNum; }
	CShader* GetShader()							{ return m_pShader; }
	void SetTransform(CTransform* transform)		{ m_pParentTransform = transform; }
	void SetWireFrame(_bool wire)					{ m_bWireFrame = wire; }
	void SetSelcted(_bool select)					{ m_bSelected = select; }
	void SetTransparency(_bool value)				{ m_bTransparency = value; }
	void SetDebugBox(_bool value)					{ m_bDebug = value; }
	void SetBillboard(_bool value)					{ m_bBiilboard = value; }
	void SetTexture(std::string texID_diff);

private:
	RESULT Ready(std::string ID, std::string filePath, std::string fileName, ModelType type, std::string shaderID, std::string texID_Diff);
	RESULT Ready_VIBuffer(ModelType type, std::string filePath, std::string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum);

	//RESULT Ready_xyz_index(std::string filePath, std::string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum);
	//RESULT Ready_xyz_normal_index(std::string filePath, std::string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum);
	//RESULT Ready_xyz_normal_texUV_index(std::string filePath, std::string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum);
	//RESULT Ready_xyz_normal_texUV_index_texNum(std::string filePath, std::string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum);
	//RESULT Ready_xyz_normal_color_index(std::string filePath, std::string fileName, VTX** pVertices, _uint** pIndices, _uint& vertexNum, _uint& indexNum);

	void Ready_Texture_Diff(std::string texID_Diff);
	void Ready_Shader(std::string shaderID);
public:
	void Ready_QuadTree(_uint depth);
	void Ready_Qctree(_uint depth);

public:
	virtual CComponent* Clone();
	static CMesh* Create(std::string ID, std::string filePath, std::string fileName, ModelType type, std::string shaderID, std::string texID_Diff);
};

NAMESPACE_END

#endif //_MESH_H_