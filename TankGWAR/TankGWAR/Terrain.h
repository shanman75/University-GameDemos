#pragma once

#include "stdafx.h"
#include "D3DObject.h"

#define DEF_TER_X 100
#define DEF_TER_Z 100
#define DEF_TER_WIDTH 5.0f

#define TER_X_BIG 10
#define TER_Z_BIG 10
#define TER_WIDTH_BIG 700.0f

#define TER_X_SMALL 170
#define TER_Z_SMALL 170
#define TER_WIDTH_SMALL 3.4f

// DEFINES
//#define		D3DFVF_MESH		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define   D3DFVF_MESH   (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL)
// STRUCTURES
struct sCustomVertex{	
	float		x, y, z;			// Position	
	float   nx, ny, nz;			// Normals	
	D3DCOLOR	diffuse;			// Colour	
	float	u, v;				// Texture Coordinates
};


class cTerrain
{
public:
	enum EVENT { RAND };
  enum ENVIRONMENT { ROCK, GRASS, MOUNTAIN, SNOW};

	cTerrain(float ter_x=DEF_TER_X, float ter_z=DEF_TER_Z, float ter_width=DEF_TER_WIDTH);
	~cTerrain(void);
	void Paint(void);
	void RandomizeMesh(void);
	void event(EVENT evnt);
	float GetHeight(float x, float y);
  void SetHeight(float x, float z, float y);

  void FlattenSquare(float x, float z, float sz);
  void FlattenSphere(D3DXVECTOR3 pos, float radius);

  void OnLostDevice(void);
  void OnResetDevice(void);

  void UpdateMeshHeights();

  void DripDrop(float x, float z, float y);
  void RandomizeTerrain(long numHills = rand()%60+40, long numDirtBalls = 50000);
  void RandomizeEnvironment(void);

private:
  ENVIRONMENT m_env;

	void _Init(void);
	ID3DXMesh *g_TerrainMesh;
	ID3DXMesh *g_TerrainMeshBig;
	//float **m_Heights;
	float **m_Heights;

  float TER_X, TER_Z;
  float TER_WIDTH;

  static LPDIRECT3DTEXTURE9 m_tertex[4];
};
