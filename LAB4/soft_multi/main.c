
#include "stdio.h"

// Nombre de pixels par ligne
#define NB_PIXELS 128

// Nombre de lignes par image
#define NB_LINES  128

// taille du block
#define BLOCK_SIZE 512

#define TRUE	1
#define FALSE	0
#define MAX	1000

__attribute__((constructor)) void display()
{
    char     byte;
    char     buf[NB_LINES][NB_PIXELS];
    int      base = 0;
    int      image = 0;
    int      npixels;      // number of pixels per processor      
    int      nblocks;       // number of blocks per processor



    npixels = NB_PIXELS*NB_LINES;
    nblocks = npixels/BLOCK_SIZE;

    // main loop
    while(1)
    {
        tty_printf("\n  *** image %d au cycle : %d *** \n", image, proctime());

        /* Phase 1 : lecture image sur le disque et transfert vers buf */
        if (ioc_read(base + nblocks, buf , nblocks))
        {
            tty_printf("\n!!! echec ioc_read au cycle : %d !!!\n", proctime()); 
            exit();
        }
        if ( ioc_completed() )
        {
            tty_printf("\n!!! echec ioc_completed au cycle : %d !!!\n", proctime());
            exit();
        }
        tty_printf("- image chargee au cycle = %d \n",proctime());

	tty_printf("test\n");
        /* Phase 3 : transfert de buf vers le frame buffer */
        if (fb_write(0,  buf, sizeof(buf)))
        { 
            tty_printf("\n!!! echec fb_write au cycle : %d !!!\n", proctime()); 
            exit();
        }
        fb_completed();
        tty_printf("- image affichee au cycle = %d \n",proctime());

        base  = base + nblocks;
        image = image + 1;

        tty_getc_irq(&byte);
	if (base == 640) base =0;

    } // end while

    exit();
} // end main
__attribute__((constructor)) void timer()
{
    char byte;
    tty_printf("enter a command: a(enable), d(disable), q(quit) \n");     
    tty_getc_irq(&byte);
    while (1) {
    switch(byte){
    case 'a':
    timer_set_period(50000);
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


__attribute__((constructor)) void prime()
{
    unsigned    prime[MAX];
    unsigned    tested_value = 2;
    unsigned    next_empty_slot = 0;
    unsigned    is_prime;
    unsigned    i;

    tty_printf("*** Starting Prime Computation ***\n\n");

    while (1) 
    {
        is_prime = TRUE;
        for( i=0 ; i<next_empty_slot ; i++ ) 
        { 
            if( tested_value%prime[i] == 0 ) is_prime = FALSE; 
        }
        if ( is_prime ) 
        {
            prime[next_empty_slot] = tested_value;
            tty_printf("prime[%d] = %d\n", next_empty_slot, tested_value);
            next_empty_slot++;
            if( next_empty_slot == MAX) 
            {
                tty_printf("prime table full\n");
                exit();
            }
        }
        tested_value++;
    }
} // end main

__attribute__((constructor)) void pgcd()
{	
    unsigned int opx;
    unsigned int opy;

    tty_printf(" Interactive PGCD \n");

    while (1)
    {
        tty_printf("\n*******************\n");
        tty_printf("operand X = ");
        tty_getw_irq(&opx);
        tty_printf("\n");
        tty_printf("operand Y = ");
        tty_getw_irq(&opy);
        tty_printf("\n");
        if( (opx == 0) || (opy == 0) )
        {
            tty_printf("operands must be larger than 0\n");
        }
        else
        {
            while (opx != opy)
            {
                if(opx > opy)   opx = opx - opy;
                else            opy = opy - opx;
            }
            tty_printf("pgcd      = %d\n", opx);
        }
    }
} // end main
