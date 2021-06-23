#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void sendErrorMsg(string message) {
	cout << "오류 발생 : " << message<<'\n';
	system("pause");
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; //포트번호 서버와 같아야 함
	char received[256]; //256바이트를 담는 문자열 버퍼
	string sent; //서버로 보낼 데이터를 담을 문자열

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		sendErrorMsg("WSAStartup()");
	}
	clientSocket=socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		sendErrorMsg("socekt()");
	}
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //자기 자신의 컴퓨터임
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) != 0) {
		sendErrorMsg("connect()");
	}
	cout << "[현재상태] : connect()\n";
	
	while (1) {
		cout << "[메시지 전송] : ";
		getline(cin, sent); //입력 스트림에서 문자열을 한 줄씩 추출
		if (sent == "") continue; //공백이므로 무시
		send(clientSocket, sent.c_str(), sent.length(), 0); //입력한 문자열을 서버로 보내기
		int length = recv(clientSocket, received, sizeof(received), 0); //서버로 부터 문자열을 받기
		received[length] = '\0'; //문자열 마지막에는 NULL값 넣기 효과적으로 처리하기 위해
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 종료]\n";
			break;
		}
		cout << "[서버 메시지]: " << received << '\n';
	}
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}