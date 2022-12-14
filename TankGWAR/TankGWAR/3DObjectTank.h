#pragma once

#include "3DObject.h"
#include "3DObjectMissile.h"
#include "D3DObject.h"
#include "Timer.h"


class cGameState;

class c3DObjectTank:
	public c3DObject
{
public:
  friend class cGameState;
  friend class minimap;

  static enum FIRE_TYPE {MISSILE,ABOMB};
  static enum EVENT {UP,DOWN,LEFT,RIGHT,PWRUP,PWRDN};
  static enum SKINS { BLUE, BROWN, CAMOBLUE, GREY, GREEN, ORANGE, RAINBOW, RED, STEELBLUE };
  static const int NUM_SKINS = 9;

  c3DObjectTank();
	~c3DObjectTank(void);
	void paint(void);
//	void SetXYZ(float x, float y, float z) { m_xPos = x; m_yPos=y; m_zPos = z; }
  c3DObject * Fire(enum c3DObjectMissile::MSLTYPE tpe = c3DObjectMissile::MSLTYPE::SHELL);
  void event(enum EVENT, float amount=1.0f);
  void skin(enum SKINS sk) { m_skin = sk; }
  void FadeOut(float secs);
  void ResetFade() { m_fading = false; }
  void ResetLevel();

  static const float tank_width;
  static const float tank_height;

  void RotateTo( float t_turretRotate, float t_barrelHeight, float t_firePower);
  BOOL RotatedTo();
  void RotateReset() { m_bRotateTo = false; }
  void PerformRotateTo();
  CTimer m_tmrRotate;

protected:
  void MakeWorldMatrix(int x);
	void _LoadGraphics(void);
	void _UnloadGraphics();

  float m_turretRotate;
  float m_barrelHeight;
  float m_firePower;

  float m_turretRotateTo;
  float m_barrelHeightTo;
  float m_firePowerTo;
  bool m_bRotateTo;

  enum SKINS m_skin;

  CTimer m_keytime, m_firetime;
  
	static LPD3DXMESH			        m_tankmesh;
  static DWORD				          m_tankNmat;
  static LPDIRECT3DTEXTURE9*	  m_tanktex;
  static LPDIRECT3DTEXTURE9*    m_skintex;

  static D3DMATERIAL9*		      m_tankmat;
	static int					          m_graph_init;

  float m_fade;
  float m_fadedur;
  BOOL m_fading;
  CTimer m_tmFade;
};
