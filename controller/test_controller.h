#include "../domain/entities.h"
#include "../repository/repository.h"
#include "../controller/controller.h"
#include <assert.h>
#include <stdlib.h>

Controller* initController();
void test_CtrlGetLength();
void test_CtrlAddItem();
void test_CtrlUpdateItem();
void test_CtrlExpiredMaterialsByName();
void test_CtrlExpiredMaterialsBySupplier();
void test_CtrlExpiredMaterialsByQty();
void test_CtrlShortOnSupply();
void runCtrlTests();
