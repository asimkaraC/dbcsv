#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#pragma warning (disable: 4996)

/* CONSTANTS */
#define STRING_SIZE 30
#define BUFFER_SIZE 100
#define INPUT_SIZE 3
#define MAX_ROWS 40
#define READ 1
#define ADD 2
#define UPDATE 3
#define DELETE 4
#define EXIT 5

/* FUNCTION PROTOTYPES */
struct Inventory* createItem(int id, char* name, char* category, int quantity, float price);
void read(char* filename);
void add(char* filename, struct Inventory* addedItem);
void update(char* filename, int productID, struct Inventory* itemToUpdate);
void deleteItem(char* filename, int productID);

/* STRUCT */
typedef struct Inventory
{
	int productID;
	char* name;
	char* category;
	int quantity;
	float price;
} Inventory;

/* MAIN */
int main(void)
{
	char filename[] = "inventory.csv";	

	int menuInput = 0; // Integer value which will be converted from a string to perform the desired CRUD operation from the menu
	int idInput = 0; // Integer value which will be converted from a string for the delete function
	char menuBuffer[INPUT_SIZE] = { 0 }; // String to get input for menu selection
	char inputBuffer[STRING_SIZE] = { 0 }; // String to get input from the user

	do
	{
		printf("1- Display inventory data\n");
		printf("2- Add a new product\n");
		printf("3- Update a product\n");
		printf("4- Delete a product\n");
		printf("5- Exit\n");

		printf("Selection: ");

		/* Getting user input for the initial menu option selection */
		fgets(inputBuffer, sizeof(inputBuffer), stdin); // Get the user's input from the keyboard
		if (sscanf_s(inputBuffer, "%d", &menuInput) == 1) // Validate the input format
		{
			menuInput = atoi(inputBuffer); // Convert the input string to an integer
			printf("\n");
		}

		/* Compare inputs to perform the correct operations */
		if (menuInput == READ)
		{
			read(filename);
			printf("\n"); // Display a blank line before displaying the menu after the reading is done
		}
		else if (menuInput == ADD)
		{
			Inventory* itemToAdd = createItem(21, (char*)"Running Shoes", (char*)"Footwear", 220, 179.99); // Instantiate an inventory item which will be appended to the file
			add(filename, itemToAdd);
			printf("Product ID %d - added\n\n", itemToAdd->productID); // Display feedback
			free(itemToAdd); // Free up allocated memory
		}
		else if (menuInput == UPDATE)
		{
			Inventory* itemToUpdate = createItem(7, (char*)"Sunglasses", (char*)"Accessories", 8, 349); // Instantiate an inventory item which will be updated on the file
			update(filename, itemToUpdate->productID, itemToUpdate);
			printf("Product ID %d - updated\n\n", itemToUpdate->productID); // Display feedback
			free(itemToUpdate); // Free up allocated memory
		}
		else if (menuInput == DELETE)
		{
			printf("Please enter the Product ID of the item that will be deleted: ");
			fgets(inputBuffer, sizeof(inputBuffer), stdin); // Get user input to delete item with the provided ID 
			if (sscanf_s(inputBuffer, "%d", &idInput) == 1) // Validate the input format
			{
				idInput = atoi(inputBuffer); // Convert the input string to an integer
				deleteItem(filename, idInput); 
				printf("\n"); // Display a blank line before displaying the menu after the reading is done
			}
		}

	} while (menuInput != 5); // Display the menu and perform operations until the user decides to exit

	return 0;
}

/* 
* FUNCTION			: createItem
* DESCRIPTION		: This function creates a new dynamically memory allocated struct 
* PARAMETERS		: int id			  : Product ID
*					  char* name		  : Product Name
*					  char* category	  : Product Category
*					  int quantity		  : Product Quantity
*					  float price		  : Product Price
* RETURNS			: struct*			  : The new struct
*/
struct Inventory* createItem(int id, char* name, char* category, int quantity, float price)
{
	Inventory* newItem = (Inventory*)malloc(sizeof(Inventory)); // Allocate memory for a new entry to the file
	if (newItem == NULL) // Error checking
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}

	newItem->name = (char*)malloc(STRING_SIZE * sizeof(char)); // Allocate memory for the item's name string
	newItem->category = (char*)malloc(STRING_SIZE * sizeof(char)); // Allocate memory for the item's category string

	if ((newItem->name == NULL) || (newItem->category == NULL)) // Validate the memory allocation
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	else // Assign the argument values to the item
	{
		newItem->productID = id;
		strcpy(newItem->name, name);
		strcpy(newItem->category, category);
		newItem->quantity = quantity;
		newItem->price = price;
	}

	return newItem;
}

/*
* FUNCTION			: read
* DESCRIPTION		: This function reads, parses and displays the contents of a csv file
* PARAMETERS		: char* filename       : Name of the file for file I/O operations
* RETURNS			: Nothing
*/
void read(char* filename)
{
	FILE* pFile = fopen(filename, "r"); // Open the file for reading
	if (pFile == NULL) // File I/O error checking
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE] = { 0 }; // Buffer to parse the lines from the file
	Inventory Item = { 0 }; // Initialize the inventory struct

	fgets(buffer, sizeof(buffer), pFile); // Skip the header row

	printf("Product ID\tName\t\t\t\tCategory\t\tQuantity\tPrice\n\n"); // Display the header row with enough spaces for each product to fit in

	while (fgets(buffer, BUFFER_SIZE, pFile) != NULL) // Read each line from the file until there are no lines
	{
		Item.name = (char*)malloc(STRING_SIZE * sizeof(char)); // Allocate memory for the item's name string
		Item.category = (char*)malloc(STRING_SIZE * sizeof(char)); // Allocate memory for the item's category string

		if ((Item.name == NULL) || (Item.category == NULL)) // Validate the memory allocation
		{
			printf("Error allocating memory\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			if (sscanf_s(buffer, "%d,%30[^,],%30[^,],%d,%f", &Item.productID, Item.name, STRING_SIZE,
				Item.category, STRING_SIZE, &Item.quantity, &Item.price) == 5) // Check if lines are properly formatted
			{
				printf("%-16d%-32s%-24s%-16d%.2f\n", Item.productID, Item.name, Item.category, Item.quantity, Item.price); // Display each line
			}
			else
			{
				printf("Invalid data\n");
			}
		}
		
		free(Item.name); // Free up allocated memory
		free(Item.category); // Free up allocated memory
	}

	if (fclose(pFile) != 0) // Error checking for closing the file
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}
}

/*
* FUNCTION			: read
* DESCRIPTION		: This function reads, parses and displays the contents of a csv file
* PARAMETERS		: char* filename       : Name of the file for file I/O operations
*					  Inventory* itemToAdd : The struct that was returned from the createItem function to add to the end of the file
* RETURNS			: Nothing
*/
void add(char* filename, Inventory* itemToAdd)
{
	FILE* pFile = fopen(filename, "a"); // Open the file to append to the file
	if (pFile == NULL) // File I/O error checking
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	fprintf(pFile, "%d,%s,%s,%d,%.2f\n", itemToAdd->productID, itemToAdd->name, itemToAdd->category, itemToAdd->quantity, itemToAdd->price); // Write to the end of the file

	if (fclose(pFile) != 0) // Error checking for closing the file
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}
}

/*
* FUNCTION			: createItem
* DESCRIPTION		: This function updates the contents of a line using an array of structs and writes the updated struct back to the file
* PARAMETERS		: char* filename      : Name of the file for file I/O operations
*					  int productID		  : Product ID to find the corresponding product to update
*					  struct Inventory* itemToUpdate	  : Product Category
* RETURNS			: Nothing
*/
void update(char* filename, int productID, struct Inventory* itemToUpdate)
{
	FILE* pFile = fopen(filename, "r"); // Open the file for reading
	if (pFile == NULL) // File I/O error checking
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	Inventory* ItemByLine = (Inventory*)malloc(sizeof(Inventory) * MAX_ROWS); // Allocate memory for the array of structs
	if (ItemByLine == NULL) // Validate memory allocation
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE] = { 0 }; // Buffer to parse the lines from the file
	int line = 0; // Keep track of the current line (array index) 

	fgets(buffer, sizeof(buffer), pFile); // Skip the header row

	/* Keep reading until there are no lines or the max possible array index is reached */
	while ((fgets(buffer, sizeof(buffer), pFile) != NULL) && (line < MAX_ROWS)) 
	{
		ItemByLine[line].name = (char*)malloc(sizeof(char) * STRING_SIZE); // Allocate memory for the item's name string
		ItemByLine[line].category = (char*)malloc(sizeof(char) * STRING_SIZE); // Allocate memory for the category's name string

		if ((ItemByLine[line].name == NULL) || (ItemByLine[line].category == NULL)) // Validate memory allocation
		{
			printf("Error allocating memory\n");
			exit(EXIT_FAILURE);
		}

		/* Read and validate each line, assign the read values to the current index of the array of structs	*/
		if (sscanf_s(buffer, "%d,%30[^,],%30[^,],%d,%f", &ItemByLine[line].productID, ItemByLine[line].name, STRING_SIZE,
			ItemByLine[line].category, STRING_SIZE, &ItemByLine[line].quantity, &ItemByLine[line].price) == 5) 
		{
			if (ItemByLine[line].productID == itemToUpdate->productID) // Find the item that will be updated
			{
				free(ItemByLine[line].name); // Delete the item's name information
				free(ItemByLine[line].category); // Delete the item's category information

				ItemByLine[line].name = (char*)malloc(sizeof(char) * STRING_SIZE); // Allocate memory for the new name
				ItemByLine[line].category = (char*)malloc(sizeof(char) * STRING_SIZE); // Allocate memory for the new category

				strcpy(ItemByLine[line].name, itemToUpdate->name); // Put the updated item's name in place of the old item in the struct
				strcpy(ItemByLine[line].category, itemToUpdate->category); // Put the updated item's category in place of the old item in the struct
				ItemByLine[line].quantity = itemToUpdate->quantity; // Put the updated item's quantity in place of the old item in the struct
				ItemByLine[line].price = itemToUpdate->price; // Put the updated item's price in place of the old item in the struct
			}
		}

		line++; // Increment to read the next line
	}

	if (fclose(pFile) != 0) // Close the file and do error checking
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}

	FILE* overwrittenFile = fopen(filename, "w"); // Open the file again to write the updated line from the struct to the file 
	if (overwrittenFile == NULL) // Open the file and do error checking
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	strcpy(buffer, ""); // Clear the buffer
	int row = 0; // Another variable to keep track of the current line

	fprintf(overwrittenFile, "Product ID,Name,Category,Quantity,Price\n"); // Write the header row to the file

	while (row < line) // Loop until the end of the file 
	{
		/* Write back the values from the array of structs to the file */
		fprintf(overwrittenFile, "%d,%s,%s,%d,%.2f\n", ItemByLine[row].productID, ItemByLine[row].name, ItemByLine[row].category, ItemByLine[row].quantity, ItemByLine[row].price);		

		free(ItemByLine[row].name); // Free up allocated memory 
		free(ItemByLine[row].category); // Free up allocated memory

		row++; // Increment the row (array index)
	}	

	free(ItemByLine); // Free up allocated memory

	if (fclose(overwrittenFile) != 0) // Close the file and do error checking
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}	
}

/*
* FUNCTION			: deleteItem
* DESCRIPTION		: This function deletes a line using an array of structs and writes the updated struct back to the file
* PARAMETERS		: char* filename      : Name of the file for file I/O operations
*					  int productID		  : Product ID to find the corresponding product to delete
* RETURNS			: Nothing
*/
void deleteItem(char* filename, int productID)
{
	FILE* pFile = fopen(filename, "r"); // Open the file for reading
	if (pFile == NULL) // File I/O error checking
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	Inventory* ItemByLine = (Inventory*)malloc(sizeof(Inventory) * MAX_ROWS); // Allocate memory for the array of structs
	if (ItemByLine == NULL) // Validate memory allocation
	{
		printf("Error allocating memory\n");
		exit(EXIT_FAILURE);
	}

	char buffer[BUFFER_SIZE] = { 0 }; // Buffer to read each line from the file
	int line = 0; // Keep track of the array index

	fgets(buffer, sizeof(buffer), pFile); // Skip the header row

	/* Keep reading until there are no lines or the max possible array index is reached */
	while ((fgets(buffer, sizeof(buffer), pFile) != NULL) && (line < MAX_ROWS)) 
	{
		ItemByLine[line].name = (char*)malloc(sizeof(char) * STRING_SIZE); // Allocate memory for the item's name string
		ItemByLine[line].category = (char*)malloc(sizeof(char) * STRING_SIZE); // Allocate memory for the category's name string;

		if ((ItemByLine[line].name == NULL) || (ItemByLine[line].category == NULL)) // Validate memory allocation
		{
			printf("Error allocating memory\n");
			exit(EXIT_FAILURE);
		}

		/* Read and validate each line, assign the read values to the current index of the array of structs	*/
		if (sscanf_s(buffer, "%d,%30[^,],%30[^,],%d,%f", &ItemByLine[line].productID, ItemByLine[line].name, STRING_SIZE,
			ItemByLine[line].category, STRING_SIZE, &ItemByLine[line].quantity, &ItemByLine[line].price) == 5)
		{
			if (ItemByLine[line].productID == productID) // Find the matching product ID and delete the line
			{
				free(ItemByLine[line].name); // Delete the item's name information
				free(ItemByLine[line].category); // Delete the item's category information
			}
			else
			{
				line++; // Increment the array index if there are no matching product IDs found
			}
		}
	}

	if (fclose(pFile) != 0) // Close the file and do error checking
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}

	FILE* fileAfterDeletion = fopen(filename, "w"); // Open the file again to write back to the file without the deleted line 
	if (fileAfterDeletion == NULL) // File I/O error checking
	{
		printf("Error opening the file\n");
		exit(EXIT_FAILURE);
	}

	strcpy(buffer, ""); // Clear the buffer
	int row = 0; // Another variable to keep track of the current line

	fprintf(fileAfterDeletion, "Product ID,Name,Category,Quantity,Price\n"); // Write the header row to the file

	while (row < line) // Loop until the end of the file 
	{
		/* Write back the values from the array of structs to the file */
		fprintf(fileAfterDeletion, "%d,%s,%s,%d,%.2f\n", ItemByLine[row].productID, ItemByLine[row].name, ItemByLine[row].category, ItemByLine[row].quantity, ItemByLine[row].price);

		free(ItemByLine[row].name); // Free up allocated memory 
		free(ItemByLine[row].category); // Free up allocated memory 

		row++; // Increment the array index
	}

	free(ItemByLine); // Free up allocated memory

	if (fclose(fileAfterDeletion) != 0) // Close the file and do error checking
	{
		printf("Error closing the file\n");
		exit(EXIT_FAILURE);
	}
}
