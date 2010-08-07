#ifndef _EXPLORE_H_ /*需要连接库文件 -lole32 */
#define _EXPLORE_H_

#include <shlobj.h>

//在资源浏览器中打开并且选中
BOOL explore(wchar_t *path)
{
	BOOL bResult;
	LPSHELLFOLDER IpsfDeskTop;
	LPITEMIDLIST pidl;
	HRESULT hres;
	SHGetDesktopFolder(&IpsfDeskTop);
	hres = IpsfDeskTop ->ParseDisplayName(NULL, NULL, path, NULL, &pidl, NULL);
	hres = IpsfDeskTop->Release();
	if (FAILED(hres)) return FALSE;
	typedef HRESULT (WINAPI *MYFUNC)(LPCITEMIDLIST*, UINT, LPCITEMIDLIST*, DWORD);
	HINSTANCE ghShell32 = LoadLibrary("Shell32.DLL");
	if (ghShell32 != NULL)
	{
		MYFUNC SHOpen = (MYFUNC)GetProcAddress(ghShell32, "SHOpenFolderAndSelectItems");
		if (SHOpen != NULL)
		{
			CoInitialize(NULL);
			SHOpen((LPCITEMIDLIST*)pidl, 0, NULL, 0);
			CoUninitialize();
			bResult = TRUE;
		}
		FreeLibrary(ghShell32);
		bResult = FALSE;
	}
	return bResult;
}
#endif /* _EXPLORE_H_ */
