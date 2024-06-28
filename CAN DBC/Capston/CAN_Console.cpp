#include "stdafx.h"
#include "CAN_DLL/CAN_Access.h"
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

#define MAX_FRAMES 100
typedef struct msg_frame 
{
    long _rid;
    int _rlen;
    char _rdata[8] = { 0, };
} msgFrame;
msgFrame* frames[MAX_FRAMES] = { nullptr, };
std::mutex frames_mutex;
std::mutex CAN_mutex;

std::vector<std::string> can_send_parts;
std::string can_send_part;
void SendCanMessage(CAN_HANDLE h, const char *inputdata) 
{
    can_send_parts.clear();
    std::istringstream ss(inputdata);
    while (getline(ss, can_send_part, ' ')) 
    {
        can_send_parts.push_back(can_send_part);
    }
    if (can_send_parts.size() != 10) 
    {
        printf("%d\n", can_send_parts.size());
        printf("Error: Invalid input format. Expected 9 parts.\n");
        return ;
    }
    long sid = std::strtol(can_send_parts[0].c_str(), nullptr, 16);
    unsigned char sdata[8];
    for (size_t i = 0; i < 8; ++i) 
    {
        sdata[i] = static_cast<char>(std::strtol(can_send_parts[i + 2].c_str(), nullptr, 16));
    }
    //std::lock_guard<std::mutex> lock(CAN_mutex);
    int ret1 = CAN_Send(h, sid, 8, (char*)sdata, 0, 0);
    if (!ret1) 
    {
        printf("Send failed.\n");
    }
    else 
    {
        printf("CAN SEND: %03X 8 %02X %02X %02X %02X %02X %02X %02X %02X\n", sid, 
            static_cast<unsigned char>(sdata[0]),
            static_cast<unsigned char>(sdata[1]),
            static_cast<unsigned char>(sdata[2]),
            static_cast<unsigned char>(sdata[3]),
            static_cast<unsigned char>(sdata[4]),
            static_cast<unsigned char>(sdata[5]),
            static_cast<unsigned char>(sdata[6]),
            static_cast<unsigned char>(sdata[7]));
    }
}

void RecvCanMessage(CAN_HANDLE h)
{
    char rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    long rid;
    int rlen, ext, rtr;
    int ret = CAN_Recv(h, &rid, &rlen, rdata, &ext, &rtr);   
    if (ret) 
    {     
        std::lock_guard<std::mutex> lock(frames_mutex);
        int idx_frame=0;
        for (msgFrame* frame : frames) 
        {
            if (frame == nullptr) 
            {
                break;
            }
            if (frame->_rid == rid) 
            {
                for (int i = 0; i < 8; i++) 
                {
                    frame->_rdata[i] = rdata[i];
                }
                return;
            }
            idx_frame ++;
        }
        frames[idx_frame] = new msgFrame();
        frames[idx_frame]->_rid = rid;
        frames[idx_frame]->_rlen = rlen;
        for (int i = 0; i < 8; i++) 
        {
            frames[idx_frame]->_rdata[i] = rdata[i];
        } 
    }
    else { printf("CAN READ FAIL\n"); }
}
void threadReadCAN(CAN_HANDLE h) 
{
    while (1) {
        if (CAN_CountRxQueue(h) > 0) {
            RecvCanMessage(h);
        }
        else { printf("Read Fail\n"); }
    }
    // 정리
    CAN_Close(h);
}
#define PACKET_SIZE 30
void threadReadSock(CAN_HANDLE h, SOCKET sock)  
{
    char buffer[PACKET_SIZE] = {};
    while (!WSAGetLastError()) 
    {
        ZeroMemory(&buffer, PACKET_SIZE); 
        int bytesRead = recv(sock, buffer, PACKET_SIZE-1, 0); 
        if (bytesRead > 0) 
        {
            buffer[PACKET_SIZE-1] = '\0';
            printf("Socket Recv: %s\t", buffer);
            SendCanMessage(h, buffer);
        }
    }
}
void threadSendSock(SOCKET sock) 
{
    while (1) 
    {
        std::vector<msgFrame> local_frames;
        {
            std::lock_guard<std::mutex> lock(frames_mutex);
            for (msgFrame* frame : frames) 
            {
                if (frame == nullptr) 
                {
                    break;
                }
                local_frames.push_back(*frame);
            }
        }
        //puts("==================================================");
        for (const msgFrame& frame : local_frames) 
        {
            // 수신된 메시지 포맷 지정
            char recv_message[100];
            sprintf(recv_message,
                "%03X %d %02X %02X %02X %02X %02X %02X %02X %02X",
                frame._rid, frame._rlen,
                (int)(unsigned char)frame._rdata[0],
                (int)(unsigned char)frame._rdata[1],
                (int)(unsigned char)frame._rdata[2],
                (int)(unsigned char)frame._rdata[3],
                (int)(unsigned char)frame._rdata[4],
                (int)(unsigned char)frame._rdata[5],
                (int)(unsigned char)frame._rdata[6],
                (int)(unsigned char)frame._rdata[7]
            );
            printf("%s\n", recv_message);
            if (send(sock, recv_message, strlen(recv_message), 0) < 0) 
            {
                printf("Send to server failed: %d\n", WSAGetLastError());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}
int _tmain(int argc, _TCHAR* argv[]) 
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        printf("Failed to initialize Winsock.\n");
        return -1;
    }

    // 소켓 생성 및 서버 연결
    SOCKET sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) 
    {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return -1;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    
    while (1) 
    {
        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) >= 0) 
        {
            printf("Connect Success\n");
            break;
        }
        else 
        {
            printf("Connect error : %d\n", WSAGetLastError());
            Sleep(1000);
        }
    }
    // CAN 핸들 설정
    int noDevice = CAN_Fifo_ScanSerialNumber();
    if (noDevice <= 0) 
    {
        printf("Can't find CAN Module\n");
        exit(0);
    }

    CAN_HANDLE h = CAN_OpenFifo(CAN_Fifo_GetSerialNumber(0));
    if (h < 0) 
    {
        printf("USB2CAN open failed.\n");
        exit(0);
    }
    int speed= 250;
    HANDLE hEvent = CreateEvent(NULL, false, false, "");
    CAN_SetRxEventNotification(h, hEvent);
    CAN_SetConfig(h, 250, 0x01, 0x01,1,NULL);
    CAN_SetTransferMode(h, 1);
    std::thread read_CAN(threadReadCAN,h);
    std::thread send_sock(threadSendSock, sock);
    std::thread read_sock(threadReadSock,h, sock);
    read_CAN.join();
    send_sock.join();
    CloseHandle(hEvent);
    closesocket(sock);
    WSACleanup();
    getchar();

    return 0;
}