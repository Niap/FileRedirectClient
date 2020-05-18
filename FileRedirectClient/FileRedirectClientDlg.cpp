
// FileRedirectClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileRedirectClient.h"
#include "FileRedirectClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:

	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileRedirectClientDlg 对话框



CFileRedirectClientDlg::CFileRedirectClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileRedirectClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileRedirectClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, SourcePath, m_source_path);
	DDX_Control(pDX, RedirectPath, m_redirect_path);
	DDX_Control(pDX, FilterList, m_filter_list);
}

BEGIN_MESSAGE_MAP(CFileRedirectClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(BtnConnect, &CFileRedirectClientDlg::OnBnClickedBtnconnect)
ON_BN_CLICKED(BtnAddFilter, &CFileRedirectClientDlg::OnBnClickedBtnaddfilter)
ON_BN_CLICKED(BtnClose, &CFileRedirectClientDlg::OnBnClickedBtnclose)
END_MESSAGE_MAP()


// CFileRedirectClientDlg 消息处理程序

BOOL CFileRedirectClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CRect rect;
   
	m_filter_list.GetClientRect(&rect);
	m_filter_list.SetExtendedStyle(m_filter_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_filter_list.InsertColumn(0, _T("源路经"), LVCFMT_CENTER, rect.Width() / 2, 0);
	m_filter_list.InsertColumn(1, _T("重定向路经"), LVCFMT_CENTER, rect.Width() / 2, 1);

	


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileRedirectClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileRedirectClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileRedirectClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileRedirectClientDlg::OnBnClickedBtnconnect()
{
	if (InitialCommunicationPort() != S_OK){
		MessageBox(L"连接失败", L"错误", MB_OK);
		return;
	}
	isConnected = true;
}


void CFileRedirectClientDlg::OnBnClickedBtnaddfilter()
{
	if (!isConnected){
		MessageBox(L"还未连接", L"错误", MB_OK);
		return;
	}

	CString source_path;
	CString redirect_path;
	TCHAR source_path_device[MAX_PATH] = { 0 };
	TCHAR redirect_path_device[MAX_PATH] = { 0 };

	m_source_path.GetWindowText(source_path);
	m_redirect_path.GetWindowText(redirect_path);

	if (source_path.IsEmpty()){
		MessageBox(L"源地址不能为空", L"错误", MB_OK);
		return;
	}

	if (redirect_path.IsEmpty()){
		MessageBox(L"重定向地址不能为空", L"错误", MB_OK);
		return;
	}

	COMMAND_MESSAGE data;
	data.Command = ENUM_ADD_FILTER;
	FILEFILTERITEM filter;

	QueryDosDevice(source_path.Left(2), source_path_device, MAX_PATH);
	QueryDosDevice(redirect_path.Left(2), redirect_path_device, MAX_PATH);

	source_path.Replace(source_path.Left(2), source_path_device);
	redirect_path.Replace(redirect_path.Left(2), redirect_path_device);

	filter.sourcePath = (LPWSTR)(LPCWSTR)source_path;
	filter.redirectPath = (LPWSTR)(LPCWSTR)redirect_path;

	data.filter = filter;
	SendMessageToPort(&data);

	m_filter_list.InsertItem(filterIndex, source_path);
	m_filter_list.SetItemText(filterIndex, 1, redirect_path);

	m_source_path.SetWindowText(_T(""));
	m_redirect_path.SetWindowText(_T(""));
	filterIndex++;

}


void CFileRedirectClientDlg::OnBnClickedBtnclose()
{
	// TODO:  在此添加控件通知处理程序代码

	if (!isConnected){
		MessageBox(L"还未连接", L"错误", MB_OK);
		return;
	}

	COMMAND_MESSAGE data;
	data.Command = ENUM_CLEAR_FILTER;
	SendMessageToPort(&data);
	m_filter_list.DeleteAllItems();
	filterIndex = 0;
}
