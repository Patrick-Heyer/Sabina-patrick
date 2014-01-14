
#include "TiposBasicos.h"
#include "read_prm.h"

#include "../Shared_memory/Robot.h"
#include "../Shared_memory/Location.h"

Robot *patrolbot;


list<Location> read_prm (CProbot *prm_p_a, int x_initial, int y_initial, int x_final, int y_final) {
    
    list<Location> ruta_temp;
    Location punto_temp;
    double a, a2;
    int hussein = 0;
    std::vector<uu_Point> metas;
    
    patrolbot->getInstance().set_Route(ruta_temp);
    
    printf("PRM: Planificando trayectoria...\n");
    /* Leer destino (x, y)*/
    a=0;
    a2=0;
    
    hussein = prm_p_a->ProcesarChelas(x_initial, y_initial, a, x_final, y_final, a2);
    
    // Esta condición la agregué yo HUSSEIN
    if (hussein == 0)
    {
	printf("\nLo siento!! Pero el programa PRM no pudo obtener una ruta\n");
    }
    else
    {
	metas = prm_p_a->Metas();
	int x_antes, y_antes;
	for (unsigned int h = 0; h< metas.size(); h++)
	{
	    if(metas[h].X()!=x_antes || metas[h].Y()!=y_antes)
	    {
	    cout<<h<<": "<<metas[h].X()<<"  "<<metas[h].Y()<<"  "<<metas[h].Z() << endl;
	    punto_temp.set(metas[h].X(), metas[h].Y(), metas[h].Z());
	    ruta_temp.push_back(punto_temp);
	    x_antes=metas[h].X();
	    y_antes=metas[h].Y();
	    }
	    
	}
	
		return ruta_temp;
    }// fin del if-else HUSSEIN
    
    
    
}
