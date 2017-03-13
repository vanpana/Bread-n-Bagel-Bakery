#include "repository.h"
#include <stdlib.h>
#include <string.h>

Repository* createRepository()
{
    /*
    Constructor for repository struct.
    */
    Repository* r = (Repository*)malloc(sizeof(Repository));
    r->length = 0;

    return r;
}

void destroyRepository(Repository* r)
{
    /*
    Frees the memory allocated for Repository struct.
    */
    for (int i = 0; i < r->length; i++)
    destroyMaterial(r->items[i]);

    free(r);
}

material* findMaterialByName(Repository* r, char* name)
{
    /*
    Finds material by input name.
    input: name - string
    return: material object OR NULL if no material with input name found
    */
    if (r->length == 0)
        return NULL;
    for (int i = 0; i < r->length; i++)
    if (strcmp(r->items[i]->name, name) == 0)
      return r->items[i];
    return NULL;
}

int getMaterialPosition(Repository* r, char* name)
{
    /*
    Gets position of material by input name.
    input: name - string
    return: material position in repository or -1 if not found
    */
    if (r->length == 0)
    return -1;
    for (int i = 0; i < r->length; i++)
    if (strcmp(r->items[i]->name, name) == 0)
      return i;
    return -1;
}

int getLength(Repository* r)
{
    /*
    Gets the total items stored in Repository.
    return: length - integer
    */
    return r->length;
}

void addItem(Repository* r, material* m)
{
    /*
    Adds an item to the repo. If item with specified name already exists,
        it won't be overridden, but the quantities will be added
    input: m - material
    */
    int materialposition = getMaterialPosition(r, m->name);
    if (materialposition != -1)
        r->items[materialposition] = setQty(r->items[materialposition], m->qty);
    else
    {
        r->items[r->length] = m;
        r->length++;
    }
}

int delItem(Repository* r, char* name)
{
    /*
    Deletes an item by name.
    input: name - string
    return: 1 if found and deleted, 0 if not found
    */
    int materialposition = getMaterialPosition(r, name);
    if (materialposition == -1)
            return 0;

    for (int i = materialposition; i < r->length - 1; i++)
        r->items[i] = r->items[i+1];

    r->length--;
    return 1;
}

int updateItem(Repository* r, material* m)
{
    /*
    Updates an item by name.
    input: m - material
    return: 1 if found and updated, 0 if not found
    */
    int materialposition = getMaterialPosition(r, m->name);
    if (materialposition == -1)
            return 0;

    r->items[materialposition] = m;
    return 1;
}
