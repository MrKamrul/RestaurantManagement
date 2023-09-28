#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void readFooditems();
void title();
void menu();
int authentication();
void addFoodItemInfo();
void saveFoodItemInfo();
void viewAllFoodItemInformation();
void updateFoodItemInfo();
void removeFoodInfo();
void createOrder();
void sellReport();

int availableFoodItems = 0;

struct foodItem
{
  char foodName[35], foodCode[20], foodDetails[50];
  float unitPrice;
  int stock;
};

struct foodItem foodItemInfo;
FILE *fileToSave, *tempFile;

struct orderDetails
{
  char foodName[35];
  int amount;
  float price;
};

struct orderDetails orderDetailsInfo;

int main()
{
  readFooditems();
  title();
lebel3:
  if(authentication()==0)
  {
    menu();
  }
  else
  {
    printf("\n\t\t\t\t::\t  Your password is incorrect. Please try again.");
    goto lebel3;
  }

  return 0;
}

void readFooditems()
{
  fileToSave=fopen("fooditeminfo.txt", "r");
  while(fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave))
  {
    availableFoodItems++;
  }
  fclose(fileToSave);
}

void createOrder()
{
  system("cls");
  printf("\n\n\t\t\t\t::\t\t\t====  Order Creation  ====\n\n");
  printf("\t\t\t\t::\t\t\tAvailable food items are:\n\n");
  printf("\t\t\t\t::\t\t\tSerial\t\tFood Name\t\tUnit Price\n");
  int n = 1;
  fileToSave=fopen("fooditeminfo.txt", "r");

  while(fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave))
  {
    printf("\t\t\t\t::\t\t\t%d.\t\t%s\t\t%.2f\n", n, foodItemInfo.foodName, foodItemInfo.unitPrice);
    ++n;
  }

  fclose(fileToSave);
  int orderSerials[100] = {}, orderQuantities[100] = {};
  int size = 0;

  int orderSerial, orderCount;
  char yesno;
  int nItems = 0;
  printf("\n\t\t\t\t::\t\t\tHow many unique items do you want to order? : ");
  scanf("%d", &nItems);
  printf("\t\t\t\t::\t\t\tEnter the serial numbers of the desired orders (with whitespace): ");

  for (int i=0; i < nItems; ++i)
  {
    scanf("%d", &orderSerial);
    orderSerials[i] = orderSerial;
    ++size;
  }

  printf("\t\t\t\t::\t\t\tEnter quantities of each unique items (with whitespace): ");

  for (int i=0; i < nItems; ++i)
  {
    scanf("%d", &orderCount);
    orderQuantities[i] = orderCount;
  }

  float orderPrice[100] = {};
  int orderAmount[100] = {};
  int psize = 0, qsize=1;

  for (int i=0; i < nItems; ++i)
  {
    fileToSave=fopen("fooditeminfo.txt", "r");
    while(fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave))
    {
      if (orderSerials[i] == qsize)
      {
        orderPrice[orderSerials[i] - 1] = foodItemInfo.unitPrice * orderQuantities[i];
        orderAmount[orderSerials[i] - 1] = orderQuantities[i];
        qsize=1;
        break;
      }
      ++qsize;
    }
    fclose(fileToSave);
  }

  printf("\n\n\t\t\t\t::\t\t\t==== INVOICE ====\n\n");
  printf("\t\t\t\t::\t\t\tItem Name\tAmount\t\tPrice\n");
  qsize=1;

  for (int i=0; i < nItems; ++i)
  {
    fileToSave=fopen("fooditeminfo.txt", "r");
    tempFile=fopen("orderDetails.txt", "a");
    while(fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave))
    {
      if (orderSerials[i] == qsize)
      {
        strcpy(orderDetailsInfo.foodName, foodItemInfo.foodName);
        orderDetailsInfo.amount = orderAmount[orderSerials[i] - 1];
        orderDetailsInfo.price = orderPrice[orderSerials[i] - 1];
        fwrite(&orderDetailsInfo, sizeof(struct orderDetails), 1, tempFile);
        printf("\t\t\t\t::\t\t\t%s\t\t%d\t\t%.2f\n", foodItemInfo.foodName, orderAmount[orderSerials[i] - 1], orderPrice[orderSerials[i] - 1]);
        qsize=1;
        break;
      }
      ++qsize;
    }
    fclose(fileToSave);
    fclose(tempFile);
  }

  printf("\t\t\t\t::\t\t\tDue Bill : \t\t\t");
  float totalPrice = 0;
  for (int i=0; i < availableFoodItems; ++i)
  {
    totalPrice += orderPrice[i];
  }
  printf("%.2f\n", totalPrice);

  printf("\n\n\t\t\t\t::\t\t\t Press any key to continue: ");
  getch();
  menu();
}

void sellReport()
{
  system("cls");
  printf("\n\n\t\t\t\t::\t\t\t==== Sell Report ====\n\n");
  printf("\t\t\t\t::\t\t\tItem Name\tAmounts Sold\tReceived Cost\n");
  fileToSave = fopen("orderDetails.txt", "r");
  int totalAmount = 0;
  float totalCost = 0;
  while(fread(&orderDetailsInfo, sizeof(struct orderDetails), 1, fileToSave))
  {
    printf("\t\t\t\t::\t\t\t%s\t\t%d\t\t%.2f\n", orderDetailsInfo.foodName, orderDetailsInfo.amount, orderDetailsInfo.price);
    totalAmount += orderDetailsInfo.amount;
    totalCost += orderDetailsInfo.price;
  }
  fclose(fileToSave);
  printf("\n\t\t\t\t::\t\t\t==== Total %d items sold of cost %.2f taka\n", totalAmount, totalCost);

  printf("\n\n\t\t\t\t::\t\t\t Press any key to continue: ");
  getch();
  menu();
}

void title()
{
  printf("\n\n\n\n\n\n\n\t\t\t\t::::::::::::::::::::==================::::::::::::::::::::");
  printf("\n\t\t\t\t::\t\t\t WELCOME TO\t\t\t::");
  printf("\n\t\t\t\t::\t\t       Take Out Fast Food \t\t::");
  printf("\n\t\t\t\t::::::::::::::::::::==================::::::::::::::::::::");
}

void menu()
{
  system("cls");
  title();
  printf("\n\t\t\t\t::\t\t\t  1. Show All Food Items \n\t\t\t\t::\t\t\t  2. Create Food Item \n\t\t\t\t::\t\t\t  3. Update Food Items \n\t\t\t\t::\t\t\t  4. Delete Food Items \n\t\t\t\t::\t\t\t  5. Create Order and Generate Invoice  \n\t\t\t\t::\t\t\t  6. Show Sell Report \n\n\t\t\t\t::\t\t\t   Please enter your choice (1-6, 'q' to exit) : ");
  char choice;
lebel1:
  scanf("%c", &choice);
  if (choice=='2')
  {
    addFoodItemInfo();
  }
  else if (choice=='1')
  {
    viewAllFoodItemInformation();
  }
  else if (choice=='3')
  {
    updateFoodItemInfo();

  }
  else if (choice=='4')
  {
    removeFoodInfo();
  }
  else if (choice=='5')
  {
    createOrder();
  }
  else if (choice=='6')
  {
    sellReport();
  }
  else if (choice == 'q'){ exit(0); }
  else
  {
    printf("\n\t\t\t\t::\t\t\tInvalid choice. You should enter a choice between 1 to 6.\n\t\t\t\t::\t\t\tPlease enter your choice again (1-6, 'q' to exit) : ");
    goto lebel1;
  }
}

int authentication()
{
  char fixPassword[15]="112233";
  char userPassword[15];
  printf("\n\t\t\t\t::\t\t\t  Enter Password: ");
  fflush(stdin);
  gets(userPassword);
  return strcmp(userPassword, fixPassword);
}

void addFoodItemInfo()
{
  system("cls");
  title();
  printf("\n\t\t\t\t::\t\t\t==== Please fill up all necessary information ====\n");
  printf("\n\t\t\t\t::\t\t\t==== Enter the right information as asked ====");
  printf("\n\n\t\t\t\t::\t\t\tFood Name : ");
  fflush(stdin);
  gets(foodItemInfo.foodName);
  printf("\n\t\t\t\t::\t\t\tFood Code : ");
  fflush(stdin);
  gets(foodItemInfo.foodCode);
  printf("\n\t\t\t\t::\t\t\tFood Details : ");
  fflush(stdin);
  gets(foodItemInfo.foodDetails);
  printf("\n\t\t\t\t::\t\t\tFood Quantity/Stock : ");
  fflush(stdin);
  scanf("%d", &foodItemInfo.stock);
  printf("\n\t\t\t\t::\t\t\tUnite Price : ");
  fflush(stdin);
  scanf("%f", &foodItemInfo.unitPrice);

  saveFoodItemInfo(foodItemInfo, fileToSave);
}

//                 *********++++++++++++++++++++++++++++++++++*******


//  saveFoodInformation

void saveFoodItemInfo(struct foodItem foodItemInfo,FILE *file)
{
  file=fopen("fooditeminfo.txt", "a");
  fwrite(&foodItemInfo, sizeof(struct foodItem), 1, file);
  if(fwrite)
  {
    printf("\n\t\t\t\t::\t\t\tInformation saved successfully.");
    availableFoodItems++;
  }
  else
  {
    printf("\n\t\t\t\t::\t\t\tFailed to save information");
  }
  fclose(file);

  printf("\n\t\t\t\t::\t\t\tDo you want to add another food info? \nPress Y/y for yes and N/n for no:  ");
  char choice;
  fflush(stdin);
  scanf("%c",&choice);
  if(choice == 'Y' || choice =='y')
  {
    addFoodItemInfo();
  }
  else
  {
    menu();
  }
}

void viewAllFoodItemInformation()
{
  system("cls");
  title();
  printf("\n\n\t\t\t\t::\t\t\t==== All Food Items Information ====\n");
  printf("\n\t\t\t\t::\t\t\tFood Name\tFood Code\tDetails\t\tStock\tUnit Price\n\n");
  fileToSave=fopen("fooditeminfo.txt","r");
  while(fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave))
  {
    printf("\t\t\t\t::\t\t\t%s\t\t%s\t\t%s\t\t%d\t\t%.2f\n", foodItemInfo.foodName, foodItemInfo.foodCode, foodItemInfo.foodDetails, foodItemInfo.stock, foodItemInfo.unitPrice);
  }

  fclose(fileToSave);
  printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
  getch();
  menu();
}

///update food info
void updateFoodItemInfo()
{
  system("cls");
  title();
  printf("\n\nA. Update General Info\nB. Update Price & Quantity Info\n\nPLease enter your choice :");
  char choice;
  fflush(stdin);
lebel4:
  scanf("%c",&choice);
  choice=toupper(choice);
  printf("\nPLease enter name of the food : ");
  char search_name[30];
  fflush(stdin);
  gets(search_name);
  //Define the selection
  switch(choice)
  {
  case 'A':
    fileToSave = fopen("fooditeminfo.txt","r+");
    int found=0;
    while(fread(&foodItemInfo,sizeof(struct foodItem),1,fileToSave))
    {
      if(strcmpi(foodItemInfo.foodName,search_name)==0)
      {
        found = 1;
        printf("\nEnter New Name: ");
        fflush(stdin);
        gets(foodItemInfo.foodName);
        printf("Enter New Code: ");
        fflush(stdin);
        gets(foodItemInfo.foodCode);
        printf("Enter New Details: ");
        fflush(stdin);
        gets(foodItemInfo.foodDetails);
        fseek(fileToSave,sizeof(foodItemInfo),SEEK_CUR);
        fwrite(&foodItemInfo,sizeof(struct foodItem),1,fileToSave);
        fclose(fileToSave);

      }
    }
    if(found == 1)
    {
      printf("Food information updated");
    }
    else
    {
      printf("Food not found in database");
    }
    break;
  case 'B':
    fileToSave = fopen("fooditeminfo.txt","r+");
    int found1=0;
    while(fread(&foodItemInfo,sizeof(struct foodItem),1,fileToSave))
    {
      if(strcmpi(foodItemInfo.foodName,search_name)==0)
      {
        found1 = 1;
        printf("\n\n\tUpdated Price : ");
        fflush(stdin);
        scanf("%f", &foodItemInfo.unitPrice);
        printf("\n\n\tUpdated Quantity : ");
        fflush(stdin);
        scanf("%d", &foodItemInfo.stock);
        fseek(fileToSave,sizeof(foodItemInfo),SEEK_CUR);
        fwrite(&foodItemInfo,sizeof(struct foodItem),1,fileToSave);
        fclose(fileToSave);
      }
    }
    if(found1 == 1)
    {
      printf("Food information updated successfully");
    }
    else
    {
      printf("Food not found in database");
    }

    break;

  default:
    printf("\nInput is wrong.!\nTry again!!");
  }
  printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
  getch();
  menu();
}

///delete food info
void removeFoodInfo()
{
  char searchName[24];
  system("cls");
  title();    // call title function

  printf("\n\n\t\t\t================= Delete/Remove food Information =================");

  fileToSave = fopen("fooditeminfo.txt", "r");    // open the original file
  tempFile = fopen("temporaryfoodinfo.txt", "w");

  printf("\n\n\t\t\tEnter the food name you want to delete :  ");
  fflush(stdin);
  gets(searchName);
  while(fread(&foodItemInfo,sizeof(struct foodItem),1,fileToSave))
  {
    if(strcmp(foodItemInfo.foodName, searchName) != 0)
    {
      fwrite(&foodItemInfo, sizeof(struct foodItem), 1, tempFile);
    }
  }
  fclose(fileToSave);
  fclose(tempFile);
  remove("fooditeminfo.txt");

  if(rename("temporaryfoodinfo.txt", "fooditeminfo.txt"))
  {

  }
  availableFoodItems--;
  printf("\n\n\t\t======================\tRecord Deleted Successfully\t======================");
  printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
  getch();
  menu();
}

//SYSTEM BYY **************************Kamrul Hasan*************************)




