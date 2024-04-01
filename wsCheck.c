#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <unistd.h>

int main(){
    for(int i=0;i<100;i++){
        printf("Hello World\n");
        Sleep(100);
    }
    return 0;
}