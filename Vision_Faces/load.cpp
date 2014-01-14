
#include "sift.h"
#include "imgfeatures.h"
#include "kdtree.h"
#include "utils.h"
#include "xform.h"
#include "cv.h"
#include "highgui.h"
#include "tratamiento.h"
#include "facedetect.h"
#include <stdarg.h>
#include <math.h>

#define KDTREE_BBF_MAX_NN_CHKS 200
#define NN_SQ_DIST_RATIO_THR 0.49
#define DIV_ANCHO 3
#define DIV_ALTO 4
#define THRESHOLD 3
#define BD 15
#define MAX_LENGHT_NAME 40
#define SIZE 4

int sujetos=0;
char **Nombres;
IplImage * Sujeto[BD];
struct feature *Ptos_ec[BD];
struct kd_node *Arbol_ec[BD];
int ptos_ec[BD]; 	/*Almacena el total de ptos en una imagen de la BD */
int ptos_ojos_ec[BD];   /*Almacena el total de puntos encontrados en la zona T */
int ptos_BD_ec[10*BD];  /*Almacena todas las coors del grid para las imagenes de la BD, c/pers tiene 10 ptos */
int pt10[BD];		/*Almacena las coordenadas del grid para el frame activo */
int threshold[BD];
double similaridades[BD];
double prob_Absoluta[BD];
double bayes_C1_A[BD];
int ultimo_frame_considerado=0;
double primero=0,segundo=0;

int InitDB();
int InitClasifiers();
int GeneraPersona(int apun);
int calcula_total_puntos_zona_T(feature *Ptos_SIFT, int num_ptos, int BD1[],int apun);
int calcula_coor_pto_4Zonas(int x,int y,int ptos[],int ind);
int ComparaBD(IplImage* compa,int frame);
int eyes(IplImage* img,kd_node* arbol[],int puntos_del_grid[],int num_ptos_ojosT[],int frame);
int match(feature* features_nuevos ,int num_feat_nuevos ,kd_node *arbol_BD ,int BD1[], int apun);
int determina_calculo(int frame,double suma_total_N,int indice_mayor,double primero,double segundo);
int criterio_1(int i,double mayor);
void calcula_prob_Absoluta(double suma_total);
int aplica_bayes(int frame,double vprob[],double vbayes[]);
int get2max(double *m,int n);
void IniciaProbabilidades();
void AjustaProbabilidades(int frame);
int AprendeRostro(IplImage* img);

int InitDB() {
    int i;
    char name[20];
    fprintf(stdout,"\nLoading face database...");
    for(i=1; i<=2; i++) {
        printf("paso1");
        sprintf(name,"%d.jpg",i);
        printf("paso0");
        fprintf(stdout,"\nTrying %s..",name);
        printf("paso1");
        Sujeto[sujetos]=cvLoadImage(name, 0 );
        printf("paso2");
        if(GeneraPersona(sujetos)) sujetos++;
    }
    for(i=0; i<sujetos; i++)
        printf("\nLoading %d con %d , %d",i,ptos_ec[i],threshold[i]);
    return 1;
}

int InitClasifiers() {
    fprintf(stdout,"\nLoading clasifiers...");
    if( !initFaceDet("haarcascade_frontalface_default.xml"))return 0;
    if( !initEyeDet("parojos.xml"))return 0;
    if( !initEyeDetI("ojoI.xml")) return 0;
    if( !initEyeDetD("ojoD.xml")) return 0;
    return 1;
}

int GeneraPersona(int apun) {
    IplImage *equaliza_img=0,*eq_and_comp_img=0,*c=0;
    int ojos_detectados=0;
    equaliza_img=cvCreateImage(cvSize(Sujeto[apun]->width,Sujeto[apun]->height),8,1);
    eq_and_comp_img=cvCreateImage(cvSize(Sujeto[apun]->width,Sujeto[apun]->height),8,1);
    histogramEqualize2(Sujeto[apun],equaliza_img);
    eq_and_comp_img = mejora_imagen(equaliza_img);

    c=detectEyes(Sujeto[apun],ptos_BD_ec,apun*10,&ojos_detectados);
    if(ojos_detectados) {
        ptos_ec[apun]=sift_features( eq_and_comp_img, &Ptos_ec[apun] );
        Arbol_ec[apun]= kdtree_build( Ptos_ec[apun], ptos_ec[apun] );
        ptos_ojos_ec[apun]=calcula_total_puntos_zona_T(Ptos_ec[apun],ptos_ec[apun],ptos_BD_ec,apun*10);
        threshold[apun]=(ptos_ojos_ec[apun]*THRESHOLD)/100;

    }
    else {
        printf("\nA %s no le detecte ojos en la imagen original",Nombres[apun]);
        return 0;
    }
    cvReleaseImage(&eq_and_comp_img);
    cvReleaseImage(&equaliza_img);
    cvReleaseImage(&c);
    return 1;
}

int calcula_total_puntos_zona_T(feature *Ptos_SIFT, int num_ptos, int BD1[],int apun) {
    struct feature *feat;
    int i,coor1,total=0;
    int c11=0,c22=0,c33=0,c44=0;
    for( i = 0; i < num_ptos ; i++ ) {
        feat = Ptos_SIFT + i;
        coor1=calcula_coor_pto_4Zonas(cvRound(feat->x),cvRound(feat->y),BD1,apun);
        if (coor1==1) c11++;
        if (coor1==2) c22++;
        if (coor1==3) c33++;
        if (coor1==3) c44++;
    }
    total=c11+c22+c33+c44;
    return total;
}

int calcula_coor_pto_4Zonas(int x,int y,int ptos[],int ind) {
    if ( ptos[ind+0] <= y) {
        if( y <= ptos[ind+1]) {
            if(ptos[ind+5]<=x && x<=ptos[ind+7]) return 1; /*estas en la franja de frente*/
            else  return 0;
        }
        if( y <= ptos[ind+2]) {
            if( x <= ptos[ind+6] ) {       /*puede ser ojo izq*/
                if ( ptos[ind+4] <= x ) return 2;
                else   return 0;
            }
            else {                      /*puede ser ojo der*/
                if ( x <= ptos[ind+8] ) return 3;
                else return 0;
            }
        }
        if(y <= ptos[ind+3]) {
            if( ptos[ind+5]<=x && x<=ptos[ind+7]) return 4;
            else return 0;
        }
    }
    return 0;
}

void IniciaProbabilidades() {
    //printf("TOTAL DE SUJETOS %d",sujetos);
    double pesosProb = (1.0/sujetos);
    int i;
    ultimo_frame_considerado=0;
    for(i=0; i<sujetos; i++) {
        similaridades[i]=0.0;
        bayes_C1_A[i]=pesosProb;
    }
}

void AjustaProbabilidades(int frame) {
    if( (frame-ultimo_frame_considerado) == 10) {
        IniciaProbabilidades();
        printf("\n\t\tReseteando bayes_T_1");
    }
}

int ComparaBD(IplImage* compa,int frame) {
    IplImage* equa,*img_comp,*ec ;
    equa=cvCreateImage(cvSize(compa->width,compa->height),8,1);
    histogramEqualize2(compa,equa);
    ec=cvCreateImage(cvSize(compa->width,compa->height),8,1);
    ec=mejora_imagen(equa);

    return eyes(ec,Arbol_ec,ptos_BD_ec,ptos_ojos_ec,frame);
}

int eyes(IplImage* img,kd_node* arbol[],int puntos_del_grid[],int num_ptos_ojosT[],int frame) {
    IplImage *ojos;
    int puntos,i;
    double sum=0,tot_m=0,prim=0,seg=0;
    int indice_mayor=-1,encontre_ojos=0;
    struct feature *features_nuevos;
    ojos=detectEyes(img,pt10,0,&encontre_ojos);

    if(encontre_ojos) {
        puntos=sift_features( img, &features_nuevos );
        //printf("\nPtos Sift: %d   ",puntos);
        for(i=0; i<sujetos; i++) {
            tot_m=match(features_nuevos,puntos,arbol[i],puntos_del_grid,i);
            if(prim <= tot_m) {
                seg  = prim;
                prim = tot_m;
                indice_mayor=i;
            }
            else {
                if ( seg < tot_m )
                    seg = tot_m;
            }
            if(tot_m==0) tot_m=0.1;
            sum=sum+tot_m;
            similaridades[i]=tot_m;
            printf(" %2.0f ",similaridades[i]);
        }
        cvReleaseImage(&ojos);
        return determina_calculo(frame,sum,indice_mayor,prim,seg);

    }
    else {
        printf("\n\t No encontre ojos");
    }
    cvReleaseImage(&ojos);
    return 0;

}

int match(feature* features_nuevos ,int num_feat_nuevos ,kd_node *arbol_BD ,int BD1[], int apun) {
    struct feature* feat, **nbrs;
    int k,i,coincideCoor=0,coor1,coor2;
    double d0,d1;
    CvPoint pt1, pt2;
    for( i = 0; i < num_feat_nuevos ; i++ ) {
        feat = features_nuevos + i;
        k = kdtree_bbf_knn( arbol_BD, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
        if( k == 2 ) {
            d0 = descr_dist_sq( feat, nbrs[0] );
            d1 = descr_dist_sq( feat, nbrs[1] );
            if( d0 < d1 * NN_SQ_DIST_RATIO_THR ) {
                coor1=calcula_coor_pto_4Zonas(cvRound(feat->x),cvRound(feat->y),pt10,0);
                coor2=calcula_coor_pto_4Zonas(cvRound(nbrs[0]->x),cvRound(nbrs[0]->y),BD1,(apun*10));
                if (coor1==coor2 && coor1>0) {
                    pt1 = cvPoint( cvRound( feat->x )-1, cvRound( feat->y)-1 );
                    pt2 = cvPoint( cvRound( nbrs[0]->x-1 ), cvRound( nbrs[0]->y-1 ) );
                    coincideCoor++;
                }
                features_nuevos[i].fwd_match = nbrs[0];
            }
        }
        free( nbrs );
    }
    return coincideCoor;
}

int determina_calculo(int frame,double suma_total_N,int indice_mayor,double primero,double segundo) {
    int regresa=0,persona=0;
    if(suma_total_N>0) {
        if( criterio_1(indice_mayor,primero) ) {
            calcula_prob_Absoluta(suma_total_N);
            persona=aplica_bayes(frame,prob_Absoluta,bayes_C1_A);
            ultimo_frame_considerado=frame;
        }
    }
    return persona;
}

int criterio_1(int i,double mayor) {
    //Solo si el mayor rebasa al umbral del % de ptos en zona T
    if(threshold[i] <= mayor)
        return 1;
    return 0;
}

void calcula_prob_Absoluta(double suma_total) {
    int i;
    for(i=0; i<sujetos; i++)
        prob_Absoluta[i]=similaridades[i]/suma_total;
}

int aplica_bayes(int frame,double vprob[],double vbayes[]) {
    int j,mejor_postor,retorno=0;
    double suma=0.0;
    double multi[BD];
    for(j=0; j<sujetos; j++) {
        multi[j]=vbayes[j]*vprob[j];
        suma=suma+multi[j];
    }
    for(j=0; j<sujetos; j++) {
        float b=multi[j]/suma;
        vbayes[j]=b;
    }
    /*printf("\nSIMILITUD: ");
    for(j=0;j<sujetos;j++)printf("  %f  ",similaridades[j]);
    printf("\nPROBABILIDAD: ");
    for(j=0;j<sujetos;j++)printf("  %f  ",vprob[j]);
    printf("\nBAYES: ");
    for(j=0;j<sujetos;j++)printf("  %f  ",vbayes[j]);*/
    mejor_postor=get2max(vbayes,sujetos);
    //printf("\nDespuess get %f, %f",primero,segundo);
    if ( (primero-segundo)>(segundo*2) ) {
        retorno=mejor_postor+1;
        //printf("\n\t %d --> Es %s",frame,Nombres[retorno]);
        IniciaProbabilidades();
    }
    else {
        //printf("\n\t %d -->  Aun no tengo evidendia de alguien",frame);
    }
    return retorno;

}

int get2max(double *m,int n) {
    int i,ind=-1;
    primero=segundo=0;
    for(i=0; i<n; i++) {
        if(primero< m[i]) {
            // printf("\ncambie seg=%lf prim=%lf",primero,m[i]);
            segundo=primero;
            primero=m[i];
            ind=i;
        }
        else if(segundo<m[i]) {
            //printf("\ncambie seg=%lf",m[i]);
            segundo=m[i];
        }
    }
    //printf("\nles puse %lf,%lf",primero,segundo);
    return ind;
}

int AprendeRostro(IplImage* compa) {
    IplImage* equa,*ec,*c;
    char ruta[20];
    int ojos_detectados=0;
    if(sujetos<BD) {
        fprintf( stdout, "\n1..");
        c=detectEyes(compa,ptos_BD_ec,sujetos*10,&ojos_detectados);
        if(ojos_detectados) {
            fprintf( stdout, "\n1..");
            equa=cvCreateImage(cvSize(compa->width,compa->height),8,1);
            histogramEqualize2(compa,equa);
            ec=cvCreateImage(cvSize(compa->width,compa->height),8,1);
            ec=mejora_imagen(equa);
            ptos_ec[sujetos]=sift_features( ec, &Ptos_ec[sujetos] );
            Arbol_ec[sujetos]= kdtree_build( Ptos_ec[sujetos], ptos_ec[sujetos] );
            ptos_ojos_ec[sujetos]=calcula_total_puntos_zona_T(Ptos_ec[sujetos],ptos_ec[sujetos],ptos_BD_ec,sujetos*10);
            threshold[sujetos]=(ptos_ojos_ec[sujetos]*THRESHOLD)/100;
            sprintf(ruta,"%d.jpg",sujetos+1);
            printf("Mucho gusto %s",ruta);
            cvSaveImage(ruta,compa);
            sujetos++;
            return sujetos;
        }
        else {
            printf("\nAcercate casi no te veo");
            return 0;
        }
    }
    else {
        printf("\nMi memoria esta llena, sorry");
        return 0;
    }
    return 0;
}
