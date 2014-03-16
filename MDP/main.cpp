#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <iostream>
#include <utility>

#include "pspudd_pat.hh"


#define red   "\033[0;31m"        /* 0 -> normal ;  31 -> red */
#define cyan  "\033[1;36m"        /* 1 -> bold ;  36 -> cyan */
#define green "\033[4;32m"        /* 4 -> underline ;  32 -> green */
#define blue  "\033[9;34m"        /* 9 -> strike ;  34 -> blue */

#define black  "\033[0;30m"
#define brown  "\033[0;33m"
#define magenta  "\033[0;35m"
#define gray  "\033[0;37m"

#define none   "\033[0m"        /* to flush the previous property */



typedef std::map<std::string, int> Var_type;

Var_type *Variables;
Var_type *val_1ores;

int *varval_1s_pat; //Vector para mapear el estado act_1ual

DdNode *act_1, *val_1;


char * policyFile_1;

void Init_MDP( char *Filename)
{

    policyFile_1=Filename;
    // read in val_1ueDD, act_1ionDD, ovars, vars
    DdNode **UPList =  (DdNode **)malloc(2*(sizeof(DdNode *)));

    gbm = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,MAXMEM);

    // load from this file into dd: act_1[numpolicies] and val_1[numpolicies]
    int numread = readDualOptimal(gbm,policyFile_1,&UPList);

    if (numread == 0) {
        fprintf(stderr,"error reading dual policy from %s\n",policyFile_1);
        exit(1);
    } else {
        act_1 = UPList[0];
        Cudd_Ref(act_1);

        val_1 = UPList[1];
        Cudd_Ref(val_1);
        free(UPList);
    }
    varval_1s_pat=new int[numorigvars];
    val_1ores =new Var_type[numorigvars];
    Variables= new Var_type[numorigvars];

    for(int j=0; j <numorigvars; j++)
    {
        varval_1s_pat[j]=0;
    }


    fill(gbm,val_1,act_1,policyFile_1,orig_vars,vars,numvars,numorigvars,  Variables, val_1ores  );
}
string cambiar_estado( string variable, string val_1or)
{


    Var_type::iterator iter = Variables->begin();

    iter = Variables->find(variable);
    if (iter != Variables->end() )
    {
        Var_type::iterator iter_val_1ores = val_1ores[iter->second].begin();
        iter_val_1ores = val_1ores[iter->second].find(val_1or);
        if (iter != val_1ores[iter->second].end() )
        {
            varval_1s_pat[iter->second]=iter_val_1ores->second;
        }
    }

    printf("********************************************************\n");
    printf("Best accion defined by MDP is: %s%s%s \n", green, pQuery(gbm,val_1,act_1,policyFile_1,orig_vars,vars,numvars,numorigvars,varval_1s_pat ), none);
    printf("********************************************************\n");
    return pQuery(gbm,val_1,act_1,policyFile_1,orig_vars,vars,numvars,numorigvars,varval_1s_pat ) ;
}
