#pragma once
#include "obj.h"
#include "Timer.h"
#include "Texture.h"

class CObjEnemy :
	public CObj
{
public:
	CObjEnemy(void);
	virtual ~CObjEnemy(void);
	void Fire();
private:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[1];
	static CTexture *m_firing[4];

	static int m_graph_init;

	static RECT my_boundrects[4];

	int m_fir_seq;
	CTimer m_ani_time;
	CTimer m_fire_time;
};
