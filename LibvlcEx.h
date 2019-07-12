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
	@ ������Ƶ
	*/
	bool play(const std::string url);
	/*
	@��ͣ����
	*/
	bool pause();
	/*
	@�ָ�����ͣ�Ĳ���
	*/
	bool resume();
	/*
	@ֹͣ����
	*/
	bool stop();
	/*
	@���õ�Ӱ���ű���
	*/
	bool setRate(float rate);
	/*
	@��ȡ��ǰ��Ӱ���ű���
	*/
	float getRate();
	/*
	@���ò�������
	*/
	bool setAudioVolume(int volume);
	/*
	@��ȡ��ǰ���ŵ�����
	*/
	int getAudioVolume();
	/*
	@��ȡ��ǰ��Ƶ״̬
	*/
	libvlc_state_t getState();
	/*
	@���ز�������� �����ⲿ�Լ���չ
	*/
	libvlc_media_player_t* GetPlayerHandle() const;
	/*
	@��ȡ��ǰ������Ƶ�Ƿ�ȫ��״̬
	*/
	bool GetVideoFullScreenStatus();
	/*
	@���õ�ǰ��ƵΪȫ������ȡ��ȫ��״̬
	*/
	bool SetVideoFullScreenStatus(bool status);
	/*
	@ע���첽�¼��ص� event_type ���Բμ�libvlc_event_e ö������
	*/
	bool InstallEvent(libvlc_event_type_t event_type, libvlc_callback_t eventCallback,void* userData = nullptr);
	/*
	@��ע���첽�¼��ص� event_type ���Բμ�libvlc_event_e ö������
	*/
	bool unInstallEvent(libvlc_event_type_t event_type, libvlc_callback_t eventCallback, void* userData = nullptr);
private:
	/*
	@��ʼ��
	*/
	bool Init();
	/*
	@ �ж��Ƿ���http/https ·��
	*/
	bool checkUrl(const std::string url);
	/*
	@�����¼���������� �����ⲿ�Լ���չ�Լ��첽�¼�
	*/
	bool GetEventManagerHandle(libvlc_event_manager_t** EventManager);
	/*
	@ת�����յĻص�����ȫ���°���Esc���˳�ȫ��
	*/
	static LRESULT CALLBACK StaticWinProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
private:
	HWND m_AttachHwnd; //���ӵĴ��ھ��
	libvlc_instance_t* m_instance;	//libvlc_new ����
	libvlc_media_t*  m_media;	//ý���ļ����
	libvlc_media_player_t* m_vlcPlayer;	//���������
	libvlc_time_t m_media_file_time;	//ý���ļ���ʱ��
	bool m_media_type;	//�ǲ��ű����ļ����������ϵ� 0�Ǳ��� 1 ������
	//�л�ȫ��״̬ʹ��
	HWND m_hWndParent;
	RECT m_rectPlayer;	//�����ڴ�С
	DWORD m_lStyle;	//��������ʽ
	DWORD m_lStyleEx; //��������չ��ʽ
	WNDPROC m_oldProc;
};

