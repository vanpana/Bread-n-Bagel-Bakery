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

    CtrlAddItem(ctrl, "0.5L Water", "Aqua Carpatica", 01, 01, 2020, 1000);
    CtrlAddItem(ctrl, "1.5L Water", "Aqua Carpatica", 01, 01, 2020, 1003);
    CtrlAddItem(ctrl, "1L Water", "Aqua Carpatica", 01, 01, 2020, 1001);
    CtrlAddItem(ctrl, "White Bread", "Panemar", 11, 03, 2017, 100);
    CtrlAddItem(ctrl, "Strudel", "Panemar", 20, 03, 2017, 76);

    Console* ui = createConsole(ctrl);
    runApp(ui);
    printf("See you soon! :)\n\n");
    printf("\n---------------\n");
}
