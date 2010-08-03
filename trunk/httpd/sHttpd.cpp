#define SERVER_PORT 80
#define SERVER_URL "free.stonept.org"
#define SERVER_NAME "sx-server"
#define SERVER_VER "1.0.0"

#define MaxRequestBytes 1024*20

#include <stdio.h>
#include <winsock.h>
#include <time.h>

#include "request.h"
#include "response.h"


int main(int argc, char* argv[])
{
	SOCKET sockfd, new_fd;

	//初始化WSAStartup
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 1), &WSAData))
	{
		printf("ERROR: WSA failed\n");
		WSACleanup();
		return -1;
	}

	if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 )
	{
		perror("ERROR: socket is error\n;");
		return -1;
	}
	struct sockaddr_in server;
	struct sockaddr_in client;
	int len = sizeof(client);
	server.sin_family = PF_INET;
	server.sin_port = htons(SERVER_PORT);  //the port need listened
	client.sin_port = htons(SERVER_PORT);
	server.sin_addr.s_addr = INADDR_ANY;

	//sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_TCP);
	if (bind(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1)
	{
		perror("ERROR: bind is error\n");
		return -1;
	}
	if (listen(sockfd, 512) == -1)
	{
		perror("ERROR: listen is error\n");
		return -1;
	}
	while (true)
	{
		int sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *) & client, &sin_size)) == -1)
		{
			perror("WARNING: accept is error\n");
			continue;
		}

		Request receive = Request(new_fd);
		Response sender = Response(new_fd);

		if (stricmp(receive.getMethod(), "get") != 0)
		{
			//不支持此方法
			sender.error( 501, "Not Implemented", (char*) 0, "That method is not implemented." );
			closesocket(new_fd);
			continue;
		}
		if (receive.getPath() == 0)
		{
			//路径错误
			sender.error( 400, "Not Found", receive.getPath(), "<p>The requested URL /%s was not found on this server.</p>" );
			closesocket(new_fd);
			continue;
		}
		switch (receive.GetFileType())
		{
		case NOT_EXIST:sender.error( 404, "Not Found", receive.getPath(), "<p>The requested URL /%s was not found on this server.</p>"); break;
		case DOCUMENT:sender.file(200, "OK", receive.getFileMime(), receive.GetFileSize(), receive.getPath()); break;
		case DIRECTORY:sender.dir(200, "OK", receive.getPath()); break;
		}
		closesocket(new_fd);
	}
	//结束清理
	closesocket(sockfd);
	WSACleanup();
	getchar();
	return 0;
}