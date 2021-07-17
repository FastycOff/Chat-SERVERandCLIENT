#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#pragma warning(disable: 4996) // отключение ошибки 
SOCKET Connections[100];
int counter = 0;

void ClientHandler(int index)
{
	char msg[256];
	char nickname[256];
	while (true)
	{
		recv(Connections[index], nickname, sizeof(nickname), NULL); // принимает имя клиента
		recv(Connections[index], msg, sizeof(msg), NULL); // принимает собщение клиента
		for (int to_send = 0; to_send < counter; ++to_send) // проходится по всем подключенным клиентам
		{
			if (to_send != index) // Если клиент(to_send) не является отправителем(index), то ему отправляется сообщение отправителя
			{
				send(Connections[to_send], nickname, sizeof(nickname), NULL);
				send(Connections[to_send], msg, sizeof(msg), NULL);
			}
		}
	}
}

int main(int argc, char* argv[])
{
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

	
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);// создание сокета "sListen", хранит результат выполнения функции "socket"
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN); //Указание количества допустимых запросов в очереди

	SOCKET newConnection;
	for (int person = 0; person < 100; ++person)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);//addr - информация об ip-addres того, кто подключился

		if (newConnection == 0)	//если функция возвращает 0, то клиент не смог подключиться
		{
			std::cout << "Error #2. Connection Error" << std::endl;
		}
		else
		{
			std::cout << "Client #" << person << " connected successful" << std::endl;
			Connections[person] = newConnection; // запись клиента в массив 
			++counter; // счетчик клиентов
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(person), NULL, NULL); // создание нового потока в обработчиком клиентов.
		}
	}
	system("pause");
	return 0;
}