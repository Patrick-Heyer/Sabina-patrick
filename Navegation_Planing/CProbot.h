
#ifndef CProbot_H
#define CProbot_H

#include <vector>

#include "CAmbiente.h"
#include "CSamples.h"

#include "PlanningParameters.h"
#include "CSampler.h"

#include "CLocal.h"
#include "CNearNode.h"
#include "CGraph.h"


class  CProbot
{
private:

    CAmbiente     * Ambiente_p_m;
    CSamples      * Samples_p_m;
    uu_Sampler    * Sampler_p_m;
    uu_LocalPlanner  * LocalPlanner_p_m;
    uu_Graph      * Graph_p_m ;
    uu_PlanningParameters Parametros_m;

public:

    std::vector<int> Counters ;
    std::vector < uu_Point >	listametas;
    std::vector <CPersonajeConfig> recorrido;

    CProbot();
    ~CProbot();

    int BuildRoadMap();
    int RoadMapReady();
    void Calcularmetas();
    void Smooth();

    int ComputePath();
    int ComputeRecorridoC(const CPersonajeConfig &, const CPersonajeConfig &);
    int ComputeRecorridoLF(const CPersonajeConfig &);

    const std::vector<uu_Point> & Metas()const;
    const CAmbiente * Ambiente()const;
    const uu_PlanningParameters & Parametros() const ;
    const CSamples * Samples()const;
    const uu_Graph * Graph()const;
    const uu_LocalPlanner * LocalPlanner()const;


    void SetSampler(const uu_Sampler *);
    void SetLocalPlanner(const uu_LocalPlanner *);

    void setPlanningParametros( const uu_PlanningParameters & ) ;

    void SetGraph (const uu_Graph *);
    void SetSamples(const CSamples *);

    void Limpiar();
    void AddEdges( int , const std::vector<NearNode> &);
    void ComputeNeighbours( std::vector<NearNode> &, int  );

    int ProcesarChelas( int row1, int col1, double ori1, int row2, int col2, double ori2);
    int ProcesarLostF(int row1, int col1, double ori1);
    int ProcesarLostFFile(int row1, int col1, double ori1);

};


#endif
