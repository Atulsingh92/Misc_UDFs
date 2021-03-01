#include "udf.h"
#include <stdio.h>
#include<math.h>

FILE *fout;
Domain *domain;
real p, uc, vc, wc, temp, stat_p, dens, vmag, dyn_p, total_p, Amag, p_averaged;
real total_area=0.0, p_sum=0.0, total_p=0.0;  
real area[ND_ND];

void Print_dynPressure(Domain *domain, int id)
 {
 	cell_t c;
    Thread *t;
    face_t f;
    t = Lookup_Thread(domain, id);

    begin_f_loop(f,t)
    {
        uc = F_U(f,t); 
        vc = F_V(f,t); 
        wc = F_W(f,t); 
        temp = F_T(f,t); 
        stat_p = F_P(f,t); 
        dens = F_R(f,t);

        vmag = sqrt(pow(uc,2) + pow(vc,2) + pow(wc,2));
        dyn_p = 0.5*dens*pow(vmag,2);
        total_p = stat_p + dyn_p;

        F_AREA(area,f,t);
        Amag = NV_MAG(area);
        total_area += Amag;

        p_sum += total_p * Amag;

        Message("x-velocity: %g\n",uc);
        Message("y-velocity: %g\n",vc);
        Message("vmag is: %g\n",vmag); 
        Message("dyna is: %g\n",dyn_p);
        Message("press is: %g\n",stat_p);
        Message("total_p is: %g\n",total_p);
        Message("area total: %g\n", total_area);
        Message("current area: %g\n", Amag);
        Message("p*A = %g\n", p_sum);
        Message("\n");
    }
    end_f_loop(f,t)
 }

 DEFINE_ON_DEMAND(get_dyn_p)
 {
    Domain *domain;
    domain = Get_Domain(1);
    Print_dynPressure(domain, 6);
    p_averaged = p_sum / total_area;
    Message("******************************");
    Message("P_area_averaged = %g\n", p_averaged);
 }
