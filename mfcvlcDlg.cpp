
// mfcvlcDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "mfcvlc.h"
#include "mfcvlcDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <string>
#include "resource.h"
#include "Conver.hpp"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmfcvlcDlg �Ի���



CmfcvlcDlg::CmfcvlcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCVLC_DIALOG, pParent)
	, libvlctemp(nullptr)
	, m_rate(0)
	, m_valome(0)
	, m_cx(0)
	,m_cy(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CmfcvlcDlg::~CmfcvlcDlg()
{
	if (libvlctemp)
	{
		delete libvlctemp;
	}
}

void CmfcvlcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, m_rate);
	DDX_Text(pDX, IDC_EDIT4, m_valome);
	DDV_MinMaxInt(pDX, m_valome, 0, 100);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}

BEGIN_MESSAGE_MAP(CmfcvlcDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CmfcvlcDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CmfcvlcDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CmfcvlcDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CmfcvlcDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CmfcvlcDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CmfcvlcDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CmfcvlcDlg::OnBnClickedButton7)
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON8, &CmfcvlcDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CmfcvlcDlg ��Ϣ�������

BOOL CmfcvlcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	this->SetDlgItemTextW(IDC_STATIC_3, _T("00:00:00"));
	UpdateData(FALSE);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	RECT rc;
	::GetClientRect(GetSafeHwnd(), &rc);
	m_cx = rc.right - rc.left;
	m_cy = rc.bottom - rc.top;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmfcvlcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmfcvlcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CmfcvlcDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 100:
	{
		if (!libvlctemp)
		{
			break;
		}
		if (libvlctemp->getState() == libvlc_Playing)
		{
			CString str1, str2;
			int tns, thh, tmm, tss;
			libvlc_time_t nCurren, nTotal;
			auto vlcPlay = libvlctemp->GetPlayerHandle();
			if (vlcPlay) {
				nCurren = libvlc_media_player_get_time(vlcPlay);
				if (nCurren!=0)
				{
					tns = nCurren / 1000;
					thh = tns / 3600;
					tmm = (tns % 3600) / 60;
					tss = (tns % 60);
					str1.Format(_T("%02d:%02d:%02d"), thh, tmm, tss);
				}
				nTotal = libvlc_media_player_get_length(vlcPlay);
				if (nTotal!=0)
				{
					tns = nTotal / 1000;
					thh = tns / 3600;
					tmm = (tns % 3600) / 60;
					tss = (tns % 60);
					str2.Format(_T("%s//%02d:%02d:%02d"),str1.GetBuffer(), thh, tmm, tss);
					this->SetDlgItemTextW(IDC_STATIC_3, str2);
				}
				float pos = libvlc_media_player_get_position(vlcPlay);
				int npos = (int)(pos * 100);
				m_slider.SetPos(npos);
			}
				
		}
		if (libvlctemp->getState() == libvlc_Ended)
		{
			this->SetDlgItemTextW(IDC_STATIC_3, _T("00:00:00"));
			OnBnClickedButton2();
		}
	}
	break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CmfcvlcDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HWND sub_hwnd = GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd();
	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	std::string buf = Coding_Conver::Conver::UnicodeToAnsi(str.GetBuffer());
	if (!libvlctemp)
	{
		
		libvlctemp = new CLibvlcEx(sub_hwnd);
	}
	libvlctemp->play(buf.c_str());
	SetTimer(100,1000, NULL);
	auto playerPtr = libvlctemp->GetPlayerHandle();
	if (libvlctemp->getState() == libvlc_Playing)
	{
		//����ʾ��ע��ص����� const struct libvlc_event_t *p_event, void *p_data
		auto bResult = libvlctemp->InstallEvent(libvlc_MediaPlayerPaused, [](const struct libvlc_event_t *p_event, void *p_data) {
			//�������Ǽ򵥵Ĵ�ӡ���һ���ı�
			if (p_event)
			{
				TRACE(_T("event Type = %d \n"), p_event->type);
			}
			TRACE(_T("video Paused\n"));
		});
		if (bResult)
		{
			TRACE(_T("ע��ɹ�\n"));
		}
#if 0
		int width = libvlc_video_get_width(playerPtr);
		int height = libvlc_video_get_height(playerPtr);
		RECT rc;
		::GetClientRect(sub_hwnd, &rc);
		width -= rc.left;
		height -= rc.top;
		::MoveWindow(sub_hwnd, rc.left, rc.top, width, height, TRUE);
#endif
	}
	
}


void CmfcvlcDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (libvlctemp) {
		libvlctemp->stop();
		KillTimer(100);
	}
}


void CmfcvlcDlg::OnBnClickedButton3()
{
	if (libvlctemp) {
		libvlctemp->pause();
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CmfcvlcDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (libvlctemp)
	{
		CString str;
		str.Format(L"���ű���%f", libvlctemp->getRate());
		this->SetDlgItemTextW(IDC_EDIT2, str.GetBuffer());
	}
}


void CmfcvlcDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (libvlctemp)
	{
		libvlctemp->setRate(m_rate);
	}
}


void CmfcvlcDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (libvlctemp)
	{
		CString str;
		str.Format(L"��ǰ����%d", libvlctemp->getAudioVolume());
		this->SetDlgItemTextW(IDC_EDIT2, str.GetBuffer());
	}
}


void CmfcvlcDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (libvlctemp)
	{
		libvlctemp->setAudioVolume(m_valome);
	}
}


void CmfcvlcDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pScrollBar->GetSafeHwnd() == m_slider.GetSafeHwnd())
	{
		float posf = 0.0;
		if (nSBCode == SB_THUMBPOSITION)
		{
			posf = (float)nPos / 100.0;
			libvlc_media_player_set_position(libvlctemp->GetPlayerHandle(), posf);
		}

	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CmfcvlcDlg::OnSize(UINT nType, int cx, int cy)
{
	/*if (nType!=SIZE_MINIMIZED)
	{
		int xdistine, ydistine;
		xdistine = cx - m_cx;
		ydistine = cy - m_cy;
		HWND sub_Hwnd = this->GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd();
		RECT rc;
		::GetClientRect(sub_Hwnd, &rc);
		int video_x, video_y;
		video_x = rc.right - rc.left;
		video_y = rc.bottom - rc.top;
		//::MoveWindow(sub_Hwnd, rc.left, rc.top, video_x + xdistine, video_y + ydistine, FALSE);
		::SetWindowPos(sub_Hwnd, HWND_TOP, 0, 0, video_x + xdistine, video_y + ydistine, SWP_NOMOVE);
	}
	m_cx = cx;
	m_cy = cy;*/
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


void CmfcvlcDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (libvlctemp)
	{
		libvlctemp->SetVideoFullScreenStatus(!libvlctemp->GetVideoFullScreenStatus());
	}
}

