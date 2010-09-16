#ifndef _CONFIG_H_
#define _CONFIG_H_
class config
{
private:
public:
    TCHAR szClassName[MAX_PATH];
    TCHAR szTitleText[MAX_PATH];

    TCHAR szProcess[MAX_PATH];

    TCHAR szAbout[MAX_PATH];
    TCHAR szWebsite[MAX_PATH];

    BOOL single;
    SIZE size;
    SIZE about;
    UINT timer;
    UINT pro;

    config();
};
config::config()
{
    _tcscpy(szClassName,_T("通用游戏修改器"));
    xMainNodeSet = XMLNode::openFileHelper(_T("config.xml"), _T("通用游戏修改器")).getChildNode(_T("程序设置"));
    XMLNode xNode;
    //
    xNode=xMainNodeSet.getChildNode(_T("程序标题"));
    if(!xNode.isEmpty()) _tcscpy(szTitleText,xNode.getText());
    else _tcscpy(szTitleText,szClassName);
    //
    xNode=xMainNodeSet.getChildNode(_T("是否多开"));
    if(!xNode.isEmpty() && _tcscmp(xNode.getText(),_T("是"))!=0) single=TRUE;
    else single=FALSE;
    //
    xNode=xMainNodeSet.getChildNode(_T("窗口大小"));
    if(!xNode.isEmpty()) _stscanf(xNode.getText(),_T("%d,%d"),&size.cx,&size.cy);
    else size.cx=size.cy=400;
    //
    xNode=xMainNodeSet.getChildNode(_T("关于我们")).getChildNode(_T("坐标"));
    if(!xNode.isEmpty()) _stscanf(xNode.getText(),_T("%d,%d"),&about.cx,&about.cy);
    else about.cx=about.cy=10;

    xNode=xMainNodeSet.getChildNode(_T("关于我们")).getChildNode(_T("说明"));
    if(!xNode.isEmpty()) _tcscpy(szAbout,xNode.getText());
    else _tcscpy(szAbout,_T("欢迎使用耍下开发的通用游戏修改器。"));

    xNode=xMainNodeSet.getChildNode(_T("关于我们")).getChildNode(_T("网址"));
    if(!xNode.isEmpty()) _tcscpy(szWebsite,xNode.getText());
    else _tcscpy(szWebsite,_T("http://www.shuax.com"));

    _tcscat(szAbout,_T("\n"));
    _tcscat(szAbout,szWebsite);

    xNode=xMainNodeSet.getChildNode(_T("获得进程")).getChildNode(_T("刷新"));
    if(!xNode.isEmpty()) _stscanf(xNode.getText(),_T("%d"),&timer);
    else timer=500;
    //
    xNode=xMainNodeSet.getChildNode(_T("获得进程")).getChildNode(_T("名称"));
    if(!xNode.isEmpty()) _tcscpy(szProcess,xNode.getText());
    else
    {
        MessageBox(0,_T("配置文件缺少必要标签，程序必须退出！"),_T("错误"),MB_ICONERROR);
        exit(0);
    }

    xNode=xMainNodeSet.getChildNode(_T("获得进程")).getChildNode(_T("类型"));
    if(!xNode.isEmpty())
    {
        if(_tcscmp(xNode.getText(),_T("进程名称"))==0) pro=0;
        else if(_tcscmp(xNode.getText(),_T("窗口标题"))==0) pro=1;
        else if(_tcscmp(xNode.getText(),_T("窗口类名"))==0) pro=2;
        else pro=0;
    }
    else pro=0;

}
#endif /*_CONFIG_H_*/
