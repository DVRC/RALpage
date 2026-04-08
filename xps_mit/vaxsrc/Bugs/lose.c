#include <stdio.h>

main()
{
     char *s1, *p1;
     unsigned char *s2, *p2;

     s1 = (char *) malloc(20 * sizeof(char));
     s2 = (unsigned char *)malloc(20*sizeof(unsigned char));

     p1 = s1;
     p2 = s2;

     *p1++ = (char) 193;
     *p1 = '\0';

     *p2++ = (unsigned char) 193;
     *p2 = '\0';

     printf("%s %d\n", s1, *s1);
     printf("%s %d\n", s2, *s2);

}
     
