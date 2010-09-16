#ifndef _FUNCTION_H_
#define _FUNCTION_H_

SIZE sz;

struct Func
{
	TCHAR szName[MAX_PATH];
	BOOL bEnabled;
	int mod;
	int key;
};

class Function
{
private:
	void HotKeyRegister(const TCHAR *key,int i);
public:
	Function();
	Func *items;
	int count;
};
void Function::HotKeyRegister(const TCHAR *key,int i)
{
	if(!key) return;
	items[i].mod = items[i].key = 0;

}
Function::Function()
{
	XMLNode xMainNode = XMLNode::openFileHelper(_T("config.xml"), _T("通用游戏修改器")).getChildNode(_T("游戏数据"));
	XMLNode xNode=xMainNode.getChildNode(_T("坐标"));
	if(!xNode.isEmpty()) _stscanf(xNode.getText(),_T("%d,%d"),&sz.cx,&sz.cy);
	else sz.cx=sz.cy=20;
	count = xMainNode.nChildNode(_T("功能"));
	items = (Func*)malloc(count*sizeof(Func));
	int i,my;
	for(i=0,my=0; i<count; i++)
	{
        XMLNode xNode = xMainNode.getChildNode(_T("功能"), &my);
        _stprintf(items[i].szName,_T("%s[%s]"),xNode.getChildNode(_T("名称")).getText(),xNode.getChildNode(_T("按键")).getText());
        HotKeyRegister(xNode.getChildNode(_T("按键")).getText(),i);
	}
}
#endif /* _FUNCTION_H_ */
