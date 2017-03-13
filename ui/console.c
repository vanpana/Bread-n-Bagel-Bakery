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
    char* name = (char*)malloc(sizeof(char)), * supplier = (char*)malloc(sizeof(char));
    int qty;
    int* date = (int*)malloc(sizeof(int));
    printf("Input name: ");
    scanf("%s", name);

    printf("Input supplier: ");
    scanf("%s", supplier);

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
    char* name = (char*)malloc(sizeof(char)), * supplier = (char*)malloc(sizeof(char));
    printf("Input name: ");
    scanf("%s", name);
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
    char* name = (char*)malloc(sizeof(char)), * supplier = (char*)malloc(sizeof(char));
    int qty;
    int* date = (int*)malloc(sizeof(int));
    printf("Input name: ");
    scanf("%s", name);

    printf("Input supplier: ");
    scanf("%s", supplier);

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
    // fgets(needle, 200, stdin);

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

    // if (strcmp(needle, "-") != 0)
    // {
    //     expiredItems = CtrlExpiredMaterialsBySupplier(ui->controller, needle);
    //     printf("\nExpired items for supplier containing string \"%s\":\n", needle);
    //
    //     i = 0;
    //     while (strcmp(expiredItems[i], "") != 0)
    //     {
    //         printf("%s\n", expiredItems[i]);
    //         i++;
    //     }
    // }
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


    printf("Input supplier (-d if you want descending):");
    getline(&supplier, &size, stdin);
    chars = getline(&supplier, &size, stdin);

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
    int integer = atoi(str);
    if (integer == 0)
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

    strcpy(temp, &str[0]);

    date[0] = atoi(temp);
    if (date[0] == 0)
    {
        date[0] = -1;
        date[1] = -1;
        date[2] = -1;
        return date;
    }

    strcpy(temp, &str[3]);

    date[1] = atoi(temp);
    if (date[1] == 0)
    {
        date[0] = -1;
        date[1] = -1;
        date[2] = -1;
        return date;
    }

    strcpy(temp, &str[6]);
    date[2] = atoi(temp);
    if (date[1] == 0)
    {
        date[0] = -1;
        date[1] = -1;
        date[2] = -1;
        return date;
    }

    return date;
}

int validateCommand(Console* ui, char* command)
{
    /*
    Function to validate the commands.
    */
    if (strlen(command) > 1)
    {
    if (strcmp(command, "exit") != 0 && strcmp(command, "print") != 0)
      return 0;

    return 2; //is string
    }
    else
    {
    for (int i = 1; i <= 6; i++)
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
          else uiListAllMaterials(ui);
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
