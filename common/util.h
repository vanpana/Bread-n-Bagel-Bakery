#pragma once
#include "../domain/entities.h"

int validateDate(int day, int month, int year);
int getNumberMonth(char* month);
int* getCurrentDate();
int checkPastDate(int* productDate, int* currentDate);
void sortByQty(material** items, int size, int desc);
void sortByExpMonth(material** items, int size, int desc);
void sortBySupl(material** items, int size, int desc);
void runAllTests();
