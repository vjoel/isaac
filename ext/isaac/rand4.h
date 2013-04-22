/*
------------------------------------------------------------------------------
rand4.h: rand.h specialized to RANDSIZL=4
------------------------------------------------------------------------------
*/

#include <stdint.h>

#include "rand.h"

#ifndef RAND4
#define RAND4
#define RANDSIZ4    (1<<4)

/* context of random number generator */
struct randctx4
{
  ub4 randcnt;
  ub4 randrsl[RANDSIZ4];
  ub4 randmem[RANDSIZ4];
  ub4 randa;
  ub4 randb;
  ub4 randc;
};
typedef  struct randctx4  randctx4;

/*
------------------------------------------------------------------------------
 If (flag==TRUE), then use the contents of randrsl[0..RANDSIZ4-1] as the seed.
------------------------------------------------------------------------------
*/
void isaac_init4(randctx4 *r, int flag);

void isaac_rand4(randctx4 *r);


/*
------------------------------------------------------------------------------
 Call rand4(/o_ randctx4 *r _o/) to retrieve a single 32-bit random value
------------------------------------------------------------------------------
*/
#define rand4(r) \
   (!(r)->randcnt-- ? \
     (isaac_rand4(r), (r)->randcnt=RANDSIZ4-1, (r)->randrsl[(r)->randcnt]) : \
     (r)->randrsl[(r)->randcnt])

#endif  /* RAND4 */


