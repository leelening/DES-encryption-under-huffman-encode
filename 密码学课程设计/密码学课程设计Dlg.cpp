
// ����ѧ�γ����Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "����ѧ�γ����.h"
#include "����ѧ�γ����Dlg.h"
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

const unsigned short MaxSize=16384;//14�ֽ������
  
  void compress(FILE *,FILE *);//ѹ��
  void decompress(FILE *,FILE *);//��ѹ��
  void wbuffer(FILE *,char *,int);/*��������Ϣд�뻺����*/
  void buf_init(char *,int);//�����ʼ��
  void rbuffer(FILE *,char *,int);//�����嵽�ļ�

  void RC4(FILE *InFile,FILE *outFile,char *cKey,const int iKeyLen);

  unsigned long RLE(FILE *,char *,int,unsigned long);//�γ�ѹ����������д���ֽ���
  void decode(FILE *,char *,int);//���룬������
  DWORD getBMPsize(FILE *ifp);//��ȡλͼ��С
  int IsFeasible(char *,char *,int);//�ж�ѹ�������ԣ�����



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	char* infile_name;

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// C����ѧ�γ����Dlg �Ի���




C����ѧ�γ����Dlg::C����ѧ�γ����Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C����ѧ�γ����Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C����ѧ�γ����Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, shuifilename);
	//  DDX_Control(pDX, IDC_EDIT2, ckey);
}

BEGIN_MESSAGE_MAP(C����ѧ�γ����Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &C����ѧ�γ����Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C����ѧ�γ����Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &C����ѧ�γ����Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &C����ѧ�γ����Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C����ѧ�γ����Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &C����ѧ�γ����Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &C����ѧ�γ����Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &C����ѧ�γ����Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &C����ѧ�γ����Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &C����ѧ�γ����Dlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// C����ѧ�γ����Dlg ��Ϣ�������

BOOL C����ѧ�γ����Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C����ѧ�γ����Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C����ѧ�γ����Dlg::OnPaint()
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
HCURSOR C����ѧ�γ����Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




FILE *ifp,*ofp,*input,*output;
CString strPath,ifilename,ofilename,dname,hofilename,hdname,outputname,jieminame;
CString key;
char *ckey;
unsigned short SeqRandNumber[1024];

void C����ѧ�γ����Dlg::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE);
	if (dlg.DoModal()==IDOK)
	strPath=dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1,strPath);// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void C����ѧ�γ����Dlg::OnBnClickedButton2()
{
	GetDlgItemText(IDC_EDIT2, key);
}


void C����ѧ�γ����Dlg::OnBnClickedButton3()
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
		MessageBox("δѡ���ļ�!","ERROR");
		return;			
	}
           ifp=fopen(strPath,"rb");//�γ�ѹ��
           ofp=fopen(ofilename,"wb");
		   compress(ifp,ofp);
		   fclose(ifp);
           fclose(ofp);
        }


void C����ѧ�γ����Dlg::OnBnClickedButton4()
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
		MessageBox("δѡ���ļ�!","ERROR");
		return;			
	}

	ifp=fopen(ofilename,"rb");//�γ̽���
    ofp=fopen(dname,"wb");
	decompress(ifp,ofp);
    fclose(ifp);
    fclose(ofp);// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void C����ѧ�γ����Dlg::OnBnClickedButton5()
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
		MessageBox("δѡ���ļ�!","ERROR");
		return;			
	}
		  

	char *ch1,*ch2;
	ch1=(char *)malloc(sizeof(char)*500);
	ch2=(char *)malloc(sizeof(char)*500);

	ch1=(LPSTR)(LPCTSTR)dname;
	ch2=(LPSTR)(LPCTSTR)hofilename;
	compress(ch1,ch2);
}


void C����ѧ�γ����Dlg::OnBnClickedButton6()
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
		MessageBox("δѡ���ļ�!","ERROR");
		return;			
	}
		  

	char *ch1,*ch2;
	ch1=(char *)malloc(sizeof(char)*500);
	ch2=(char *)malloc(sizeof(char)*500);

	ch1=(LPSTR)(LPCTSTR)hofilename;
	ch2=(LPSTR)(LPCTSTR)hdname;
	uncompress(ch1,ch2);// TODO: �ڴ���ӿؼ�֪ͨ����������
}



void C����ѧ�γ����Dlg::OnBnClickedButton7()
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
		MessageBox("δѡ���ļ�!","ERROR");
		return;			
	}
		   
           input=fopen(strPath,"rb");//���ļ�
		   output=fopen(outputname,"wb");
		   ckey=(LPSTR)(LPCTSTR)key;
		   RC4(input,output,ckey,64);
  fclose(input);
  fclose(output);
}


void C����ѧ�γ����Dlg::OnBnClickedButton8()
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
		MessageBox("δѡ���ļ�!","ERROR");
		return;			
	}
		   
           input=fopen(outputname,"rb");//���ļ�
		   output=fopen(jieminame,"wb");
		   ckey=(LPSTR)(LPCTSTR)key;
		   RC4(input,output,ckey,64);
  fclose(input);
  fclose(output);// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void RC4(FILE *InFile,FILE *outFile,char *cKey,const int iKeyLen)
{
	//��ʼ�� Sbox
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

	//RC4 ����
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
/**************************��������ʼ��****************************/
void buf_init(char *buffer,int size)
{
    memset(buffer,-1,size);
}
/******************************************************************************************/
/************************************������ѹ���㷨****************************************/
/******************************************************************************************/
void compress(FILE *ifp,FILE *ofp)
{
          
        unsigned long input,output=0;
        input=getBMPsize(ifp);
        char buffer[MaxSize];/*������*/
        while(!feof(ifp))
        {
            wbuffer(ifp,buffer,MaxSize);
            output=RLE(ofp,buffer,MaxSize,output);
        }
        printf("Compressing Rate= %f %% ",output*100.0/input);
        //printf("%d",sizeof(OutUnit));
}

/************************************�������Ƶ�λд�뻺����***************************************/
void wbuffer(FILE *ifp,char *buffer,int size)
{
          char *tpbuf=buffer;
          unsigned char temp=0;
          int i;
          /* fseek(ifp,62,0); */
          buf_init(buffer,size);
          
          while(size>0&&!feof(ifp))
          {
               temp=fgetc(ifp);/*ÿ��ȡ8������*/
               if(feof(ifp))break;//BUG֮����,����Ҫ�ٶ�һ���ֽ�,���������ж��ļ�β
               
               for(i=7;i>=0;--i,++tpbuf)
               {
                   *tpbuf=(temp>>i)&1;/*д��һ��ת��Ϊ������,��λ�������*/
                    /* printf("%d",buffer[MaxSize-size][j]); */
               }
               size-=8;
               
          }
          //bufdisplay("codisplay.txt",buffer,MaxSize);
          //getch();
}
/**********************************�Ի���������ֵ���б��룬������������**************************************/
unsigned long RLE(FILE *ofp,char *buffer,int size,unsigned long output)
{
              unsigned short ounit=0,count=0;
              int i=0;             
              char temp;
              char *tpbuf=buffer;
              
              temp=0;
              
              while(*tpbuf!=-1&&(i<size-1))//ȷ������������������������Ч
              {
                      temp=*tpbuf;
                      ounit=count=0;
                      //currentbit=temp;
                      if(!IsFeasible(tpbuf,buffer,MaxSize))
                      {
                          int k=0;
                          for(k=13;k>=0&&*tpbuf!=-1&&tpbuf!=(buffer+size-1);--k,++tpbuf,++i)//��������Խ���ˣ�������
                          {
                              count+=(*tpbuf)<<k;
                          }
                          ounit=count;//��ѹ��
                          //ounit->id=0;//����ֵ
                          fwrite(&ounit,sizeof(unsigned short),1,ofp);
                          
                      }
                      else
                      {
                          count=0;
                          ounit+=1<<15;//ѹ��
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
/************************************�����ǽ�ѹ���㷨****************************************/
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
/********************************���ݱ��������н���************************************/
void decode(FILE *ifp,char *buffer,int size)
{
        int i=0;
        unsigned short temp=0;
        char *tpbuf=buffer;
                    
        buf_init(buffer,MaxSize);
        while(i<size-1&&!feof(ifp))
        {
                fread(&temp,sizeof(unsigned short),1,ifp);
                if(feof(ifp))   break;//BUG֮����,�����,û�����ͻ���32λ
                if(temp>(1<<15))//��һλΪ��ѹ�������򰴷�ѹ������
                {
                   if(((temp>>14)&1)==1)//�ڣ�λΪ����������Ϊ��,����Ϊ
                       //һ��Ҫ����������λ�ͣ��룬���ɽ�����λ�Ͳ�������
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
                else//��ѹ��
                {
                       int k;
                       for(k=13;k>=0&&(tpbuf-buffer)<size-1;--k,++tpbuf,++i)//�������14λ�м�λȡ��λ
                       {
                           //if(*)
                           *tpbuf=(temp>>k)&1;
                       }
                }
                //int a=temp;
       } 
        
       //bufdisplay("dedisplay.txt",buffer,MaxSize); 
}
/*******************************��ȡ�������ж����ƣ�������ļ�**********************************/
void rbuffer(FILE *ofp,char *buffer,int size)
{
            unsigned char temp=0;
            int i=0;
            char *tpbuf=buffer;
            
            while(size>0&&*tpbuf!=-1)
            {
                   for(i=7;i>=0;--i,++tpbuf)//��λ��Ϣת��Ϊ���ֽ���������������ļ�
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


void C����ѧ�γ����Dlg::OnBnClickedButton9()
{
	ofilename="1.bmp",hofilename="2.bmp",outputname="3.bmp";

	CFileDialog dlg(TRUE);
	if (dlg.DoModal()==IDOK)
	strPath=dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1,strPath);


	ifp=fopen(strPath,"rb");//�γ�ѹ��
    ofp=fopen(ofilename,"wb");
	compress(ifp,ofp);
	fclose(ifp);
    fclose(ofp);
      


	char *ch1,*ch2;								//huffman����
	ch1=(char *)malloc(sizeof(char)*500);
	ch2=(char *)malloc(sizeof(char)*500);
	ch1=(LPSTR)(LPCTSTR)ofilename;
	ch2=(LPSTR)(LPCTSTR)hofilename;
	compress(ch1,ch2);




	input=fopen(hofilename,"rb");//���ļ�
	output=fopen(outputname,"wb");
	ckey=(LPSTR)(LPCTSTR)key;
	RC4(input,output,ckey,64);
  fclose(input);
  fclose(output);
}


void C����ѧ�γ����Dlg::OnBnClickedButton10()
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
		MessageBox("δѡ���ļ�!","ERROR");
		return;			
	}
		   
           input=fopen(outputname,"rb");//���ļ�
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


	ifp=fopen(hdname,"rb");//�γ̽���
    ofp=fopen(dname,"wb");
	decompress(ifp,ofp);
    fclose(ifp);
    fclose(ofp);

}
