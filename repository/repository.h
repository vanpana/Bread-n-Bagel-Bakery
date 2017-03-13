#pragma once
#include "../domain/entities.h"

typedef struct
{
  material* items[100];
  int length;
}Repository;

Repository* createRepository();
void destroyRepository(Repository* r);
material* findMaterialByName(Repository* r, char* name);
int getMaterialPosition(Repository* r, char* name);
int getLength(Repository* r);
void addItem(Repository* r, material* m);
int delItem(Repository* r, char* name);
int updateItem(Repository* r, material* m);
