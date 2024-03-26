/*  Using C to control the instrument       */
/*  Ni Visa components are used             */
/*  Last maintainance:  Rhete 2024-03-26    */
/*                                          */
/*                                          */
/*                                          */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <visa.h> // importance headlers
/* for a successful compile, Library Directories and Additional Dependencies are necessary. */
/* -I headlers      */
/* -L Library       */
/* -l dependencies  */

static ViStatus status;
static ViSession defaultRM, instr1;





int main()
{
    // open default resource maneger
    status = viOpenDefaultRM (&defaultRM);
    if (status < VI_SUCCESS)
    {
        printf("Could not open a session to the VISA Resource Manager!\n");
        exit (EXIT_FAILURE);
    }  


    
    return 0;
}