//Î´Íê³É
#include <windows.h>
#include <stdio.h>
bool locator(char *regedit)
{
	HKEY hKey;
	char direc[1024];
	DWORD dwLength = 1024;
	
	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit", 0, KEY_ALL_ACCESS + 0x100, &hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey, "LastKey", 0, 0, (unsigned char *)direc, &dwLength)==ERROR_SUCCESS)
		{
			if(strchr(direc,'\\'))
            {
            	*strchr(direc,'\\')=0;
            }
			strcat(direc,"\\");
			strcat(direc,regedit);
			
			if(RegSetValueEx(hKey, "LastKey", 0, REG_SZ, (unsigned char *)direc, strlen(direc))==ERROR_SUCCESS)
			{
				ShellExecute(0, NULL, "regedit.exe",NULL, NULL, SW_SHOWNORMAL);
				return true;
			}
		}
	}
	return false;
}
bool Favorites(char *path)
{
	HKEY hKey;
	char direc[1024];
	DWORD dwLength = 1024;
	char regedit[1024];
	strcpy(regedit,path);
	if(RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit\\Favorites", 0, KEY_ALL_ACCESS + 0x100, &hKey)==ERROR_SUCCESS)
	{
		char keys[128];
		keys[0]=0;
		int j,i;
		if(regedit[strlen(regedit)-1]=='\\') regedit[strlen(regedit)-1]=0;
        if(strrchr(regedit,'\\'))
		{
		    j = strrchr(regedit,'\\') - regedit;
		    for(i=0;i<j;i++)
		    {
                keys[i] = regedit[j+i+1];
            }
		    keys[i]=0;
        }
        else strcpy(keys,regedit);
        while(RegQueryValueEx(hKey, keys, 0, 0, 0,0)==ERROR_SUCCESS)
        {
            strcat(keys,"_");          
        }
		if(RegSetValueEx(hKey, keys, 0, REG_SZ, (unsigned char *)path, strlen(path))==ERROR_SUCCESS)
		{
            return true;
		}
	}
	return false;	
}
int main()
{
	//char reg[]="HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit\\";
	//locator(reg);
	Favorites("Computer\\HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer");
	///puts(reg);
	getchar();
}
