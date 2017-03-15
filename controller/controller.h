#pragma once
#include "../repository/repository.h"

typedef struct
{
  Repository* repository;
  Repository* backup;
  int backupSize, backupPos;



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
char** CtrlExpiredMaterialsByName(Controller* c, char* needle);
char** CtrlExpiredMaterialsBySupplier(Controller* c, char* needle);
char** CtrlExpiredMaterialsByQty(Controller* c, int qty);
material** CtrlShortOnSupply(Controller* c, char* supplier, int qty, int descending);
material** CtrlGetSupplierByExpMonth(Controller* c, char* supplier, int descending);
material** CtrlGetSupplierDescending(Controller* c, char* name, int descending);
Repository* CtrlGetRepository(Controller* c);
void addToUndoList(Controller* c);
int undoOperation(Controller* c);
int redoOperation(Controller* c);
