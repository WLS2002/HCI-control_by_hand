#include <winsock2.h>
#include <utils.h>

struct communication{

    SOCKET serSocket;
    SOCKET clientSocket;
    const int BUF_SIZE = 6000;
    char* receive_buffer;

    communication(){
        WSADATA wsadata;
        WSAStartup(MAKEWORD(2, 2), &wsadata);

        serSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        sockaddr_in sockAddr;
        memset(&sockAddr, 0, sizeof(sockAddr));  //用0填充每个字节
        sockAddr.sin_family = PF_INET;     //使用IPv4地址
        sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //具体的ip地址
        sockAddr.sin_port = htons(1234);   //具体的端口号

        //绑定套接字
        bind(serSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
        listen(serSocket, 20);

        //接受客户端请求
        SOCKADDR clientAddr;
        int clientAddr_size = sizeof(clientAddr);
        clientSocket = accept(serSocket, (SOCKADDR*)&clientAddr, &clientAddr_size);

        receive_buffer = new char [BUF_SIZE];
    }

    int send_int(int message){
        char tmp[10];
        int_to_char(tmp, message);
        return send(clientSocket, tmp, strlen(tmp), 0);
    }

    int send_char(char* message){
        return send(clientSocket, message, strlen(message), 0);
    }

    int send_img(cv::Mat img, int length){
        return send(clientSocket, (char*)img.data, length, 0);
    }

    int receive(){
        memset(receive_buffer, '\0', sizeof (char) * BUF_SIZE);
        return recv(clientSocket, receive_buffer, BUF_SIZE, 0);
    }

    int close(){
        closesocket(clientSocket);
        closesocket(serSocket);
        WSACleanup();
        return 0;
    }
};

