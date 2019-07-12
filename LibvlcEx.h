#pragma once
#include "vlc/vlc.h"
#include <string>
class CLibvlcEx
{
public:
	explicit CLibvlcEx(HWND hwnd);
	~CLibvlcEx();
public:
	/*
	@ 播放视频
	*/
	bool play(const std::string url);
	/*
	@暂停播放
	*/
	bool pause();
	/*
	@恢复被暂停的播放
	*/
	bool resume();
	/*
	@停止播放
	*/
	bool stop();
	/*
	@设置电影播放倍数
	*/
	bool setRate(float rate);
	/*
	@获取当前电影播放倍数
	*/
	float getRate();
	/*
	@设置播放音量
	*/
	bool setAudioVolume(int volume);
	/*
	@获取当前播放的音量
	*/
	int getAudioVolume();
	/*
	@获取当前视频状态
	*/
	libvlc_state_t getState();
	/*
	@返回播放器句柄 用于外部自己扩展
	*/
	libvlc_media_player_t* GetPlayerHandle() const;
	/*
	@获取当前播放视频是否全屏状态
	*/
	bool GetVideoFullScreenStatus();
	/*
	@设置当前视频为全屏或者取消全屏状态
	*/
	bool SetVideoFullScreenStatus(bool status);
	/*
	@注册异步事件回调 event_type 可以参见libvlc_event_e 枚举类型
	*/
	bool InstallEvent(libvlc_event_type_t event_type, libvlc_callback_t eventCallback,void* userData = nullptr);
	/*
	@反注册异步事件回调 event_type 可以参见libvlc_event_e 枚举类型
	*/
	bool unInstallEvent(libvlc_event_type_t event_type, libvlc_callback_t eventCallback, void* userData = nullptr);
private:
	/*
	@初始化
	*/
	bool Init();
	/*
	@ 判断是否是http/https 路径
	*/
	bool checkUrl(const std::string url);
	/*
	@返回事件管理器句柄 用于外部自己扩展自己异步事件
	*/
	bool GetEventManagerHandle(libvlc_event_manager_t** EventManager);
	/*
	@转发接收的回调处理全屏下按下Esc键退出全屏
	*/
	static LRESULT CALLBACK StaticWinProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
private:
	HWND m_AttachHwnd; //附加的窗口句柄
	libvlc_instance_t* m_instance;	//libvlc_new 返回
	libvlc_media_t*  m_media;	//媒体文件句柄
	libvlc_media_player_t* m_vlcPlayer;	//播放器句柄
	libvlc_time_t m_media_file_time;	//媒体文件总时长
	bool m_media_type;	//是播放本地文件还是网络上的 0是本地 1 是网络
	//切换全屏状态使用
	HWND m_hWndParent;
	RECT m_rectPlayer;	//本窗口大小
	DWORD m_lStyle;	//本窗口样式
	DWORD m_lStyleEx; //本窗口扩展样式
	WNDPROC m_oldProc;
};

