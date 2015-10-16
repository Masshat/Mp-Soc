#include "stdio.h"

__attribute__((constructor)) void main()
{
    char byte;
    tty_printf("enter a command: a(enable), d(disable), q(quit) \n");     
    tty_getc_irq(&byte);
    while (1) {
         switch(byte){
    case 'a':
    timer_set_period(500000);
    timer_set_mode(0x3);
    break;
    case 'd':
    //timer_reset_irq();
    timer_set_mode(0x0);
    break;
    case 'q' :
    exit();
    break;
    default:
    tty_printf("bad command line, choose between a, q, or q! \n");
    break;

        }

    tty_getc_irq(&byte);
    }
    
} // end main

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4
