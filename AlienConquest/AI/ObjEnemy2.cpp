#include "StdAfx.h"
#include "objenemy2.h"
#include "objenemyweapon2.h"
#include "objmgr.h"

int CObjEnemy2::m_graph_init = 0;

CTexture **CObjEnemy2::m_regular = new CTexture*[3];
CTexture **CObjEnemy2::m_firing = new CTexture*[9];
CTexture **CObjEnemy2::m_jetting = new CTexture*[12];

void CObjEnemy2::move(void)
{
	float cur_spd_x;
	if (m_state == JETTING) { cur_spd_x = m_speed_x; m_speed_x = -250; CObj::move(); m_speed_x=cur_spd_x;  }
	else CObj::move();
}

CObjEnemy2::CObjEnemy2(void)
{
	CObj();

	if (!m_graph_init++) _LoadGraphics();
	m_state = REGULAR;
	m_max_x= m_org_max_x = 120;
	m_max_y=50;
	m_type = ENEMY;
	m_curdamage = 0;

	m_boundrectnum = 4;
	m_boundrects = new RECT [m_boundrectnum];
	SetRect((LPRECT)&m_boundrects[0],35,7,72,69);
	SetRect((LPRECT)&m_boundrects[1],72,27,99,43);
	SetRect((LPRECT)&m_boundrects[2],55,69,61,84);
	SetRect((LPRECT)&m_boundrects[3],8,86,47,91);

}

CObjEnemy2::~CObjEnemy2(void)
{
	if(!--m_graph_init)_UnloadGraphics();
	delete []m_boundrects;
}

void CObjEnemy2::paint()
{
	D3DXVECTOR2 pnt;
	pnt.x = m_dpos_x;
	pnt.y = m_dpos_y;

//	if (m_state == REGULAR && (m_fire_time.PeekTime() > 1300)) { m_fire_time.Reset(); Fire(); }
//	if (m_state == REGULAR && (m_jet_time.PeekTime() > 1500)) { m_jet_time.Reset(); Jet(); }

	switch (m_state) {
		case JETTING:
			m_curtexture = m_jetting[m_jet_seq%4];
			if (m_jet_seq >= 10) m_state = REGULAR;
			if (m_ani_time.PeekTime() > 120) { m_jet_seq++; m_ani_time.Reset(); }
			break;
		case FIRING:
			m_curtexture = m_firing[m_fir_seq%3];
			if (m_fir_seq >= 3) m_state = REGULAR;
			if (m_ani_time.PeekTime() > 95) { m_fir_seq++; m_ani_time.Reset(); }
		    break;
		case REGULAR:
		default:
			if (m_regular[m_curdamage] != NULL)
				m_curtexture = m_regular[m_curdamage];
			break;
	};
	CObj::paint();
}

void CObjEnemy2::Jet()
{
	if (m_state == REGULAR) {
		m_state = JETTING;
		m_jet_seq = 0;
		// Save the previous accel and speed for jettin!
		//m_jet_spd_x = m_speed_x;
		//m_jet_accel_x = m_accel_x;
		//accel(-55,0);
	}
}
void CObjEnemy2::Fire()
{
	if (m_state == REGULAR) {
		m_state = FIRING;
		m_fir_seq = 0;
		CObjEnemyWeapon2 *bull = new CObjEnemyWeapon2;
		bull->SetPosition(m_dpos_x+8,m_dpos_y+87);
		bull->SetSpeed(-750,0);
		bull->SetAccel(-50,0);
		g_ObjMgr->add(bull);
	}
}

void CObjEnemy2::_LoadGraphics()
{
   RECT trect;
   int dmg=0;
   SetRect(&trect,1,1,126,97);
   OutputDebugString("Loading Cenemy2 graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead2.bmp",0xFFFF00FF,&trect);
   m_regular[1] = new CTexture("resource/enemyhead2damage.bmp",0xFFFF00FF,&trect);
   m_regular[2] = new CTexture("resource/enemyhead2damage2.bmp",0xFFFF00FF,&trect);
//   SetRect(&trect,1,195,126,291);
   SetRect(&trect,1,1,126,97);
   for (int x=0; x < 3; x++) {
	   int offset = dmg*m_numdamage;
	   trect.left=x*126+1;
	   trect.right = trect.left +125;
	   m_firing[x]=new CTexture("resource/enemyhead2fireing.bmp",0xFFFF00FF,&trect);
   }
   SetRect(&trect,1,1,126,97);
   for (int x=0; x < 4; x++) {
	   trect.left=x*126+1;
	   trect.right = trect.left +125;
	   m_jetting[x]=new CTexture("resource/enemyhead2moving.bmp",0xFFFF00FF,&trect);
   }
}

void CObjEnemy2::Collision(CObj *colwith)
{
   if (++m_curdamage > 0)
	   m_state=DEAD;
}
void CObjEnemy2::_UnloadGraphics()
{
	delete m_regular[0];
	for (int x=0; x < 3; x++)
	  delete m_firing[x];
	for (int x=0; x < 4; x++)
	  delete m_jetting[x];

}