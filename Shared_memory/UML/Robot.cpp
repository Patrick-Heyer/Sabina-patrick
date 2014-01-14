
#include "Robot.h"
#include "Destination.h"
#include "Person.h"
#include "Camera.h"

Location Robot::get_Position() {
  return Position;
}

void Robot::setPosition(Location value) {
  Position = value;
}

float Robot::get_Lineal_velocity() {
  return Lineal_velocity;
}

void Robot::set_Lineal_velocity(float value) {
  Lineal_velocity = value;
}

float Robot::get_Angular_velocity() {
  return Angular_velocity;
}

void Robot::set_Angular_velocity(float value) {
  Angular_velocity = value;
}

string Robot::get_Current_destination() {
  return Current_destination;
}

void Robot::set_Current_destination(string value) {
  Current_destination = value;
}

string Robot::get_Last_objective() {
  return Last_objective;
}

void Robot::set_Last_objective(string value) {
  Last_objective = value;
}

list<Location> & Robot::get_Route() {
  return Route;
}

void Robot::set_Route(list<Location> & value) {
  Route = value;
}

void Robot::setPosition(int x, int y, float angle) {
  this->Position.set_X(x);
  this->Position.set_Y(y);
  this->Position.set_Angle(angle);
}

void Robot::set_Current_destination(string name) {
}

