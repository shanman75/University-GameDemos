#pragma once
//#define DIRECTINPUT_VERSION 0x800
#include "Timer.h"
#include <dinput.h>
#include "Hero.h"
#include "Hero2.h"
#include "ObjBmp.h"
#include "ObjMenu.h"

#define SafeRelease(x) if (x) {x->Release(); x=NULL;}

class D3DInput
{
public:
	D3DInput(void);
	~D3DInput(void);
	void GetInput(CHero *hero);
	void GetInput(CHero2 *hero);
	void GetInput(CObjBmp *bmp);
	void GetInput(CObjMenu *mnu);
private:
	IDirectInput8 *m_DInput;
	IDirectInputDevice8 *m_DIKB;
};

extern D3DInput *g_D3DInput;
