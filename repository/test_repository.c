#include "../domain/entities.h"
#include "../repository/repository.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Repository* initRepository()
{
    Repository* r = (Repository*)malloc(sizeof(Repository));
    r->length = 0;
    addItem(r, createMaterial("0.5L Water", "Aqua Carpatica", 01, 01, 2020, 1000));
    return r;
}

void test_findMaterialByName()
{
    Repository* r = initRepository();

    assert(findMaterialByName(r, "0.5L Water") == r->items[0]);
}

void test_getMaterialPosition()
{
    Repository* r = initRepository();

    assert(getMaterialPosition(r, "0.5L Water") == 0);
}

void test_getLength()
{
    Repository* r = initRepository();

    assert(getLength(r) == 1);
}

void test_addItem()
{
    Repository* r = initRepository();

    addItem(r, createMaterial("White Bread", "Panemar", 11, 03, 2017, 100));
    assert(getLength(r) == 2);

    addItem(r, createMaterial("White Bread", "Panemar", 11, 03, 2017, 101));
    assert(r->items[1]->qty == 201);
}

void test_delItem()
{
    Repository* r = initRepository();

    addItem(r, createMaterial("White Bread", "Panemar", 11, 03, 2017, 100));
    delItem(r, "White Bread");
    assert(getLength(r) == 1);
}

void test_updateItem()
{
    Repository* r = initRepository();

    updateItem(r, createMaterial("0.5L Water", "Aquz Carpatica", 01, 01, 2020, 1000));
    assert(strcmp(getSupplier(r->items[0]), "Aquz Carpatica") == 0);
}

void runRepoTests()
{
    test_findMaterialByName();
    test_getMaterialPosition();
    test_getLength();
    test_addItem();
    test_delItem();
    test_updateItem();

}
