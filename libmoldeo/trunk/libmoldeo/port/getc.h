/* getch.h */

#include <stdio.h>

#ifndef _GETCH_H_
#define _GETCH_H_

inline int getch(void)
{
    int ch, charactr;
    while((ch=getchar()) != '\n') { charactr = ch; }
}

/*


#define BUFSZ 100
static int bufp = 0;
char buf[BUFSZ];

int getch(void)
{
	return((bufp > 0 ? buf[--bufp] : getchar()) );
}

void ungetch(int c) 
{
	if(bufp >= BUFSZ) {
		printf("ungetch: Buffer is full !!\n");
	} else {
		buf[bufp++] = c;
	}

	return;
} 
*/

#endif /* _GETCH_H_ */

