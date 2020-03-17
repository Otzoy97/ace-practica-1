#ifndef ROUTEHANDLER_H
#define ROUTEHANDLER_H
//#include <ListLib.h>

typedef struct Vector {
  char dir; //0 ADELANTE - 1 ATRÁS - 2 DERECHA - 3 IZQUIERDA
  char dur;
  Vector() : dir('\0'), dur('\0') {}
} Vector;

typedef struct Route {
  char name_[6];
  char dirsCount;
  struct Vector dirs[8];
  Route() : name_(), dirsCount(0) {}
} Route;

#endif //ROUTEHANDLER_H
