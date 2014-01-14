#include "observations.h"
#include <math.h>

using namespace std;

observations::observations():size(0),values(NULL),
    x_hand_b(0), y_hand_b(0), z_hand_b(0) { }

int observations::get_size() {
    return size;
};

NODO * observations::get_values(int beg) {
    NODO * actual;

    actual = values;
    for (int i = 0; i < beg; i++)
        actual=actual->next;
    //cout << actual->data << endl;
    /*
    	while(actual!=NULL){
    		cout << actual->data << endl;
    		actual=actual->next;
    	}*/
    return actual;
};


void  observations::add_observation(double x_hand, double y_hand, double z_hand, double x_neck, double y_neck) {
    if (size == 0) {
        //values[size] = 0;
        values=(nodo)malloc(sizeof(NODO));
        values->data=0;
        values->next=NULL;
        size++;
        x_hand_b=x_hand;
        y_hand_b=y_hand;
        z_hand_b=z_hand;
    }
    else {
        int acum = 0;
        int i = 0;
        NODO * actual,* nuevo;

        acum = acum + (y_hand > y_neck)*pow(2,i);
        i++;
        acum = acum + (x_hand > x_neck)*pow(2,i);
        i++;
        acum = acum + (z_hand > z_hand_b)*pow(2,i);
        i++;
        acum = acum + (y_hand > y_hand_b)*pow(2,i);
        i++;
        acum = acum + (x_hand > x_hand_b)*pow(2,i);
        nuevo=(nodo)malloc(sizeof(NODO));
        nuevo->data=acum;
        nuevo->next=NULL;
        actual = values;
        while(actual->next!=NULL)
            actual=actual->next;
        nuevo->next=actual->next;
        actual->next=nuevo;
        //values[size] = acum;
        size++;
        //Storing the actual hand coordinates like previous values
        //for the next calculus
        x_hand_b=x_hand;
        y_hand_b=y_hand;
        z_hand_b=z_hand;
    }
};
