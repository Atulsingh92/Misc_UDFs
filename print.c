/*****************************************************************
    Example of UDF for single phase that uses Get_Domain utility
 ******************************************************************/
 
 #include "udf.h"
  FILE *fout;
  Domain *domain;
  real flow=0.;
  real p=0.;

 void Print_Thread_Face_centroids(Domain *domain, int id)
 {
    real FC[3];
    face_t f;
    Thread *t = Lookup_Thread(domain, id);
    fprintf(fout,"thread id %d\n", id);
    begin_f_loop(f,t)
      {
      F_CENTROID(FC,f,t);
      fprintf(fout, "f%d %g %g %g\n", f, FC[0], FC[1], FC[2]);
      }
    end_f_loop(f,t)
    fprintf(fout, "\n");
 }

 void Print_Pressure_mdot(Domain *domain, int id)
 {

    cell_t c;
    Thread *t;
    face_t f;
    t = Lookup_Thread(domain, id);
    begin_f_loop(f,t)
    {
      flow += F_FLUX(f,t);
      p += F_P(f,t); //write area aveg pressure.
    }
    end_f_loop(f,t)
    //p = C_P(c,t);
    fprintf(fout, "for zoneid = %d \n", id);
    fprintf(fout, "pressure = %g \n", p);
    fprintf(fout, "mass Flow Rate: %g \n", flow);
 }

 DEFINE_ON_DEMAND(get_pressure)
 {
    Domain *domain;
    domain = Get_Domain(1); //existing fluid domain
    fout = fopen("outputs.txt", "w");
    //Print_Thread_Face_centroids(domain, 6); //outlet
    Print_Pressure_mdot(domain, 6);
    fclose(fout);
 } 