/*  Using C to control the instrument       */
/*  Ni Visa components are used             */
/*  Last maintainance:  Rhete 2024-03-26    */
/*                                          */
/*                                          */
/*                                          */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "visa.h" // importance headlers
/* for a successful compile, Library Directories and Additional Dependencies are necessary. */
/* -I headlers      */
/* -L Library       */
/* -l dependencies  */

void ErrCodePrt(ViStatus status){
    switch (status)
    {
    case VI_ERROR_NSUP_MODE: 
        printf("VISA工具不支持给定的模式！\n");
        break;
    
    case VI_ERROR_NSUP_INTR:
        printf("中断要求等级过高或不满足要求的状态ID值！\n");
        break;

    case VI_ERROR_INV_MODE:
        printf("mode参数值无效！\n");
        break;

    case VI_ERROR_INTR_PENDING:
        printf("上次请求的中断仍处于待定状态!\n");
        break;

    case VI_ERROR_BERR:
        printf("传输过程中总线错误！\n");
        break;

    case VI_ERROR_RSRC_LOCKED:
        printf("特定的操作由于目标资源禁止访问而无法执行！\n");
        break;
        
    case VI_ERROR_NSUP_OPER:
        printf("目标资源不支持此操作！\n");
        break;

    case VI_ERROR_INV_OBJECT:
        printf("目标无效！\n");
        break;

    default :
        printf("未知的错误类型\n");
        break;
    }
    system("pause");
    return ;
}



int main()
{
    static ViStatus status;
    static ViSession defaultRM, VNA;
    
    SetConsoleOutputCP( CP_UTF8 );

    // open default resource maneger
    status = viOpenDefaultRM (&defaultRM);
    if (status < VI_SUCCESS)
    {
        printf("Could not open a session to the VISA Resource Manager!\n");
        ErrCodePrt(status);
        exit (EXIT_FAILURE);
    }  

    //Open a vi Session
    ViConstRsrc VNA_Rsrc = "TCPIP0::K-E5080B-01675::inst0::INSTR";
    status = viOpen(defaultRM, VNA_Rsrc, VI_NULL, 20, &VNA);
    if(status < VI_SUCCESS)
    {
        printf("Could not start a session to %s!\n", VNA_Rsrc);
        ErrCodePrt(status);
        exit (EXIT_FAILURE);
    }

    
    printf("Exiting\n");
    system("pause");
    return 0;
}