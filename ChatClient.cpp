#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>

#pragma warning(disable: 4996) // отключение ошибки 
SOCKET Connection;

void ClientHandler()
{
	char msg[256];
	while (true)
	{
		char nickname[256];
		recv(Connection, nickname, sizeof(nickname), NULL);
		recv(Connection, msg, sizeof(msg), NULL); // получение сообщения бесконечным циклом
		std::cout << nickname << ": " << msg << std::endl; // вывод
	}
}

int main(int argc, char* argv[])
{
	char nickname[256];
	std::cout << "Enter nickname (up to 12 characters): ";
	std::cin.getline(nickname, 256);
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);// указание версии
	if (WSAStartup(DLLVersion, &wsaData) != 0)// Если функция WSAStartup вернула не 0, то произошла ошибка, закрытие программы
	{
		std::cout << "Error #1. Startup Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr; // создание класса под названием addr для хранение адресса
	int sizeofaddr = sizeof(addr); // Переменная, содержащая размер
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Указание ip, на котором стоит  (localhost)
	addr.sin_port = htons(1111);// Указание порта
	addr.sin_family = AF_INET; // семейство протоколов для интернет протоколов константа "AF_INET"

	Connection = socket(AF_INET, SOCK_STREAM, NULL); // создание сокета для подключения клиента к серверу.
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) // Если подключение успешно, то функция вернет 0
	{
		std::cout << "Fail #1. Failed to connect to the server." << std::endl;
		return 1;
	}
	std::cout << "Connected successful" << std::endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	char msg_tosend[256];
	while (true)
	{
		std::cin.getline(msg_tosend, sizeof(msg_tosend)); // бесконечный цикл считывания введенной строки
		send(Connection, nickname, sizeof(nickname), NULL); // отправка имени
		Sleep(10);
		send(Connection, msg_tosend, sizeof(msg_tosend), NULL); // отправка введенной строки
		Sleep(10); // небольшая пауза
	}

	system("pause");
	std::cout << 1;
	return 0;
}