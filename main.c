#include "repository/repository.h"
#include "controller/controller.h"
#include "ui/console.h"
#include "common/util.h"
#include <stdio.h>

int main()
{
    printf("Started test running...\n");
    runAllTests();
    printf("All tests passed!\n");

    Repository* repo = createRepository();
    Controller* ctrl = createController(repo);

    CtrlAddItem(ctrl, "0.5L Water", "Aqua Carpatica", 01, 02, 2029, 1000);
    CtrlAddItem(ctrl, "1L Water", "Borsec", 01, 03, 2017, 100);
    CtrlAddItem(ctrl, "1.5L Water", "Aqua Carpatica", 01, 01, 2019, 1003);
    CtrlAddItem(ctrl, "1L Mineral Water", "Aqua Carpatica", 01, 03, 2025, 1001);
    CtrlAddItem(ctrl, "5L Water", "Dorna", 01, 03, 2017, 67);
    CtrlAddItem(ctrl, "White Bread", "Panemar", 11, 03, 2017, 100);
    CtrlAddItem(ctrl, "Strudel", "Panemar", 20, 03, 2017, 76);
    CtrlAddItem(ctrl, "Cuib", "La casa", 14, 03, 2017, 10);

    Console* ui = createConsole(ctrl);
    runApp(ui);
    printf("See you soon! :)\n\n");
    printf("\n---------------\n");
}
