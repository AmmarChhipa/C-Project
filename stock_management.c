/*
add update name & update price
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// variable declaration
FILE *new;
FILE *sales_data;
char a[50], stock[100];
char arr[50][20], data, dump, choice[10], sitem[30], sale_item[20];
int i = 0, j, amt[20], k = 0, count = 1, srno, n, sqty, sale_qty, sale_list, Flag = 0;
float pur[20], sale[20], p, s;
int item_loc; // sale item location
char mark1[] = "+++++++++++====================+++++++++++";

// Function prototypes for purchase
void sale_data();
void display();
void display_name();
void display_price();
void display_all();
void display_sellData();
void display_sellDataAsPerDate();
void core();
void see();
void add();
void delete();
void delete_item();
void deduct_qty();
void update();
void update_qty();
void update_all();
void update_name();
void update_price();

// Function to handle sales data
void sale_data()
{
	char line[500][50], currentDate[50], fileDate[50];
	int dateFound = 0; // Flag to check if the current date already exists
	char end[] = "\nEND\n";
	char mark2[] = "---------------------------\nTotal:\n";
	int total_sales = 0;
	float profit = 0.0;

	sales_data = fopen("sale_data.txt", "a+"); // Open the file in append mode
	if (sales_data == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		rewind(sales_data); // Rewind to the beginning of the file
		// getting todays date from the system
		time_t current_time = time(NULL);
		struct tm *local_time = localtime(&current_time);
		sprintf(currentDate, "Date: %02d/%02d/%d\n", local_time->tm_mday, local_time->tm_mon + 1, local_time->tm_year + 1900);
		// Read through the file
		sale_list = 0; // used for storing data 'line' array
		int space = 0; // for storing space index
		int flag = 0;

		// reading the sale data file
		while (fgets(line[sale_list], sizeof(line[sale_list]), sales_data) != NULL)
		{
			if (strncmp(line[sale_list], "Date: ", 5) == 0) // Check if the line starts with "Date: "
			{
				if (strncmp(currentDate, line[sale_list], 16) == 0) // Compare the dates
				{
					dateFound = 1; // Current date already exists
				}
			}
			if (dateFound == 1)
			{
				// converting item name into small case
				for (int i = strlen(line[sale_list]); i >= 0; i--) // reverse loop
				{
					// finding for space which will be used to make difference between qty and item name
					if (line[sale_list][i] == ' ')
					{
						space = i; // storing the space index in space variable
						break;
					}
				}

				if (space != 0 && strncmp(line[sale_list], "END", 2) != 0)
				{
					for (int i = 0; i < space; i++)
					{
						sale_item[i] = line[sale_list][i];
					}
					sale_item[space] = '\0';

					// strncmp = alows to compare using there length
					if (strcasecmp(sale_item, sitem) == 0) // searching if the item name is already present
					{
						sale_qty = atoi(&line[sale_list][space + 1]);		  // converting the qty into int
						sale_qty = sale_qty + sqty;							  // adding the current selled qty with previous selled qty
						sprintf(line[sale_list], "%s %d\n", sitem, sale_qty); // written the updated qty in array
						flag = 1;											  // flag for showing the item was already present
					}
				}
				// if there will be no record of current item then new line will be add
				// the new line will be enterd at the position of END
				else if (strncmp(line[sale_list], "END", 2) == 0 && flag == 0) // finding END
				{
					sprintf(line[sale_list], "%s %d", sitem, sqty); // overwriting END with new item
					sale_list++;									// increment array to add END
					strcpy(line[sale_list], end);					// writing end in array
				}

				// updating the 'total' part
				// for total sale
				if (strncmp(line[sale_list], "Total sales", 10) == 0) // searching for 'Total sales'
				{
					for (int i = strlen(line[sale_list]); i >= 0; i--) // reverse loop
					{
						if (line[sale_list][i] == ' ') // searching for space
						{
							space = i;
							break;
						}
					}
					total_sales = atoi(&line[sale_list][space + 1]);			// converting the previous total sales into int
					total_sales = total_sales + sqty;							// adding current sale
					sprintf(line[sale_list], "Total sales: %d\n", total_sales); // storing updated total sales in array
				}

				// for total profit
				if (strncmp(line[sale_list], "Total profit", 11) == 0) // sarching for Total profit
				{
					for (int i = strlen(line[sale_list]); i >= 0; i--)
					{
						if (line[sale_list][i] == ' ')
						{
							space = i;
							break;
						}
					}
					profit = atof(&line[sale_list][space + 1]); // coverting string into float
					profit = profit + (sqty * (sale[item_loc] - pur[item_loc]));
					sprintf(line[sale_list], "Total profit: %.2f\n", profit);
				}
			}
			sale_list++;
		}

		// If the current date does not exist
		if (dateFound == 0)
		{
			sale_list = 0; // reseting array to 0
			fprintf(sales_data, "\n%s\n", mark1);
			fprintf(sales_data, "\n%s", currentDate);
			fprintf(sales_data, "\n%s %d", sitem, sqty);
			sscanf(sitem, "%s", sale_item);
			sale_qty = sqty;
			fprintf(sales_data, "%s", end);
			fprintf(sales_data, "%s", mark2);
			total_sales = total_sales + sqty;
			profit = total_sales * (sale[item_loc] - pur[item_loc]);
			fprintf(sales_data, "Total sales: %d\nTotal profit: %.2f\n", total_sales, profit);
			fprintf(sales_data, "\n%s", mark1);
		}
	}
	fclose(sales_data);

	// rewriting the updated data into the file
	if (dateFound == 1)
	{
		sales_data = fopen("sale_data.txt", "w");
		if (sales_data == NULL)
		{
			printf("ERROR: File not found\n");
		}
		else
		{
			// writing data into the file
			for (int i = 0; i < sale_list; i++)
			{
				fprintf(sales_data, "%s", line[i]);
			}
			fclose(sales_data);
		}
	}
}

// Display items (Commonly used in code to see items)
void display()
{
	printf("________________________________________________________________________\n");
	printf("\nEnter 'n' to see stock item name and its quantity");
	printf("\nEnter 'p' to see stock item name and its price");
	printf("\nEnter 'a' to see all (stock name, quantity, purchase & sales price)");
	printf("\nEnter 's' to see sell data");
	printf("\nEnter 'd' to see sell data as per date");
Display_choice:
	printf("\nEnter your choice: \n");
	scanf(" %s", choice);
	printf("\n________________________________________________________________________\n\n");
	if (strlen(choice) > 1)
	{
		strcpy(choice, "x");
	}
	switch (choice[0])
	{
	case 'n':
	case 'N':
		display_name();
		break;
	case 'p':
	case 'P':
		core();
		display_price();
		break;
	case 'a':
	case 'A':
		display_all();
		break;
	case 's':
	case 'S':
		display_sellData();
		break;
	case 'd':
	case 'D':
		display_sellDataAsPerDate();
		break;
	default:
		printf("\nERROR: Invalid choice!!\n");
		goto Display_choice;
	}
	printf("________________________________________________________________________\n");
}

// Display stock item name and its quantity (Commonly used in code to see items)
void display_name()
{
	new = fopen("items.txt", "a+");
	if (new == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		count = 1;
		int i = 1;
		while ((fgets(a, sizeof(a), new)) != NULL)
		{
			if (count % 2 != 0)
			{
				printf("%d. %s", i, a);
				i++;
			}
			count++;
		}
		fclose(new);
	}
}

// To display name, quantity and price
void display_price()
{
	new = fopen("items.txt", "a+");
	if (new == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		count = 1;
		int i = 1;
		while ((fgets(a, sizeof(a), new)) != NULL)
		{
			if (count % 2 != 0)
			{
				printf("%d. %s", i, a);
				printf("sale price %.2f\n", sale[i - 1]);
				i++;
			}
			count++;
		}
		fclose(new);
	}
}

// Display all stock items
void display_all()
{
	new = fopen("items.txt", "a+");
	if (new == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		count = 1;
		int i = 1;
		while ((fgets(a, sizeof(a), new)) != NULL)
		{
			if (count % 2 != 0)
			{
				printf("%d. %s", i, a);
				i++;
			}
			else
			{
				printf("%s", a);
			}
			count++;
		}
		fclose(new);
		printf("\n");
	}
}

// Display sale data from sales data
void display_sellData()
{
	new = fopen("sale_data.txt", "r");
	if (new == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		char line[100];
		while ((fgets(line, sizeof(line), new)) != NULL)
		{
			printf("%s", line);
		}
		fclose(new);
		printf("\n");
	}
}

// Display sale data from sales data as per date
void display_sellDataAsPerDate()
{
	char date[20];
	int y, m, d;
Year:
	printf("Enter year: ");
	scanf(" %[^\n]s", date); // getting year
	int count = 0;
	for (int i = 0; i < strlen(date); i++)
	{
		if (!isdigit(date[i])) // searching char
		{
			count = 1;
			break;
		}
	}
	if (count == 1)
	{
		printf("ERROR: Invalid Input. Only digits are allowed.....\n");
		goto Year;
	}
	else
	{
		sscanf(date, " %d", &y); // converting string to int
	}
Month:
	printf("Enter month (in numeric form): ");
	scanf(" %[^\n]s", date); // getting year
	count = 0;
	for (int i = 0; i < strlen(date); i++)
	{
		if (!isdigit(date[i])) // searching char
		{
			count = 1;
			break;
		}
	}
	if (count == 1)
	{
		printf("ERROR: Invalid Input. Only digits are allowed.....\n");
		goto Month;
	}
	else
	{
		sscanf(date, " %d", &m); // converting string to int
	}

Day:
	printf("Enter day: ");
	scanf(" %[^\n]s", date); // getting year
	count = 0;
	for (int i = 0; i < strlen(date); i++)
	{
		if (!isdigit(date[i])) // searching char
		{
			count = 1;
			break;
		}
	}
	if (count == 1)
	{
		printf("ERROR: Invalid Input. Only digits are allowed.....\n");
		goto Day;
	}
	else
	{
		sscanf(date, " %d", &d); // converting string to int
	}

	sprintf(date, "Date: %02d/%02d/%04d\n", d, m, y);
	new = fopen("sale_data.txt", "r");
	if (new == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		char line[100];
		int flag = 0;
		while ((fgets(line, sizeof(line), new)) != NULL)
		{
			if (strncmp(line, date, 16) == 0)
			{
				printf("\n%s\n", mark1);
				flag = 1;
			}
			if (flag == 1)
			{
				printf("%s", line);
			}
			if (strncmp(line, mark1, 5) == 0 && flag == 1)
			{
				break;
			}
		}
		if (flag == 0)
		{
			printf("\nERROR: No data found for the given date........\n");
		}
		fclose(new);
	}
}
// Core function (Commonly used in code to see items)
void core()
{
	new = fopen("items.txt", "r");
	if (new == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		count = 1;
		while ((fgets(a, sizeof(a), new)) != NULL)
		{
			int len = 0;
			if (count % 2 != 0)
			{
				for (int j = strlen(a); j >= 0; j--)
				{
					if (a[j] == ' ')
					{
						len = j;
						break;
					}
				}
				for (int i = 0; i < len; i++)
				{
					arr[k][i] = a[i];
				}
				arr[k][len] = '\0';
				amt[k] = atoi(&a[len + 1]);
				k++;
			}
			else
			{
				char trash[10];
				sscanf(a, "%s %f %s %f", trash, &pur[i], trash, &sale[i]);
				i++;
			}
			n = i;
			count++;
		}
		fclose(new);
	}
}

// case s = see in purchase function
void see()
{
	display();
}

// case a = Add item in purchase function
void add()
{
	scanf("%c", &dump);
	printf("\n------------------------------------------------------------\n\n");
	core();
	new = fopen("items.txt", "a+");
	if (new == NULL)
	{
		printf("ERROR: File not found\n");
	}
	else
	{
		int comp = 1;
		char s_qty[10], str[10];
	Add_item:
		printf("Enter the name of the stock item\n");
		scanf("%[^\n]s", stock); // getting stock item and quantity from user
		for (int i = 0; i < n; i++)
		{
			// comparing input string and stock items name for item availability
			comp = strcasecmp(arr[i], stock);
			if (comp == 0)
			{
				printf("\nERROR: Item already available in stock!!");
				while ((getchar()) != '\n')
					; // Clearing stock data
				goto Add_item;
			}
		}
	Add_qty:
		printf("Enter the quantity of the stock item\n");
		scanf(" %[^\n]s", s_qty); // getting quantity
		int count = 0;
		for (int i = 0; i < strlen(s_qty); i++)
		{
			if (!isdigit(s_qty[i])) // searching any char
			{
				count = 1;
				break;
			}
		}
		if (count == 1)
		{
			printf("ERROR: Invalid quantity. Only digits are allowed.\n");
			goto Add_qty;
		}
		else
		{
			fprintf(new, "\n%s %s", stock, s_qty); // uploding item and quantity to text file
		}
	Purchase:
		printf("Enter purchase price:\n");
		scanf(" %[^\n]s", str); // Taking input in string
		int decimal = 0;		// decimal (.) count
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (isdigit(str[i]) == 0) // checking for any char
			{
				if (str[i] == '.' && decimal == 0) // If it's not a digit
				{
					decimal++; // Found the first decimal point
				}
				else
				{
					printf("ERROR: Invalid Input\n"); // if it's an invalid character
					while ((getchar()) != '\n')
						; // Clearing purchase data
					goto Purchase;
				}
			}
		}
		sscanf(str, "%f", &p); // converting string to float
	Sale_price:
		// same method for sale price
		printf("Enter sale price:\n");
		scanf(" %[^\n]s", str); // Taking input in string
		decimal = 0;			// decimal (.) count
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (isdigit(str[i]) == 0) // checking for any char
			{
				if (str[i] == '.' && decimal == 0) // If it's not a digit
				{
					decimal++; // Found the first decimal point
				}
				else
				{
					printf("ERROR: Invalid Input\n"); // if it's an invalid character
					while ((getchar()) != '\n')
						; // Clearing purchase data
					goto Sale_price;
				}
			}
		}
		sscanf(str, "%f", &s); // converting string to float

		// comparing sale price and purchase price
		if (s < p)
		{
			printf("ERROR: Sale price should be greater than purchase price\n");
			printf("Pls re-enter both the prices.......\n");
			while ((getchar()) != '\n')
				; // Clearing purchase data
			goto Purchase;
		}

		fprintf(new, "\npurchase %.2f sale %.2f", p, s); // uploding purchase & sale price to text file
		printf("\nDetail entered sucesssfully!");
		fclose(new);
		n = n + 1; // No. of items increased
		printf("\n\n------------------------------------------------------------\n\n");
		i=0;
		k=0;
	}
}

// case d = Delete item in purchase function
void delete()
{
	printf("\n**********************************************\n\n");
	printf("Press 'i' to delete whole item\n");
	printf("Press 'q' to deduct  quantity\n");
Delete_choice:
	printf("Enter your choice:\n");
	scanf(" %s", choice);
	if (strlen(choice) > 1)
	{
		strcpy(choice, "x");
	}
	switch (choice[0])
	{
	case 'i':
	case 'I':
		delete_item();
		break;
	case 'q':
	case 'Q':
		deduct_qty();
		break;
	default:
		printf("ERROR: Invalid choice. Please try again.\n");
		goto Delete_choice;
	}
}

// for deletetion of items
void delete_item()
{
	printf("\n\n------------------------------------------------------------\n\n");
	display_all();
	printf("\n------------------------------------------------------------\n");
	core();
Dele:
	printf("\nEnter the Sr.No. of stock item which you want to delete: \n");
	char trash;
	if (scanf("%d%c", &srno, &trash) == 2 && trash == '\n')
	{
		if (srno > n || srno < 1)
		{
			printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
			goto Dele;
		}
		for (i = srno - 1; i < n - 1; i++)
		{
			amt[i] = amt[i + 1];
			strcpy(arr[i], arr[i + 1]);
			pur[i] = pur[i + 1];
			sale[i] = sale[i + 1];
		}
		n = n - 1; // No of item - 1 as one item is deleted
		new = fopen("items.txt", "w");
		for (i = 0; i < n; i++)
		{
			fprintf(new, "%s %d", arr[i], amt[i]);
			fprintf(new, "\npurchase %.2f sale %.2f", pur[i], sale[i]);
			if (i != n - 1)
			{
				fprintf(new, "\n");
			}
		}
		fclose(new);
		printf("\n------------------------------------------------------------\n\n");
		for (i = 0; i < n; i++)
		{
			printf("%d. %s %d\n", i + 1, arr[i], amt[i]);
			printf("purchase: %.2f sale: %.2f\n", pur[i], sale[i]);
		}
		printf("\n------------------------------------------------------------\n");
		printf("\nStock item deleted sucesssfully!!!\n");
		i = 0;
		k = 0;
	}
	else
	{
		printf("ERROR: Invalid input\n");
		while ((getchar()) != '\n')
			;
		goto Dele;
	}
}

// for deducting quantity of items
void deduct_qty()
{
	printf("\n------------------------------------------------------------\n");
	display_name();
	printf("\n------------------------------------------------------------\n");
	core();
Deduct_qty:
	printf("\nEnter the Sr.No. of stock item from which you want to deduct qty: \n");
	char trash;
	if (scanf("%d%c", &srno, &trash) == 2 && trash == '\n')
	{
		if (srno > n || srno < 1)
		{
			printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
			goto Deduct_qty;
		}
		else
		{
			char s_qty[10];
		D_qty:
			printf("\nHow much qty u want to deduct from stock: \n");
			scanf(" %[^\n]s", s_qty); // getting quantity
			int count = 0;
			for (int i = 0; i < strlen(s_qty); i++)
			{
				if (!isdigit(s_qty[i])) // searching any char
				{
					count = 1;
					break;
				}
			}
			if (count == 1)
			{
				printf("ERROR: Invalid quantity. Only digits are allowed.....\n");
				goto D_qty;
			}
			else
			{
				int temp_qty;
				sscanf(s_qty, "%d", &temp_qty); // converting string to int
				if (temp_qty > amt[srno - 1])
				{
					printf("ERROR: Insufficient quantity\nPlease re-enter.....\n");
					goto D_qty;
				}
				amt[srno - 1] = amt[srno - 1] - temp_qty;
			}
			printf("\n------------------------------------------------------------\n\n");
			new = fopen("items.txt", "w");
			for (i = 0; i < n; i++)
			{
				printf("%d. %s %d\n", i + 1, arr[i], amt[i]);
				fprintf(new, "%s %d\n", arr[i], amt[i]);
				fprintf(new, "purchase %.2f sale %.2f", pur[i], sale[i]); // adding pur sale without update
				if (i != n - 1)
				{
					fprintf(new, "\n");
				}
			}
			fclose(new);
			printf("\n------------------------------------------------------------\n\n");
			printf("\nItem quantity deducted successfully!\n");
			i = 0;
			k = 0;
		}
	}
	else
	{
		printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
		while ((getchar()) != '\n')
			;
		goto Deduct_qty;
	}
}
// case a = update quantity
void update_qty()
{
	printf("\n------------------------------------------------------------\n");
	display_name();
	printf("\n------------------------------------------------------------\n");
	core();
Update_qty:
	printf("\nEnter the Sr.No. of stock item which you want to update: \n");
	char trash;
	// validation for Sr. No.
	if (scanf("%d%c", &srno, &trash) == 2 && trash == '\n')
	{
		if (srno > n || srno < 1)
		{
			printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
			// while ((getchar()) != '\n')
			// 	;
			goto Update_qty;
		}
		else
		{
			char s_qty[10];
		Updt_qty:
			printf("\nEnter the new quantity of the stock item: \n");
			scanf(" %s", s_qty); // getting quantity
			int count = 0;
			for (int i = 0; i < strlen(s_qty); i++)
			{
				if (!isdigit(s_qty[i])) // searching any char
				{
					count = 1;
					break;
				}
			}
			if (count == 1)
			{
				printf("ERROR: Invalid quantity. Only digits are allowed.\n");
				goto Updt_qty;
			}
			else
			{
				int temp_qty;
				sscanf(s_qty, "%d", &temp_qty); // converting string to int
				amt[srno - 1] = amt[srno - 1] + temp_qty;
			}
			printf("\n------------------------------------------------------------\n\n");
			new = fopen("items.txt", "w");
			for (i = 0; i < n; i++)
			{
				printf("%d. %s %d\n", i + 1, arr[i], amt[i]);
				fprintf(new, "%s %d\n", arr[i], amt[i]);
				fprintf(new, "purchase %.2f sale %.2f", pur[i], sale[i]); // adding pur sale without update
				if (i != n - 1)
				{
					fprintf(new, "\n");
				}
			}
			fclose(new);
			printf("\n------------------------------------------------------------\n\n");
			printf("\nItem quantity updated successfully!\n\n");
			i = 0;
			k = 0;
		}
	}
	else
	{
		printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
		while ((getchar()) != '\n')
			;
		goto Update_qty;
	}
}

// case b = update name and quantity
void update_name()
{
	printf("\n------------------------------------------------------------\n");
	display_name();
	printf("\n------------------------------------------------------------\n");
	core();
Update_name:
	printf("\nEnter the Sr.No. of stock item which you want to update: \n");
	char trash;
	// validation for Sr. No.
	if (scanf("%d%c", &srno, &trash) == 2 && trash == '\n')
	{
		if (srno > n || srno < 1)
		{
			printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
			goto Update_name;
		}
		else
		{
			char s_qty[10];
			printf("\nEnter the new name of the stock item: \n");
			scanf(" %[^\n]", arr[srno - 1]); // item new name
		Updt_item:
			printf("\nEnter the new quantity of that item: \n");
			scanf(" %[^\n]s", s_qty); // getting quantity
			int count = 0;
			for (int i = 0; i < strlen(s_qty); i++)
			{
				if (!isdigit(s_qty[i])) // searching any char
				{
					count = 1;
					break;
				}
			}
			if (count == 1)
			{
				printf("ERROR: Invalid quantity. Only digits are allowed.\n");
				goto Updt_item;
			}
			else
			{
				sscanf(s_qty, "%d", &amt[srno - 1]); // converting string to int
			}
			printf("\n------------------------------------------------------------\n\n");
			new = fopen("items.txt", "w");
			for (i = 0; i < n; i++)
			{
				printf("%d. %s %d\n", i + 1, arr[i], amt[i]);
				fprintf(new, "%s %d\n", arr[i], amt[i]);
				fprintf(new, "purchase %.2f sale %.2f", pur[i], sale[i]); // adding pur sale without update
				if (i != n - 1)
				{
					fprintf(new, "\n");
				}
			}
			fclose(new);
			printf("\n------------------------------------------------------------\n\n");
			printf("\nStock updated successfully!\n");
			i = 0;
			k = 0;
		}
	}
	else
	{
		printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
		while ((getchar()) != '\n')
			;
		goto Update_name;
	}
}

// case c = update sale and purchase prices
void update_price()
{
	printf("\n------------------------------------------------------------\n");
	display_all();
	printf("\n------------------------------------------------------------\n");
	core();
Update_price:
	printf("\nEnter The Sr.No. of stock item which you want to update: \n");
	char trash;
	// validation for Sr. No.
	if (scanf("%d%c", &srno, &trash) == 2 && trash == '\n')
	{
		if (srno > n || srno < 1)
		{
			printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
			goto Update_price;
		}
		else
		{
			char str[10];
		Updt_pur_price:
			printf("\nEnter the new purchase price: \n");
			scanf(" %[^\n]s", str); // Taking input in string
			int decimal = 0;		// decimal (.) count
			for (int i = 0; str[i] != '\0'; i++)
			{
				if (isdigit(str[i]) == 0) // checking for any char
				{
					if (str[i] == '.' && decimal == 0) // If it's not a digit
					{
						decimal++; // Found the first decimal point
					}
					else
					{
						printf("ERROR: Invalid Input\n"); // if it's an invalid character
						while ((getchar()) != '\n')
							; // Clearing purchase data
						goto Updt_pur_price;
					}
				}
			}
			sscanf(str, "%f", &pur[srno - 1]); // converting string to float
											   // Now taking sale price
		Updt_sale_price:
			printf("\nEnter the new sale price: \n");
			scanf(" %[^\n]s", str); // Taking input in string
			decimal = 0;			// decimal (.) count
			for (int i = 0; str[i] != '\0'; i++)
			{
				if (isdigit(str[i]) == 0) // checking for any char
				{
					if (str[i] == '.' && decimal == 0) // If it's not a digit
					{
						decimal++; // Found the first decimal point
					}
					else
					{
						printf("ERROR: Invalid Input\n"); // if it's an invalid character
						while ((getchar()) != '\n')
							; // Clearing purchase data
						goto Updt_sale_price;
					}
				}
			}
			sscanf(str, "%f", &sale[srno - 1]); // converting string to float

			// comparing purchase price and sale price
			if (pur[srno - 1] > sale[srno - 1])
			{
				printf("ERROR: Purchase price cannot be greater than or equal to sale price\n");
				printf("Pls re-enter both the prices\n");
				goto Updt_pur_price;
			}
			printf("\n------------------------------------------------------------\n\n");
			new = fopen("items.txt", "w");
			for (i = 0; i < n; i++)
			{
				fprintf(new, "%s %d\n", arr[i], amt[i]);
				printf("%d. %s %d\n", i + 1, arr[i], amt[i]);
				fprintf(new, "purchase %.2f sale %.2f", pur[i], sale[i]); // adding pur sale without update
				printf("purchase %.2f sale %.2f\n", pur[i], sale[i]);
				if (i != n - 1)
				{
					fprintf(new, "\n");
				}
			}
			fclose(new);
			printf("\n------------------------------------------------------------\n\n");
			printf("\nStock updated successfully!\n");
			i = 0;
			k = 0;
		}
	}
	else
	{
		printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
		while ((getchar()) != '\n')
			;
		goto Update_price;
	}
}

// case d = update all (name, quantity & pricing)
void update_all()
{
	printf("\n------------------------------------------------------------\n");
	display_all();
	printf("\n------------------------------------------------------------\n");
	core();
Update_all:
	printf("\nEnter the Sr.No. of stock item which you want to update: \n");
	char trash;
	// validation for Sr. No.
	if (scanf("%d%c", &srno, &trash) == 2 && trash == '\n')
	{
		if (srno > n || srno < 1)
		{
			printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
			goto Update_all;
		}
		else
		{
			char s_qty[10], str[10];
			printf("\nEnter the new name of the stock item: \n");
			scanf(" %[^\n]", arr[srno - 1]); // item new name
		Updt_qty_d:
			printf("\nEnter the new quantity of that item: \n");
			scanf(" %[^\n]s", s_qty); // getting quantity
			int count = 0;
			for (int i = 0; i < strlen(s_qty); i++)
			{
				if (!isdigit(s_qty[i])) // searching any char
				{
					count = 1;
					break;
				}
			}
			if (count == 1)
			{
				printf("ERROR: Invalid quantity. Only digits are allowed.\n");
				goto Updt_qty_d;
			}
			else
			{
				sscanf(s_qty, "%d", &amt[srno - 1]); // converting string to int
			}
		Updt_pur_price_d:
			printf("\nEnter the new purchase price: \n");
			// scanf("%f", &pur[srno - 1]); // item new purchase price
			scanf(" %[^\n]s", str); // Taking input in string
			int decimal = 0;		// decimal (.) count
			for (int i = 0; str[i] != '\0'; i++)
			{
				if (isdigit(str[i]) == 0) // checking for any char
				{
					if (str[i] == '.' && decimal == 0) // If it's not a digit
					{
						decimal++; // Found the first decimal point
					}
					else
					{
						printf("ERROR: Invalid Input\n"); // if it's an invalid character
						while ((getchar()) != '\n')
							; // Clearing purchase data
						goto Updt_pur_price_d;
					}
				}
			}
			sscanf(str, "%f", &pur[srno - 1]); // converting string to float
		Updt_sale_price_d:
			printf("\nEnter the new sale price: \n");
			scanf(" %[^\n]s", str); // Taking input in string
			decimal = 0;			// decimal (.) count
			for (int i = 0; str[i] != '\0'; i++)
			{
				if (isdigit(str[i]) == 0) // checking for any char
				{
					if (str[i] == '.' && decimal == 0) // If it's not a digit
					{
						decimal++; // Found the first decimal point
					}
					else
					{
						printf("ERROR: Invalid Input\n"); // if it's an invalid character
						while ((getchar()) != '\n')
							; // Clearing purchase data
						goto Updt_sale_price_d;
					}
				}
			}
			sscanf(str, "%f", &sale[srno - 1]); // converting string to float
			printf("\n------------------------------------------------------------\n\n");
			new = fopen("items.txt", "w");
			for (i = 0; i < n; i++)
			{
				printf("%d. %s %d\n", i + 1, arr[i], amt[i]);
				fprintf(new, "%s %d\n", arr[i], amt[i]);
				fprintf(new, "purchase %.2f sale %.2f", pur[i], sale[i]); // uploading updated sales and purchase prices
				if (i != n - 1)
				{
					fprintf(new, "\n");
				}
			}
			fclose(new);
			printf("\n------------------------------------------------------------\n\n");
			printf("\nStock updated successfully!\n");
			i = 0;
			k = 0;
		}
	}
	else
	{
		printf("ERROR: Sr.No. not found\nPlease re-enter.....\n");
        while ((getchar())!= '\n')
            ;
        goto Update_all;
	}
}

// case p = purchase 2nd main menu
void purchase()
{
Pur_choice:
	// Option selection
	printf("\n+++++++++~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+++++++++\n");
	printf("\nAlter stock items as per your purhase!!\n\n");
	printf("To only see the stock enter 's'\n");
	printf("To add new item enter 'a'\n");
	printf("To delete item enter 'd'\n");
	printf("To update item enter 'u'\n");
	printf("To exit program enter 'e'\n");
	printf("Enter your choice:\n");
	scanf(" %s", choice);
	if (strlen(choice) > 1)
	{
		strcpy(choice, "x");
	}
	switch (choice[0])
	{
	case 's':
	case 'S':
		see(); // See available items
		break;
	case 'a':
	case 'A':
		add(); // Add items
		break;
	case 'd':
	case 'D':
		delete (); // Delete items
		break;
	case 'u':
	case 'U':
		update(); // Update items
		break;
	case 'e':
	case 'E':
		printf("\nThank you for using this program.....\n");
		printf("Have a nice day.....\n");
		printf("\n+++++++++++============================+++++++++++\n");
		exit(0); // Exit program
	default:
		printf("\nERROR: Invalid choice!!\n");
		goto Pur_choice;
	}
	printf("\n+++++++++~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+++++++++\n\n");
Pur_option:
	if (Flag != 1)
	{
		printf("\nTo purchase OR update OR delete more items enter 'p'");
		printf("\nReturn to main menu enter 'm'\n");
		scanf(" %s", choice);
		if (strlen(choice) > 1)
		{
			strcpy(choice, "x");
		}
		switch (choice[0])
		{
		case 'p':
		case 'P':
			goto Pur_choice;
		case 'm':
		case 'M':
			printf("\n+++++++++++====================+++++++++++\n");
			break;
		default:
			printf("\nERROR: Invalid choice.....\n");
			printf("Please re-enter....\n");
			goto Pur_option;
		}
	}
	Flag = 0;
}

// case u = update
void update()
{
	printf("\n+++++++++~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+++++++++\n");
updt_choice:
	printf("\nIf u want to update quantity 'q' ");
	printf("\nIf u want to update item name and quantity press 'n' ");
	printf("\nIf u want to update purchase and sale price press 'p' ");
	printf("\nIf u want to update both please enter 'a'\n");
	printf("Enter your choice:\n");
	scanf(" %s", choice);
	if (strlen(choice) > 1)
	{
		strcpy(choice, "x");
	}
	switch (choice[0])
	{
	case 'q':
	case 'Q':
		update_qty();
		break;
	case 'n':
	case 'N':
		update_name(); // update item name and quantity
		break;
	case 'p':
	case 'P':
		update_price(); // update purchase and sale price
		break;
	case 'a':
	case 'A':
		update_all(); // update all name, quantity, purchase price and sale price
		break;
	default:
		printf("\nERROR: Invalid choice!!\n");
		goto updt_choice;
	}
	printf("\n+++++++++~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+++++++++\n\n");
Updt_option:
	printf("\nTo update more enter 'u'");
	printf("\nReturn to main menu enter 'm'\n");
	scanf(" %s", choice);
	if (strlen(choice) > 1)
	{
		strcpy(choice, "x");
	}
	switch (choice[0])
	{
	case 'u':
	case 'U':
		goto updt_choice;
	case 'm':
	case 'M':
		Flag = 1;
		break;
	default:
		printf("\nERROR: Invalid choice.....\n");
		printf("Please re-enter....\n");
		goto Updt_option;
	}
}

// case s = sales
void sales()
{
	core();
	printf("\n------------------------------------------------------------\n");
	display_price();
	printf("\n------------------------------------------------------------\n");
Sell:
	printf("\nEnter item name:\n");
	scanf(" %[^\n]", sitem); // Scanning item name
	char s_qty[10];
	printf("\nEnter quantity how much you want to sell:\n");
	scanf(" %[^\n]s", s_qty); // getting quantity
	int count = 0;
	for (int i = 0; i < strlen(s_qty); i++)
	{
		if (!isdigit(s_qty[i])) // searching char
		{
			count = 1;
			break;
		}
	}
	if (count == 1)
	{
		printf("ERROR: Invalid quantity. Only digits are allowed.....\n");
		goto Sell;
	}
	else
	{
		sscanf(s_qty, " %d", &sqty); // converting string to int
	}
	int comp = 1, itemAvailability = 0;
	// searching item in the stock
	for (int i = 0; i < n; i++)
	{
		// comparing input string and stock items name for item availability
		comp = strcasecmp(arr[i], sitem);
		if (comp == 0)
		{
			item_loc = i;
			// checking quantity availability
			if (amt[i] >= sqty)
			{
				amt[i] = amt[i] - sqty;
				printf("\nItem sold successfully!\n");
				itemAvailability = 1; // item available flag
				break;
			}
			else
			{
				printf("\n%s = %d Qty", arr[i], amt[i]);
				printf("\nERROR: Not enough stock available.....\n");
				printf("Please re-enter....\n");
				goto Sell;
			}
		}
	}

	// for item not available
	if (comp != 0)
	{
		printf("\nERROR: Item not found.....\n");
		printf("Please re-enter....\n");
		goto Sell;
	}

	// calling sales file
	sale_data();

	// After selling item diducting item from stock & displaying stock
	printf("\n------------------------------------------------------------\n\n");
	printf("Items available in stock:\n");
	new = fopen("items.txt", "w");
	for (int i = 0; i < n; i++)
	{
		printf("%d. %s %d\n", i + 1, arr[i], amt[i]);
		// updating details in txt file
		fprintf(new, "%s %d\n", arr[i], amt[i]);				  // Updating items & Quantity
		fprintf(new, "purchase %.2f sale %.2f", pur[i], sale[i]); // adding pur sale without update
		if (i != n - 1)											  // n=total no of items it starts from 0 so n-1
		{
			fprintf(new, "\n");
		}
	}
	fclose(new);
	printf("\n------------------------------------------------------------\n\n");

	// Sell more item or return to main menu
Choose:
	printf("\nTo enter sell enter 's'");
	printf("\nReturn to main menu enter 'm'\n");
	scanf(" %s", choice);
	if (strlen(choice) > 1)
	{
		strcpy(choice, "x");
	}
	switch (choice[0])
	{
	case 's':
	case 'S':
		goto Sell;
	case 'm':
	case 'M':
		printf("\n+++++++++++====================+++++++++++\n");
		break;
	default:
		printf("\nERROR: Invalid choice.....\n");
		printf("Please re-enter....\n");
		goto Choose;
	}
}

int main()
{
	// Main menu
	printf("\n+++++++++++====== WELCOME ========+++++++++++\n");
	while (1)
	{
		i = 0;
		k = 0;
		n = 0;
		count = 1;
		printf("\nPress 'p' to purchase OR update OR delete items\n");
		printf("Press 's' to sell items\n");
		printf("Press 'd' to display stock items\n");
		printf("Press 'q' to quit\n");
		printf("Enter your choice:\n");
		scanf(" %s", choice);
		if (strlen(choice) > 1)
		{
			strcpy(choice, "x");
		}
		switch (choice[0])
		{
		case 'p':
		case 'P':
			purchase(); // Purchase items
			break;
		case 's':
		case 'S':
			sales(); // Sales
			break;
		case 'd':
		case 'D':
			scanf("%c", &dump);
			display(); // Items will be displayed
			break;
		case 'q':
		case 'Q':
			printf("\nThank you for using this program.....\n");
			printf("Have a nice day.....\n");
			printf("\n+++++++++++============================+++++++++++\n");
			exit(0); // exit
		default:
			printf("\nERROR: Invalid choice!!\n");
		}
	}
}