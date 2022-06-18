#include <communication.h>
#include <utils.h>

Communication::Communication() {
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);

    serSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);

    bind(serSocket, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
    listen(serSocket, 20);

    SOCKADDR clientAddr;
    int clientAddr_size = sizeof(clientAddr);
    clientSocket = accept(serSocket, (SOCKADDR*)&clientAddr, &clientAddr_size);

    receive_buffer = new char [BUF_SIZE];
}

int Communication::send_int(int message) const {
    char tmp[10];
    int_to_char(tmp, message);
    return send(clientSocket, tmp, strlen(tmp), 0);
}

int Communication::send_char(char *message) const {
    return send(clientSocket, message, strlen(message), 0);
}

int Communication::send_img(cv::Mat img, int length) const{
    return send(clientSocket, (char*)img.data, length, 0);
}

int Communication::receive() const{
    memset(receive_buffer, '\0', sizeof (char) * BUF_SIZE);
    return recv(clientSocket, receive_buffer, BUF_SIZE, 0);
}

int Communication::close() const{
    closesocket(clientSocket);
    closesocket(serSocket);
    WSACleanup();
    return 0;
}
