#pragma once

typedef struct
{
  char* name;
  char* supplier;
  int day, month, year, qty;
}material;

material* createMaterial(char* name, char* supplier, int day, int month,
  int year, int qty);
void destroyMaterial(material* m);
char* getName(material* m);
material* setName(material* m, char* value);
char* getSupplier(material* m);
material* setSupplier(material* m, char* value);
int* getExpiration(material* m);
material* setExpiration(material* m, int day, int month, int year);
int getQty(material* m);
material* setQty(material* m, int value);
void printMaterial(material* m);
