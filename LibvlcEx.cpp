#include "stdafx.h"
#include "LibvlcEx.h"
#include "Conver.hpp"
CLibvlcEx::CLibvlcEx(HWND hwnd)
	:m_AttachHwnd(hwnd)
	, m_instance(nullptr)
	, m_media(nullptr)
	, m_vlcPlayer(nullptr)
	, m_media_type(false)
	, m_hWndParent(NULL)
	, m_oldProc(nullptr)

{
	try
	{
		if (!Init())
		{
			throw std::logic_error("Vlc 初始化失败");
		}
	}
	catch (std::exception* e)
	{
		//
		OutputDebugStringA(e->what());
	}
}


CLibvlcEx::~CLibvlcEx()
{
	if (m_vlcPlayer)
	{
		libvlc_media_player_release(m_vlcPlayer);
		m_vlcPlayer = nullptr;
	}
	if (m_media)
	{
		libvlc_media_release(m_media);
		m_media = nullptr;
	}
	if (m_instance)
	{
		libvlc_release(m_instance);
		m_instance = nullptr;
	}
	
}

bool CLibvlcEx::play(const std::string url)
{
	bool bResult = false;
	if (url.empty() || !m_instance)
	{
		return bResult;
	}
	m_media_type = checkUrl(url) ? 1 : 0;
	if (m_media)
	{
		libvlc_media_release(m_media);
		m_media = nullptr;
	}
	if (m_vlcPlayer)
	{
		libvlc_media_player_release(m_vlcPlayer);
		m_vlcPlayer = nullptr;
	}
	if (m_media_type)
	{
		//
		m_media = libvlc_media_new_location(m_instance, url.c_str());
	}
	else
	{
		//
		m_media = libvlc_media_new_path(m_instance, url.c_str());
	}
	if (!m_media)
	{
		return bResult;
	}
	//解析媒体文件
	libvlc_media_parse(m_media);
	//创建媒体播放器
	m_vlcPlayer = libvlc_media_player_new_from_media(m_media);
	if (!m_vlcPlayer)
	{
		return bResult;
	}
	//设置附加到指定窗口播放
	libvlc_media_player_set_hwnd(m_vlcPlayer, m_AttachHwnd);
	if (!libvlc_media_player_is_playing(m_vlcPlayer))
	{
		if (libvlc_media_player_play(m_vlcPlayer) != -1)
		{
			bResult = true;
			
		}
	}
	else
	{
		bResult = true;
	}
	DWORD dwBeg = ::GetTickCount();
	DWORD dwEnd = 0;
	while (true)
	{
		dwEnd = ::GetTickCount();
		if ((dwEnd-dwBeg) > 5000 || libvlc_media_player_is_playing(m_vlcPlayer))
		{
			break;
		}
	}
	return bResult;
}

bool CLibvlcEx::pause()
{
	bool bResult = false;
	if (!m_vlcPlayer)
	{
		return bResult;
	}
	if (libvlc_media_player_is_playing(m_vlcPlayer))
	{
		libvlc_media_player_set_pause(m_vlcPlayer, 1);
		bResult = true;
	}
	else
	{
		libvlc_media_player_set_pause(m_vlcPlayer, 0);
		bResult = true;
	}
	return bResult;
}

bool CLibvlcEx::resume()
{
	bool bResult = false;
	if (!m_vlcPlayer)
	{
		return bResult;
	}
	if (libvlc_media_player_is_playing(m_vlcPlayer))
	{
		libvlc_media_player_set_pause(m_vlcPlayer, 1);
		bResult = true;
	}
	else
	{
		libvlc_media_player_set_pause(m_vlcPlayer, 0);
		bResult = true;
	}
	return bResult;
}

bool CLibvlcEx::setRate(float rate)
{
	bool bResult = false;
	if (!m_vlcPlayer)
		return bResult;
	if (libvlc_media_player_set_rate(m_vlcPlayer,rate)!=-1)
	{
		bResult = true;
	}
	return bResult;
}

float CLibvlcEx::getRate()
{
	float fResult = 0.0;
	if (!m_vlcPlayer)
	{
		return fResult;
	}

	fResult = libvlc_media_player_get_rate(m_vlcPlayer);
	return fResult;
}

bool CLibvlcEx::setAudioVolume(int volume)
{
	bool bResult = false;
	if (!m_vlcPlayer)
		return bResult;
	if (-1 != libvlc_audio_set_volume(m_vlcPlayer, volume))
		bResult = true;
	return bResult;
}

int CLibvlcEx::getAudioVolume()
{
	int volume = -1;
	if (!m_vlcPlayer)
		return volume;
	volume = libvlc_audio_get_volume(m_vlcPlayer);
	return volume;
}

libvlc_state_t CLibvlcEx::getState()
{
	libvlc_state_t state = libvlc_NothingSpecial;
	if (!m_vlcPlayer)
		return state;
	state = libvlc_media_player_get_state(m_vlcPlayer);
	return state;
}

bool CLibvlcEx::stop()
{
	bool bResult = false;
	if (!m_vlcPlayer)
	{
		return bResult;
	}
	if (libvlc_media_player_is_playing(m_vlcPlayer))
	{
		if (this->GetVideoFullScreenStatus())
		{
			this->SetVideoFullScreenStatus(false);
		}
		libvlc_media_player_stop(m_vlcPlayer);
		libvlc_media_release(m_media);
		m_media = nullptr;
		libvlc_media_player_release(m_vlcPlayer);
		m_vlcPlayer = nullptr;
		bResult = true;
	}
	else
	{
		if (m_media)
		{
			libvlc_media_release(m_media);
			m_media = nullptr;
		}
		if (m_vlcPlayer)
		{
			libvlc_media_player_release(m_vlcPlayer);
			m_vlcPlayer = nullptr;
		}
		bResult = true;
	}
	return bResult;
}


libvlc_media_player_t* CLibvlcEx::GetPlayerHandle() const
{
	return m_vlcPlayer;
}

bool CLibvlcEx::GetVideoFullScreenStatus()
{
	bool bResult = false;
	if (m_vlcPlayer)
	{
		bResult = libvlc_get_fullscreen(m_vlcPlayer);
		
	}
	return bResult;
}
LRESULT CALLBACK CLibvlcEx::StaticWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto pThis = (CLibvlcEx*)::GetWindowLongPtr(hwnd, DWL_MSGRESULT);
	if (!pThis)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	else
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_ESCAPE:
			{
				pThis->SetVideoFullScreenStatus(false);
				break;
			}
			default:
				break;
			}
			return TRUE;
		}
		default:
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
bool CLibvlcEx::SetVideoFullScreenStatus(bool status)
{
	bool bResult = false;
	if (m_vlcPlayer)
	{
		
		//libvlc_set_fullscreen(m_vlcPlayer,status);
		if (status)
		{
			//pause();
			int cx = GetSystemMetrics(SM_CXSCREEN);
			int cy = GetSystemMetrics(SM_CYSCREEN);
			GetWindowRect(m_AttachHwnd,&m_rectPlayer);  //保留本视频窗口大小 
			//获得其窗口风格
			m_lStyle = GetWindowLong(m_AttachHwnd, GWL_STYLE);
			m_lStyleEx = GetWindowLong(m_AttachHwnd, GWL_EXSTYLE);
			m_oldProc = (WNDPROC)::GetWindowLongPtr(m_AttachHwnd, GWLP_WNDPROC);
			::SetWindowLongPtr(m_AttachHwnd, DWL_MSGRESULT, (LONG)this);
			SetWindowLongPtr(m_AttachHwnd, GWL_WNDPROC, (LONG)&CLibvlcEx::StaticWinProc);
			/* WS_BORDER| WS_POPUP| WS_VISIBLE|SS_NOTIFY*/
			SetWindowLong(m_AttachHwnd, GWL_STYLE, WS_BORDER | WS_POPUP | WS_VISIBLE | SS_NOTIFY);
			SetWindowLong(m_AttachHwnd, GWL_EXSTYLE, m_lStyleEx | ~WS_EX_CLIENTEDGE);
			HWND hWndDesktop = GetDesktopWindow();
			m_hWndParent = ::SetParent(m_AttachHwnd, hWndDesktop);
			HWND hTemp = ::GetParent(m_AttachHwnd);
			::MoveWindow(m_AttachHwnd, 0, 0, cx, cy, TRUE);
			//resume();
			//设置全屏信息提示
			libvlc_video_set_marquee_string(m_vlcPlayer, libvlc_marquee_Text, Coding_Conver::Conver::AnsiToUtf8("按ESC退出全屏模式").c_str());	//文字必须是UTF-8编码的
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_X, 10);
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_Y, 10);
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_Opacity,100);
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_Timeout,10000);
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_Size, 20);
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_Position, 5);
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_Color, 0xFFFFFF);
			libvlc_video_set_marquee_int(m_vlcPlayer, libvlc_marquee_Enable, 1);
		}
		else
		{
			pause();
			//恢复原来的父窗口
			::SetParent(m_AttachHwnd, m_hWndParent);
			POINT leftTop,rightButtom;
			leftTop.x = m_rectPlayer.left;
			leftTop.y = m_rectPlayer.top;
			rightButtom.x = m_rectPlayer.right;
			rightButtom.y = m_rectPlayer.bottom;
			ScreenToClient(m_hWndParent, &leftTop);
			ScreenToClient(m_hWndParent, &rightButtom);
			//恢复hook
			if (m_oldProc)
			{
				::SetWindowLongPtr(m_AttachHwnd, DWL_MSGRESULT, (LONG)0);
				SetWindowLongPtr(m_AttachHwnd, GWL_WNDPROC, (LONG)m_oldProc);
				m_oldProc = nullptr;
			}
			//恢复其窗口属性
			SetWindowLong(m_AttachHwnd, GWL_STYLE, m_lStyle);
			SetWindowLong(m_AttachHwnd, GWL_EXSTYLE, m_lStyleEx);
			::MoveWindow(m_AttachHwnd, leftTop.x, leftTop.y, m_rectPlayer.right - m_rectPlayer.left, m_rectPlayer.bottom - m_rectPlayer.top, TRUE);
			::SetWindowPos(m_AttachHwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			resume();
		}
		libvlc_toggle_fullscreen(m_vlcPlayer);
		bResult = true;
	}
	return bResult;
}

bool CLibvlcEx::InstallEvent(libvlc_event_type_t event_type, libvlc_callback_t eventCallback, void* userData)
{
	
	bool bResult = false;
	if (!eventCallback)
		return bResult;
	libvlc_event_manager_t* EventManagerPtr = nullptr;
	if (!GetEventManagerHandle(&EventManagerPtr) || !EventManagerPtr)
		return bResult;
	if (ENOMEM != libvlc_event_attach(EventManagerPtr, event_type, eventCallback, userData))
	{
		bResult = true;
	}
	return bResult;
}

bool CLibvlcEx::unInstallEvent(libvlc_event_type_t event_type, libvlc_callback_t eventCallback, void* userData)
{
	bool bResult = false;
	if (!eventCallback)
		return bResult;
	libvlc_event_manager_t* EventManagerPtr = nullptr;
	if (!GetEventManagerHandle(&EventManagerPtr) || !EventManagerPtr)
		return bResult;
	libvlc_event_detach(EventManagerPtr, event_type, eventCallback, userData);
	bResult = true;
	return bResult;
}

bool CLibvlcEx::Init()
{
	bool bResut = false;
	try {
		char* exp[] {"--sub-filter=marq"};
		m_instance = libvlc_new(sizeof(exp)/sizeof(char*), exp);
		if (!m_instance)
		{
			return bResut;
		}
		bResut = true;
	}
	catch (...) {

	}
	return bResut;
}
bool CLibvlcEx::GetEventManagerHandle(libvlc_event_manager_t** EventManager)
{
	bool bResult = false;
	if (!EventManager)
		return bResult;
	if (!m_vlcPlayer)
		return bResult;
	auto TempEventPtr = libvlc_media_player_event_manager(m_vlcPlayer);
	if (TempEventPtr)
	{
		*EventManager = TempEventPtr;
		bResult = true;
	}
	return bResult;
}
bool CLibvlcEx::checkUrl(const std::string url)
{
	bool bResult = false;
	if (url.empty())
	{
		return bResult;
	}
	int nPos = url.find("http://");
	int nPos_t = url.find("https://");
	if (nPos!=std::string::npos || nPos_t != std::string::npos)
	{
		bResult = true;
	}
	return bResult;
}


