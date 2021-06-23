#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void showErrorMsg(string message) {
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);  
}

int main() {
	
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876; //서버포트 명시 
	char received[256]; 

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { //winsock초기화
		showErrorMsg("WSAStartUp()"); //0반환이 성공임
	}
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);//TCP 소켓 생성

	if (serverSocket == INVALID_SOCKET) {
		showErrorMsg("socket()");
	}
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); //2바이트 정수 네트워크 바이트 형식으로

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		showErrorMsg("bind()");
	}
	cout << "[현재 상태]: bind()\n";
	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		showErrorMsg("listen()");
	}
	cout << "[현재상태]: listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	//클라이언트의 요청 기다림
	cout << "현재상태]: accept()\n";

	if (clientSocket == INVALID_SOCKET) {
		showErrorMsg("accept()\n");
	}

	while (1) {//클라이언트의 메시지를 받아서 그대로 다시 전달
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트 메시지] : " << received << '\n';
		cout << "[메시지 전송] : " << received << '\n';
		if (strcmp(received, "[exit]") == 0) { //두 문자열 비교 후 같으면 0
			send(clientSocket, received, sizeof(received) -1, 0); //마지막 공백 문자 제거
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) -1, 0);
	}
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}