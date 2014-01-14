
#include "Location.h"

int Location::get_X() {
  return X;
}

void Location::set_X(int value) {
  X = value;
}

int Location::get_Y() {
  return Y;
}

void Location::set_Y(int value) {
  Y = value;
}

float Location::get_Angle() {
  return Angle;
}

void Location::set_Angle(float value) {
  Angle = value;
}

void Location::set(int x, int y, float angle) {
  this->X=x;
  this->Y=y;
  this->Angle=angle;
}

