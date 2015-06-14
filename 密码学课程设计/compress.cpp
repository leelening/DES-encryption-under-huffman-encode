#include <stdio.h>
#include "StdAfx.h"
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "windows.h"
struct head
{
 unsigned char b;             /*the charactor*/
 long count;                  /*the frequency*/
 long parent,lch,rch;         /*make a tree*/
 char bits[256];              /*the haffuman code*/
}
header[512],tmp;

void compress(char *filename,char *outputfile)//压缩函数
{
	char buf[512];//filename[255]:用于储存文件地址
	unsigned char c;
	long i,j,m,n,f;
	long min1,pt1,flength;
	FILE *ifp,*ofp;//文件指针
//	printf("source filename:");
// 	gets(filename);
	ifp=fopen(filename,"rb");//只读打开一个二进制文件，只允许读数据 
	if(ifp==NULL)
	{
	 //printf("source file open error!\n");
		::MessageBox(NULL,"source file open error!","提示",MB_OK);
	 return;
	}
//	printf("destination filename:");
//	gets(outputfile);
	ofp=fopen(outputfile,"wb");//只写打开或建立一个二进制文件，只允许写数据
	if(ofp==NULL)
	{
		//printf("destination file open error!\n");
		::MessageBox(NULL,"destination file open error!","提示",MB_OK);
		return;
	}
	flength=0;//文件长度
	while(!feof(ifp))//feof(fp)有两个返回值:如果遇到文件结束，函数feof（fp）的值为1，否则为0。
	{
		 fread(&c,1,1,ifp);//从一个流中读数据     size_tfread(void*buffer,size_tsize,size_tcount,FILE*stream);　
						   /*
		 参 数： 　　1.用于接收数据的地址（指针）（buffer） 　　
		2.单个元素的大小（size） ：单位是字节而不是位，例如读取一个int型数据就是4个字节 　　
		3.元素个数（count） 　　4.提供数据的文件指针（stream） 　　
		返回值：读取的元素的个数*/
   		 header[c].count++;//ASCII码
		 flength++;
	}
	flength--;
	header[c].count--;
	for(i=0;i<512;i++)
	{
		if(header[i].count!=0)//如果输入的字符不是0个，即有输入的字符
			header[i].b=(unsigned char)i;//b是改i结点表示的字符
		else 
			header[i].b=0;//0表示空字符，没有输入
		header[i].parent=-1;
		header[i].lch=header[i].rch=-1;
	}
	for(i=0;i<256;i++)//执行完后，i从0到255，count由大到小，因此count=0的结点都在后面
	{
		for(j=i+1;j<256;j++)
		{
			if(header[i].count<header[j].count)
			{
			tmp=header[i];
			header[i]=header[j];
			header[j]=tmp;
			}	
		}
	}
	for(i=0;i<256;i++) 
		if(header[i].count==0) 
			break;
	n=i;//i是输入的有效字符的长度
	m=2*n-1;
	for(i=n;i<m;i++)
	{
		min1=999999999;
		for(j=0;j<i;j++)
		{
			if(header[j].parent!=-1) 
				continue;
			if(min1>header[j].count)
			{
			pt1=j;
			min1=header[j].count;
			continue;
			}
		}
		header[i].count=header[pt1].count;
		header[pt1].parent=i;
		header[i].lch=pt1;
		min1=999999999;
		for(j=0;j<i;j++)
		{
			if(header[j].parent!=-1) continue;
			if(min1>header[j].count)
			{
				pt1=j;
				min1=header[j].count;
				continue;
			}
		}
		header[i].count+=header[pt1].count;
		header[i].rch=pt1;
		header[pt1].parent=i;
	}
	for(i=0;i<n;i++)
	{
		f=i;
		header[i].bits[0]=0;
		while(header[f].parent!=-1)
		{
			j=f;
			f=header[f].parent;
			if(header[f].lch==j) 
			{
				j=strlen(header[i].bits);
				memmove(header[i].bits+1,header[i].bits,j+1);//void *memmove( void* dest, const void* src, size_t count );  
				//由src所指内存区域复制count个字节到dest所指内存区域。
				header[i].bits[0]='0';
			}
			else
			{
				j=strlen(header[i].bits);
				memmove(header[i].bits+1,header[i].bits,j+1);
				header[i].bits[0]='1';
			}
		}
 }
 fseek(ifp,0,SEEK_SET);
 /*
 int fseek(FILE *stream, long offset, int fromwhere);
 函数设置文件指针stream的位置。如果执行成功，stream将指向以fromwhere
 （偏移起始位置：文件头0，当前位置1，文件尾2）为基准，偏移offset（指针偏移量）
 个字节的位置。如果执行失败(比如offset超过文件自身大小)，则不改变stream指向的位置。

  　SEEK_SET： 文件开头
 */
	fwrite(&flength,sizeof(int),1,ofp);
	fseek(ofp,8,SEEK_SET);
	buf[0]=0;
	f=0;
	pt1=8;
	while(!feof(ifp))
	{
		c=fgetc(ifp);
		f++;
		for(i=0;i<n;i++)
		{
			if(c==header[i].b) break;
		}
		strcat(buf,header[i].bits);
		j=strlen(buf);
		c=0;
		while(j>=8)
		{
			for(i=0;i<8;i++)
			{
				if(buf[i]=='1') c=(c<<1)|1;
				else c=c<<1;
			}		
			fwrite(&c,1,1,ofp);
			pt1++;
			strcpy(buf,buf+8);
			j=strlen(buf);
		}
		if(f==flength) break;
	}
	if(j>0)
	{
		strcat(buf,"00000000");
		for(i=0;i<8;i++)
		{
			if(buf[i]=='1') c=(c<<1)|1;
			else c=c<<1;
		}
		fwrite(&c,1,1,ofp);
		pt1++;
	}
	fseek(ofp,4,SEEK_SET);
	fwrite(&pt1,sizeof(long),1,ofp);
	fseek(ofp,pt1,SEEK_SET);
	fwrite(&n,sizeof(long),1,ofp);
	for(i=0;i<n;i++)
	{
		fwrite(&(header[i].b),1,1,ofp);
		c=strlen(header[i].bits);
		fwrite(&c,1,1,ofp);
		j=strlen(header[i].bits);
		if(j%8!=0) 
		{
			for(f=j%8;f<8;f++)
			strcat(header[i].bits,"0");
		}
			while(header[i].bits[0]!=0)
	{
	   c=0;
	   for(j=0;j<8;j++)
	   {
		if(header[i].bits[j]=='1') c=(c<<1)|1;
		else c=c<<1;
	   }
	   strcpy(header[i].bits,header[i].bits+8);
	   fwrite(&c,1,1,ofp);
	  }
	 }
	 fclose(ifp);
	 fclose(ofp);
	 printf("compress successfully!\n");
	 return;
	}


	void uncompress(char *filename,char *outputfile)//解压函数
	{
	 char buf[255],bx[255];
	 unsigned char c;
	 long i,j,m,n,f,p,l;
	 long flength;
	 FILE *ifp,*ofp;
	 printf("source filename:");
	 gets(filename);
	 ifp=fopen(filename,"rb");
	 if(ifp==NULL)
	 {
	  ::MessageBox(NULL,"source file open error!","提示",MB_OK);
	  return;
	 }
	 printf("destination filename:");
	 gets(outputfile);
	 ofp=fopen(outputfile,"wb");
	 if(ofp==NULL)
	 {
	  ::MessageBox(NULL,"source file open error!","提示",MB_OK);
	  return;
	 }
	 fread(&flength,sizeof(long),1,ifp);
	 fread(&f,sizeof(long),1,ifp);
	 fseek(ifp,f,SEEK_SET);
	 fread(&n,sizeof(long),1,ifp);
	 for(i=0;i<n;i++)
	 {
	  fread(&header[i].b,1,1,ifp);
	  fread(&c,1,1,ifp);
	  p=(long)c;
	  header[i].count=p;
	  header[i].bits[0]=0;
	  if(p%8>0) m=p/8+1;
	  else m=p/8;
	  for(j=0;j<m;j++)
	  {
	   fread(&c,1,1,ifp);
	   f=c;
	   itoa(f,buf,2);
	   f=strlen(buf);
	   for(l=8;l>f;l--)
	   {
		strcat(header[i].bits,"0");
	   }
	   strcat(header[i].bits,buf);
	  }
	  header[i].bits[p]=0;
	 }
	 for(i=0;i<n;i++)
	 {
	  for(j=i+1;j<n;j++)
	  {
	   if(strlen(header[i].bits)>strlen(header[j].bits))
	   {
		tmp=header[i];
		header[i]=header[j];
		header[j]=tmp;
	   }
	  }
	 }
	 p=strlen(header[n-1].bits);
	 fseek(ifp,8,SEEK_SET);
	 m=0;
	 bx[0]=0;
	 while(1)
	 {
	  while(strlen(bx)<(unsigned int)p)
	  {
	   fread(&c,1,1,ifp);
	   f=c;
	   itoa(f,buf,2);
	   f=strlen(buf);
	   for(l=8;l>f;l--)
	   {
		strcat(bx,"0");
	   }
	   strcat(bx,buf);
	  }
	  for(i=0;i<n;i++)
	  {
	   if(memcmp(header[i].bits,bx,header[i].count)==0) break;
	  }
	  strcpy(bx,bx+header[i].count);
	  c=header[i].b;
	  fwrite(&c,1,1,ofp);
	  m++;
	  if(m==flength) break;
	 }
	 fclose(ifp);
	 fclose(ofp);
	 printf("Uncompress successfully!\n");
	 return;
	}
