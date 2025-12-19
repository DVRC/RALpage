/**/

#include "main.h"

typedef struct vector {
     float vx, vy, vt } Vector;

extern Vector NewVector ();
Vector foo ();
static Vector bar();

main() {

     Vector v1;

     v1 = foo ();
     printf("[%f, %f, %f]\n", v1.vx, v1.vy, v1.vt);

     v1 = bar ();
     printf("[%f, %f, %f]\n", v1.vx, v1.vy, v1.vt);
     
}

Vector foo () {
     Vector v;
     v = NewVector (14.0, 27.0, 2.0);
     printf("foo: [%f, %f, %f] ", v.vx, v.vy, v.vt);
     return v;
}

static Vector bar () {
     Vector v;
     if (1 == 1) {
	  v = NewVector (14.0, 0.0, 0.0);
	  printf("bar: [%f, %f, %f] ", v.vx, v.vy, v.vt);
     }
     printf("[%f, %f, %f]\n\t ", v.vx, v.vy, v.vt);
     return v;
}
