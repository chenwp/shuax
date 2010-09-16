#ifndef _CONFIG_H_
#define _CONFIG_H_
class config
{
private:
public:
    TCHAR szClassName[MAX_PATH];
    TCHAR szTitleText[MAX_PATH];
    TCHAR szIconPath[MAX_PATH];
    TCHAR szImagePath[MAX_PATH];
    TCHAR szProcess[MAX_PATH];
    BOOL single;
    SIZE size;
    UINT timer;
    UINT pro;

    config();
};
config::config()
{
    _tcscpy(szClassName,_T("通用游戏修改器"));
    XMLNode xMainNode = XMLNode::openFileHelper(_T("config.xml"), _T("通用游戏修改器")).getChildNode(_T("程序设置"));
    XMLNode xNode;
    //
    xNode=xMainNode.getChildNode(_T("标题"));
    if(!xNode.isEmpty()) _tcscpy(szTitleText,xNode.getText());
    else _tcscpy(szTitleText,szClassName);
    //
    xNode=xMainNode.getChildNode(_T("多开"));
    if(!xNode.isEmpty() && _tcscmp(xNode.getText(),_T("是"))!=0) single=TRUE;
    else single=FALSE;
    //
    xNode=xMainNode.getChildNode(_T("大小"));
    if(!xNode.isEmpty()) _stscanf(xNode.getText(),_T("%d,%d"),&size.cx,&size.cy);
    else size.cx=size.cy=400;

    //
    xNode=xMainNode.getChildNode(_T("图标"));
    if(!xNode.isEmpty()) _tcscpy(szIconPath,xNode.getText());
    else _tcscpy(szIconPath,_T(""));
    //
    xNode=xMainNode.getChildNode(_T("图片"));
    if(!xNode.isEmpty()) _tcscpy(szImagePath,xNode.getText());
    else _tcscpy(szImagePath,_T(""));
    //
    xNode=xMainNode.getChildNode(_T("刷新"));
    if(!xNode.isEmpty()) _stscanf(xNode.getText(),_T("%d"),&timer);
    else timer=500;
    //
    xNode=xMainNode.getChildNode(_T("进程"));
    if(!xNode.isEmpty() && xNode.getAttribute(_T("类型")))
    {
        _tcscpy(szProcess,xNode.getText());
        if(_tcscmp(xNode.getAttribute(_T("类型")),_T("进程名称"))==0) pro=0;
        else if(_tcscmp(xNode.getAttribute(_T("类型")),_T("窗口标题"))==0) pro=1;
        else if(_tcscmp(xNode.getAttribute(_T("类型")),_T("窗口类名"))==0) pro=2;
        else pro=0;
    }
    else
    {
        MessageBox(0,_T("配置文件缺少标签：\"<进程 类型=\"\"></进程>\"，程序必须退出！"),_T("错误"),MB_ICONERROR);
        exit(0);
    }
}
#endif /*_CONFIG_H_*/
