#pragma once

#include "Timer.h"
#include "World.h"
#include "Text.h"
#include "Obj.h"
#include "Hero.h"
#include "Hero2.h"
#include "BkGround.h"
#include "ObjEnemy.h"
#include "ObjEnemy2.h"
#include "ObjEnemy3.h"
#include "ObjEnemyWeapon.h"
#include "ObjEnemyWeapon2.h"
#include "ObjEnemyWeapon3.h"
#include "ObjHeroWeaponMain.h"
#include "ObjHeroWeaponMissile.h"
#include "ObjHeroWeaponLaser.h"
#include "ObjBmp.h"
#include "ObjPowerUp.h"

class CObjMgr
{
public:
	CObjMgr(void);
	~CObjMgr(void);
	void move();
	void paint();
	void coldet();

	void add(CObj *);
	void del(CObj *);
	void spawn(void);
	void reset(void);
	static const int m_numz;
	void Score(int numtoscore, int playernum);
	void DrawText(const char *textstr, int x, int y);

	int GetNumObj(void) { return m_numobj; }

	const static int MAX_OBJECTS = 500;
	const static int MAX_ANIMATE = 10;
private:
	CObj *m_obj[MAX_OBJECTS];
	int m_numobj;
	CTimer m_time;
	CWorld m_world;
	CTimer m_spawn_tim;

	int m_spawn_interval;

	void spawnOne(void);
	int m_player1_score;
	D3DXVECTOR2 m_player1_scorexy;
	D3DXVECTOR2 m_player1_livesxy;

	static char CObjMgr::m_scoreline[500];
	CText m_D3DText;
	void PaintDashBoards();

	CTexture *m_scoreboard;
	CTexture *m_scoremiddle;
	CTexture *m_powerups[4];
};

extern CObjMgr *g_ObjMgr;
