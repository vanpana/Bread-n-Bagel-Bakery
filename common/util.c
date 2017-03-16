#include "util.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../domain/entities.h"
#include "../controller/test_controller.h"
#include "../repository/test_repository.h"

int validateDate(int day, int month, int year)
{
    if (month < 1 || month > 12 || day < 1 || day > 31)
    return 0;

    if (month == 2)
    {
        if (year%4 == 0 && day > 28)
          return 0;

        else if (year%4 == 1 && day > 29)
          return 0;
    }

    if (month < 7 && month%2 == 0 && day > 30)
        return 0;

    if (month > 7 && month%1 == 0 && day > 30)
        return 0;

    return 1;
}

int getNumberMonth(char* month)
{
    if (strcmp(month, "Jan") == 0) return 1;
    if (strcmp(month, "Feb") == 0) return 2;
    if (strcmp(month, "Mar") == 0) return 3;
    if (strcmp(month, "Apr") == 0) return 4;
    if (strcmp(month, "May") == 0) return 5;
    if (strcmp(month, "Jun") == 0) return 6;
    if (strcmp(month, "July") == 0) return 7;

    if (strcmp(month, "Aug") == 0) return 8;
    if (strcmp(month, "Sep") == 0) return 9;
    if (strcmp(month, "Oct") == 0) return 10;
    if (strcmp(month, "Nov") == 0) return 11;
    if (strcmp(month, "Dec") == 0) return 12;
    return 0;
}

int* getCurrentDate()
{
    time_t t;
    time(&t);

    int* date = (int*)malloc(sizeof(date));
    char* day = (char*)malloc(sizeof(char));
    char* month = (char*)malloc(sizeof(char));
    char* year = (char*)malloc(sizeof(char));

    day[0] = ctime(&t)[8];
    day[1] = ctime(&t)[9];
    date[0] = atoi(day);

    month[0] = ctime(&t)[4];
    month[1] = ctime(&t)[5];
    month[2] = ctime(&t)[6];
    date[1] = getNumberMonth(month);

    year[0] = ctime(&t)[20];
    year[1] = ctime(&t)[21];
    year[2] = ctime(&t)[22];
    year[3] = ctime(&t)[23];
    date[2] = atoi(year);

    return date;
}

int checkPastDate(int* productDate, int* currentDate)
{
    if (productDate[2] < currentDate[2])
        return 0;
    if (productDate[2] == currentDate[2])
    {
        if (productDate[1] < currentDate[1])
            return 0;
        if (productDate[1] == currentDate[1])
        {
            if (productDate[0] < currentDate[0])
                return 0;
        }
    }
    return 1;
}

void sortByQty(material** items, int size, int desc)
{
    for (int i = 0; i < size-1; i++)
        for (int j = i+1; j < size; j++)
        {
            if (desc == 0 && items[i]->qty > items[j]->qty)
            {

                material* aux = items[i];
                items[i] = items[j];
                items[j] = aux;
            }
            else if (desc == 1 && items[i]->qty < items[j]->qty)
            {
                material* aux = items[i];
                items[i] = items[j];
                items[j] = aux;
            }
        }
}

void sortByExpMonth(material** items, int size, int desc)
{
    for (int i = 0; i < size-1; i++)
        for (int j = i+1; j < size; j++)
        {
            if (desc == 0 && items[i]->month > items[j]->month)
            {

                material* aux = items[i];
                items[i] = items[j];
                items[j] = aux;
            }
            else if (desc == 1 && items[i]->month < items[j]->month)
            {
                material* aux = items[i];
                items[i] = items[j];
                items[j] = aux;
            }
        }
}

void sortBySupl(material** items, int size, int desc)
{
    for (int i = 0; i < size-1; i++)
        for (int j = i+1; j < size; j++)
        {
            if (desc == 0 && strcmp(items[i]->supplier, items[j]->supplier) < 0)
            {

                material* aux = items[i];
                items[i] = items[j];
                items[j] = aux;
            }
            else if (desc == 1 && strcmp(items[i]->supplier, items[j]->supplier) < 0)
            {
                material* aux = items[i];
                items[i] = items[j];
                items[j] = aux;
            }
        }
}

material** sortGen(material** container, int size, int desc, int (*compar)(material*, material*))
{
    for (int i = 0; i < size - 1; i++)
        for (int j = i+1; j < size; j++)
            if (compar(container[i], container[j]) == 1 && desc == 0)
            {
                material* aux = container[i];
                container[i] = container[j];
                container[j] = aux;
            }
            else if (compar(container[i], container[j]) == -1 && desc == 1)
            {

                material* aux = container[i];
                container[i] = container[j];
                container[j] = aux;
            }
    return container;
}

int sortByNameGen(material* a, material*b)
{
    if (strcmp(a->name, b->name) < 0)
        return -1;
    else if (strcmp(a->name, b->name) == 0)
        return 0;
    return 1;
}

int sortBySuplGen(material* a, material*b)
{
    if (strcmp(a->supplier, b->supplier) < 0)
        return -1;
    else if (strcmp(a->supplier, b->supplier) == 0)
        return 0;
    return 1;
}

int sortByQtyGen(material* a, material*b)
{
    if (a->qty > b->qty)
        return 1;
    else if (a->qty == b->qty)
        return 0;
    return -1;
}

int sortByExpDateGen(material* a, material* b)
{
    if (a->year < b->year)
        return 1;
    else if (a->year > b->year)
        return -1;
    else if (a->year == b->year)
    {
        if (a->month < b->month)
            return 1;
        else if (a->month > b->month)
            return -1;
        else if (a->month == b->month)
        {
            if (a->day < b->day)
                return 1;
            else if (a->day > b->day)
                return -1;
        }
    }
    return 0;
}

void runAllTests()
{
    runRepoTests();
    runCtrlTests();
}
