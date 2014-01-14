#include "dynamic_win.h"

int dynamic_win::get_start_pnt() {
    return start_pnt;
}
int dynamic_win::get_end_pnt() {
    return end_pnt;
}
double dynamic_win::get_probability() {
    return probability;
}
void dynamic_win::set_start_pnt(int start_pnt2) {
    start_pnt = start_pnt2;
}
void dynamic_win::set_end_pnt(int end_pnt2) {
    end_pnt = end_pnt2;
}
void dynamic_win::set_probability(double probability2) {
    probability = probability2;
}
