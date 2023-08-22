
// clientappDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "clientapp.h"
#include "clientappDlg.h"
#include "afxdialogex.h"
#include <afxwin.h> 
#include <string>
#include <iostream>
#include <ctime>
#include <iomanip>

#define _USE_WIHTTP_INTERFACE

#include "../restclient-cpp/textconv_helper.hpp"
#include "../restclient-cpp/restclient.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CclientappDlg dialog



CclientappDlg::CclientappDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CclientappDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CclientappDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CclientappDlg::OnBnClickedBtnSendLicenseInfo)
END_MESSAGE_MAP()


// CclientappDlg message handlers

BOOL CclientappDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CclientappDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CclientappDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CclientappDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


std::string getCurrentTime() {
	std::time_t currentTime = std::time(nullptr);
	struct tm timeInfo;

	// Use localtime_s to get the local time
	localtime_s(&timeInfo, &currentTime);

	char timeBuffer[20]; // Buffer to hold the formatted time string
	strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo); // Format the time
	return timeBuffer;
}

std::string getLicense() {
	return "4FG45S04JD";
}
void postLicenseInfo() {
	RestClient::Request request;
	request.timeout = 3000;
	request.followRedirects = false;
	request.headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36";
	request.headers["Cookie"] = "name=value;";
	std::string start_date = "2023-06-05";//change start date of license
	std::string end_date = "2023-09-05";// change end date of license
	std::string cur_time = getCurrentTime();
	std::string license = getLicense();

	std::string start_date_key = "start_date";
	std::string end_date_key = "end_date";
	std::string cur_time_key = "cur_time";
	std::string license_key = "license";

	std::string json_data = "{\"start_date\": \"" + start_date + "\"}";

	// Append the new key-value pairs to the JSON string
	json_data = "{\"" + start_date_key + "\": \"" + start_date
		+ "\", \"" + end_date_key + "\": \"" + end_date
		+ "\", \"" + cur_time_key + "\": \"" + cur_time
		+ "\", \"" + license_key + "\": \"" + license
		+ "\"}";


	RestClient::Response response = RestClient::post("http://localhost:3000/license", "application/json", json_data, &request);

	CString myCString(response.body.c_str());

	int result = AfxMessageBox(myCString, MB_OK);
	if (result == IDOK) {
		// user clicked OK button
	}
	else if (result == IDCANCEL) {
		// user clicked Cancel button
	}
}

void CclientappDlg::OnBnClickedBtnSendLicenseInfo()
{
	// TODO: Add your control notification handler code here
	//get_test();
	postLicenseInfo();
}
