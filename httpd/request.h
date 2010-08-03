#ifndef ___SJJ_HTTP_REQUEST_H_
#define ___SJJ_HTTP_REQUEST_H_

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum _FileType
	{
		NOT_EXIST = 0,
		DOCUMENT,
		DIRECTORY,
	} FileType;

	class Request
	{
	private:
		char *header;
		char *method;
		char *path;
		wchar_t *w_path;
		char *protocol;

		char GetHex(char ch)
		{
			if (ch >= '0' && ch <= '9') return ch - '0';
			if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
			if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
			return -1;
		}
		void UrlDecode(char* text)
		{
			char *code = text;
			while (*text != '\0')
			{
				if (*text == '%' && GetHex(*(text + 1)) != -1 && GetHex(*(text + 2)) != -1)
				{
					*code = GetHex(*++text) * 16 + GetHex(*++text);
				}
				else *code = *text;
				code++;
				text++;
			}
			*code = '\0';
		}
		void UTF8toANSI(char *text)
		{
			DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, text, -1, NULL, 0);
			w_path = (wchar_t*)malloc(dwNum * sizeof(wchar_t));
			MultiByteToWideChar(CP_UTF8, 0, text, -1, w_path, dwNum);
			dwNum = WideCharToMultiByte(CP_ACP, 0, w_path, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, w_path, -1, text, dwNum, NULL, NULL);
		}
	public:
		Request(SOCKET s)
		{
			path = (char*)malloc(10240 * sizeof(char));
			method = (char*)malloc(32 * sizeof(char));
			protocol = (char*)malloc(32 * sizeof(char));
			header = (char*)malloc(MaxRequestBytes * sizeof(char));
			recv(s, header, MaxRequestBytes, 0);

			if (sscanf(header, "%[^ ] %[^ ] %[^ ]", method, path, protocol) == 3 && *path == '/')
			{
				if(strcmp(path,"/")!=0) path++;
				UrlDecode(path);
				UTF8toANSI(path);
			}
			else path = 0;
			free(header);
		}
		char* getMethod()
		{
			return method;
		}
		char* getPath()
		{
			return path;
		}
		char* getProtocol()
		{
			return protocol;
		}

		int GetFileType()
		{
			DWORD dwAttributes = GetFileAttributesW(w_path);
			if (dwAttributes != 0xFFFFFFFF)
			{
				if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) return DIRECTORY;
				else return DOCUMENT;
			}
			return NOT_EXIST;
		}

		long GetFileSize()
		{
			long file_size ;
			FILE* fp;
			fp = fopen( path, "rb");
			fseek( fp, 0, SEEK_END);
			file_size = ftell(fp);
			fclose(fp);
			return file_size;
		}
		char *getFileMime()
		{
			char* dot;

			dot = strrchr( path, '.' );
			if ( dot == (char*) 0 )
				return "text/plain; charset=iso-8859-1";
			if ( strcmp( dot, ".html" ) == 0 || strcmp( dot, ".htm" ) == 0 )
				return "text/html; charset=iso-8859-1";
			if ( strcmp( dot, ".jpg" ) == 0 || strcmp( dot, ".jpeg" ) == 0 )
				return "image/jpeg";
			if ( strcmp( dot, ".gif" ) == 0 )
				return "image/gif";
			if ( strcmp( dot, ".png" ) == 0 )
				return "image/png";
			if ( strcmp( dot, ".css" ) == 0 )
				return "text/css";
			if ( strcmp( dot, ".au" ) == 0 )
				return "audio/basic";
			if ( strcmp( dot, ".wav" ) == 0 )
				return "audio/wav";
			if ( strcmp( dot, ".avi" ) == 0 )
				return "video/x-msvideo";
			if ( strcmp( dot, ".mov" ) == 0 || strcmp( dot, ".qt" ) == 0 )
				return "video/quicktime";
			if ( strcmp( dot, ".mpeg" ) == 0 || strcmp( dot, ".mpe" ) == 0 )
				return "video/mpeg";
			if ( strcmp( dot, ".vrml" ) == 0 || strcmp( dot, ".wrl" ) == 0 )
				return "model/vrml";
			if ( strcmp( dot, ".midi" ) == 0 || strcmp( dot, ".mid" ) == 0 )
				return "audio/midi";
			if ( strcmp( dot, ".mp3" ) == 0 )
				return "audio/mpeg";
			if ( strcmp( dot, ".ogg" ) == 0 )
				return "application/ogg";
			if ( strcmp( dot, ".pac" ) == 0 )
				return "application/x-ns-proxy-autoconfig";
			return "text/plain; charset=iso-8859-1";
		}
	};

#ifdef __cplusplus
}
#endif

#endif /* ___SJJ_HTTP_REQUEST_H_ */
