 struct node
{
	HWND  d_h;
	int d;
    node *next;
};
 class  d_list
{
 private:
 node *head;
 node *tail;
 int d_nCount;
 int now_pos;

 public:                  //form
 d_list();
 ~d_list();
 void get();
 int d_haveSame(HWND h);
 int d_insert(HWND h,int n);
 int d_delete(HWND h,int n);
 int d_setpos(int n,int num);

 public:                  //read out
 HWND d_read(int n);
 int d_getnCount();
 d_list &operator=(d_list &m_list);
 int d_comp(d_list &m_list);
 int d_read_add(d_list &m_list,int way);
};
