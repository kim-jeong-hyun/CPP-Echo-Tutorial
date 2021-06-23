#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void showErrorMsg(string message) {
	cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1);  
}

int main() {
	
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876; //������Ʈ ��� 
	char received[256]; 

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { //winsock�ʱ�ȭ
		showErrorMsg("WSAStartUp()"); //0��ȯ�� ������
	}
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);//TCP ���� ����

	if (serverSocket == INVALID_SOCKET) {
		showErrorMsg("socket()");
	}
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); //2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		showErrorMsg("bind()");
	}
	cout << "[���� ����]: bind()\n";
	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		showErrorMsg("listen()");
	}
	cout << "[�������]: listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	//Ŭ���̾�Ʈ�� ��û ��ٸ�
	cout << "�������]: accept()\n";

	if (clientSocket == INVALID_SOCKET) {
		showErrorMsg("accept()\n");
	}

	while (1) {//Ŭ���̾�Ʈ�� �޽����� �޾Ƽ� �״�� �ٽ� ����
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[Ŭ���̾�Ʈ �޽���] : " << received << '\n';
		cout << "[�޽��� ����] : " << received << '\n';
		if (strcmp(received, "[exit]") == 0) { //�� ���ڿ� �� �� ������ 0
			send(clientSocket, received, sizeof(received) -1, 0); //������ ���� ���� ����
			cout << "[���� ����]\n";
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