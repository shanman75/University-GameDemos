#include "StdAfx.h"
#include "camerabehindtank.h"
#include "GameState.h"

cCameraBehindTank::cCameraBehindTank(void)
{
}

cCameraBehindTank::~cCameraBehindTank(void)
{
}


void cCameraBehindTank::SetCamera()
{
    D3DXMATRIX matView;

    D3DXVECTOR3 tpos, tor;

    g_GameState->GetCurrentTankState(&tpos, &tor);

    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( tpos.x - 5*sin(tor.x), tpos.y + 2, tpos.z -5*cos(tor.x) );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( tpos.x, tpos.y+1.3f, tpos.z );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );


    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)WIDTH) / HEIGHT;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 33500.0f );
	  g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );   
}