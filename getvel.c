//********************
// Prints 12.x for pressure at outlet , correct value is around 8.
// *******************
#include "udf.h"
#include<math.h>

  FILE *fout;
  Domain *domain;

 void print_velocity_mdot(Domain *domain, int id)
 {
    cell_t c;
    Thread *t;
    face_t f;
    //mdot
    real flow=0.0;
    // stat p
    real p=0.0;
    real area[ND_ND];
    real total_area = 0.0;
    real resultp = 0.0;
    real Amag;
    real pglob = 0.0;
    real areaglob = 0.0;
    real total_p;

    //dyn p
    real vsquared, d;
    real velocity_squared = 0.0;
    real density = 0.0;
    real velocity_squared_glob = 0.0;
    real density_glob = 0.0;
    real p1 ;

    t = Lookup_Thread(domain, id);

    /*begin_c_loop(c,t)
    {
      vsquared = pow(C_U(c,t), 2)+ pow(C_V(c,t),2) + pow(C_W(c,t),2);
      velocity_squared += v;
      d = C_R(c,t);
      density += d;
    }
    end_c_loop(c,t)*/

    begin_f_loop(f,t)
    {
      flow += F_FLUX(f,t);
      //p += F_P(c,t) ; //write area avg pressure. / obtained = area avg total pressure./ doesn't print always
      F_AREA(area,f,t);
      Amag = NV_MAG(area);
      total_area += Amag;
      p = C_P(f,t); //F_P(f,t) ;
      total_p += p * total_area; //NV_MAG(area);

      vsquared = pow(C_U(f,t), 2)+ pow(C_V(f,t),2) + pow(C_W(f,t),2);
      velocity_squared += vsquared * total_area;
      d = C_R(f,t);
      density += d * total_area;
    
    }
    end_f_loop(f,t)

    pglob = PRF_GRSUM1(total_p);
    areaglob = PRF_GRSUM1(total_area);

    velocity_squared_glob = PRF_GRSUM1(velocity_squared);
    density_glob = PRF_GRSUM1(density);
    
    p1 = (pglob/areaglob) + ((0.5 * density_glob * velocity_squared_glob)/areaglob) ;
      
    fprintf(fout, "for zoneid = %d \n", id);
    fprintf(fout, "mass Flow Rate = %g \n", flow);
    fprintf(fout, "total pressure is : %g \n", p1);
   }

 DEFINE_ON_DEMAND(get_pressure)
 {
    Domain *domain;
    domain = Get_Domain(1); //existing fluid domain
    fout = fopen("outputs.txt", "w");
    print_velocity_mdot(domain, 6);
    fclose(fout);
 } 