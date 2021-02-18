/*****************************************************************
    Example of UDF for single phase that uses Get_Domain utility
 ******************************************************************/
 
 #include "udf.h"
  FILE *fout;
  Domain *domain;
  
 void Print_Pressure_mdot(Domain *domain, int id)
 {
    cell_t c;
    Thread *t;
    face_t f;
    real flow=0.0;
    real p=0.0;
    real area[ND_ND];
    real total_area = 0.0;
    real p_0 = RP_Get_Float("operating-pressure");
    real resultp = 0.0;
    real Amag;
    real pglob = 0.0;
    real areaglob = 0.0;
    real total_p;

    t = Lookup_Thread(domain, id);
    begin_f_loop(f,t)
    {
      flow += F_FLUX(f,t); // prints below the given fprintf command
      //p += F_P(f,t); //write area aveg pressure, tallies with area averaged total pressure, but not always, mostly 0.
      F_AREA(area,f,t);
      Amag = NV_MAG(area);
      total_area += Amag;
      p = C_P(f,t);
      total_p += p * Amag; //NV_MAG(area);
    }
    end_f_loop(f,t)
    
    pglob = PRF_GRSUM1(total_p);
    areaglob = PRF_GRSUM1(total_area);
    
    resultp  = pglob / areaglob;
    
    fprintf(fout, "for zoneid = %d \n", id);
    fprintf(fout, "pressure = %g \n", resultp);
    fprintf(fout, "mass Flow Rate = %g \n", flow);
    fprintf(fout, "total area  = %g \n", total_area);
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
