#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void sendErrorMsg(string message) {
	cout << "���� �߻� : " << message<<'\n';
	system("pause");
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; //��Ʈ��ȣ ������ ���ƾ� ��
	char received[256]; //256����Ʈ�� ��� ���ڿ� ����
	string sent; //������ ���� �����͸� ���� ���ڿ�

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		sendErrorMsg("WSAStartup()");
	}
	clientSocket=socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		sendErrorMsg("socekt()");
	}
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //�ڱ� �ڽ��� ��ǻ����
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) != 0) {
		sendErrorMsg("connect()");
	}
	cout << "[�������] : connect()\n";
	
	while (1) {
		cout << "[�޽��� ����] : ";
		getline(cin, sent); //�Է� ��Ʈ������ ���ڿ��� �� �پ� ����
		if (sent == "") continue; //�����̹Ƿ� ����
		send(clientSocket, sent.c_str(), sent.length(), 0); //�Է��� ���ڿ��� ������ ������
		int length = recv(clientSocket, received, sizeof(received), 0); //������ ���� ���ڿ��� �ޱ�
		received[length] = '\0'; //���ڿ� ���������� NULL�� �ֱ� ȿ�������� ó���ϱ� ����
		if (strcmp(received, "[exit]") == 0) {
			cout << "[���� ����]\n";
			break;
		}
		cout << "[���� �޽���]: " << received << '\n';
	}
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}