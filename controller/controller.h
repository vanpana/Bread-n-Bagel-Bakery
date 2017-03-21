#pragma once
#include "../repository/repository.h"

typedef struct
{
  Repository* repository;
  Repository* backup;
  int backupSize, backupPos;
  char** backupInstructions;



}Controller;

typedef int (*compfn)(const void*, const void*);
Controller* createController(Repository* r);
void destroyController(Controller* c);
int CtrlGetLength(Controller* c);
void CtrlAddItem(Controller* c, char* name, char* supplier, int day, int month,
   int year, int qty);
int CtrlDeleteItem(Controller* c, char* name);
int CtrlUpdateItem(Controller* c, char* name, char* supplier, int day, int month,
   int year, int qty);
material** CtrlSort(Controller* c, char* name, char* supplier, int qty, int desc);
Repository* CtrlGetRepository(Controller* c);
void addToUndoList(Controller* c);
int undoOperation(Controller* c);
int redoOperation(Controller* c);
