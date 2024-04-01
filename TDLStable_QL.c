/*  TDL Stable C version                    */
/*  Ni Visa components are used             */
/*  Last maintainance:  Rhete 2024-03-28    */


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <winsock2.h> 
#include <mswsock.h>
#include <time.h>
#include <windows.h>
#include <unistd.h>   
#include <ws2tcpip.h>

#include "visa.h"

void viWhichStatus(ViStatus status);
ssize_t soWrite(SOCKET sock, const void * data, size_t size);
ssize_t soRead(SOCKET sock, void *buffer, size_t size);
int genSocketfp(SOCKET* sock, struct timeval timeout_read, struct timeval timeout_write, ssize_t inBufSize, ssize_t OutBufSize);
void setConInfo(struct sockaddr_in *pSAddr, const char * IpStr, unsigned short int Num_host);
int initMDL(SOCKET sock, const char * unit, const int spd);
int setDelay(SOCKET sock, const float delay);

int main(){
    // declaire variables
    ViStatus status; //用于接受操作返回值来判定操作是否完成
    ViSession defRM, kVna; // 会话对象, defRM: 默认资源管理器, kVNA: 矢网
    const char kVnaRsrc[] = "TCPIP0::k-E5080B-01675::inst0::INSTR";// 唯一资源标识符, kVNARsrc: 矢网
    ViPBuf inBuff, ouBuff;

    WSADATA wsaData;
    SOCKET gpTdl;
    struct sockaddr_in gpTdlAddr;
    struct timeval timeout;

    timeout.tv_sec = 60;


    // 设置控制台输出为UTF-8格式
    SetConsoleOutputCP( CP_UTF8 );

    // 打开VISA资源管理器
    printf("正在打开默认资源管理器......\n");
    viWhichStatus(viOpenDefaultRM(&defRM));

    // 初始化Win SOCKET API
    int result;
    result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(0 != result){
        printf("初始化win socket api失败，失败代码： %d\n",result);
        return 1;
    }

    // 创建SOCKET
    gpTdl = socket(AF_INET, SOCK_STREAM, 0);
    if(gpTdl == INVALID_SOCKET){
        printf("创建SOCKET失败，失败代码：%d\n", WSAGetLastError);
        return 1;
    }
    // setsockopt(gpTdl, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));


    // 设置时延线信息
    setConInfo(&gpTdlAddr, "192.168.6.2", 8886);

    // 与时延线建立连接
    result = connect(gpTdl, (struct sockaddr*) &gpTdlAddr, sizeof(gpTdlAddr));
    if (result == SOCKET_ERROR){
        printf("连接到SOCKET失败，失败代码：%d\n", WSAGetLastError());
        return 1;
    }

    // 初始化时延线
    initMDL(gpTdl, "ps", 4);

    // 设置时延为130ps
    setDelay(gpTdl, 130.0);


    // 创建与VNA的会话, 
    printf("正在创建与矢网的会话......\n");
    viWhichStatus(viOpen(defRM, kVnaRsrc, VI_NULL, 100000, &kVna));

    // 设置缓存区大小
    viWhichStatus(viSetBuf(kVna, VI_READ_BUF, 1000000*sizeof(double)));
    viWhichStatus(viSetBuf(kVna, VI_WRITE_BUF, 1000*sizeof(char)));

    return 0;
}

void viWhichStatus(ViStatus status){
    if(status >= VI_SUCCESS) // 成功完成码
    {
        printf("    成功!\n");
        switch(status){
            case VI_SUCCESS:

                break;
            default :

                break;
        }
    }
    else 
    {
        printf("    失败\n");
        switch(status){
            default :

                break;
        }
        exit(EXIT_FAILURE);
    }
    return ;
}


// int genSocketfp(SOCKET* sock, struct timeval timeout_read, struct timeval timeout_write, ssize_t inBufSize, ssize_t OutBufSize){
//     *sock = socket(AF_INET, SOCK_STREAM, 0);
//     if(*sock == INVALID_SOCKET){
//         printf("创建SOCKET失败，失败代码：%d\n", WSAGetLastError);
//         return 1;
//     }
    
//     setsockopt(*sock, SOL_SOCKET, SO_RCVTIMEO, &timeout_read, sizeof(timeout_read));
//     setsockopt(*sock, SOL_SOCKET, SO_SNDTIMEO, &timeout_write, sizeof(timeout_write));
//     setsockopt(*sock, SOL_SOCKET, SO_RCVBUF, &inBufSize, sizeof(inBufSize));
//     setsockopt(*sock, SOL_SOCKET, SO_SNDBUF, &OutBufSize, sizeof(OutBufSize));
    
//     return 0;
// }

ssize_t soWrite(SOCKET sock, const void * data, size_t size){
    
    int result;
    size_t sent = 0;
    const char* ptr = (const char *) data;
    while (sent<size){
        int result = send(sock, ptr + sent, (int)(size - sent), 0);
        if(-1 == result){
            printf("发送错误：%d\n", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        sent += result;
    }
    return sent;
}

ssize_t soRead(SOCKET sock, void *buffer, size_t size){
    
    int result;
    ssize_t total_received = 0;
    char *ptr = (char *)buffer;

    while (total_received < size){
        int result = recv(sock, ptr + total_received, (int)(size - total_received), 0);
        if(-1 == result){
            printf("recv failed:%d\n", WSAGetLastError);
            exit(EXIT_FAILURE);
        }
        total_received += result;
    }
    return total_received;
}

void setConInfo(struct sockaddr_in *pSAddr, const char * IpStr, unsigned short int Num_host){
    
    int result;
    // 初始化server_addr变量，用memset将server_addr结构体所指向的内存值置为0
    memset(pSAddr, 0, sizeof(*pSAddr)); 
    // 设置socket类
    pSAddr->sin_family = AF_INET;               
    // 设置端口，htons: 将无符号短整型数从主机字节顺序转为网络字节顺序
    pSAddr->sin_port = htons(8886);             
    // 用inet_pton设置server_addr.sin_addr值
    result = inet_pton(AF_INET, IpStr, &(pSAddr->sin_addr));
    
    /* inet_pton: 将用点分或者冒号分隔的地址转为网络字节序的二进制形式 */
    if (result <= 0){
        printf("非法IP地址，错误代码：%d\n", WSAGetLastError());
        return ;
    } 
}

int initMDL(SOCKET sock, const char * unit, const int spd){
    char buf[4]={0};
    buf[3] = '\0';
    // 将时延线归零
    printf("正在归零时延线...\n");
    soWrite(sock, "_ORG_$", 6);
    soRead(sock, &buf, 2);
    if(strcmp(buf, "NO")==0)
        printf("%s\n归零失败\n", buf);
    if(strcmp(buf, "OK")==0)
        printf("%s\n归零成功\n",buf);

    // 设置时延线数据单位 mm or ps
    printf("正在设置时延线单位...\n");
    if(strcmp(unit, "ps")==0)
        soWrite(sock, "_PSU_$", 6);
    else if(strcmp(unit, "mm")==0)
        soWrite(sock, "_MMU_$", 6);
    else
        printf("未知单位，请重试\n");

    soRead(sock, &buf, 2);
    if(strcmp(buf, "NO")==0)
        printf("%s\n设置单位失败\n", buf);
    if(strcmp(buf, "OK")==0)
        printf("%s\n设置成功\n", buf);

    // 设置时延线速度
    printf("正在设置时延线速度...\n");
    char tmp[7];
    sprintf(tmp, "_SPD_%d$", spd);
    soWrite(sock, tmp, 7);
    soRead(sock, &buf, 2);
    if(strcmp(buf, "NO")==0)
        printf("%s\n设置速度失败\n", buf);
    if(strcmp(buf, "OK")==0)
        printf("%s\n设置成功\n", buf);

    return 0;
}

int setDelay(SOCKET sock, const float delay){
    char * tmp;
    char buf[4];
    tmp = (char *) malloc(sizeof(char)*20);

    printf("正在将时延设置成%f...\n", delay);
    sprintf(tmp, "_ABS_%6.3f$", delay);
    soWrite(sock, tmp, strlen(tmp));
    soRead(sock, &buf, 2);
    if(strcmp(buf, "NO")==0)
        printf("%s\n设置时延失败\n", buf);
    if(strcmp(buf, "OK")==0)
        printf("%s\n设置成功\n", buf);
    return 0;
}