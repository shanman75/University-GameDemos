#include "StdAfx.h"
#include "skybox.h"
#include "GameState.h"
#include <stdio.h>

LPDIRECT3DTEXTURE9	  *cSkyBox::m_tertex[SKY_MAX];
ID3DXMesh *cSkyBox::m_SkyMesh;

LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbback;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbfront;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbleft;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbright;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbtop;

const DWORD cSkyBox::m_FVF=D3DFVF_XYZ|D3DFVF_TEX1; //flexible vertex format


cSkyBox::cSkyBox(void)
{
	m_w = SKY_WIDTH;
	m_h = SKY_HEIGHT;
  SetSky(SKYTYPES::MORNING);
  if (!_Init())
    exit (1);
}

cSkyBox::~cSkyBox(void)
{
  OnLostDevice();
}

void cSkyBox::Paint()
{
    D3DXMATRIXA16 matView, matViewSave;
    g_D3DObject->m_d3ddevice9->GetTransform( D3DTS_VIEW, &matViewSave );
    matView = matViewSave;

    matView._41 = 0; matView._42 = 0.0f; matView._43 = 0;
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW,      &matView );

    g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZENABLE, FALSE );
    g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    g_D3DObject->m_d3ddevice9->SetRenderState ( D3DRS_LIGHTING, FALSE);
	  D3DXMATRIXA16 matwrld;
	  D3DXMatrixIdentity(&matwrld);
	  D3DXVECTOR3 pos, blah;

	  g_D3DObject->m_d3ddevice9->SetTransform(D3DTS_WORLD,&matwrld);

    D3DMATERIAL9 mtrl,mtrlSave;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Ambient = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
    mtrl.Specular = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
    mtrl.Diffuse = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
    mtrl.Emissive = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
    mtrl.Power = 0.0f;
    g_D3DObject->m_d3ddevice9->GetMaterial(&mtrlSave);
    g_D3DObject->m_d3ddevice9->SetMaterial(&mtrl);
    
	if (m_sbtop!= NULL)
  {    
    g_D3DObject->m_d3ddevice9->SetFVF(m_FVF); 

	  g_D3DObject->m_d3ddevice9->SetTexture      (0,m_tertex[m_skytype][0]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource (0,m_sbfront,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive   (D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture      (0,m_tertex[m_skytype][1]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource (0,m_sbright,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive   (D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture      (0,m_tertex[m_skytype][2]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource (0,m_sbback,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive   (D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture      (0,m_tertex[m_skytype][3]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource (0,m_sbleft,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive   (D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture      (0,m_tertex[m_skytype][4]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource (0,m_sbtop,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive   (D3DPT_TRIANGLESTRIP,0,2);
  }

  g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW,      &matViewSave );
  g_D3DObject->m_d3ddevice9->SetMaterial(&mtrlSave);
  g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZENABLE, TRUE );
  g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
  g_D3DObject->m_d3ddevice9->SetRenderState ( D3DRS_LIGHTING, FALSE);
}

BOOL cSkyBox::_Init()
{

  /*
  D3DXCreateMeshFVF( 2, 4 , 
	  D3DXMESH_32BIT | D3DXMESH_SYSTEMMEM, 
	  D3DFVF_SKYMESH, 
	  g_D3DObject->m_d3ddevice9, 
	  &m_SkyMesh 
	);
  */

  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				m_FVF,
				D3DPOOL_DEFAULT,
				&m_sbback,
				NULL))
	)
        return FALSE; //bail if failed

  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				m_FVF,
				D3DPOOL_DEFAULT,
				&m_sbfront,
				NULL))
	)
        return FALSE; //bail if failed

    if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				m_FVF,
				D3DPOOL_DEFAULT,
				&m_sbleft,
				NULL))
	)
        return FALSE; //bail if failed


  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				m_FVF,
				D3DPOOL_DEFAULT,
				&m_sbright,
				NULL))
	)
        return FALSE; //bail if failed


  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				m_FVF,
				D3DPOOL_DEFAULT,
				&m_sbtop,
				NULL))
	)
        return FALSE; //bail if failed

  
  float h = m_h;
  float w = m_w;

  BILLBOARDVERTEX *pTetVB = NULL; //Pointer to vertex buffer.

#define SK_1 0.994f
#define SK_0 0.006f

//#define SK_1 1.0f
//#define SK_0 0.0f

  if(SUCCEEDED(m_sbleft->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3  (-w/2,-h/2,-w/2);    pTetVB->tu=SK_0;  	pTetVB->tv=SK_1;
  (++pTetVB)->p=D3DXVECTOR3(-w/2, h/2,-w/2);    pTetVB->tu=SK_0;    pTetVB->tv=SK_0;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2,-h/2, w/2);    pTetVB->tu=SK_1;	pTetVB->tv=SK_1;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2, h/2, w/2);	 pTetVB->tu=SK_1;	  pTetVB->tv=SK_0;
    m_sbleft->Unlock();
  }

  if(SUCCEEDED(m_sbright->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3  ( w/2,-h/2, w/2);    pTetVB->tu=SK_0;  	pTetVB->tv=SK_1;
  (++pTetVB)->p=D3DXVECTOR3( w/2, h/2, w/2);    pTetVB->tu=SK_0;    pTetVB->tv=SK_0;
    
  (++pTetVB)->p=D3DXVECTOR3( w/2,-h/2,-w/2);    pTetVB->tu=SK_1;	pTetVB->tv=SK_1;
    
  (++pTetVB)->p=D3DXVECTOR3( w/2, h/2,-w/2);	 pTetVB->tu=SK_1;	  pTetVB->tv=SK_0;
    m_sbright->Unlock();
  }

  if(SUCCEEDED(m_sbback->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3  ( w/2,-h/2,-w/2);    pTetVB->tu=SK_0;  	pTetVB->tv=SK_1;
  (++pTetVB)->p=D3DXVECTOR3( w/2, h/2,-w/2);    pTetVB->tu=SK_0;    pTetVB->tv=SK_0;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2,-h/2,-w/2);    pTetVB->tu=SK_1;	pTetVB->tv=SK_1;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2, h/2,-w/2);	 pTetVB->tu=SK_1;	  pTetVB->tv=SK_0;
    m_sbback->Unlock();
  }

  if(SUCCEEDED(m_sbfront->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3  (-w/2,-h/2, w/2);    pTetVB->tu=SK_0;  pTetVB->tv=SK_1;
  (++pTetVB)->p=D3DXVECTOR3(-w/2, h/2, w/2);    pTetVB->tu=SK_0;  pTetVB->tv=SK_0;    
  (++pTetVB)->p=D3DXVECTOR3( w/2,-h/2, w/2);    pTetVB->tu=SK_1;	pTetVB->tv=SK_1;    
  (++pTetVB)->p=D3DXVECTOR3( w/2, h/2, w/2);	  pTetVB->tu=SK_1;	pTetVB->tv=SK_0;
    m_sbfront->Unlock();
  }

  if(SUCCEEDED(m_sbtop->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
  //  pTetVB->p  =D3DXVECTOR3(-w/2,h/2, w/2);    pTetVB->tu=SK_0;  	pTetVB->tv=SK_1;
  //(++pTetVB)->p=D3DXVECTOR3(-w/2,h/2,-w/2);    pTetVB->tu=SK_0;   pTetVB->tv=SK_0;
  //(++pTetVB)->p=D3DXVECTOR3( w/2,h/2, w/2);    pTetVB->tu=SK_1;	  pTetVB->tv=SK_1;
  //(++pTetVB)->p=D3DXVECTOR3( w/2,h/2,-w/2);	   pTetVB->tu=SK_1;	  pTetVB->tv=SK_0;

    (pTetVB)->p=D3DXVECTOR3(-w/2,h/2,-w/2);    pTetVB->tu=SK_0;   pTetVB->tv=SK_1;
  (++pTetVB)->p=D3DXVECTOR3( w/2,h/2,-w/2);	   pTetVB->tu=SK_0;	  pTetVB->tv=SK_0;
  (++pTetVB)->p  =D3DXVECTOR3(-w/2,h/2, w/2);    pTetVB->tu=SK_1;  	pTetVB->tv=SK_1;
  (++pTetVB)->p=D3DXVECTOR3( w/2,h/2, w/2);    pTetVB->tu=SK_1;	  pTetVB->tv=SK_0;

    m_sbtop->Unlock();
  }


  char texpath[1024];
  LPDIRECT3DTEXTURE9 tempt;

  for (int c=0; c< (int)SKY_MAX; c++)  {
  char *basedir;
  switch ((SKYTYPES)c) {
    case ANTARCTICA: basedir = "worlds\\dawn"; break;
    case DAWN:       basedir = "worlds\\dawn"; break;
    case MORNING:    basedir = "worlds\\morning"; break;
    default: exit(0);
  }
  char *basefname = "newsky000";
//  char *basedir = "attica";
// char *basefname = "attica000";
  m_tertex[c] = new LPDIRECT3DTEXTURE9 [5];

  sprintf (texpath,"resource\\%s\\%s%s.png",basedir,basefname,"1");
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return false;
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
                                  exit(1);
  m_tertex[c][0] = tempt;

  sprintf (texpath,"resource\\%s\\%s%s.png",basedir,basefname,"2");
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return false;
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
                                  exit(1);
  m_tertex[c][1] = tempt;

  sprintf (texpath,"resource\\%s\\%s%s.png",basedir,basefname,"3");
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return false;
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
                                  exit(1);
  m_tertex[c][2] = tempt;

  sprintf (texpath,"resource\\%s\\%s%s.png",basedir,basefname,"4");
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return false;
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
                                  exit(1);
  m_tertex[c][3] = tempt;

  sprintf (texpath,"resource\\%s\\%s%s.png",basedir,basefname,"5");
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return false;
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
                                  exit(1);
  m_tertex[c][4] = tempt;
  }

  sSkyVertex *VertexPtr;
	DWORD *IndexPtr;

	IndexPtr =NULL;
	VertexPtr =NULL;

	//m_SkyMesh->LockVertexBuffer( 0, (LPVOID *) &VertexPtr );	
	//m_SkyMesh->LockIndexBuffer( 0, (LPVOID *) &IndexPtr );	
    
	/*
    VertexPtr[0].p=D3DXVECTOR3(0,m_h,0); 
    VertexPtr[0].tu=1.0f; VertexPtr[0].tv=0.0f;

    VertexPtr[1].p=D3DXVECTOR3(0,m_h,-m_w);
    VertexPtr[1].tu=0.0f; VertexPtr[1].tv=0.0f;
    
    VertexPtr[2].p=D3DXVECTOR3(0,0,0);
    VertexPtr[2].tu=1.0f; VertexPtr[2].tv=1.0f;
    
    VertexPtr[3].p=D3DXVECTOR3(0,0,-m_w);
    VertexPtr[3].tu=	0.0f; VertexPtr[3].tv=1.0;


        posx[0].p=D3DXVECTOR3(w,h,-w);
    posx[0].tu=1.0f; posx[0].tv=0.0f;
    posx[1].p=D3DXVECTOR3(w,h,0);
    posx[1].tu=0.0f; posx[1].tv=0.0f;
    posx[2].p=D3DXVECTOR3(w,0,-w);
    posx[2].tu=1.0f; posx[2].tv=1.0f;
    posx[3].p=D3DXVECTOR3(w,0,0);
    posx[3].tu=0.0f; posx[3].tv=1.0f;

    */

// Draw the four sides
   //*IndexPtr++ = 0;   *IndexPtr++ = 1;    *IndexPtr++ = 2;
   //*IndexPtr++ = 3;   *IndexPtr++ = 2;    *IndexPtr++ = 1;

   /*
   *IndexPtr++ = 1;   *IndexPtr++ = 5;    *IndexPtr++ = 3;
   *IndexPtr++ = 7;   *IndexPtr++ = 3;    *IndexPtr++ = 5;

   *IndexPtr++ = 3;   *IndexPtr++ = 7;    *IndexPtr++ = 2;
   *IndexPtr++ = 6;   *IndexPtr++ = 2;    *IndexPtr++ = 7;

   *IndexPtr++ = 2;   *IndexPtr++ = 6;    *IndexPtr++ = 0;
   *IndexPtr++ = 4;   *IndexPtr++ = 0;    *IndexPtr++ = 6;

// Then the top
   *IndexPtr++ = 5;  *IndexPtr++ = 4;     *IndexPtr++ = 7;
   *IndexPtr++ = 6;  *IndexPtr++ = 7;     *IndexPtr++ = 4;
   */

//	m_SkyMesh->UnlockIndexBuffer();
//  m_SkyMesh->UnlockVertexBuffer();	

	/*
  m_SkyMesh->OptimizeInplace(
                            D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                            NULL, NULL, NULL, NULL);
    */
	//D3DXComputeNormals(g_SkyMesh,NULL);
	return true;
}

void cSkyBox::OnLostDevice(void)
{
  for (int c = 0; c < (int) SKY_MAX; c++) {
    SAFE_RELEASE(m_tertex[c][0]);
    SAFE_RELEASE(m_tertex[c][1]);
    SAFE_RELEASE(m_tertex[c][2]);
    SAFE_RELEASE(m_tertex[c][3]);
    SAFE_RELEASE(m_tertex[c][4]);
  }
  if (m_SkyMesh) m_SkyMesh->Release();
  m_sbtop->Release();
  m_sbback->Release();
  m_sbfront->Release();
  m_sbleft->Release();
  m_sbright->Release();
}

void cSkyBox::OnResetDevice()
{
  _Init();
}

void cSkyBox::SetSky(SKYTYPES sky)
{
   m_skytype = sky;
}

void cSkyBox::RandomizeSky()
{
  SKYTYPES st;
  switch (rand()%4)
  {
  case 3:
    st = SKYTYPES::MORNING; break;
  case 2:
    st = SKYTYPES::DAWN; break;
  case 1:
    st = SKYTYPES::ANTARCTICA; break;
  default: 
    st = SKYTYPES::MORNING; break;
  }
   SetSky (st);
}