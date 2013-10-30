#include <freevic.h>

#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 30
#endif


void freevic_hexdump(void *mem, unsigned int len)
{
        unsigned int i, j;
        
        for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
        {
                /* print offset */
                if(i % HEXDUMP_COLS == 0)
                {
                        debug_printf("0x%06x: ", i);
                }
 
                /* print hex data */
                if(i < len)
                {
                        debug_printf("%02x ", 0xFF & ((char*)mem)[i]);
                }
                else /* end of block, just aligning for ASCII dump */
                {
                        debug_printf("   ");
                }
                
                /* print ASCII dump */
                if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
                {
                        for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                        {
                                if(j >= len) /* end of block, not really printing */
                                {
                                        putchar(' ');
                                }
                                else if(isprint(((char*)mem)[j])) /* printable char */
                                {
                                        debug_printf("%c", 0xFF & ((char*)mem)[j]);        
                                }
                                else /* other char */
                                {
                                        debug_printf(".");
                                }
                        }
                        putchar('\n');
                }
        }
}