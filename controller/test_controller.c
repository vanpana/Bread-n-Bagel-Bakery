#include "../domain/entities.h"
#include "../repository/repository.h"
#include "../controller/controller.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Controller* initController()
{
    Repository* r = createRepository();
    Controller* c = createController(r);
    CtrlAddItem(c, "0.5L Water", "Aqua Carpatica", 01, 01, 2007, 1000);
    return c;
}

void test_CtrlGetLength()
{
    Controller* c = initController();

    assert(CtrlGetLength(c) == 1);
}

void test_CtrlAddItem()
{
    Controller* c = initController();

    CtrlAddItem(c, "White Bread", "Panemar", 11, 03, 2017, 100);
    assert(CtrlGetLength(c) == 2);
}

void test_CtrlUpdateItem()
{
    Controller* c = initController();

    CtrlUpdateItem(c, "0.5L Water", "Aquz Carpatica", 01, 01, 2020, 1000);
    assert(strcmp(getSupplier(c->repository->items[0]), "Aquz Carpatica") == 0);
}

void test_CtrlExpiredMaterialsByName()
{
    Controller* c = initController();
    char** expiredItems = CtrlExpiredMaterialsByName(c, "Water");

    int i = 0;
    while (strcmp(expiredItems[i], "") != 0)
        i++;

    assert(i == 1);
}

void test_CtrlExpiredMaterialsBySupplier()
{
    Controller* c = initController();
    char** expiredItems = CtrlExpiredMaterialsBySupplier(c, "q");

    int i = 0;
    while (strcmp(expiredItems[i], "") != 0)
        i++;

    assert(i == 1);
}

void test_CtrlExpiredMaterialsByQty()
{
    Controller* c = initController();
    char** expiredItems = CtrlExpiredMaterialsByQty(c, 1);

    int i = 0;
    while (strcmp(expiredItems[i], "") != 0)
        i++;

    assert(i == 1);
}

void test_CtrlShortOnSupply()
{
    Controller* c = initController();
    material** shortItems = CtrlShortOnSupply(c, "Aqua Carpatica", 1024, 0);

    int i = 0;
    for (i = 0; strcmp(shortItems[i]->name, "") != 0; i++)
        i = i;

    assert(i == 1);
}

void runCtrlTests()
{
    test_CtrlGetLength();
    test_CtrlAddItem();
    test_CtrlUpdateItem();
    test_CtrlExpiredMaterialsByName();
    test_CtrlExpiredMaterialsBySupplier();
    test_CtrlExpiredMaterialsByQty();
    test_CtrlShortOnSupply();
}
