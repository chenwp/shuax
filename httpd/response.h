#ifndef ___SJJ_HTTP_RESPONSE_H_
#define ___SJJ_HTTP_RESPONSE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define PROTOCOL "HTTP/1.0"
#define RFC2616FMT "Date: %a, %d %b %Y %H:%M:%S GMT\r\n"

	class Response
	{
	private:
		SOCKET sockfd;
		void header(int status, char* title, char* mime_type, long fsize, char* content)
		{
			char *header = (char*)malloc(2048 * sizeof(char));
			char *temp = (char*)malloc(128 * sizeof(char));
			sprintf(temp, "%s %d %s\r\n", PROTOCOL, status, title);
			strcpy(header, temp);
			sprintf(temp, "Server: %s\r\n", SERVER_NAME);
			strcat(header, temp);
			time_t now = time(NULL);
			strftime(temp, 128, RFC2616FMT, gmtime(&now));
			strcat(header, temp);
			sprintf(temp, "Content-Type: %s\r\n", mime_type);
			strcat(header, temp);
			sprintf(temp, "Content-Length: %ld\r\n", fsize);
			strcat(header, temp);
			sprintf(temp, "Connection: close\r\n\r\n");
			strcat(header, temp);
			strcat(header, content);
			send(sockfd, header, strlen(header), 0);
			free(temp);
			free(header);
		}
		void Localtion(int status, char* title, char *path)
		{
			char *content = (char*)malloc(20480 * sizeof(char));
			char *temp = (char*)malloc(1024 * sizeof(char));

			sprintf( temp, "<html>\n<head><title>%d %s</title></head>\n<body bgcolor=\"white\"><h1>%d %s</h1>\n", status, title, status, title );
			strcpy(content, temp);
			sprintf( temp, "<p>The document has moved.</p>\n");
			strcat(content, temp);
			sprintf( temp, "<hr>\n<center>%s/%s</center>\n</body>\n</html>", SERVER_NAME, SERVER_VER);
			strcat(content, temp);
			//
			if (strrchr(path, '/') != 0) path = strrchr(path, '/') + 1;
			char *temp2 = (char*)malloc(1024 * sizeof(char));
			UrlEncode(path, temp2);
			sprintf(temp, "text/html\r\nLocation: %s/", temp2);
			free(temp2);
			header(status, title, temp, strlen("The document has moved" ), "The document has moved" );
			free(temp);
			free(content);
		}

		char ToHex(unsigned char ch, int mode)
		{
			static char hex[] = "0123456789ABCDEF";
			if (mode == 1)
			{
				return hex[ch%16];
			}
			else return hex[ch/16];
		}
		void ANSItoUTF8(char *text)
		{
			DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, text, -1, NULL, 0);
			wchar_t *w_path = (wchar_t*)malloc(dwNum * sizeof(wchar_t));
			MultiByteToWideChar(CP_ACP, 0, text, -1, w_path, dwNum);
			dwNum = WideCharToMultiByte(CP_UTF8, 0, w_path, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, w_path, -1, text, dwNum, NULL, NULL);
			free(w_path);
		}
		void UrlEncode(char* word, char *neww)
		{
			ANSItoUTF8(word);
			while (*word != '\0')
			{
				if (*word <= 32 || *word >= 123)
				{
					*neww++ = '%';
					*neww++ = ToHex(*word, 0);
					*neww++ = ToHex(*word, 1);
				}
				else *neww++ = *word;
				word++;
			}
			*neww = '\0';
		}
		char *makesize(WIN32_FIND_DATA ffbuf)
		{
			char *temp = (char*)malloc(32 * sizeof(char));
			double result = ffbuf.nFileSizeLow / 1024.0 + ffbuf.nFileSizeHigh * 1024.0 * 1024.0 * 4;
			if (result < 1024.0) sprintf( temp, "%10.2f KB", result);
			else
			{
				result = result / 1024.0;
				if (result < 1024.0) sprintf( temp, "%10.2f MB", result);
				else
				{
					result = result / 1024.0;
					sprintf( temp, "%10.2f GB", result);
				}
			}
			return temp;
		}
	public:
		Response(SOCKET s)
		{
			sockfd = s;
		}

		void error(int status, char* title, char* path, char* text)
		{
			char *error = (char*)malloc(20480 * sizeof(char));
			char *temp = (char*)malloc(1024 * sizeof(char));
			sprintf( temp, "<html>\n<head><title>%d %s</title></head>\n<body bgcolor=\"white\"><h1>%d %s</h1>\n", status, title, status, title );
			strcpy(error, temp);
			sprintf( temp, text, path);
			strcat(error, temp);
			sprintf( temp, "<hr>\n<center>%s/%s</center>\n</body>\n</html>", SERVER_NAME, SERVER_VER);
			strcat(error, temp);
			header(status, title, "text/html", strlen(error), error);
			free(error);
			free(temp);
		}
		void file(int status, char* title, char* mime_type, long fsize, char* path)
		{
			header(status, title, mime_type, fsize, "");
			char *buffer = (char *)malloc(1024 * 64 * sizeof(char));
			FILE *fp;
			fp = fopen(path, "rb");
			if (fp != NULL)
			{
				while (ftell(fp) < fsize)
				{
					send(sockfd, buffer, fread(buffer, sizeof(char), 1024*64, fp), 0);
				}
				fclose(fp);
			}
			free(buffer);
		}
		void dir(int status, char* title, char* path)
		{

			char *content = (char*)malloc(20480 * sizeof(char));
			char *temp = (char*)malloc(1024 * sizeof(char));
			if (path[strlen(path)-1] != '/')
			{
				Localtion(301, "Moved Permanently", path);
				free(content);
				free(temp);
				return;
			}
			sprintf( temp, "<html>\n<head><title>Index of /%s</title></head>\n<body bgcolor=\"white\">\n<h1>Index of /%s</h1>\n<hr><pre>\n<a href=\"../\">../</a>\n", path, path);
			strcpy(content, temp);

			sprintf(temp, "./%s*.*", path);

			WIN32_FIND_DATA ffbuf;
			HANDLE hfind = FindFirstFile(temp, &ffbuf);

			if (hfind != INVALID_HANDLE_VALUE)
			{
				FindNextFile(hfind, &ffbuf);
				while (FindNextFile(hfind, &ffbuf))
				{
					if (FILE_ATTRIBUTE_DIRECTORY == ffbuf.dwFileAttributes) strcat(ffbuf.cFileName, "/");
					sprintf(temp, "<a href=\"%s\">%s</a>", ffbuf.cFileName, ffbuf.cFileName);
					strcat(content, temp);
					for (int i = strlen(ffbuf.cFileName); i < 60; i++)
						strcat(content, " ");

					SYSTEMTIME   st;
					FileTimeToSystemTime(&ffbuf.ftLastWriteTime, &st);
					sprintf(temp, "%04d-%02d-%02d %02d:%02d:%02d\t\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
					strcat(content, temp);

					if (FILE_ATTRIBUTE_DIRECTORY != ffbuf.dwFileAttributes) sprintf( temp, "%s", makesize(ffbuf));
					else sprintf( temp, "%13s", "-");
					strcat(content, temp);
					strcat(content, "\n");

				}
				FindClose(hfind);
			}
			sprintf( temp, "</pre><hr></body>\n</html>");
			strcat(content, temp);
			header(status, title, "text/html", strlen(content), content);
			free(content);
			free(temp);
		}
	};

#ifdef __cplusplus
}
#endif

#endif /* ___SJJ_HTTP_RESPONSE_H_ */