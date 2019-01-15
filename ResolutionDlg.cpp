// ResolutionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resolution.h"
#include "ResolutionDlg.h"

//////////////////////////////////////////////////////
//my includes
#include <string>
#include "CNF.h"
#include "Resolve.h"
//my global variables
string inputstr;
string thecons;
vector<string> distrib;
clauses startclauses;
clauses progclauses;
/////////////////////////////////////////////////////
//#include <iostream>
#include <fstream>
using std::ofstream;
using std::ios;
/////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CHelpDlg : public CDialog
{
public:
	CHelpDlg();
	CHelpDlg(UINT myID);

// Dialog Data
	UINT IDD;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CHelpDlg::CHelpDlg() : CDialog(CHelpDlg::IDD=IDD_HELP)
{
}

CHelpDlg::CHelpDlg(UINT myID):CDialog(CHelpDlg::IDD=myID)
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
END_MESSAGE_MAP()

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CResolutionDlg dialog



CResolutionDlg::CResolutionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResolutionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	stepsshown=false;
}

BEGIN_MESSAGE_MAP(CResolutionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SUBMIT, OnBnClickedSubmit)
	ON_BN_CLICKED(IDC_RESET, OnBnClickedReset)
	ON_BN_CLICKED(IDC_SELECT, OnBnClickedSelect)
	ON_BN_CLICKED(IDC_CRESET, OnBnClickedCreset)
	ON_BN_CLICKED(IDC_REMLAST, OnBnClickedRemlast)
	ON_BN_CLICKED(IDC_HELP, OnBnClickedHelp)
	ON_BN_CLICKED(IDC_OUTHLP, OnBnClickedOuthlp)
	ON_BN_CLICKED(IDC_INHELP, OnBnClickedInHelp)
	ON_BN_CLICKED(IDC_CLAUHLP, OnBnClickedClauhlp)
	ON_BN_CLICKED(IDC_CONCHECK, OnBnClickedConcheck)
	ON_BN_CLICKED(IDC_SHOWSTEPS, OnBnClickedShowsteps)
	ON_BN_CLICKED(IDC_PRESET, OnBnClickedPreset)
	ON_BN_CLICKED(IDC_AUTPHLP, OnBnClickedAutphlp)
	ON_BN_CLICKED(IDC_PREMLAST, OnBnClickedPremlast)
	ON_BN_CLICKED(IDC_PFILE, OnBnClickedPfile)
	ON_BN_CLICKED(IDC_UFILE, OnBnClickedUfile)
END_MESSAGE_MAP()


// CResolutionDlg message handlers

BOOL CResolutionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

void CResolutionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CResolutionDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CResolutionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CResolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, editinput);
	DDX_Control(pDX, IDC_ULIST, Ulistbox);
	DDX_Control(pDX, IDC_PLIST, Plistbox);
}

void CResolutionDlg::OnBnClickedSubmit()
{
	if(startclauses.ContradReached())
	{
		MessageBox("A contradiction has been reached.\nNo more clauses may be input.","Contradiction",0);

	}
	else
	{
		inputstr.clear();
		distrib.clear();
		UpdateData(TRUE);
		inputstr=editinput;
		editinput="";
		int err=0;
		int place=0;
		string mbox;
		if(inputstr.empty())
		{
			MessageBox("Input a sentence to add!","No Sentence",0);
			return;
		}
		if(parse(inputstr,thecons,place,err))
		{
			mbox.clear();
			mbox="The Sentence:\n"+inputstr;
			mbox=mbox+"\nhas been added to the list of sentences\n";
			mbox=mbox+"to be resolved.";
			MessageBox(LPCTSTR(mbox.c_str()),"Sentence Added",0);
			inputstr=NNF(inputstr);
			Distribution(inputstr,distrib);
			for(int i=0;i<distrib.size();i++)
			{
				if(!progclauses.taut(distrib[i]) && progclauses.notaclause(distrib[i]))
				{
                    progclauses.push_back(distrib[i]);
					Plistbox.AddString((progclauses.print(progclauses.numclau()-1)).c_str());
				}
				if(!startclauses.taut(distrib[i]) && startclauses.notaclause(distrib[i]))
				{
					startclauses.push_back(distrib[i]);
					Ulistbox.AddString((startclauses.print(startclauses.numclau()-1)).c_str());
				}
				else if(startclauses.taut(distrib[i]))
				{
					string temp;
					temp="The string: "+distrib[i];
					temp=temp+"\nwill not be added because it is a tautology.";
					MessageBox(temp.c_str(),"Tautology",0);
				}
			}
		}
		else
		{
			mbox.clear();
			mbox="Unable to Add Sentence.\n";
			string temp("\n");
			for(int i=0;i<=place;i++)
			{
				temp=temp+" ";
			}
			temp=temp+"^";
			switch (err){
				case 1:
					mbox=mbox+"Too many closing parenthesis in sentence:\n";
					break;
				case 2:
					mbox=mbox+"Invalid character after closing parenthesis:\n";
					break;
				case 3:
					mbox=mbox+"Invalid character after opening parenthesis:\n";
					break;
				case 4:
					mbox=mbox+"Invalid character after constant:\n";
					break;
				case 5:
					mbox=mbox+"Invalid character after negation:\n";
					break;
				case 6:
					mbox=mbox+"Invalid character after &, | or >:\n";
					break;
				case 7:
					mbox=mbox+"Incorrect amount of parenthesis:\n";
					break;
			}
			mbox=mbox+inputstr;
			if(err!=7)
			{
				mbox=mbox+temp;
			}
			MessageBox(LPCTSTR(mbox.c_str()),"Unable to Add Sentence",0);
			inputstr.clear();
		}
		UpdateData(TRUE);
	}
}

void CResolutionDlg::OnBnClickedReset()
{
	SetDlgItemText(IDC_EDIT,"");
}

void CResolutionDlg::OnBnClickedSelect()
{
	if(startclauses.ContradReached())
	{
		MessageBox("A contradiction has been reached.\nNo more clauses can be resolved.","Contradiction",0);
	}
	else
	{
		int indices[2];
		int error=0;
		if(Ulistbox.GetSelItems(2,indices) < 2)
		{
			MessageBox("You need to select 2 clauses to resolve!","More clauses needed",0);
		}
		else if(Ulistbox.GetSelItems(2,indices) > 2)
		{
			MessageBox("Unable to resolve clauses.\nOnly 2 clauses can be resolved at one time.","Error",0);
		}
		else if(startclauses.UserResClau(indices[0],indices[1],error))
		{
			MessageBox("Clauses resolved.","Success",0);
			Ulistbox.AddString(startclauses.print(startclauses.numclau()-1).c_str());
			if(startclauses.ContradReached())
			{
				MessageBox("A contradiction has been reached.\nThe set of input Logical Sentences are inconsistent.","CONTRADICTION",0);
			}
		}
		else if(error==2)
		{
			MessageBox("The selected clauses lead to a tautologous clause.","Tautologous clause",0);
		}
		else if(error ==3 || !startclauses.notaclause(indices[0],indices[1]))
		{
			MessageBox("These clauses have already been resolved.","Error",0);
		}
		else
		{
			MessageBox("Logically unable to resolve clauses.","Error",0);
		}
	}
}

void CResolutionDlg::OnBnClickedCreset()
{
	//7 is no, 6 is yes
	int i=MessageBox("This will remove all clauses from the display\nand from the clause list.\nAre you sure you want to do this?","Remove",4);
	if(i==6)
	{
		startclauses.clear();
		for(int i=Ulistbox.GetCount()-1;i>=0;i--)
		{
			Ulistbox.DeleteString(i);
		}
	}
}

void CResolutionDlg::OnBnClickedRemlast()
{
	if(Ulistbox.GetCount()>0)
	{
		//7 is no, 6 is yes
		int i=MessageBox("This will remove the last clause from the display\nand from the clause list.\nAre you sure you want to do this?","Remove",4);
		if(i==6)
		{
			Ulistbox.DeleteString(Ulistbox.GetCount()-1);
			startclauses.pop_back();
		}
	}
	else
	{
		MessageBox("No clauses to remove.","Error",0);
	}
}

void CResolutionDlg::OnBnClickedHelp()
{
	CHelpDlg myhelp(IDD_HELP);
	myhelp.DoModal();
}
void CResolutionDlg::OnBnClickedOuthlp()
{
	CHelpDlg myhelp(IDD_OUTHLP);
	myhelp.DoModal();
}
void CResolutionDlg::OnBnClickedInHelp()
{
	CHelpDlg myhelp(IDD_INHLP);
	myhelp.DoModal();
}

void CResolutionDlg::OnBnClickedClauhlp()
{
	CHelpDlg myhelp(IDD_CLAUHLP);
	myhelp.DoModal();
}

void CResolutionDlg::OnBnClickedAutphlp()
{
	CHelpDlg myhelp(IDD_AUTOHLP);
	myhelp.DoModal();
}

void CResolutionDlg::OnBnClickedConcheck()
{
	if(progclauses.numclau()>0)
	{
		progclauses.resolve();
		if(progclauses.ContradReached())
		{
			string message;
			if(!stepsshown)
			{
				message="It is possible to reach a contradiction with the input clauses.\n";
				message=message+"Do you wish to see how?\nIf you choose yes all steps will be shown.\n";
				message=message+"Some steps may not be necessary to reach a contradiction.";
				int i=MessageBox(message.c_str(),"Contradiction Reachable",4);
				if(i==6)
				{
					for(int i=Plistbox.GetCount();i<progclauses.numclau();i++)
					{
						Plistbox.AddString(progclauses.print(i).c_str());
					}
				}
			}
			else
			{
				message="It is possible to reach a contradiction with the input clauses.";
				MessageBox(message.c_str(),"Contradiction Reachable",0);
			}
		}
		else MessageBox("It is impossible to reach a contradiction with the input clauses.","Contradiction Unreachable",0);
	}
	else MessageBox("No clauses to resolve!","Error",0);
}

void CResolutionDlg::OnBnClickedShowsteps()
{
	if(progclauses.numclau()>0)
	{
		int i=MessageBox("This will show all steps.\nSome steps may not be necessary to reach a contradiction.\nDo you want to show all steps?","Warning",4);
		if(i==6)
		{
			progclauses.resolve();
			for(int i=Plistbox.GetCount();i<progclauses.numclau();i++)
			{
				Plistbox.AddString(progclauses.print(i).c_str());
			}
			stepsshown=true;
		}
	}
	else MessageBox("No clauses to resolve!","Error",0);
}

void CResolutionDlg::OnBnClickedPreset()
{
	//7 is no, 6 is yes
	int i=MessageBox("This will remove all clauses from the display\nand from the clause list.\nAre you sure you want to do this?","Remove",4);
	if(i==6)
	{
		stepsshown=false;
		progclauses.clear();
		for(int j=Plistbox.GetCount()-1;j>=0;j--)
		{
			Plistbox.DeleteString(j);
		}
	}
}
void CResolutionDlg::OnBnClickedPremlast()
{
	if(Plistbox.GetCount()>0)
	{
		//7 is no, 6 is yes
		int i=MessageBox("This will remove the last clause from the display\nand from the clause list.\nAre you sure you want to do this?","Remove",4);
		if(i==6)
		{
			Plistbox.DeleteString(Plistbox.GetCount()-1);
			progclauses.pop_back();
		}
	}
	else
	{
		MessageBox("No clauses to remove.","Error",0);
	}
}

void CResolutionDlg::OnBnClickedPfile()
{
	if(progclauses.numclau() > 0)
	{
		ofstream output("Automated.txt", ios::app);
		output<<"---------------------------------------------"<<endl;
		for(int i=0;i<progclauses.numclau();i++)
		{
			output<<progclauses.print(i)<<endl;
		}
		MessageBox("The contents of the automated list have been\nappended to the file \"Automated.txt\"\nThere is a line of dashes separating each separate send.","File Sent",0);
	}
	else MessageBox("No clauses to send.","Error",0);
}

void CResolutionDlg::OnBnClickedUfile()
{
	if(startclauses.numclau() > 0)
	{
		ofstream output("Manual.txt", ios::app);
		output<<"---------------------------------------------"<<endl;
		for(int i=0;i<startclauses.numclau();i++)
		{
			output<<startclauses.print(i)<<endl;
		}
		MessageBox("The contents of the automated list have been\nappended to the file \"Manual.txt\"\nThere is a line of dashes separating each separate send.","File Sent",0);
	}
	else MessageBox("No clauses to send.","Error",0);

}