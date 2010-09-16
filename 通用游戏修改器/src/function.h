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
	TCHAR Hotkey[128];
	_tcscpy(Hotkey,key);
	_tcslwr(Hotkey);
	const TCHAR *nptr = Hotkey;
    TCHAR* result;
    TCHAR temp[32];
	do
	{
        result=_tcschr(nptr,'+');
        _stprintf(temp,_T("%.*s"), result-nptr, nptr);
        nptr = result+1;
        if(_tcsstr(temp,_T("ctrl"))) items[i].mod+=MOD_CONTROL;
		else if(_tcsstr(temp,_T("shift"))) items[i].mod+=MOD_SHIFT;
		else if(_tcsstr(temp,_T("alt"))) items[i].mod+=MOD_ALT;
		else if(_tcslen(temp)==1)//0-9,a-z
		{
            items[i].key=temp[0];
            //if(temp[0]>='0'&&temp[0]<='9') items[i].key=temp[0];
            //if(temp[0]>='a'&&temp[0]<='z') items[i].key=temp[0];
		}
		else
		{
            if(temp[0]=='f')
            {
                _stscanf(temp,_T("f%d"),&(items[i].key));
                items[i].key=items[i].key+VK_F1-1;
            }
		}
	}while(result!=0);
}
Function::Function()
{
	//AllocConsole();
	xMainNodeData = XMLNode::openFileHelper(_T("config.xml"), _T("通用游戏修改器")).getChildNode(_T("游戏数据"));
	XMLNode xNode=xMainNodeData.getChildNode(_T("坐标"));
	if(!xNode.isEmpty()) _stscanf(xNode.getText(),_T("%d,%d"),&sz.cx,&sz.cy);
	else sz.cx=sz.cy=20;
	count = xMainNodeData.nChildNode(_T("功能"));
	items = (Func*)malloc(count*sizeof(Func));
	int i,my;
	for(i=0,my=0; i<count; i++)
	{
        XMLNode xNode = xMainNodeData.getChildNode(_T("功能"), &my);
        _stprintf(items[i].szName,_T("%s[%s]"),xNode.getChildNode(_T("名称")).getText(),xNode.getChildNode(_T("按键")).getText());
        HotKeyRegister(xNode.getChildNode(_T("按键")).getText(),i);
        items[i].bEnabled=false;
	}
}
#endif /* _FUNCTION_H_ */
