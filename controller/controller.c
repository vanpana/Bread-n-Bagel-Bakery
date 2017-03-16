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
    c->backupPos = -1;
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
    addItem(c->repository, m);
    addToUndoList(c);
}

int CtrlDeleteItem(Controller* c, char* name)
{
    /*
    Sends the delete command to repository.
    return: 1 if passed, 0 if failed
    */
    return delItem(c->repository, name);
    addToUndoList(c);
}

int CtrlUpdateItem(Controller* c, char* name, char* supplier, int day, int month,
   int year, int qty)
{
    /*
    Creates material and sends update info to repository
    return: 1 if passed, 0 if failed
    */
    material* m = createMaterial(name, supplier, day, month, year, qty);
    return updateItem(c->repository, m);
    addToUndoList(c);
}

material** CtrlSort(Controller* c, char* name, char* supplier, int qty, int desc)
{
    /*
    Returns a sorted list.
    If name is not empty, it will return a list with sorted items containing name.
    If name is not empty and supplier is not empty, it will return a list with sorted items containing name from a supplier.
    If name is not empty and qty is not empty, it will return a list with sorted items with quantity greater than qty.
    if name, supplier and qty are not empty, it will return a list with sorted items from a supplier with a quantity greatea than qty.

    If supplier is not empty, it will return a list with items sorted by name with given supplier.
    If supplier, qty are not empty, it will return a list with items short on supply from the given supplier.

    If qty is not empty, it will return a list with items sorted by quantity greater than input quantity.
    */
    material** items = (material**)malloc(10 * sizeof(material*));
    int j, counter = 0;;
    for (j = 0; j < 10; j++)
        items[j] = (material*)malloc(sizeof(material));
    Repository* r = CtrlGetRepository(c);
    if (strlen(name) != 0)
    {
        if (strlen(supplier) == 0 && qty == 0)
        //sort by items containing the name
        {
            for (int i = 0; i < r->length; i++)
                if (strstr(r->items[i]->name, name) != NULL)
                {
                    items[counter] = r->items[i];
                    counter++;
                    if (counter%10 == 0)
                    {
                        items = (material**)realloc(items, (counter + 10) * sizeof(material*));
                        for (j = counter + 1; j < counter + 10; j++)
                            items[j] = (material*)malloc(sizeof(material));
                    }
                }
            sortGen(items, counter, desc, sortByNameGen);
        }
        if (strlen(supplier) != 0 && qty == 0)
            //sort by name from supplier
        {
            for (int i = 0; i < r->length; i++)
                if (strstr(r->items[i]->name, name) != NULL && strstr(r->items[i]->supplier, supplier) != NULL)
                {
                    items[counter] = r->items[i];
                    counter++;
                    if (counter%10 == 0)
                    {
                        items = (material**)realloc(items, (counter + 10) * sizeof(material*));
                        for (j = counter + 1; j < counter + 10; j++)
                            items[j] = (material*)malloc(sizeof(material));
                    }
                }
            sortGen(items, counter, desc, sortByNameGen);
        }
        if (strlen(supplier) != 0 && qty > 0)
            //sort by name from supl with quantity > qty
        {
            for (int i = 0; i < r->length; i++)
                if (strstr(r->items[i]->name, name) != NULL && strstr(r->items[i]->supplier, supplier) != NULL && r->items[i]->qty > qty)
                {
                    items[counter] = r->items[i];
                    counter++;
                    if (counter%10 == 0)
                    {
                        items = (material**)realloc(items, (counter + 10) * sizeof(material*));
                        for (j = counter + 1; j < counter + 10; j++)
                            items[j] = (material*)malloc(sizeof(material));
                    }
                }
            sortGen(items, counter, desc, sortByQtyGen);
        }
    }

    else if (strlen(supplier) != 0)
    {
        if (qty == 0)
            //sort by name from supplier
        {
            for (int i = 0; i < r->length; i++)
                if (strstr(r->items[i]->supplier, supplier) != NULL)
                {
                    items[counter] = r->items[i];
                    counter++;
                    if (counter%10 == 0)
                    {
                        items = (material**)realloc(items, (counter + 10) * sizeof(material*));
                        for (j = counter + 1; j < counter + 10; j++)
                            items[j] = (material*)malloc(sizeof(material));
                    }
                }
            sortGen(items, counter, desc, sortByNameGen);
        }
        if (qty > 0)
            //sort by short on supply
        {
            for (int i = 0; i < r->length; i++)
                if (strstr(r->items[i]->supplier, supplier) != NULL && r->items[i]->qty <= qty)
                {
                    items[counter] = r->items[i];
                    counter++;
                    if (counter%10 == 0)
                    {
                        items = (material**)realloc(items, (counter + 10) * sizeof(material*));
                        for (j = counter + 1; j < counter + 10; j++)
                            items[j] = (material*)malloc(sizeof(material));
                    }
                }

            sortGen(items, counter, desc, sortByQtyGen);
        }
    }

    else if (qty > 0)
        //sort items by quantity > qty
    {
        for (int i = 0; i < r->length; i++)
            if (r->items[i]->qty > qty)
            {

                items[counter] = r->items[i];
                counter++;
                if (counter%10 == 0)
                {
                    items = (material**)realloc(items, (counter + 10) * sizeof(material*));
                    for (j = counter + 1; j < counter + 10; j++)
                        items[j] = (material*)malloc(sizeof(material));
                }
            }
        sortGen(items, counter, desc, sortByQtyGen);
    }

    items[counter] = createMaterial("","",0,0,0,0);

    return items;
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
    //needs function pointers

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

material** CtrlGetSupplierDescending(Controller* c, char* name, int descending)
{
    material** items = (material**)malloc(10 * sizeof(material*));
    int i, j;
    for (j = 0; j < 10; j++)
        items[j] = (material*)malloc(sizeof(material));

    int counter = 0;

    Repository* r = c->repository;
    for (int i = 0; i < r->length; i++)
    {
        if (strstr(r->items[i]->name, name) != NULL)
        {
            items[counter] = r->items[i];
            counter++;
            if (counter%10 == 0)
            {
                items = (material**)realloc(items, (counter + 10) * sizeof(material*));
                for (j = counter + 1; j < counter + 10; j++)
                    items[j] = (material*)malloc(sizeof(material));
            }
        }
    }

    sortBySupl(items, counter, 1);
    items[counter] = createMaterial("-1","",0,0,0,0);

    return items;
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
    // c->backupSize = 0;
    // c->backupPos = -1;

    c->backupSize++;
    c->backup[++c->backupPos] = *CtrlGetRepository(c);
}

int undoOperation(Controller* c)
{
    if (c->backupPos == -1)
        return 0;

    c->repository = &c->backup[--c->backupPos];
    return 1;

}

int redoOperation(Controller* c)
{
    if (c->backupPos == c->backupSize - 1)
        return 0;
    c->repository = &c->backup[++c->backupPos];
    return 1;
}
