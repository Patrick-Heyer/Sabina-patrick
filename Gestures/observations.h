#include <stdlib.h>

#pragma once

typedef struct NODO NODO;

struct NODO {
    int data;
    NODO *next;
};

typedef NODO *nodo;

class observations {
private:
    // Members
    int size; //Size of he observations mesured in frames
    NODO *values; //List of observations obtained from the coordinates from the hand and neck
    double x_hand_b, y_hand_b, z_hand_b; //Coordinates from left hand and neck from previous frame
public:

    // Constructors
    observations();
    ~observations() {
        NODO * actual,* after;
        actual = values;
        after = values->next;
        while(actual!=NULL) {
            free(actual);
            actual=after;
            if (after != NULL)
                after=after->next;
        }
    };
    // Functions members
    int get_size();
    NODO * get_values(int beg);
    void add_observation(double x_hand, double y_hand, double z_hand, double x_neck, double y_neck);
};
