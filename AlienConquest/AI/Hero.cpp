#include "StdAfx.h"
#include "hero.h"

int CHero::m_graph_init = 0;
CTexture *CHero::m_regular[1];

CHero::CHero(void)
{
  if (!m_graph_init) _LoadGraphics();
	m_state = REGULAR;
	m_max_x= 180;
	m_max_y= 170;
	m_time.GetTime();
}

CHero::~CHero(void)
{
	_UnloadGraphics();
}

void CHero::_LoadGraphics(void)
{
   RECT trect;
   SetRect(&trect,1,1,126,97);
   OutputDebugString("Loading Hero graphics\n");
   m_regular[0] = new CTexture("resource/herodumdum2.bmp",0xFFFF00FF,NULL);
}

void CHero::_UnloadGraphics(void)
{
	delete m_regular[0];
}

void CHero::paint(void)
{
	D3DXVECTOR2 pnt;
	pnt.x = (int)m_pos_x;
	pnt.y = (int)m_pos_y;
	switch (m_state) {
		case REGULAR:
		default:
			if (m_regular[0] != NULL)
				m_regular[0]->Paint(&pnt);
			break;
	};
}