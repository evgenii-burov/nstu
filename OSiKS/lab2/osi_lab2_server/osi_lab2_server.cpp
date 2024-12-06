#include <stdio.h> 
#include <stdlib.h> 
#include <WinSock2.h> 
#include <ws2tcpip.h> 
#include <string> 
#include <iostream>

// Задаем постоянные величины 
const int MAX_USERNAME_LEN = 32;
const int MAX_MESSAGE_LEN = 1000;

const int MAX_CLIENTS_COUNT = 50;

const char ERR_CODE[] = "Возникла ошибка при работе сервера. Код ошибки: %ld\n";
const char ERR_GET_ADDR[] = "Ошибка при получении адреса сервера.\n";

const char ERR_INIT[] = "Ошибка при инициализации работы с сокетами.\n";
const char ERR_CREATE_SOCK[] = "Ошибка при создании сокета.\n";
const char ERR_BIND_SOCK[] = "Ошибка при связке сокета.\n";
const char ERR_LISTEN_SOCK[] = "Ошибка при прослушивании сокета.\n";
const char ERR_ACCEPT_SOCK[] = "Ошибка при соединении с клиентом.\n";

const char ERR_MAX_CLIENTS[] = "Достигнут максимум клиентов.\nСоединение завершено.\n";

const char INFO_SERVER_LAUNCH[] = "Сервер запущен на: %s:%d\n";
const char INFO_ACCEPT_CONN[] = "Установлено новое соединение с: %s:%d\n";
const char INFO_CLOSE_CONN[] = "Закрыто соединение с %s:%d\n";
const char INFO_COUNT_CONN[] = "Текущее количество клиентов: %i\n";

const char CLIENT_NEW[] = "Новый клиент %s(%s)\n";
const char CLIENT_DISCONNECT[] = "Клиент %s(%s) покинул чат.\n";

const char MESSAGE_FULL[] = "Извините. В данный момент сервер переполнен.\n";
const char MESSAGE_NAME[] = "Введите ваше имя: ";

HANDLE ghMutex;

DWORD WINAPI ClientThread(void* param);

class Client
{
public:
    SOCKET Sock = SOCKET_ERROR;
    SOCKADDR_IN SockAddr;
    char IP[INET_ADDRSTRLEN];
    char Name[MAX_USERNAME_LEN]{};

    // Создание экземпляра класса 
        Client(SOCKET sock, SOCKADDR_IN addr)
    {
        Sock = sock;
        SockAddr = addr;
        inet_ntop(AF_INET, &(SockAddr.sin_addr), IP, INET_ADDRSTRLEN);
    }

    Client(SOCKET sock, SOCKADDR_IN addr, char name[MAX_USERNAME_LEN])
    {
        Sock = sock;
        SockAddr = addr;
        inet_ntop(AF_INET, &(SockAddr.sin_addr), IP, INET_ADDRSTRLEN);
        strcpy_s(Name, name);
    }

    USHORT GetPort()
    {
        return SockAddr.sin_port;
    }

    char* GetIP()
    {
        return IP;
    }
};

class Server
{
public:
    WSAData wsaData;   // Структура WSA 
    SOCKET sock = SOCKET_ERROR; // Сокет сервера 

    SOCKADDR_IN sin;   // Структура адреса сервера 
    char HostName[256];  // Имя хоста 
    char Addr[INET_ADDRSTRLEN];

    int retVal = 0;

    int Error = 0;

    Server()
    {
        if (!InitWSA())
            return;

        // Создаем сокет 
        if (!CreateSock())
            return;

        // Связываем сокет 
        if (!BindSock())
            return;

        // Получаем IP адрес и порт 
        if (!GetAddr())
            return;

        // Выводим на консоль информацию о том что сервер запущен 
        printf_s(INFO_SERVER_LAUNCH, Addr, htons(sin.sin_port));

        // Получаем IP адрес и порт 
        if (!ListenSock())
            return;

        // Создаем мьютекс  
        ghMutex = CreateMutex(NULL, FALSE, NULL);

        // Запускаем сервер 
        RunServer();
    }

    bool InitWSA()
    {
        retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (retVal == SOCKET_ERROR)
            {
                printf_s(ERR_INIT);
                Error = WSAGetLastError();
                return false;
            }

        return true;
    }

    // Создание сокета 
    bool CreateSock()
    {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET)
        {
            ExceptError(ERR_CREATE_SOCK);
            return false;
        }

        return true;
    }

    // Задаем параметры сервера 
    bool BindSock()
    {
        // Получаем IP и порт сервера 
        sin.sin_family = AF_INET;
        sin.sin_port = htons(2011);
        sin.sin_addr.s_addr = INADDR_ANY;

        // Связываем сокет 
        retVal = bind(sock, (LPSOCKADDR)&sin, sizeof(sin));
        if (retVal == SOCKET_ERROR) {
            ExceptError(ERR_BIND_SOCK);
            return false;
        }

        return true;
    }

    // Получаем IP адрес и порт сервера 
    bool GetAddr()
    {
        // Получаем Hostname компьютера 
        if (!gethostname(HostName, 256))
        {
            struct addrinfo* feed_server = NULL;
            struct sockaddr_in saddr;

            // Получаем информацию о хосте 
            getaddrinfo(HostName, NULL, NULL, &feed_server);

            while (feed_server->ai_next != NULL)
            {
                feed_server = feed_server->ai_next;

                // Преобразуем структуру addrinfo в sockaddr_in 
                saddr = *(struct sockaddr_in*)feed_server->ai_addr;

                // Преобразуем структуру в адрес 
                inet_ntop(AF_INET, &(saddr.sin_addr), Addr, INET_ADDRSTRLEN);
                if (strcmp("0.0.0.0", Addr))
                    break;
            }

                // Освобождаем выделенное место 
                freeaddrinfo(feed_server);

            return true;
        }

        ExceptError(ERR_GET_ADDR);
        return false;
    }

    // Переводим сокет в состояние "прослушки" 
    bool ListenSock()
    {
        retVal = listen(sock, MAX_CLIENTS_COUNT);
        if (retVal == SOCKET_ERROR) {
            ExceptError(ERR_LISTEN_SOCK);
            return false;
        }

        return true;
    }

    // Принимаем соединение 
    bool AcceptSock(SOCKET& clientSock, SOCKADDR_IN& from)
    {
        int fromLen = sizeof(from);
        clientSock = accept(sock, (struct sockaddr*)&from, &fromLen);
        if (clientSock == INVALID_SOCKET)
        {
            WaitForSingleObject(ghMutex, INFINITE);

            ExceptError(ERR_ACCEPT_SOCK);
            printf_s(ERR_CODE, WSAGetLastError());

            ReleaseMutex(ghMutex);

            return false;
        }

        return true;
    }

    void RunServer()
    {
        while (true)
        {
            // Принимаем сокет 
            SOCKET clientSock;
            SOCKADDR_IN from;
            if (!AcceptSock(clientSock, from))
                continue;

            // Создаем экземпляр клиента 
            Client* client = new Client(clientSock, from);

            // Запускаем поток для обработки клиента 
            DWORD threadID;
            CreateThread(NULL, NULL, ClientThread, (void*)client, NULL, &threadID);
        }

        // Завершаем работу сервера 
        closesocket(sock);
        WSACleanup();
    }

    // Вызывается если возникла ошибка 
    // Выводит сообщение на экран, записывает номер ошибки 
    // и очищает структуру WSA 
    void ExceptError(const char* message)
    {
        printf_s(message);
        Error = WSAGetLastError();
        WSACleanup();
    }
};


// Функция выполняющаяся в потоке 
DWORD WINAPI ClientThread(LPVOID param)
{
    //clients.RemoveClient(client); 

    Client* client = (Client*)param;

    // Спрашиваем имя пользователя 
    int retVal = send(client->Sock, MESSAGE_NAME, strlen(MESSAGE_NAME) + 1, 0);
    if (retVal == INVALID_SOCKET)
    {
        closesocket(client->Sock);
        ExitThread(1);
    }

    retVal = recv(client->Sock, client->Name, MAX_USERNAME_LEN, 0);
    if (retVal == SOCKET_ERROR)
    {
        closesocket(client->Sock);
        ExitThread(2);
    }

    // Блокируем мьютекс для обработки массива клиентов 
    WaitForSingleObject(ghMutex, INFINITE);

    // Если достигнут предел клиентов 
    if (!clients.AddClient(client))
    {
        send(client->Sock, MESSAGE_FULL, strlen(MESSAGE_FULL) + 1, 0);
        closesocket(client->Sock);
        printf_s(ERR_MAX_CLIENTS);
        ReleaseMutex(ghMutex);
        ExitThread(3);
    }

    // Сообщаем о новом соединении в консоль сервера 
    printf_s(INFO_ACCEPT_CONN, client->GetIP(), client->GetPort());
    printf_s(INFO_COUNT_CONN, clients.curCount);

    char buf[100]{};
    sprintf_s<100>(buf, CLIENT_NEW, client->Name, client->GetIP());
    int bufLen = strlen(buf) + 1;

    printf_s(buf);

    // Сообщаем о новом клиенте прочим клиентам 
    for (int i = 0; i < clients.curCount; i++)
        if (clients.data[i] != client)
            send(clients.data[i]->Sock, buf, bufLen, 0);

    // Разблокируем мьютекс 
        ReleaseMutex(ghMutex);

    // Теперь начинаем обрабатывать клиента 
    while (true)
    {
        // Получаем сообщение 
        char message[MAX_MESSAGE_LEN]{};
        retVal = recv(client->Sock, message, MAX_MESSAGE_LEN, 0);
        if ((retVal == SOCKET_ERROR) || (retVal == 0))
            break;

        // Если это сообщение о выходе, то отключаем клиента 
        if (!strcmp("exit\n", message))
            break;

        // Отправляем всем 
        WaitForSingleObject(ghMutex, INFINITE);
        char Mes[MAX_MESSAGE_LEN]{};
        sprintf_s(Mes, MAX_MESSAGE_LEN, "%s: %s", client->Name, message);
        printf_s(Mes); // На сервер 
        for (int i = 0; i < clients.curCount; i++) // Клиентам 
            if (clients.data[i] != client)
            {
                send(clients.data[i]->Sock, Mes, strlen(Mes) + 1, 0);
            }
        ReleaseMutex(ghMutex);
    }

    // Завершаем работу 
    WaitForSingleObject(ghMutex, INFINITE);

    // Удаляем клиента 
    clients.RemoveClient(client);
    printf_s(INFO_CLOSE_CONN, client->IP, client->GetPort());
    printf_s(INFO_COUNT_CONN, clients.curCount);

    // Сообщаем всем что клиент покинул чат 
    sprintf_s<100>(buf, CLIENT_DISCONNECT, client->Name, client->GetIP());
    bufLen = strlen(buf) + 1;

    printf_s(buf);

    for (int i = 0; i < clients.curCount; i++)
        if (clients.data[i] != client)
            send(clients.data[i]->Sock, buf, bufLen, 0);

    ReleaseMutex(ghMutex);
    ExitThread(0);

};

int main()
{

}
