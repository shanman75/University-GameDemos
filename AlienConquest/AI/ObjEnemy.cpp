#include "StdAfx.h"
#include "objenemy.h"
#include "ObjEnemyWeapon.h"
#include "objmgr.h"

int CObjEnemy::m_graph_init = 0;
CTexture *CObjEnemy::m_regular[1];
CTexture *CObjEnemy::m_firing[4];


CObjEnemy::CObjEnemy(void)
{
	CObj();
	if (!m_graph_init) _LoadGraphics();
	m_max_x=5;
	m_max_y=2;
	m_time.GetTime();
}

CObjEnemy::~CObjEnemy(void)
{
	_UnloadGraphics();
}

void CObjEnemy::paint()
{
	switch (m_state) {
		case FIRING:
			m_curtexture = m_firing[m_fir_seq];
			if (m_fir_seq >= 4) m_state = REGULAR;
			if (m_ani_time.PeekTime() > 135) { m_fir_seq++; m_ani_time.Reset(); }
		    break;
		case REGULAR:
		default:
			m_curtexture = m_regular[0];
			break;
	};
	CObj::paint();
}

void CObjEnemy::Fire()
{
	if (m_state != FIRING) {
		m_state = FIRING;
		m_fir_seq = 0;
		CObjEnemyWeapon *bull = new CObjEnemyWeapon;
		bull->SetPosition(m_dpos_x+32,m_dpos_y+47);
		bull->SetSpeed(-350,0);
		bull->SetAccel(-45,0);
		g_ObjMgr->add(bull);
	}
}

void CObjEnemy::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,1,1,130,96);
   OutputDebugString("Loading Cenemy graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead1.bmp",0xFFFF00FF,&trect);
   SetRect(&trect,1,97,130,193);
   for (int x=0; x < 3; x++) {
	   trect.left=x*130+1;
	   trect.right = trect.left +130;
	   //m_firing[x]=new CTexture("resource/enemyhead1.bmp",0xFFFF00FF,&trect);
   }
   SetRect(&trect,1,193,130,193+95);
   for (int x=0; x < 4; x++) {
	   trect.left=x*131+1;
	   trect.right = trect.left +130;
	   m_firing[x]=new CTexture("resource/enemyhead1.bmp",0xFFFF00FF,&trect);
   }
   m_graph_init = 1;
}

void CObjEnemy::_UnloadGraphics()
{
	delete m_regular[0];
	for (int x=0; x < 4; x++)
	  delete m_firing[x];
}