#include "console.h"
#include "stdio.h"
#include "../controller/controller.h"
#include "../common/util.h"
#include <stdlib.h>
#include <string.h>

Console* createConsole(Controller* c)
{
    /*
    Constructor for console struct.
    */
    Console* ui = (Console*)malloc(sizeof(Console));
    ui->controller = c;

    return ui;
}

void destroyConsole(Console* ui)
{
    destroyController(ui->controller);
    free(ui);
}

void printMenu()
{
    printf("\n---------------\n");
    printf("1. Add an item.\n");
    printf("2. Delete an item.\n");
    printf("3. Update an item.\n");
    printf("4. Search expired items.\n");
    printf("5. Search expired items past some quantity.\n");
    printf("6. Search short on supply materials by supplier.\n");
    printf("7 (bad activity). See all materials from supplier sorted by expiration month.\n");
    printf("activity. See all materials with a name sorted by supplier.\n");
    printf("8. Undo\n");
    printf("9. Redo\n");
    printf("print. Print all items.\n");
    printf("exit. Exit the program.\n");
}

void uiListAllMaterials(Console* ui)
{
    /*
    Lists all the materials existent in the Repository
    */
    Repository* r = CtrlGetRepository(ui->controller);
    if (r->length == 0)
    printf("There's no material added yet!\n");
    else
    {
    for (int i = 0; i < r->length; i++)
      printMaterial(r->items[i]);
    }
}

void uiAddMaterials(Console* ui)
{
    /*
    Gets input from the user and validates, then sends the info to the controller
        to add the material.
    */
    size_t size;
    char* name = (char*)malloc(sizeof(char)), * supplier = (char*)malloc(sizeof(char));
    int qty;
    int* date = (int*)malloc(sizeof(int));

    printf("Input name: ");
    getline(&name, &size, stdin); //dummy
    name = getString();

    printf("Input supplier: ");
    supplier = getString();

    printf("Input expiration date (dd.mm.yyyy): ");
    date = getDateIntegers();
    while (validateDate(date[0], date[1], date[2]) == 0 || date[0] == -1)
    {
        printf("Invalid date\n");

        printf("Input expiration date (dd.mm.yyyy): ");
        date = getDateIntegers();
    }

    printf("Input quantity: ");
    qty = getInteger();
    while (qty == -1)
    {
        printf("Invalid integer\n");

        printf("Input quantity: ");
        qty = getInteger();
    }

    CtrlAddItem(ui->controller, name, supplier, date[0], date[1], date[2], qty);

}

void uiDeleteMaterials(Console* ui)
{
    /*
    Gets input from the user and validates, then sends the info to the controller
        to delete the material.
    */
    size_t size;
    char* name = (char*)malloc(sizeof(char)), * supplier = (char*)malloc(sizeof(char));

    printf("Input name: ");
    getline(&name, &size, stdin); //dummy
    name = getString();

    if (CtrlDeleteItem(ui->controller, name) == 0)
        printf("Item not found!\n");
    printf("Item deleted!\n");
}

void uiUpdateMaterials(Console* ui)
{
    /*
    Gets input from the user and validates, then sends the info to the controller
        to update the material.
    */
    size_t size;
    char* name = (char*)malloc(sizeof(char)), * supplier = (char*)malloc(sizeof(char));
    int qty;
    int* date = (int*)malloc(sizeof(int));

    printf("Input name: ");
    getline(&name, &size, stdin); //dummy
    name = getString();

    printf("Input supplier: ");
    supplier = getString();

    printf("Input expiration date (dd.mm.yyyy): ");
    date = getDateIntegers();
    while (validateDate(date[0], date[1], date[2]) == 0 || date[0] == -1)
    {
        printf("Invalid date\n");

        printf("Input expiration date (dd.mm.yyyy): ");
        date = getDateIntegers();
    }

    printf("Input quantity: ");
    qty = getInteger();
    while (qty == -1)
    {
        printf("Invalid integer\n");

        printf("Input quantity: ");
        qty = getInteger();
    }

    if (CtrlUpdateItem(ui->controller, name, supplier, date[0], date[1], date[2], qty) == 0)
        printf("Item not found!\n");
    printf("Item updated!\n");
}

void uiGetExpired(Console* ui)
{
    /*
    Gets input from the user and validates, then sends the info to the controller
        to get the expired materials.
    */
    char* needle = (char*)malloc(sizeof(char));

    printf("Input searched string: ");
    scanf("%s", needle); // "-" is for no needle

    char** expiredItems = CtrlExpiredMaterialsByName(ui->controller, needle);

    if (strcmp(needle, "-") != 0)
        printf("\nExpired items containing string \"%s\":\n", needle);
    else printf("\nAll expired items: \n");

    int i = 0;
    while (strcmp(expiredItems[i], "") != 0)
    {
        printf("%s\n", expiredItems[i]);
        i++;
    }
}

void uiGetExpiredByQty(Console* ui)
{
    /*
    Gets input from the user and validates, then sends the info to the controller
        to get the expired materials by quantity.
    */
    int qty = 0;

    printf("Input quantity: ");
    qty = getInteger();

    printf("\nExpired items past quantity %d:\n", qty);
    char** expiredItems = CtrlExpiredMaterialsByQty(ui->controller, qty);

    int i = 0;
    while (strcmp(expiredItems[i], "") != 0)
    {
        printf("%s\n", expiredItems[i]);
        i++;
    }
}

void uiGetShortOnSupply(Console* ui)
{
    /*
    Gets input from the user and validates, then sends the info to the controller
        to get the short on supply materials.
    */
    size_t size = 32;
    int chars;
    char* supplier = (char*)malloc(size * sizeof(char));
    int qty;
    material** shortItems;


    printf("Input supplier (-d if you want descending): ");
    getline(&supplier, &size, stdin);
    //chars = getline(&supplier, &size, stdin);
    getline(&supplier, &size, stdin);

    printf("Input quantity: ");
    qty = getInteger();

    supplier[strlen(supplier) - 1] = '\0';
    if (strstr(supplier, "-d")== NULL)
    {
        if (strstr(supplier, "-") != NULL)
            printf("Invalid Command");
        else
            shortItems = CtrlShortOnSupply(ui->controller, supplier, qty, 0);
    }
    else
    {
        if (supplier[strlen(supplier) - 3] == ' ')
            supplier[strlen(supplier) - 3] = '\0';
        else
            supplier[strlen(supplier) - 2] = '\0';

        shortItems = CtrlShortOnSupply(ui->controller, supplier, qty, 1);
    }


    for (int i = 0; strcmp(shortItems[i]->name, "") != 0; i++)
        printMaterial(shortItems[i]);
}

void uiGetSupplierByExpMonth(Console* ui)
{
    /*
    Gets input from the user and validates, then sends the info to the controller
        to get all the elements from certain supplier.
    */
    size_t size = 32;
    int chars;
    char* supplier = (char*)malloc(size * sizeof(char));
    material** shortItems;


    printf("Input supplier (-a if you want ascending): ");
    getline(&supplier, &size, stdin);
    chars = getline(&supplier, &size, stdin);

    supplier[strlen(supplier) - 1] = '\0';
    if (strstr(supplier, "-a")== NULL)
    {
        if (strstr(supplier, "-") != NULL)
            printf("Invalid Command");
        else
            shortItems = CtrlGetSupplierByExpMonth(ui->controller, supplier, 1);
    }
    else
    {
        if (supplier[strlen(supplier) - 3] == ' ')
            supplier[strlen(supplier) - 3] = '\0';
        else
            supplier[strlen(supplier) - 2] = '\0';

        shortItems = CtrlGetSupplierByExpMonth(ui->controller, supplier, 0);
    }


    for (int i = 0; strcmp(shortItems[i]->name, "") != 0; i++)
        printMaterial(shortItems[i]);
}


void uiGetSupplierDescending(Console* ui)
{
    size_t size;
    material** items;
    char* name = (char*)malloc(sizeof(char));

    printf("Input name: ");
    getline(&name, &size, stdin); //dummy
    name = getString();
    items = CtrlGetSupplierDescending(ui->controller, name, 1);

    for (int i = 0; items[i]->name[0] != '-'; i++)
        printMaterial(items[i]);
}

void uiUndo(Console* ui)
{
    undoOperation(ui->controller);
}

void uiRedo(Console* ui)
{
    redoOperation(ui->controller);
}

char* getCommand()
{
    /*
    Function to get input command.
    */
    printf("Input command: ");
    char* command = (char*)malloc(sizeof(char));
    scanf("%s", command);
    return command;
}

int getInteger()
{
    /*
    Function to convert given string from standard input to integer.
    */
    char* str = (char*)malloc(sizeof(char));
    scanf("%s", str);
    if (strstr(str, " "))
        return -1;
    int integer = atoi(str);
    if (integer <= 0)
        return -1;
    return integer;
}

int* getDateIntegers()
{
    /*
    Function to convert given string from standard input to date type integers.
    */
    char* str = (char*)malloc(sizeof(char)), * temp = (char*)malloc(sizeof(char));
    int* date = (int*)malloc(sizeof(int));
    scanf("%s", str);

    if (strlen(str) != 10 || str[2] != '.' || str[5] != '.')
    {
        date[0] = -1;
        date[1] = -1;
        date[2] = -1;
        return date;
    }

    strcpy(temp, &str[0]);

    date[0] = atoi(temp);
    if (date[0] <= 0)
    {
        date[0] = -1;
        date[1] = -1;
        date[2] = -1;
        return date;
    }

    strcpy(temp, &str[3]);

    date[1] = atoi(temp);
    if (date[1] <= 0)
    {
        date[0] = -1;
        date[1] = -1;
        date[2] = -1;
        return date;
    }

    strcpy(temp, &str[6]);
    date[2] = atoi(temp);
    if (date[1] <= 0)
    {
        date[0] = -1;
        date[1] = -1;
        date[2] = -1;
        return date;
    }

    return date;
}

char* getString()
{
    /*
    Function which gets a string with getline and strips \n.
    */
    size_t size = 32;
    char* str = (char*)malloc(size * sizeof(char));
    getline(&str, &size, stdin);
    str[strlen(str) - 1] = '\0';
    return str;
}

int validateCommand(Console* ui, char* command)
{
    /*
    Function to validate the commands.
    */
    if (strlen(command) > 1)
    {
        if (strcmp(command, "exit") != 0 && strcmp(command, "print") != 0 && strcmp(command, "activity") != 0)
          return 0;

        return 2; //is string
    }
    else
    {
        for (int i = 1; i <= 70; i++)
          if ((int)command[0] - '0'== i)
            return 1;
        return 0;
    }
}

void loop(Console* ui)
{
    /*
    Main program loop.
    */
    while (1)
    {
        printMenu();
        char* cmd = getCommand();
        if (validateCommand(ui, cmd) == 0)
          printf("Invalid command\n");
        else if (validateCommand(ui, cmd) == 2)
        {
            if (strcmp(cmd, "exit") == 0)
                break;
            else if (strcmp(cmd, "print") == 0)
                uiListAllMaterials(ui);
            else uiGetSupplierDescending(ui);
        }
        else
        {
          int command = (int)cmd[0] - '0';

          if (command == 1) uiAddMaterials(ui);
          if (command == 2) uiDeleteMaterials(ui);
          if (command == 3) uiUpdateMaterials(ui);
          if (command == 4) uiGetExpired(ui);
          if (command == 5) uiGetExpiredByQty(ui);
          if (command == 6) uiGetShortOnSupply(ui);
          if (command == 7) uiGetSupplierByExpMonth(ui);
          if (command == 8) uiUndo(ui);
          if (command == 9) uiRedo(ui);
        }
    }
}

void runApp(Console* ui)
{
    /*
    App starting.
    */
    loop(ui);
}
