#include "StdAfx.h"
#include "terrain.h"
#include <d3dx9.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <sys/types.h>
#include <fcntl.h>
#include "GameState.h"



LPDIRECT3DTEXTURE9	  cTerrain::m_tertex[4];

cTerrain::cTerrain(float x,float z,float w)
{
//  m_tertex = NULL;
  m_env = ENVIRONMENT::ROCK;

  TER_X = x;  TER_Z = z; 
  TER_WIDTH = w;   // Ignored
  m_Heights = new float*[(int)(TER_X+1)];
  for (int i = 0; i < (TER_X+1); i++) {
    m_Heights[i] = new float[(int)(TER_Z+1)];
    for (int j = 0; j < (TER_Z+1); j++) {
//       m_Heights[i][j] = (float)((rand()%500)/15);
       m_Heights[i][j] = 44.0f;
    }
  }

	_Init();
}


void cTerrain::RandomizeEnvironment(void)
{
  m_env = (cTerrain::ENVIRONMENT) ((int)rand()%4);
  RandomizeMesh();
}
void cTerrain::RandomizeTerrain(long numHills, long numDirtBalls)
{
  BYTE c;
  BYTE d[1024];
  int iC;
  int fTerrain;
  char path[1024];
  sprintf (path,"resource\\terrains\\terrain%i.hmp",rand()%17);
  fTerrain = open(path,_O_BINARY|_O_RDONLY);

  if (fTerrain > 0) {
  for (int x = 0; x < 100; x++)
	  read(fTerrain,&c,1);
  // Number of hills 40 through 100
//  int numHills = rand()%60 + 10;
//  numHills = 0;
//  numHills = 10;
  //m_Heights = new float*[(int)(TER_X+1)];
  //float squash = 4 - numHills/102*4;
  float squash = 7 / (g_GameState->GetNumHills() + 1);
  for (int i = 0; i < TER_X; i++) {
    //m_Heights[i] = new float[(int)(TER_Z+1)];
    if (int g = _read (fTerrain,&d,1024) <= 0)
		exit(g);
	else {
		char deb[255];
		sprintf (deb,"Read %i bytes\n",g);
		OutputDebugString(deb);
	}
    for (int j = 0; j < TER_Z; j++) {
//       m_Heights[i][j] = (float)((rand()%500)/15);	   		   
       m_Heights[i][j] = (float) ((unsigned int) d[j])/squash + 44.0f;
       //m_Heights[i][j] = m_Heights[i][j];
       //read (fTerrain,&m_Heights[i][j],1);
    }
	//m_Heights[i][(int)floor(TER_Z)+1] = (float) ((unsigned int) d[j])/3 + 44.0f;
  }
 close(fTerrain);
 UpdateMeshHeights();
 return;
 }

  for (int i = 0; i < (TER_X+1); i++) {
    m_Heights[i] = new float[(int)(TER_Z+1)];
    for (int j = 0; j < (TER_Z+1); j++) {
//       m_Heights[i][j] = (float)((rand()%500)/15);
       m_Heights[i][j] = 88.0f;
    }
  }

  float xsize = (TER_X_SMALL+1) * TER_WIDTH_SMALL;
  float zsize = (TER_Z_SMALL+1) * TER_WIDTH_SMALL;
  for (long i =0; i < numHills; i++)
  {
     static char hilldebg[255];
     sprintf(hilldebg,"Generating hill %i of %i\n",i,numHills);
     OutputDebugString(hilldebg);
      float tx = ((float)(rand() % (int)(xsize*10)))/10 - xsize/2;
      float tz = ((float)(rand() % (int)(zsize*10)))/10 - zsize/2;
      float ty = (float)(rand()%4500+550);
//      SetHeight(tx,tz,ty);
      for (int t=0; t< (int)ty; t++)
      {
        float dx = (rand()%1000)/100 - 5.0f;
        float dz = (rand()%1000)/100 - 5.0f;
        DripDrop(tx+dx,tz+dz,1.0f + (rand()%5)/10);
      }
  }
  for (long i = 0; i < numDirtBalls; i++) {
    float tx = ((float)(rand() % (int)(xsize*10)))/10 - xsize/2;
    float tz = ((float)(rand() % (int)(zsize*10)))/10 - zsize/2;
    DripDrop(tx,tz,(0.3f+(float)(rand()%70))/100);
  }
    //SetHeight((float)(rand() % 3000)/10-150.0f,(float)(rand() % 3000)/10-150.0f,((float)(rand()%100))/100);
  // Pass 1
  /*
  for (int t = 1; t < 2; t++)  for (int i = 1; i < TER_X-1; i++)
  for (int j = 1; j < TER_Z-1; j+=2)
    m_Heights[i][j] = 
                   ( m_Heights[i][j-1] + m_Heights[i][j] + 
                     m_Heights[i][j+1] +0
                    )/3;
  // Pass 2
  for (int bb = 1; bb < 4; bb++)
  for (int t = 1; t < 4; t++)
  for (int i = t; i < TER_X-1; i++)
  for (int j = t; j < TER_Z-1; j+=t)
    m_Heights[i][j] = 
                   ( 
                    m_Heights[i][j-1] + m_Heights[i][j+1] + 
                     m_Heights[i][j] + m_Heights[i+1][j] +
                     m_Heights[i-1][j] + 0
                    )/5;
  */
  UpdateMeshHeights();
}

void cTerrain::DripDrop(float x, float z, float y)
{
   D3DXVECTOR3 r_p;
   D3DXVECTOR3 t_p;

BreakIt:
   r_p = D3DXVECTOR3(x,1000000.0f*1000000.0f,z);
   
   t_p = D3DXVECTOR3(x,GetHeight(x,z),z);
   if (t_p.y < r_p.y) r_p = t_p;
   t_p = D3DXVECTOR3(x-1,GetHeight(x-1,z),z);
   if (t_p.y < r_p.y) r_p = t_p;
   t_p = D3DXVECTOR3(x-1,GetHeight(x+1,z),z);
   if (t_p.y < r_p.y) r_p = t_p;

   t_p = D3DXVECTOR3(x,GetHeight(x,z-1),z-1);
   if (t_p.y < r_p.y) r_p = t_p;
   t_p = D3DXVECTOR3(x-1,GetHeight(x-1,z-1),z-1);
   if (t_p.y < r_p.y) r_p = t_p;
   t_p = D3DXVECTOR3(x-1,GetHeight(x+1,z-1),z-1);
   if (t_p.y < r_p.y) r_p = t_p;

   t_p = D3DXVECTOR3(x,GetHeight(x,z+1),z+1);
   if (t_p.y < r_p.y) r_p = t_p;
   t_p = D3DXVECTOR3(x-1,GetHeight(x-1,z+1),z+1);
   if (t_p.y < r_p.y) r_p = t_p;
   t_p = D3DXVECTOR3(x-1,GetHeight(x+1,z+1),z+1);
   if (t_p.y < r_p.y) r_p = t_p;

   if (r_p.x != x || r_p.z != z)
   {
     x = r_p.x; z = r_p.z;
//     t_p = r_p;
     goto BreakIt;
   }
//   if (t_p.x == r_p.x && t_p.z == r_p.z)
     SetHeight(r_p.x,r_p.z,r_p.y+y);
//   else 
//     DripDrop(t_p.x,t_p.z,y);
}

void cTerrain::Paint()
{
  D3DXMATRIXA16 matwrld;
  D3DMATERIAL9 mtrl;
  ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
  mtrl.Ambient = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
  mtrl.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
  mtrl.Emissive = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
  mtrl.Specular = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
  mtrl.Power = 1.0f;

	D3DXMatrixIdentity(&matwrld);
	g_D3DObject->m_d3ddevice9->SetTransform(D3DTS_WORLD,&matwrld);
//  g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_LIGHTING,FALSE);
  g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_LIGHTING,TRUE);

  if (g_TerrainMesh != NULL)
  {   
    g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[(int)m_env]);
    //g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_WRAP0, D3DWRAPCOORD_0 | D3DWRAPCOORD_1 |D3DWRAPCOORD_2|D3DWRAPCOORD_3);
	  g_TerrainMesh->DrawSubset(0);
    g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_WRAP0, 0);
    g_TerrainMeshBig->DrawSubset(0);
  }
  g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_LIGHTING,TRUE);
}

void cTerrain::_Init()
{

  // Create the smaller VB
  if (FAILED(D3DXCreateMeshFVF(
    2*(TER_X_SMALL)*(TER_Z_SMALL), 
    (TER_X_SMALL+1) * (TER_Z_SMALL+1), 
    //D3DPOOL_DEFAULT | D3DXMESH_32BIT,  //	  
    D3DXMESH_32BIT | D3DXMESH_MANAGED, 
	  D3DFVF_MESH,
	  g_D3DObject->m_d3ddevice9, 
	  &g_TerrainMesh 
	))) 
    exit(1);

  if (FAILED(D3DXCreateMeshFVF(
    2*(TER_X_BIG)*(TER_Z_BIG), 
    (TER_X_BIG+1) * (TER_Z_BIG+1), 
    //D3DPOOL_DEFAULT | D3DXMESH_32BIT,  //	  
    D3DXMESH_32BIT | D3DXMESH_MANAGED, 
	  D3DFVF_MESH,
	  g_D3DObject->m_d3ddevice9, 
	  &g_TerrainMeshBig
	)))
      exit(1);

	LPDIRECT3DVERTEXBUFFER9 pTetVB = NULL; //Pointer to vertex buffer.

  char texpath[1024];
  LPDIRECT3DTEXTURE9 tempt;

  //m_tertex = new LPDIRECT3DTEXTURE9 [1];
  //sprintf (texpath,"resource\\%s","BUMP_Dents_bp.dds");
  sprintf (texpath,"resource\\%s","rock_bw.dds");
  
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return;
  if (FAILED(D3DXCreateTextureFromFileEx(
                                  g_D3DObject->m_d3ddevice9,    // Device
                                  texpath,
                                  D3DX_DEFAULT,D3DX_DEFAULT,    // Height/Width
                                  D3DX_DEFAULT,                 // MIP Maps
                                  0,                            // Usage
                                  D3DFMT_A8R8G8B8,              // Format
                                  D3DPOOL_DEFAULT,              // Pool
                                  D3DX_DEFAULT,                 // Filter
                                  D3DX_DEFAULT,                 // MIP Filter (Default = BOX)
                                  0,                            // Color Key (0 = disabled)
                                  NULL,                         // Ptr to SRCINFO
                                  NULL,                         // Palette entry
                                  &tempt                       // Ptr to Texture
                                  )))
                                  exit(0);
  m_tertex[0] = tempt;
  m_tertex[2] = tempt;

  sprintf (texpath,"resource\\%s","grass_bw.dds");
  
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return;
  if (FAILED(D3DXCreateTextureFromFileEx(
                                  g_D3DObject->m_d3ddevice9,    // Device
                                  texpath,
                                  D3DX_DEFAULT,D3DX_DEFAULT,    // Height/Width
                                  D3DX_DEFAULT,                 // MIP Maps
                                  0,                            // Usage
                                  D3DFMT_A8R8G8B8,              // Format
                                  D3DPOOL_DEFAULT,              // Pool
                                  D3DX_DEFAULT,                 // Filter
                                  D3DX_DEFAULT,                 // MIP Filter (Default = BOX)
                                  0,                            // Color Key (0 = disabled)
                                  NULL,                         // Ptr to SRCINFO
                                  NULL,                         // Palette entry
                                  &tempt                       // Ptr to Texture
                                  )))
                                  exit(0);
  m_tertex[1] = tempt;
  m_tertex[3] = tempt;

	RandomizeMesh();
}

D3DCOLOR randcolor(cTerrain::ENVIRONMENT env)
{
  switch (env) {
    case cTerrain::ROCK:
	   switch (rand()%3) {
		   case 3:
			   return D3DCOLOR_RGBA(128,64,0,255); break;
		   case 2:
			   return D3DCOLOR_RGBA(113,56,0,255); break;
		   case 1:
			   return D3DCOLOR_RGBA(128,64,0,255); break;
		   default:
			   return D3DCOLOR_RGBA(170,85,0,255); break;
	   }
    case cTerrain::GRASS:
	   switch (rand()%3) {
		   case 3:
			   return D3DCOLOR_RGBA(0,128,0,255); break;
		   case 2:
			   return D3DCOLOR_RGBA(84,138,2,255); break;
		   case 1:
			   return D3DCOLOR_RGBA(0,150,0,255); break;
		   default:
			   return D3DCOLOR_RGBA(100,255,0,255); break;
	   }
    case cTerrain::SNOW:
	   switch (rand()%3) {
		   case 3:
			   return D3DCOLOR_RGBA(228,254,240,255); break;
		   case 2:
			   return D3DCOLOR_RGBA(213,255,230,255); break;
		   case 1:
			   return D3DCOLOR_RGBA(228,254,200,255); break;
		   default:
			   return D3DCOLOR_RGBA(210,215,210,255); break;
	   }
    case cTerrain::MOUNTAIN:
	   switch (rand()%3) {
		   case 3:
			   return D3DCOLOR_RGBA(0,28,140,255); break;
		   case 2:
			   return D3DCOLOR_RGBA(84,38,54,255); break;
		   case 1:
			   return D3DCOLOR_RGBA(0,50,67,255); break;
		   default:
			   return D3DCOLOR_RGBA(100,55,90,255); break;
	   }
    default:
	   switch (rand()%3) {
		   case 3:
			   return D3DCOLOR_RGBA(36,171,5,255); break;
		   case 2:
			   return D3DCOLOR_RGBA(245,210,3,255); break;
		   case 1:
			   return D3DCOLOR_RGBA(38,190,3,255); break;
		   default:
			   return D3DCOLOR_RGBA(38,100,3,255); break;
	   }
      break;
  }
}

void cTerrain::event(EVENT evnt)
{
	  switch (evnt) {
		  case RAND: RandomizeMesh(); break;
		  default: break;
	  }
}
void cTerrain::RandomizeMesh(void)
{
	sCustomVertex *VertexPtr;
	DWORD *IndexPtr;

	IndexPtr =  NULL;
	VertexPtr= NULL;
  
  if(FAILED(g_TerrainMesh->LockVertexBuffer( 0, (LPVOID *) &VertexPtr )))
    return;
  if(FAILED(g_TerrainMesh->LockIndexBuffer( 0, (LPVOID *) &IndexPtr )))
    return;
  for (int i = 0; i < TER_X_SMALL+1; i++)
	  for (int j = 0; j < TER_Z_SMALL+1; j++) {
	    VertexPtr->x = (i - (TER_X_SMALL-1)/2) * TER_WIDTH_SMALL;
	    VertexPtr->z = (j - (TER_Z_SMALL-1)/2) * TER_WIDTH_SMALL;
		  //VertexPtr->y = (float) (rand()%55/5)+33/TER_WIDTH;
		  VertexPtr->y = GetHeight(VertexPtr->x,VertexPtr->z);
      VertexPtr->diffuse = randcolor(m_env);
//		  VertexPtr->diffuse = D3DCOLOR_RGBA(255,255,255,255);
 		  VertexPtr->u = (float)(i%2);
		  VertexPtr->v = (float)(j%2);
 		  VertexPtr->u = (float)(i%5)/6;
		  VertexPtr->v = (float)(j%5)/6;

		  VertexPtr++;
	}
    for (int i = 0; i < (TER_X_SMALL); i++)
	    for (int j = 0; j < (TER_Z_SMALL); j++) {  		  
		    DWORD te = i*(TER_Z_SMALL+1) + j;

		    *IndexPtr++ = te;
		    *IndexPtr++ = te+1;

		    *IndexPtr++ = te + TER_Z_SMALL + 1;
		    *IndexPtr++ = te + TER_Z_SMALL + 2; 

		    *IndexPtr++ = te + TER_Z_SMALL + 1;
		    *IndexPtr++ = te + 1;
      }
	g_TerrainMesh->UnlockIndexBuffer();
  g_TerrainMesh->UnlockVertexBuffer();	

	IndexPtr =  NULL;
	VertexPtr= NULL;

  if(FAILED(g_TerrainMeshBig->LockVertexBuffer( 0, (LPVOID *) &VertexPtr )))
    return;
  if(FAILED(g_TerrainMeshBig->LockIndexBuffer( 0, (LPVOID *) &IndexPtr )))
    return;
  for (int i = 0; i < (TER_X_BIG+1); i++)
	  for (int j = 0; j < (TER_Z_BIG+1); j++) {
      const float out_pt = 500.0f;
		  VertexPtr->x = (i - (TER_X_BIG-1)/2) * TER_WIDTH_BIG;
		  VertexPtr->z = (j - (TER_Z_BIG-1)/2) * TER_WIDTH_BIG;

      float dx_big = (out_pt - ((TER_X_SMALL-1)/2*TER_WIDTH_SMALL))/5;
      float dz_big = (out_pt - ((TER_Z_SMALL-1)/2*TER_WIDTH_SMALL))/5;

      if ( i < (TER_X_BIG-1)/2) {
         //VertexPtr->x = -out_pt + dx_big;
      }
      else {
         //VertexPtr->x = (out_pt - dx_big)-(TER_X_BIG-1)/2 + dx_big;
      }
      if ( j < (TER_Z_BIG-1)/2) {
         //VertexPtr->x = -out_pt + dz_big;
      }
      else {
         //VertexPtr->x = out_pt - dz_big-(TER_Z_BIG-1)/2 + dz_big;
      }
		  //VertexPtr->y = (float) (rand()%55/5)+33/TER_WIDTH;
      if ((fabs(VertexPtr->x) >= (TER_X_SMALL+1)/2*TER_WIDTH_SMALL) ||
          (fabs(VertexPtr->z) >= (TER_Z_SMALL+1)/2*TER_WIDTH_SMALL))
        VertexPtr->y = max(GetHeight(VertexPtr->x,VertexPtr->z),1.0f);
      else 
        VertexPtr->y = -150.0f;
      VertexPtr->y = -1.0f;
      VertexPtr->diffuse = randcolor(m_env);
//		  VertexPtr->diffuse = D3DCOLOR_RGBA(255,255,255,255);
//      VertexPtr->diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
 		  VertexPtr->u = (float)(i%2);
		  VertexPtr->v = (float)(j%2);
//      SetHeight(VertexPtr->x,VertexPtr->z,VertexPtr->y);
		  VertexPtr++;
	}

    for (int i = 0; i < (TER_X_BIG); i++)
	    for (int j = 0; j < (TER_Z_BIG); j++) {  		  
		    DWORD te = i*(TER_Z_BIG+1) + j;

		    *IndexPtr++ = te;
		    *IndexPtr++ = te+1;

		    *IndexPtr++ = te + TER_Z_BIG + 1;
		    *IndexPtr++ = te + TER_Z_BIG + 2; 

		    *IndexPtr++= te + TER_Z_BIG + 1;
		    *IndexPtr++ = te + 1;
      }
	g_TerrainMeshBig->UnlockIndexBuffer();
  g_TerrainMeshBig->UnlockVertexBuffer();
  g_TerrainMesh->OptimizeInplace(
                            D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                            NULL, NULL, NULL, NULL);
	D3DXComputeNormals(g_TerrainMesh,NULL);
	D3DXComputeNormals(g_TerrainMeshBig,NULL);
}

cTerrain::~cTerrain(void)
{
  OnLostDevice();
 for (int i = 0; i < (TER_X+1); i++)
    delete [] m_Heights[i];
delete [] m_Heights;
}

void cTerrain::OnLostDevice(void)
{
  SAFE_RELEASE(m_tertex[0]);
  SAFE_RELEASE(m_tertex[1]);
  m_tertex[2] = NULL;
  m_tertex[3] = NULL;
	SAFE_RELEASE(g_TerrainMesh);
	SAFE_RELEASE(g_TerrainMeshBig);
}

void cTerrain::OnResetDevice(void)
{
  _Init();
}

float cTerrain::GetHeight(float x, float z)
{
  //return 1;
 x = x + (TER_X+1)/2;
 z = z + (TER_Z+1)/2;
 if (!(x >= 0 && z >= 0 && x < (TER_X+1) && z < (TER_Z+1)))
    return 5.0f;   // Default Value

 // Find triangle face
 D3DXVECTOR3 pt1, pt2, pt3;
// float       wt1, wt2, wt3;
 float       dt1, dt2, dt3;

 return  (m_Heights[(int)floor(x)][(int)floor(z)]);

 pt1 = D3DXVECTOR3(floor(x),0,floor(z));
 pt3 = D3DXVECTOR3(pt1.x+1,0,pt1.z+1);

 if (fmod(x,1) > (1-fmod(z,1)))
    pt2 = D3DXVECTOR3(pt1.x+1,0,pt1.z);
 else
    pt2 = D3DXVECTOR3(pt1.x,  0,pt1.z+1);

// pt1.y = m_Heights[(int)(pt1.x*TER_Z + pt1.z)]; 
// pt2.y = m_Heights[(int)(pt2.x*TER_Z + pt2.z)]; 
// pt3.y = m_Heights[(int)(pt3.x*TER_Z + pt3.z)]; 

 dt1 = sqrt((pt1.x-x)*(pt1.x-x) + (pt1.z-z)*(pt1.z-z));
 dt2 = sqrt((pt2.x-x)*(pt2.x-x) + (pt2.z-z)*(pt2.z-z));
 dt3 = sqrt((pt3.x-x)*(pt3.x-x) + (pt3.z-z)*(pt3.z-z));

 float height = (pt1.y * dt1 + pt2.y * dt2 + pt3.y * dt3)/3;
 return height;
}

void cTerrain::SetHeight(float x, float z, float y)
{
 x = x + (TER_X+1)/2;
 z = z + (TER_Z+1)/2;

 y = max(3.0f,y);

 if (x >= 0 && z >= 0 && x < (TER_X) && z < (TER_Z))
    m_Heights[(int)(floor(x))][(int)(floor(z))] = y;
}

void cTerrain::FlattenSquare(float x, float z, float sz)
{
//	return;
  // Get Average
  sz = fabs(sz);
  int cntr = 0;
  float yval = 0;
  for (float i = (x-sz); i < (x+sz); i++)
    for (float j = z-sz; j < (z+sz); j++)
    {
      yval += GetHeight(i,j);
      cntr++;
    }
  if (cntr == 0) return;
  yval = yval / cntr;
  for (float i = (x-sz); i < (x+sz); i++)
    for (float j = (z-sz); j < (z+sz); j++)
      SetHeight(i,j,max(yval,1.0f));

  UpdateMeshHeights();
}

void cTerrain::FlattenSphere(D3DXVECTOR3 pos, float radius)
{
  int cntr = 0;
  float yval = 0;
  for (float i = pos.x-radius; i < pos.x+radius; i++)
    for (float j = pos.z-radius; j < pos.z+radius; j++)
    {
      D3DXVECTOR3 tvec = D3DXVECTOR3(pos.x-i,pos.y-GetHeight(i,j),pos.z-j);
      if (D3DXVec3Length(&tvec) < radius) 
      {
        yval = max(GetHeight(i,j) - radius + D3DXVec3Length(&tvec),1.0f);
        SetHeight(i,j,yval);
      }
    }
  UpdateMeshHeights();
}


void cTerrain::UpdateMeshHeights()
{

  RandomizeMesh();
}