#pragma once
#include "../controller/controller.h"

typedef struct
{
  Controller* controller;
}Console;

Console* createConsole(Controller* c);
void destroyConsole(Console* ui);
void printMenu();
void uiListAllMaterials();
void uiAddMaterials(Console* ui);
void uiDeleteMaterials(Console* ui);
void uiUpdateMaterials(Console* ui);
void uiGetExpired(Console* ui);
void uiGetExpiredByQty(Console* ui);
void uiGetShortOnSupply(Console* ui);
char* getCommand();
int getInteger();
int* getDateIntegers();
int validateCommand(Console* ui, char* command);
void loop(Console* ui);
void runApp(Console* ui);