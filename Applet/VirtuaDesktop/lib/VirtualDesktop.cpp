#include "stdlib.h"
#include "LinkList.h"
d_list::d_list()
{
 d_nCount=0;
 now_pos=0;
 head=(node*)malloc(sizeof(node));
 head->d_h=NULL;
 head->d=-1;
 head->next=NULL;
 tail=head;
}

d_list::~d_list()
{
	node *temp=head->next,*m_head;
	while(temp!=NULL)
	{
      m_head=temp->next;
      free(temp);
      d_nCount--;
	  temp=m_head;
	}
	tail=head;
}

void d_list::get()
{
	node *temp=head->next,*m_head;
	while(temp!=NULL)
	{
      m_head=temp->next;
      free(temp);
      d_nCount--;
	  temp=m_head;
	}
	tail=head;
}

int d_list::d_insert(HWND h,int n)
{
  node *temp=(node*)malloc(sizeof(node));
  temp->d_h=h;
  temp->d=n;
  temp->next=NULL;
  tail->next=temp;
  tail=temp;
  d_nCount++;
  return 0;
}

int d_list::d_delete(HWND h,int n)
{
  node *temp=head->next;
  node *pre=NULL;
  while(temp->d_h!=h)
  {
   pre=temp;
   temp=temp->next;
  }
  if(temp)
  {
	  pre->next=temp->next;
	  free(temp);
	  d_nCount--;
  }
  return 0;
}

int d_list::d_haveSame(HWND h)
{
 int n=0;
 node *temp=head->next;
  while(temp->d_h!=h)
  {
   temp=temp->next;
   n++;
  }
  if(temp!=NULL)
	  return n;
     else
	  return -1;
}

HWND d_list::d_read(int n)
{
    node *temp=head;
	while(n>0)
	{
	 temp=temp->next;
     n--;
	}

	return temp->d_h;
}

int d_list::d_getnCount()
{
	return d_nCount;
}

d_list &d_list::operator =(d_list &m_list)
{  // this->~d_list();
	this->get();
    int d=m_list.d_getnCount();
	int i=1;
	while(i<=d)
	{
      this->d_insert(m_list.d_read(i++),0);
	}
	return *this;
}

int d_list::d_comp(d_list &m_list)
{
 int i1=this->d_getnCount();
 int i2=m_list.d_getnCount();
 int i3=1,i4=1;
 int temp;
                                             //前者与后者比较

    temp=i1;
    while(i3<=i2)
	{
        while(i1>=1)
		{
         if(this->d_read(i1)==m_list.d_read(i3))
		 {
		   m_list.d_setpos(i3,1);            //找到了
		   break;
		 }
		   i1--;
		}
		if(i1==0)
		   m_list.d_setpos(i3,0);            //没找到
           i1=temp;
		   i3++;
	}

                                             //后者与前者相比较
	 i1=this->d_getnCount();
     i2=m_list.d_getnCount();
	 temp=i2;
    while(i4<=i1)
	{
        while(i2>=1)
		{
         if(this->d_read(i4)==m_list.d_read(i2))
		 {
			 this->d_setpos(i4,1);              //找到了
		     break;
		 }
		 i2--;
		}
		if(i2==0)
		   this->d_setpos(i4,0);            //没找到
		   i2=temp;
		   i4++;
	}

  return 0;
}

int d_list::d_setpos(int n,int num)
{
  node *temp=head;
	while(n>0)
	{
	 temp=temp->next;
     n--;
	}
    temp->d=num;
	return 0;
}

int d_list::d_read_add(d_list &m_list,int way)
{  int i=1;
   node *temp=head->next;
   while(i<=d_nCount)
   {
    if((way==0)&&(temp!=NULL)&&(temp->d==0))
      m_list.d_delete(temp->d_h,0);
	if((way==1)&&(temp!=NULL)&&(temp->d==0))
	  m_list.d_insert(temp->d_h,0);

	temp=temp->next;
	i++;
   }
   return 0;
}



int DESKTOPNUM = 4;
int pre_desk_num;
int now_desk_num;

d_list now_desk;
d_list pre_desk;
d_list desktop[9];
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	char buff[256];
	if (GetWindowLong(hwnd, GWL_STYLE) & WS_VISIBLE)
	{

		GetWindowText(hwnd, buff, 255);
		if (lstrcmp(buff, szClassName) == 0)           //自己
			return 1;
		if (lstrcmp(buff, "Program Manager") == 0)   //桌面
			return 1;
		GetClassName(hwnd, buff, 255);
		if (lstrcmp(buff, "tooltips_class32") == 0) //任务栏提示
			return 1;
		if (lstrcmp(buff, "Shell_TrayWnd") == 0) //任务栏
			return 1;
		if (lstrcmp(buff, "WorkerW") == 0) //桌面图标
			return 1;
		now_desk.d_insert(hwnd, 0);
		ShowWindow(hwnd, SW_HIDE);
	}
	return 1;
}

void ContorlVirtualDesktop(int x,HWND hWnd)
{
	if(x>=DESKTOPNUM) return;
	if(now_desk_num==x) return;
	pre_desk_num = now_desk_num;
	pre_desk = now_desk;
	now_desk_num = x;
	EnumWindows(EnumWindowsProc, LPARAM(NULL));
	pre_desk.d_comp(now_desk);
	pre_desk.d_read_add(desktop[pre_desk_num], 0);
	now_desk.d_read_add(desktop[pre_desk_num], 1);
	desktop[now_desk_num].d_comp(now_desk);
	desktop[now_desk_num].d_read_add(desktop[now_desk_num], 0);
	if(x==0)
	{
		desktop[now_desk_num].d_comp(now_desk);
		desktop[now_desk_num].d_read_add(desktop[now_desk_num],0);
	}
	int i = 1;
	while (i <= desktop[now_desk_num].d_getnCount())
		::ShowWindow(desktop[now_desk_num].d_read(i++), SW_SHOW);
	InvalidateRect(hWnd, NULL, TRUE);
}
