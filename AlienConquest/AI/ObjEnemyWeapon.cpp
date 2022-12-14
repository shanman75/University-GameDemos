#include "StdAfx.h"
#include "objenemyweapon.h"
//#include "objmgr.h"


int CObjEnemyWeapon::m_graph_init = 0;
//int CObjEnemyWeapon::m_snd_played = 0;
//CSound *CObjEnemyWeapon::m_regsnd = NULL;
CTexture *CObjEnemyWeapon::m_regular[1];
RECT CObjEnemyWeapon::m_myboundrects[1];


CObjEnemyWeapon::CObjEnemyWeapon(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=800;
	m_age=800;
	m_max_y=0;
	m_z = 2;
	m_type = ENEMYWEAPON;
	m_boundrectnum = 1;
//	m_boundrects = new RECT [m_boundrectnum];
	m_boundrects = m_myboundrects;
}

CObjEnemyWeapon::~CObjEnemyWeapon(void)
{
	if (!--m_graph_init) {_UnloadGraphics();}
}

void CObjEnemyWeapon::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,132,88,149,95);
   OutputDebugString("Loading Cenemy weapon graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead1.bmp",0xFFFF00FF,&trect);
//   m_regsnd = new CSound ("resource/enemyweapon1.wav");

   	SetRect((LPRECT)&m_myboundrects[0],0,0,18,8);
}

void CObjEnemyWeapon::_UnloadGraphics()
{
	delete m_regular[0];
//	delete m_regsnd;
}

void CObjEnemyWeapon::paint()
{
//	if (!m_snd_played) {
//		m_snd_played=1;
//		m_regsnd->play();
//	}
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[0];
			break;
	};   
	CObj::paint();
}
