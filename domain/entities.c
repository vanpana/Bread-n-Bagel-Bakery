#include "entities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

material* createMaterial(char* name, char* supplier, int day, int month,
   int year, int qty)
{
    material* m = (material*)malloc(sizeof(material));
    m->name = (char*)malloc(strlen(name) + 1);
    strcpy(m->name, name);

    m->supplier = (char*)malloc(strlen(supplier) + 1);
    strcpy(m->supplier, supplier);

    m->day = day;
    m->month = month;
    m->year = year;
    m->qty = qty;

    return m;
}

void destroyMaterial(material* m)
{
    free(m->name);
    free(m->supplier);

    free(m);
}

char* getName(material* m)
{
    return m->name;
}

material* setName(material* m, char* value)
{
    strcpy(m->name, value);
    return m;
}

char* getSupplier(material* m)
{
    return m->supplier;
}

material* setSupplier(material* m, char* value)
{
    strcpy(m->supplier, value);
    return m;
}

int* getExpiration(material* m)
{
    int* expdate = (int*)malloc(sizeof(int));

    expdate[0] = m->day;
    expdate[1] = m->month;
    expdate[2] = m->year;

    return expdate;
}

material* setExpiration(material* m, int day, int month, int year)
{
    m->day = day;
    m->month = month;
    m->year = year;

    return m;
}

int getQty(material* m)
{
    return m->qty;
}

material* setQty(material* m, int value)
{
    m->qty += value;
    return m;
}

void printMaterial(material* m)
{
    printf("%s, %s, %d, %d, %d, %d\n",m->name, m->supplier, m->day, m->month, m->year,
    m->qty);
}
