#include "stdio.h"

__attribute__((constructor)) void  main()
{
	char  byte;
	unsigned int a, b, status, i;
	i=0;
	while(1) 
        {	
		i++;
		a= rand(); b=rand();
		tty_printf("start at :%d \n", proctime());
		gcd_set_opa(a); gcd_set_opb(b);
		gcd_start();

		while(gcd_get_status(& status)){
			tty_puts("\n computing in progress \n");
		}

		gcd_get_result(&a);
		tty_printf("result is : %d \n", a);
		tty_printf("loop number : %d \n", i);
	        tty_getc(&byte);

		tty_puts("\n \n");
	}

} // end main
