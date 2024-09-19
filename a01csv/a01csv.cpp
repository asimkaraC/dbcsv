#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#pragma warning (disable: 4996)

#define STRING_SIZE 30
#define BUFFER_SIZE 100
#define INPUT_SIZE 3
#define MAX_ROWS 40
#define READ 1
#define ADD 2
#define UPDATE 3
#define DELETE 4
#define EXIT 5

void read(char* filename);
void add(char* filename, struct Inventory* addedItem);
void update(char* filename, int productID, struct Inventory* itemToUpdate);
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
	char filename[] = "inventory - Copy.csv";	

	int menuInput = 0;
	int idInput = 0;
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
			Inventory* itemToAdd = createItem(21, (char*)"Running Shoes", (char*)"Footwear", 220, 179.99);
			add(filename, itemToAdd);
			printf("Product ID %d - added\n\n", itemToAdd->productID);
			free(itemToAdd);
		}
		else if (menuInput == UPDATE)
		{
			Inventory* itemToUpdate = createItem(7, (char*)"Sunglasses", (char*)"Accessories", 8, 349);
			update(filename, itemToUpdate->productID, itemToUpdate);
			printf("Product ID %d - updated\n\n", itemToUpdate->productID);
			free(itemToUpdate);
		}
		else if (menuInput == DELETE)
		{
			/*printf("Please enter the Product ID of the item that will be deleted: ");
			fgets(inputBuffer, sizeof(inputBuffer), stdin);
			if (sscanf_s(inputBuffer, "%d", &idInput) == 1)
			{
				idInput = atoi(inputBuffer);
				deleteItem(filename, idInput);
				printf("\n");
			}*/

		}

	} while (menuInput != 5);

	return 0;
}

struct Inventory* createItem(int id, char* name, char* category, int quantity, float price)
{
	Inventory* newItem = (Inventory*)malloc(sizeof(Inventory));
	if (newItem == NULL)
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}

	newItem->name = (char*)malloc(STRING_SIZE * sizeof(char));
	newItem->category = (char*)malloc(STRING_SIZE * sizeof(char));

	if ((newItem->name == NULL) || (newItem->category == NULL))
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		newItem->productID = id;
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

	fgets(buffer, sizeof(buffer), pFile); // Skip the header row

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

void add(char* filename, Inventory* itemToAdd)
{
	FILE* pFile = fopen(filename, "a");
	if (pFile == NULL)
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	fprintf(pFile, "%d,%s,%s,%d,%.2f\n", itemToAdd->productID, itemToAdd->name, itemToAdd->category, itemToAdd->quantity, itemToAdd->price);

	if (fclose(pFile) != 0)
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}
}

void update(char* filename, int productID, struct Inventory* itemToUpdate)
{
	FILE* pFile = fopen(filename, "r");
	if (pFile == NULL)
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	Inventory* ItemByLine = (Inventory*)malloc(sizeof(Inventory) * MAX_ROWS);
	if (ItemByLine == NULL)
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE] = { 0 };
	int line = 0;

	fgets(buffer, sizeof(buffer), pFile);

	while ((fgets(buffer, sizeof(buffer), pFile) != NULL) && (line < MAX_ROWS))
	{
		ItemByLine[line].name = (char*)malloc(sizeof(char) * STRING_SIZE);
		ItemByLine[line].category = (char*)malloc(sizeof(char) * STRING_SIZE);

		if ((ItemByLine[line].name == NULL) || (ItemByLine[line].category == NULL))
		{
			printf("Error allocating memory\n");
			exit(EXIT_FAILURE);
		}

		if (sscanf_s(buffer, "%d,%30[^,],%30[^,],%d,%f", &ItemByLine[line].productID, ItemByLine[line].name, STRING_SIZE,
			ItemByLine[line].category, STRING_SIZE, &ItemByLine[line].quantity, &ItemByLine[line].price) == 5)
		{
			if (ItemByLine[line].productID == itemToUpdate->productID)
			{
				free(ItemByLine[line].name);
				free(ItemByLine[line].category);

				ItemByLine[line].name = (char*)malloc(sizeof(char) * STRING_SIZE);
				ItemByLine[line].category = (char*)malloc(sizeof(char) * STRING_SIZE);

				strcpy(ItemByLine[line].name, itemToUpdate->name);
				strcpy(ItemByLine[line].category, itemToUpdate->category);
				ItemByLine[line].quantity = itemToUpdate->quantity;
				ItemByLine[line].price = itemToUpdate->price;
			}
		}

		line++;
	}

	if (fclose(pFile) != 0)
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}

	FILE* overwrittenFile = fopen(filename, "w");
	if (overwrittenFile == NULL)
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	strcpy(buffer, "");
	int row = 0;

	fprintf(overwrittenFile, "Product ID,Name,Category,Quantity,Price\n");

	while (row < line)
	{
		fprintf(overwrittenFile, "%d,%s,%s,%d,%.2f\n", ItemByLine[row].productID, ItemByLine[row].name, ItemByLine[row].category, ItemByLine[row].quantity, ItemByLine[row].price);		

		free(ItemByLine[row].name);
		free(ItemByLine[row].category);

		row++;
	}	

	free(ItemByLine);

	if (fclose(overwrittenFile) != 0)
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}	
}

void deleteItem(char* filename, int productID)
{
	FILE* pFile = fopen(filename, "r");
	if (pFile == NULL)
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}


}
