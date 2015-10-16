#include "stdio.h"

// Nombre de pixels par ligne
#define NB_PIXELS 128

// Nombre de lignes par image
#define NB_LINES  128

// taille du block
#define BLOCK_SIZE 512

__attribute__ ((constructor)) void main()
{
    char     buf[NB_LINES][NB_PIXELS];
    char     byte;
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
