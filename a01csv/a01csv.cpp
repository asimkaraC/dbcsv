#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#pragma warning (disable: 4996)

#define STRING_SIZE 30
#define BUFFER_SIZE 100
#define INPUT_SIZE 3
#define READ 1
#define ADD 2
#define UPDATE 3
#define DELETE 4
#define EXIT 5

void read(char* filename);
void add(char* filename, struct Inventory* addedItem);
void update(char* filename, int productID, struct Inventory* updatedItem);
void deleteItem(char* filename, int productID);

struct Inventory* createItem(int id, char* name, char* category, int quantity, float price);

typedef struct Inventory
{
	int productID;
	char* name;
	char* category;
	int quantity;
	float price;
} Inventory;

int main(void)
{
	char filename[] = "inventory.csv";	

	int menuInput = 0;
	int idInput = 0;
	char nameInput[STRING_SIZE] = { 0 };
	char categoryInput[STRING_SIZE] = { 0 };
	int quantityInput = 0;
	float priceInput = 0.00;
	char menuBuffer[INPUT_SIZE] = { 0 };
	char inputBuffer[STRING_SIZE] = { 0 };

	do
	{
		printf("1- Display inventory data\n");
		printf("2- Add a new product\n");
		printf("3- Update a product\n");
		printf("4- Delete a product\n");
		printf("5- Exit\n");

		printf("Selection: ");

		fgets(inputBuffer, sizeof(inputBuffer), stdin);
		if (sscanf_s(inputBuffer, "%d", &menuInput) == 1)
		{
			menuInput = atoi(inputBuffer);
			printf("\n");
		}

		if (menuInput == READ)
		{
			read(filename);
			printf("\n");
		}
		else if (menuInput == ADD)
		{
			printf("Please enter the Product ID of the new item: ");
			fgets(inputBuffer, sizeof(inputBuffer), stdin);
			if (sscanf_s(inputBuffer, "%d", &idInput) == 1)
			{
				idInput = atoi(inputBuffer);

				printf("Please enter the Name of the new item: ");
				fgets(inputBuffer, sizeof(inputBuffer), stdin);
				if (sscanf_s(inputBuffer, "%s", nameInput, STRING_SIZE) == 1)
				{
					printf("Please enter the Category of the new item: ");
					fgets(inputBuffer, sizeof(inputBuffer), stdin);
					if (sscanf_s(inputBuffer, "%s", categoryInput, STRING_SIZE) == 1)
					{
						printf("Please enter the Quantity of the new item: ");
						fgets(inputBuffer, sizeof(inputBuffer), stdin);
						if (sscanf_s(inputBuffer, "%d", &quantityInput) == 1)
						{
							quantityInput = atoi(inputBuffer);

							printf("Please enter the Price of the new item: ");
							fgets(inputBuffer, sizeof(inputBuffer), stdin);
							if (sscanf_s(inputBuffer, "%f", &priceInput) == 1)
							{
								priceInput = atof(inputBuffer);

								Inventory* addedItem = createItem(idInput, nameInput, categoryInput, quantityInput, priceInput);
								add(filename, addedItem);
								free(addedItem);
							}
						}
					}
				}
			}
			printf("\n");
		}
		else if (menuInput == UPDATE)
		{
			//update(filename);
			printf("\n");
		}
		else if (menuInput == DELETE)
		{
			printf("Please enter the Product ID of the item that will be deleted: ");
			fgets(inputBuffer, sizeof(inputBuffer), stdin);
			if (sscanf_s(inputBuffer, "%d", &idInput) == 1)
			{
				idInput = atoi(inputBuffer);
				//deleteItem(filename, idInput);
				printf("\n");
			}
		}

	} while (menuInput != 5);
}

struct Inventory* createItem(int id, char* name, char* category, int quantity, float price)
{
	Inventory* newItem = (Inventory*)malloc(sizeof(Inventory));
	if (newItem == NULL)
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}

	newItem->productID = id;
	newItem->name = (char*)malloc(STRING_SIZE * sizeof(char));
	newItem->category = (char*)malloc(STRING_SIZE * sizeof(char));

	if ((newItem->name == NULL) || (newItem->category == NULL))
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		strcpy(newItem->name, name);
		strcpy(newItem->category, category);
		newItem->quantity = quantity;
		newItem->price = price;
	}

	return newItem;
}

void read(char* filename)
{
	FILE* pFile = fopen(filename, "r");
	if (pFile == NULL)
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE] = { 0 };
	Inventory Item = { 0 };

	fgets(buffer, sizeof(buffer), pFile);

	printf("Product ID\tName\t\t\t\tCategory\t\tQuantity\tPrice\n\n");

	while (fgets(buffer, BUFFER_SIZE, pFile) != NULL)
	{
		Item.name = (char*)malloc(STRING_SIZE * sizeof(char));
		Item.category = (char*)malloc(STRING_SIZE * sizeof(char));
		if ((Item.name == NULL) || (Item.category == NULL))
		{
			printf("Error allocating memory\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			if (sscanf_s(buffer, "%d,%30[^,],%30[^,],%d,%f", &Item.productID, Item.name, STRING_SIZE,
				Item.category, STRING_SIZE, &Item.quantity, &Item.price) == 5)
			{
				printf("%-16d%-32s%-24s%-16d%.2f\n", Item.productID, Item.name, Item.category, Item.quantity, Item.price);
			}
			else
			{
				printf("Invalid data\n");
			}
		}
		
		free(Item.name);
		free(Item.category);
	}

	if (fclose(pFile) != 0)
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}
}

void add(char* filename, Inventory* addedItem)
{
	FILE* pFile = fopen(filename, "a");
	if (pFile == NULL)
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	//fprintf(pFile, "%d,%s,%s,%d,%.2f\n", addedItem->productID, addedItem->name, addedItem->category, addedItem->quantity, addedItem->price);
	fprintf(pFile, "%d,%s,%s,%d,%f\n", addedItem->productID, addedItem->name, addedItem->category, addedItem->quantity, addedItem->price);

	if (fclose(pFile) != 0)
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}
}

