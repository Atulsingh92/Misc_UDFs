#include "udf.h"

//const char FILENAME[] = "pressure_out.txt";
//FILE *fp;

DEFINE_PROFILE(initialize_p, t, i)
{
	int f_id = 5;
	cell_t c;
	face_t f;
	//Thread *t;
	Domain *d;

	real x[ND_ND];
	//real pos[ND_ND];
	//Domain *domain;
	d = Get_Domain(1);

	//t = Lookup_Thread(d, f_id);

	begin_f_loop(f,t)
  	{
    	//F_CENTROID(x,f,t);  /* Get the global coordinates of the face */
    /* Initialize pressure at the assigned surface. */
    	F_PROFILE(f,t,i) = 100.;//*x[0]; //F_PROFILE sets bc value.
  	}
  	end_f_loop(f,t)
}
