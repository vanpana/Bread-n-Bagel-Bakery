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
    c->backupInstructions = (char**)malloc(100 * sizeof(char*));
    for (int i = 0; i < 100; i++)
        c->backupInstructions[i] = (char*)malloc(sizeof(char));

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
    if (getMaterialPosition(c->repository, name) != 1)
    {
        addToUndoList(c);
        delItem(c->repository, name);
        return 1;
    }
    return 0;

}

int CtrlUpdateItem(Controller* c, char* name, char* supplier, int day, int month,
   int year, int qty)
{
    /*
    Creates material and sends update info to repository
    return: 1 if passed, 0 if failed
    */
    material* m = createMaterial(name, supplier, day, month, year, qty);

    if (getMaterialPosition(c->repository, m->name) != 1)
    {
        addToUndoList(c);
        updateItem(c->repository, m);
        return 1;
    }
    return 0;
}

material** CtrlSort(Controller* c, char* name, char* supplier, int qty, int desc)
{
    /*
    Returns a sorted list.
    If name is not empty and qty = -1, it will return a list with sorted expiring items.
    If name is not empty, it will return a list with sorted items by supplier containing name.
    If name is not empty and supplier is not empty, it will return a list with sorted items containing name from a supplier.
    If name is not empty and qty is not empty, it will return a list with sorted items with quantity greater than qty.
    if name, supplier and qty are not empty, it will return a list with sorted items from a supplier with a quantity greatea than qty.

    If supplier is not empty and qty = -1, it will return a list with sorted expiring items from a supplier.
    If supplier is not empty, it will return a list with items sorted by expiration date with given supplier.
    If supplier, qty are not empty, it will return a list with items short on supply from the given supplier.

    If qty is not empty, it will return a list with items expiring having more in stock than qty.
    */
    material** items = (material**)malloc(10 * sizeof(material*));
    int j, counter = 0;;
    for (j = 0; j < 10; j++)
        items[j] = (material*)malloc(sizeof(material));
    Repository* r = CtrlGetRepository(c);

    if (strlen(name) == 0 && strlen(supplier) == 0 && qty == -1)
        if (qty == -1)
        {
            for (int i = 0; i < r->length; i++)
                if (checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0)
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
            sortGen(items, counter, desc, sortByExpDateGen);
        }

    if (strlen(name) != 0)
    {
        if (qty == -1)
        {
            for (int i = 0; i < r->length; i++)
                if (strstr(r->items[i]->name, name) != NULL && checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0)
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
            sortGen(items, counter, desc, sortByExpDateGen);
        }
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
            sortGen(items, counter, desc, sortBySuplGen);
        }
        if (strlen(supplier) != 0 && qty == 0)
            //sort by name from supplier
        {
            for (int i = 0; i < r->length; i++)
                if (strcmp(r->items[i]->supplier, supplier) == 0)
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
        if (qty == -1)
        {
            for (int i = 0; i < r->length; i++)
                if (strstr(r->items[i]->supplier, supplier) != NULL && checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0)
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
            sortGen(items, counter, desc, sortByExpDateGen);
        }

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
            sortGen(items, counter, desc, sortByExpDateGen);
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
            if (checkPastDate(getExpiration(r->items[i]), getCurrentDate()) == 0 && r->items[i]->qty > qty)
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
    printf("%d, %d\n", c->backupPos, c->backupSize);
}

int undoOperation(Controller* c)
{
    if (c->backupPos == -1)
        return 0;

    c->repository = &c->backup[--c->backupPos];
    printf("%d, %d\n", c->backupPos, c->backupSize);
    return 1;

}

int redoOperation(Controller* c)
{
    if (c->backupPos == c->backupSize - 1)
        return 0;
    c->repository = &c->backup[++c->backupPos];
    printf("%d, %d\n", c->backupPos, c->backupSize);
    return 1;
}

// void addToUndoList(Controller* c, int mode, material* m)
// {
//     // 1 = add, so it deletes the items (if exists, add to list a delete and add, if not, add a delete and NOTHING)
//     // 2 = update, so it deletes the item and replaces with the previous one (add to list a delete and add)
//     // 3 = delete, so it brings back the item (add an add and NOTHING)
//     char* temp = (char*)malloc(sizeof(char));
//     sprintf(temp, "%s,%s,%d,%d,%d,%d", m->name, m->supplier, itoa(m->day), itoa(m->month), itoa(m->year), itoa(m->qty));
//
//     if (getMaterialPosition(m->name) == -1 || mode == 3)
//         strcpy(c->backupInstructions[backupPos+2], "Nothing");
//     else
//         // strcpy(c->backupInstructions[backupPos+2], m->name);
//         strcpy(c->backupInstructions[backupPos+2], temp);
//     strcpy(c->backupInstructions[backupPos+1], temp);
//     backupPos += 2;
//     backupSize += 2;
// }
//
// int undoOperation(Controller* c)
// {
//     if (backupPos < 1)
//         return 0;
//     material* m = strToMaterial(backupInstructions[backupPos-1]);
//     if (strcmp(backupInstructions[backupPos-1], "Nothing" != 0)
//         delItem(c->repository, m->name);
//     addItem(m);
//     backupPos -=2;
//     return 1;
// }
//
// int redoOperation(Controller* c)
// {
//     if (backupPos > backupSize - 2)
//         return 0;
//     material* m = strToMaterial(backupInstructions[backupPos+1]);
//     delItem(c->repository, m->name);
//     if (strcmp(backupInstructions[backupPos+2], "Nothing" != 0)
//         addItem(m);
//     return 1;
//
// }
