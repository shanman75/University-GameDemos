#pragma once

#define TIMER_MAXDLTA 4294967294

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);
	const static DWORD maxtime = 65535;
	void CTimer::UpdateClock();
	DWORD CTimer::GetTime();
	DWORD CTimer::UpdateGetTime();
	void CTimer::Reset();
	DWORD CTimer::PeekTime();
	DWORD CTimer::UpdatePeekTime();
	void CTimer::TogglePause();
	void CTimer::Pause();
	BOOL CTimer::CmpTime(DWORD cmptm);
	
private:
	DWORD m_ltime;
	static DWORD m_gtime;
	static BOOL m_pause;
	static int m_dltime;
};
