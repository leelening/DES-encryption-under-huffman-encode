
// 密码学课程设计Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "密码学课程设计.h"
#include "密码学课程设计Dlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <cstring>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include"compress.h"
#include"bmpheader.h"
using namespace std;




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const unsigned short MaxSize=16384;//14字节最大数
  
  void compress(FILE *,FILE *);//压缩
  void decompress(FILE *,FILE *);//解压缩
  void wbuffer(FILE *,char *,int);/*将像素信息写入缓冲区*/
  void buf_init(char *,int);//缓冲初始化
  void rbuffer(FILE *,char *,int);//读缓冲到文件

  void RC4(FILE *InFile,FILE *outFile,char *cKey,const int iKeyLen);

  unsigned long RLE(FILE *,char *,int,unsigned long);//游程压缩，并返回写入字节数
  void decode(FILE *,char *,int);//解码，逆运算
  DWORD getBMPsize(FILE *ifp);//获取位图大小
  int IsFeasible(char *,char *,int);//判断压缩可行性，可行



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	char* infile_name;

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


// C密码学课程设计Dlg 对话框




C密码学课程设计Dlg::C密码学课程设计Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C密码学课程设计Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C密码学课程设计Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, shuifilename);
	//  DDX_Control(pDX, IDC_EDIT2, ckey);
}

BEGIN_MESSAGE_MAP(C密码学课程设计Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &C密码学课程设计Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C密码学课程设计Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &C密码学课程设计Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &C密码学课程设计Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C密码学课程设计Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &C密码学课程设计Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &C密码学课程设计Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &C密码学课程设计Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &C密码学课程设计Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &C密码学课程设计Dlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// C密码学课程设计Dlg 消息处理程序

BOOL C密码学课程设计Dlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C密码学课程设计Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C密码学课程设计Dlg::OnPaint()
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
HCURSOR C密码学课程设计Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




FILE *ifp,*ofp,*input,*output;
CString strPath,ifilename,ofilename,dname,hofilename,hdname,outputname,jieminame;
CString key;
char *ckey;
unsigned short SeqRandNumber[1024];

void C密码学课程设计Dlg::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal()==IDOK)
	strPath=dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1,strPath);// TODO: 在此添加控件通知处理程序代码
}


void C密码学课程设计Dlg::OnBnClickedButton2()
{
	GetDlgItemText(IDC_EDIT2, key);
}


void C密码学课程设计Dlg::OnBnClickedButton3()
{
	int a;
	CFileDialog OpenDlg(true);
	OpenDlg.m_ofn.lpstrFilter="*.*\0";
	if(OpenDlg.DoModal()==IDOK)
		ofilename=OpenDlg.GetPathName();
	if(!ofilename.IsEmpty())
		a= 1;
	else
		a= 0;
	if(!a)
	{	
		MessageBox("未选定文件!","ERROR");
		return;			
	}
           ifp=fopen(strPath,"rb");//游程压缩
           ofp=fopen(ofilename,"wb");
		   compress(ifp,ofp);
		   fclose(ifp);
           fclose(ofp);
        }


void C密码学课程设计Dlg::OnBnClickedButton4()
{
	int a;
	CFileDialog OpenDlg(true);
	OpenDlg.m_ofn.lpstrFilter="*.*\0";
	if(OpenDlg.DoModal()==IDOK)
		dname=OpenDlg.GetPathName();
	if(!dname.IsEmpty())
		a= 1;
	else
		a= 0;
	if(!a)
	{	
		MessageBox("未选定文件!","ERROR");
		return;			
	}

	ifp=fopen(ofilename,"rb");//游程解码
    ofp=fopen(dname,"wb");
	decompress(ifp,ofp);
    fclose(ifp);
    fclose(ofp);// TODO: 在此添加控件通知处理程序代码
}


void C密码学课程设计Dlg::OnBnClickedButton5()
{
	int a;
	CFileDialog OpenDlg(true);
	OpenDlg.m_ofn.lpstrFilter="*.*\0";
	if(OpenDlg.DoModal()==IDOK)
		hofilename=OpenDlg.GetPathName();
	if(!hofilename.IsEmpty())
		a= 1;
	else
		a= 0;
	if(!a)
	{	
		MessageBox("未选定文件!","ERROR");
		return;			
	}
		  

	char *ch1,*ch2;
	ch1=(char *)malloc(sizeof(char)*500);
	ch2=(char *)malloc(sizeof(char)*500);

	ch1=(LPSTR)(LPCTSTR)dname;
	ch2=(LPSTR)(LPCTSTR)hofilename;
	compress(ch1,ch2);
}


void C密码学课程设计Dlg::OnBnClickedButton6()
{
	int a;
	CFileDialog OpenDlg(true);
	OpenDlg.m_ofn.lpstrFilter="*.*\0";
	if(OpenDlg.DoModal()==IDOK)
		hdname=OpenDlg.GetPathName();
	if(!hdname.IsEmpty())
		a= 1;
	else
		a= 0;
	if(!a)
	{	
		MessageBox("未选定文件!","ERROR");
		return;			
	}
		  

	char *ch1,*ch2;
	ch1=(char *)malloc(sizeof(char)*500);
	ch2=(char *)malloc(sizeof(char)*500);

	ch1=(LPSTR)(LPCTSTR)hofilename;
	ch2=(LPSTR)(LPCTSTR)hdname;
	uncompress(ch1,ch2);// TODO: 在此添加控件通知处理程序代码
}



void C密码学课程设计Dlg::OnBnClickedButton7()
{

	int a;
	CFileDialog OpenDlg(true);
	OpenDlg.m_ofn.lpstrFilter="*.*\0";
	if(OpenDlg.DoModal()==IDOK)
		outputname=OpenDlg.GetPathName();
	if(!outputname.IsEmpty())
		a= 1;
	else
		a= 0;
	if(!a)
	{	
		MessageBox("未选定文件!","ERROR");
		return;			
	}
		   
           input=fopen(strPath,"rb");//打开文件
		   output=fopen(outputname,"wb");
		   ckey=(LPSTR)(LPCTSTR)key;
		   RC4(input,output,ckey,64);
  fclose(input);
  fclose(output);
}


void C密码学课程设计Dlg::OnBnClickedButton8()
{



	int a;
	CFileDialog OpenDlg(true);
	OpenDlg.m_ofn.lpstrFilter="*.*\0";
	if(OpenDlg.DoModal()==IDOK)
		jieminame=OpenDlg.GetPathName();
	if(!jieminame.IsEmpty())
		a= 1;
	else
		a= 0;
	if(!a)
	{	
		MessageBox("未选定文件!","ERROR");
		return;			
	}
		   
           input=fopen(outputname,"rb");//打开文件
		   output=fopen(jieminame,"wb");
		   ckey=(LPSTR)(LPCTSTR)key;
		   RC4(input,output,ckey,64);
  fclose(input);
  fclose(output);// TODO: 在此添加控件通知处理程序代码
}


void RC4(FILE *InFile,FILE *outFile,char *cKey,const int iKeyLen)
{
	//初始化 Sbox
	unsigned char SBox[256],Key[256];
	int i,k, j=0,t;
	unsigned char temp,r;

	for (i=0;i<256;i++)
	{
		SBox[i]=i;

	}
	for (k=i=0;i<256;i++)
	{
		Key[i]=cKey[k];
		k=(k+1)%iKeyLen;
	}

	for (i=0;i<256;i++)
	{
		j=(j+SBox[i]+Key[i])%256;
		temp=SBox[i];
		SBox[i]=SBox[j];
		SBox[j]=temp;
	}

	//RC4 加密
	i=0;
	j=0;
	while (1)
	{
		if (!(fread(&r,1,1,InFile)))
		{
			break;
		}
		i=(i+1)%256;
		j=(j+SBox[i])%256;
		temp=SBox[i];
		SBox[i]=SBox[j];
		SBox[j]=temp;
		t=(SBox[i]+SBox[j])%256;
		r=r^SBox[t];
		fwrite(&r,1,1,outFile);

	}
}
DWORD getBMPsize(FILE *ifp)
{
        DWORD size;
        fseek(ifp,2,0);
        fread(&size,4,1,ifp);
        rewind(ifp);
        return size;
}
/**************************缓冲区初始化****************************/
void buf_init(char *buffer,int size)
{
    memset(buffer,-1,size);
}
/******************************************************************************************/
/************************************以下是压缩算法****************************************/
/******************************************************************************************/
void compress(FILE *ifp,FILE *ofp)
{
          
        unsigned long input,output=0;
        input=getBMPsize(ifp);
        char buffer[MaxSize];/*缓冲区*/
        while(!feof(ifp))
        {
            wbuffer(ifp,buffer,MaxSize);
            output=RLE(ofp,buffer,MaxSize,output);
        }
        printf("Compressing Rate= %f %% ",output*100.0/input);
        //printf("%d",sizeof(OutUnit));
}

/************************************将二进制的位写入缓冲区***************************************/
void wbuffer(FILE *ifp,char *buffer,int size)
{
          char *tpbuf=buffer;
          unsigned char temp=0;
          int i;
          /* fseek(ifp,62,0); */
          buf_init(buffer,size);
          
          while(size>0&&!feof(ifp))
          {
               temp=fgetc(ifp);/*每次取8个像素*/
               if(feof(ifp))break;//BUG之所在,必须要再读一个字节,函数才能判断文件尾
               
               for(i=7;i>=0;--i,++tpbuf)
               {
                   *tpbuf=(temp>>i)&1;/*写入一行转换为二进制,高位在行左边*/
                    /* printf("%d",buffer[MaxSize-size][j]); */
               }
               size-=8;
               
          }
          //bufdisplay("codisplay.txt",buffer,MaxSize);
          //getch();
}
/**********************************对缓冲区中数值进行编码，并返回码序列**************************************/
unsigned long RLE(FILE *ofp,char *buffer,int size,unsigned long output)
{
              unsigned short ounit=0,count=0;
              int i=0;             
              char temp;
              char *tpbuf=buffer;
              
              temp=0;
              
              while(*tpbuf!=-1&&(i<size-1))//确保不超出缓冲区并且数据有效
              {
                      temp=*tpbuf;
                      ounit=count=0;
                      //currentbit=temp;
                      if(!IsFeasible(tpbuf,buffer,MaxSize))
                      {
                          int k=0;
                          for(k=13;k>=0&&*tpbuf!=-1&&tpbuf!=(buffer+size-1);--k,++tpbuf,++i)//！！！！越界了！！！！
                          {
                              count+=(*tpbuf)<<k;
                          }
                          ounit=count;//不压缩
                          //ounit->id=0;//任意值
                          fwrite(&ounit,sizeof(unsigned short),1,ofp);
                          
                      }
                      else
                      {
                          count=0;
                          ounit+=1<<15;//压缩
                          ounit+=temp<<14;
                          while(*tpbuf==temp&&i<size-1)
                          {
                              ++count;
                              ++tpbuf;
                              ++i;
                          }
                          ounit+=count;
                          fwrite(&ounit,sizeof(unsigned short),1,ofp);
                      }
                      output+=sizeof(unsigned short);
              }
              
              return output;
}
int IsFeasible(char *current,char *buffer,int size)
{
            char i,id;
            int distance=current-buffer;
            for(i=0,id=*current;i<16;++i,++distance)
            {
                if(*(current+i)!=id||*(current+i)==-1||distance>(size-1))return 0;
            }
            return 1;
}
/******************************************************************************************/
/************************************以下是解压缩算法****************************************/
/******************************************************************************************/
void decompress(FILE *ifp,FILE *ofp)
{
        char buffer[MaxSize];
        buf_init(buffer,MaxSize);
        while(!feof(ifp))
        {
            //fgetc(ifp);
            decode(ifp,buffer,MaxSize);
            rbuffer(ofp,buffer,MaxSize);
            //printf("Waiting..... ");getch();
        }
        
}
/********************************根据编码规则进行解码************************************/
void decode(FILE *ifp,char *buffer,int size)
{
        int i=0;
        unsigned short temp=0;
        char *tpbuf=buffer;
                    
        buf_init(buffer,MaxSize);
        while(i<size-1&&!feof(ifp))
        {
                fread(&temp,sizeof(unsigned short),1,ifp);
                if(feof(ifp))   break;//BUG之所在,解决了,没有这句就会多读32位
                if(temp>(1<<15))//第一位为１压缩，否则按非压缩处理
                {
                   if(((temp>>14)&1)==1)//第２位为１，则像素为１,否则为
                       //一定要将操作数移位和１与，不可将１移位和操作数与
                   {
                        unsigned short num1=temp&((1<<14)-1);
                        for(;num1>0&&i<size;--num1,++i,++tpbuf)
                            *tpbuf=1;
                   }
                   else
                   {
                        unsigned short num0=temp&((1<<14)-1);
                        for(;num0>0&&i<size;--num0,++i,++tpbuf)
                            *tpbuf=0;
                   }
                }
                else//非压缩
                {
                       int k;
                       for(k=13;k>=0&&(tpbuf-buffer)<size-1;--k,++tpbuf,++i)//如果不够14位有几位取几位
                       {
                           //if(*)
                           *tpbuf=(temp>>k)&1;
                       }
                }
                //int a=temp;
       } 
        
       //bufdisplay("dedisplay.txt",buffer,MaxSize); 
}
/*******************************读取缓冲区中二进制，输出到文件**********************************/
void rbuffer(FILE *ofp,char *buffer,int size)
{
            unsigned char temp=0;
            int i=0;
            char *tpbuf=buffer;
            
            while(size>0&&*tpbuf!=-1)
            {
                   for(i=7;i>=0;--i,++tpbuf)//将位信息转换为１字节整数，并输出到文件
                   {
                        /* printf("%d ",buffer[MaxSize-size][i]<<i); */
                        if(*tpbuf==-1)break;
                        /* getch(); */
                        temp+=*tpbuf<<i;
                        --size;
                   }
                   fputc(temp,ofp);
                   temp=0;
            }
}


void C密码学课程设计Dlg::OnBnClickedButton9()
{
	ofilename="1.bmp",hofilename="2.bmp",outputname="3.bmp";

	CFileDialog dlg(TRUE);
	if (dlg.DoModal()==IDOK)
	strPath=dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1,strPath);


	ifp=fopen(strPath,"rb");//游程压缩
    ofp=fopen(ofilename,"wb");
	compress(ifp,ofp);
	fclose(ifp);
    fclose(ofp);
      


	char *ch1,*ch2;								//huffman编码
	ch1=(char *)malloc(sizeof(char)*500);
	ch2=(char *)malloc(sizeof(char)*500);
	ch1=(LPSTR)(LPCTSTR)ofilename;
	ch2=(LPSTR)(LPCTSTR)hofilename;
	compress(ch1,ch2);




	input=fopen(hofilename,"rb");//打开文件
	output=fopen(outputname,"wb");
	ckey=(LPSTR)(LPCTSTR)key;
	RC4(input,output,ckey,64);
  fclose(input);
  fclose(output);
}


void C密码学课程设计Dlg::OnBnClickedButton10()
{
	
	dname="6.bmp",hdname="5.bmp",jieminame="4.bmp";

	int a;
	CFileDialog OpenDlg(true);
	OpenDlg.m_ofn.lpstrFilter="*.*\0";
	if(OpenDlg.DoModal()==IDOK)
		outputname=OpenDlg.GetPathName();
	if(!outputname.IsEmpty())
		a= 1;
	else
		a= 0;
	if(!a)
	{	
		MessageBox("未选定文件!","ERROR");
		return;			
	}
		   
           input=fopen(outputname,"rb");//打开文件
		   output=fopen(jieminame,"wb");
		   ckey=(LPSTR)(LPCTSTR)key;
		   RC4(input,output,ckey,64);
  fclose(input);
  fclose(output);




  char *ch1,*ch2;
	ch1=(char *)malloc(sizeof(char)*500);
	ch2=(char *)malloc(sizeof(char)*500);

	ch1=(LPSTR)(LPCTSTR)jieminame;
	ch2=(LPSTR)(LPCTSTR)hdname;
	uncompress(ch1,ch2);


	ifp=fopen(hdname,"rb");//游程解码
    ofp=fopen(dname,"wb");
	decompress(ifp,ofp);
    fclose(ifp);
    fclose(ofp);

}
