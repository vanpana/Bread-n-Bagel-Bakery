#include "controller.h"
#include "../repository/repository.h"
#include "../domain/entities.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../common/util.h"
#include "../domain/entities.h"

Controller* createController(Repository* r)
{
    /*
    Constructor for controller struct.
    */

    Controller* c = (Controller*)malloc(sizeof(Controller));
    c->repository = r;

    c->backupSize = 0;
    c->backupPos = 0;
    c->backup = (Repository*)malloc(100 * sizeof(Repository));

    return c;
}

void destroyController(Controller* c)
{
    /*
    Frees the memory allocated for Controller struct.
    */
    destroyRepository(c->repository);
    free(c);
}

int CtrlGetLength(Controller* c)
{
    /*
    Gets the number of items stored in repository.
    */
    return getLength(c->repository);
}

void CtrlAddItem(Controller* c, char* name, char* supplier, int day, int month,
   int year, int qty)
{
    /*
    Creates material and sends to repository.
    */
    material* m = createMaterial(name, supplier, day, month, year, qty);
    addToUndoList(c);
    addItem(c->repository, m);
}

int CtrlDeleteItem(Controller* c, char* name)
{
    /*
    Sends the delete command to repository.
    return: 1 if passed, 0 if failed
    */
    addToUndoList(c);
    return delItem(c->repository, name);
}

int CtrlUpdateItem(Controller* c, char* name, char* supplier, int day, int month,
   int year, int qty)
{
    /*
    Creates material and sends update info to repository
    return: 1 if passed, 0 if failed
    */
    material* m = createMaterial(name, supplier, day, month, year, qty);
    addToUndoList(c);
    return updateItem(c->repository, m);
}

char** CtrlExpiredMaterialsByName(Controller* c, char* needle)
{
    /*
    Returns a list of materials expired with a specified string from a supplier.
    input: needle - string
    output: list of expired materials
    */
    char** expiredItems = (char**)malloc(10 * sizeof(char*));
    int j;
    for (j = 0; j < 10; j++)
        expiredItems[j] = (char*)malloc(sizeof(char));
    int counter = 0;

    Repository* r = c->repository;
    if (strcmp(needle, "-") == 0)
    {

        for (int i = 0; i < r->length; i++)
            if (checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0)
            {
                strcpy(expiredItems[counter], r->items[i]->name);
                counter++;
            }
    }
    else
    {
        for (int i = 0; i < r->length; i++)
            if (checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0 && strstr(r->items[i]->name, needle) != NULL)
            {
                strcpy(expiredItems[counter], r->items[i]->name);
                counter++;
                if (counter%10 == 0)
                {
                    expiredItems = (char**)realloc(expiredItems, (counter + 10) * sizeof(char*));
                    for (j = counter + 1; j < counter + 10; j++)
                        expiredItems[j] = (char*)malloc(sizeof(char));
                }
            }
    }
    expiredItems[counter] = "";
    return expiredItems;
}

char** CtrlExpiredMaterialsBySupplier(Controller* c, char* needle)
{
    /*
    Returns a list of materials expired with a specified string from a supplier.
    input: needle - string
    output: list of expired materials
    */
    char** expiredItems = (char**)malloc(10 * sizeof(char*));
    int j;
    for (j = 0; j < 10; j++)
        expiredItems[j] = (char*)malloc(sizeof(char));
    int counter = 0;

    Repository* r = c->repository;
    if (strcmp(needle, "-") == 0)
    {

        for (int i = 0; i < r->length; i++)
            if (checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0)
            {
                strcpy(expiredItems[counter], r->items[i]->supplier);
                counter++;
            }
    }
    else
    {
        for (int i = 0; i < r->length; i++)
            if (checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0 && strstr(r->items[i]->supplier, needle) != NULL)
            {
                strcpy(expiredItems[counter], r->items[i]->supplier);
                counter++;
                if (counter%10 == 0)
                {
                    expiredItems = (char**)realloc(expiredItems, (counter + 10) * sizeof(char*));
                    for (j = counter + 1; j < counter + 10; j++)
                        expiredItems[j] = (char*)malloc(sizeof(char));
                }
            }
    }
    expiredItems[counter] = "";
    return expiredItems;
}

char** CtrlExpiredMaterialsByQty(Controller* c, int qty)
{
    /*
    Returns a list of materials expired less than a specified quantity.
    input: qty - int
    output: list of expired materials
    */
    char** expiredItems = (char**)malloc(10 * sizeof(char*));
    int j;
    for (j = 0; j < 10; j++)
        expiredItems[j] = (char*)malloc(sizeof(char));
    int counter = 0;

    Repository* r = c->repository;

    for (int i = 0; i < r->length; i++)
        if (checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0 && r->items[i]->qty > qty)
        {
            strcpy(expiredItems[counter], r->items[i]->name);
            counter++;
            if (counter%10 == 0)
            {
                expiredItems = (char**)realloc(expiredItems, (counter + 10) * sizeof(char*));
                for (j = counter + 1; j < counter + 10; j++)
                    expiredItems[j] = (char*)malloc(sizeof(char));
            }
        }
    expiredItems[counter] = "";
    return expiredItems;
}

material** CtrlShortOnSupply(Controller* c, char* supplier, int qty, int descending)
{
    /*
    Returns a list of materials short on supply from a supplier less than a quantity.
    input: supplier - string, qty - int, descending - 0 if not, 1 if yes
    output: list of sorted short on supply materials
    */
    material** shortItems = (material**)malloc(10 * sizeof(material*));
    int i, j;
    for (j = 0; j < 10; j++)
        shortItems[j] = (material*)malloc(sizeof(material));

    int counter = 0;

    Repository* r = c->repository;

    for (int i = 0; i < r->length; i++)
        if (strcmp(r->items[i]->supplier, supplier) == 0 &&r->items[i]->qty < qty)
        {
            shortItems[counter] = r->items[i];
            counter++;
            if (counter%10 == 0)
            {
                shortItems = (material**)realloc(shortItems, (counter + 10) * sizeof(material*));
                for (j = counter + 1; j < counter + 10; j++)
                    shortItems[j] = (material*)malloc(sizeof(material));
            }
        }

    if (descending == 0)
        sortByQty(shortItems, counter, 0);
    else
        sortByQty(shortItems, counter, 1);

    shortItems[counter] = createMaterial("","",0,0,0,0);

    return shortItems;
}

material** CtrlGetSupplierByExpMonth(Controller* c, char* supplier, int descending)
{
    /*
    Returns a list of materials short on supply from a supplier less than a quantity.
    input: supplier - string, qty - int, descending - 0 if not, 1 if yes
    output: list of sorted short on supply materials
    */
    material** shortItems = (material**)malloc(10 * sizeof(material*));
    int i, j;
    for (j = 0; j < 10; j++)
        shortItems[j] = (material*)malloc(sizeof(material));

    int counter = 0;

    Repository* r = c->repository;

    for (int i = 0; i < r->length; i++)
        if (strcmp(r->items[i]->supplier, supplier) == 0)
        {
            shortItems[counter] = r->items[i];
            counter++;
            if (counter%10 == 0)
            {
                shortItems = (material**)realloc(shortItems, (counter + 10) * sizeof(material*));
                for (j = counter + 1; j < counter + 10; j++)
                    shortItems[j] = (material*)malloc(sizeof(material));
            }
        }

    if (descending == 0)
        sortByExpMonth(shortItems, counter, 0);
    else
        sortByExpMonth(shortItems, counter, 1);

    shortItems[counter] = createMaterial("","",0,0,0,0);

    return shortItems;
}

Repository* CtrlGetRepository(Controller* c)
{
    /*
    Returns current repository.
    */
    return c->repository;
}

void addToUndoList(Controller* c)
{
    if (c->backupPos != c->backupSize - 1)
        for (int i = c->backupPos + 1; i < c->backupSize; i++)
            destroyRepository(&c->backup[i]);

    c->backup[c->backupPos] = *c->repository;
    c->backupPos++;
    c->backupSize = c->backupPos + 1;
    printf("pos: %d, size: %d\n", c->backupPos, c->backupSize);


}

void undoOperation(Controller* c)
{
    //check if last elements
    c->backupPos--;
    c->repository = &c->backup[c->backupPos];
    printf("pos: %d, size: %d\n", c->backupPos, c->backupSize);
}

void redoOperation(Controller* c)
{
    //check if nothing to redo
    c->repository = &c->backup[c->backupPos++];
    printf("pos: %d, size: %d\n", c->backupPos, c->backupSize);
    //c->backupPos++;
}
