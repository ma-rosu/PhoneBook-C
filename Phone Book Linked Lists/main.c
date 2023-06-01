#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/// STRUCTS AND VARIABLES

// Contact struct
struct {
    char inputName[40];
    char inputNumber[20];
    char inputEmail[40];
}contact;
// Linked Lists struct
struct node{
    char data[130];
    struct node* next;
};
// Variables
struct node *head = NULL;
char fileName[13] = "contacts.txt";


/// ORDERING AND LINKED LISTS

// Function to add a node to the linked list
void addNode(struct node **head_ref, char *new_data)
{
    struct node *new_node = (struct node*) malloc(sizeof(struct node));
    strcpy(new_node->data, new_data);
    new_node->next = *head_ref;
    *head_ref = new_node;
}
// Delete the entire linked list to not show duplicates when I show all contact after N times
void deleteList(struct node **head_ref)
{
    // Initialize a current and a next node
    struct node *current = *head_ref;
    struct node *next;

    // Traverse the list and free the memory allocated for each node
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // Set the head pointer to NULL
    *head_ref = NULL;
}
// Function to sort the linked list
void sortList(struct node **head_ref)
{
    struct node *temp;
    int flag = 1;
    if (*head_ref == NULL) {
        return;
    }
    while(flag) {
        flag = 0;
        temp = *head_ref;
        while (temp->next != NULL) {
            if (strcmp(temp->data, temp->next->data) > 0) {
                char temp_data[100];
                strcpy(temp_data, temp->data);
                strcpy(temp->data, temp->next->data);
                strcpy(temp->next->data, temp_data);
                flag = 1;
            }
            temp = temp->next;
        }
    }
}
// Order contacts in file
void orderContacts(struct node **head_ref)
{
    FILE * fPtr;
    FILE * fTemp;
    char buffer[113];
    fPtr  = fopen(fileName, "r");
    fTemp = fopen("replace.txt", "w");
    while((fgets(buffer, 113, fPtr)) != NULL)
    {
        addNode(&head, buffer);
    }
    sortList(&head);
    struct node *current = head;
    while (current != NULL) {
        strcpy(buffer, current->data);
        fputs(buffer, fTemp);
        current = current->next;
    }
    deleteList(&head);
    fclose(fPtr);
    fclose(fTemp);
    remove(fileName);
    rename("replace.txt", fileName);
}


/// SECONDARY FUNCTIONS

// Function to design the output
void designDetails(char buffer[])
{
    char *p, buffer2[150] = "\nNAME:\t";

    p = strtok(buffer, "/");
    strcat(buffer2, p);
    strcat(buffer2, "\nPHONE:\t");
    p = strtok(NULL, "/");
    strcat(buffer2, p);
    strcat(buffer2, "\nEMAIL:\t");
    p = strtok(NULL, "/");
    strcat(buffer2, p);
    strcat(buffer2, "\n");

    strcpy(buffer, buffer2);
}
// Checking if the number entered is ok
int validNumber(char number[])
{
    for(int i=0; i<strlen(number);i++)
    {
        if(!strchr("0123456789", number[i]))
            return 1;
    }
    return 0;
}
// Checking if the length of the number is ok >1
int validNumberLength(char number[])
{
    if(strlen(number) < 1)
        return 1;
    return 0;
}
// Checking if the name is ok
int validName(char name[])
{
    FILE *f;
    f = fopen(fileName, "r");
    char *p, line[40];
    while(fgets(line, 40, f))
    {
        p = strtok(line, "/");
        if(strcmp(name, p) == 0)
            {
                fclose(f);
                return 1;
            }
    }
    fclose(f);

    return 0;
}
// Checking if the name is ok
int validName2(char name[])
{
    int check = 1;
    for(int i=0; i<strlen(name); i++)
    {
        if(name[i] != ' ')
            check = 0;
    }
    if(check)
        return 1;
    else
        return 0;

}


/// ESENTIAL FUNCTIONS

// Print all contact details (Sorted)
void allContactDetails()
{
    FILE *f;
    f = fopen(fileName, "r");
    char buffer[150];
    int check = 0;
    while(fgets(buffer, 150, f))
    {
        check = 1;
        buffer[strcspn(buffer, "\n")] = 0;
        designDetails(buffer);
        printf("%s", buffer);
        printf("--------------------------------------------------------");
    }
    if(!check)
    {
        printf("\n\n[!] There are no contacts!\n\n");
        printf("---------------------------------------------------------");
    }
    fclose(f);
}
// Printing details of a contact
void contactDetails(char look[])
{
    FILE *f;
    f = fopen(fileName, "r");
    char buffer[150], tokString[150];
    char *p;
    int check = 0;
    printf("\n---------------------------------------------------------");
    while(fgets(buffer, 150, f))
    {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(tokString, buffer);
        p = strtok(tokString, "/");
        if(strstr(p, look))
        {
            check = 1;
            designDetails(buffer);
            printf("%s", buffer);
            printf("---------------------------------------------------------");
        }
        else
        {
            p = strtok(NULL, "/");
            if(strstr(p, look))
            {
                check = 1;
                designDetails(buffer);
                printf("%s", buffer);
                printf("---------------------------------------------------------");
            }
        }
    }
    if(!check)
    {
        printf("\n\n[!] There is no contact with that name or phone number!\n\n");
        printf("---------------------------------------------------------");
    }
    fclose(f);
}
// Adds a new line in contact document
void addLine(char name[], char number[], char email[])
{
    FILE *fAdd;
    char line[113];
    fAdd = fopen(fileName, "a");
    strcpy(line, name);
    strcat(line, "/");
    strcat(line, number);
    strcat(line, "/");
    strcat(line, email);
    strcat(line, "\n");
    fputs(line, fAdd);
    fclose(fAdd);
    orderContacts(&head);
}
// Edit a contact by rewriting the file and replacing only the specific line
void replaceLine(char name[])
{
    FILE * fPtr;
    FILE * fTemp;

    char newName[40] = "", newNumber[20] = "", newEmail[40] = "", number[20], email[40];
    int choice;
    printf("\n-------------------- EDITING THE NAME -------------------\n\n");
    printf("Do you want to change the name?\n");
    printf("[1] Yes.\n");
    printf("[2] No.\n");
    printf("\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    getchar();
    while(choice != 1 && choice != 2)
    {
        printf("\n");
        printf("[WARNING] Please enter a valid choice!\n");
        printf("[1] Yes.\n");
        printf("[2] No.\n");
        printf("\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        getchar();
    }
    printf("\n");
    if(choice == 1)
    {
        printf("New name: ");
        fgets(newName, 40, stdin);
        newName[strcspn(newName, "\n")] = 0;
        printf("\n");
        while(validName(newName) || validName2(newName))
            {
                if(validName(newName))
                {
                    printf("\n[WARNING] This name is not available!\n[WARNING] Please choose another one!");
                    printf("\n\nNew name: ");
                    fgets(newName, 40, stdin);
                    newName[strcspn(newName, "\n")] = 0;
                }
                else
                {
                    if(validName2(newName))
                    {
                        printf("[WARNING] You did not enter a name!");
                        printf("\n\nNew name: ");
                        fgets(newName, 40, stdin);
                        newName[strcspn(newName, "\n")] = 0;
                    }
                }
                printf("\n");

            }

    }

    printf("------------------- EDITING THE NUMBER ------------------\n\n");

    printf("Do you want to change the number?\n");
    printf("[1] Yes.\n");
    printf("[2] No.\n");
    printf("\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    getchar();
    while(choice != 1 && choice != 2)
    {
        printf("\n");
        printf("[WARNING] Please enter a valid choice!\n");
        printf("[1] Yes.\n");
        printf("[2] No.\n");
        printf("\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        getchar();
    }
    printf("\n");
    if(choice == 1)
    {
        printf("New number: ");
        fgets(newNumber, 20, stdin);
        newNumber[strcspn(newNumber, "\n")] = 0;
        printf("\n");
        while(validNumber(newNumber) || validNumberLength(newNumber))
            {
                if(validNumber(newNumber))
                {
                    printf("\n[WARNING] Please enter only digits in your number!");
                    printf("\n\nNew phone number: ");
                    fgets(newNumber, 20, stdin);
                    newNumber[strcspn(newNumber, "\n")] = 0;
                }
                else
                {
                    printf("[WARNING] You did not enter a valid phone number!");
                    printf("\n\nNew phone number: ");
                    fgets(newNumber, 20, stdin);
                    newNumber[strcspn(newNumber, "\n")] = 0;
                }

            }
    }

    printf("-------------------- EDITING THE EMAIL ------------------\n\n");

    printf("Do you want to change the email?\n");
    printf("[1] Yes.\n");
    printf("[2] No.\n");
    printf("\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    getchar();
    while(choice != 1 && choice != 2)
    {
        printf("\n");
        printf("[WARNING] Please enter a valid choice!\n");
        printf("[1] Yes.\n");
        printf("[2] No.\n");
        printf("\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        getchar();
        printf("\n");
    }
    printf("\n");
    if(choice == 1)
    {
        printf("New email: ");
        fgets(newEmail, 40, stdin);
        newEmail[strcspn(newEmail, "\n")] = 0;
        if(validName2(newEmail))
                strcpy(newEmail, "-");
        printf("\n");
    }

    printf("------------------------- SAVING -------------------------\n\n");

    printf("Do you want to save the changes?\n");
    printf("[1] Yes.\n");
    printf("[2] No.\n");
    printf("\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    getchar();
    while(choice != 1 && choice != 2)
    {
        printf("\n");
        printf("[WARNING] Please enter a valid choice!\n");
        printf("[1] Yes.\n");
        printf("[2] No.\n");
        printf("\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        getchar();
    }
    if(choice == 1)
    {
        char buffer[113], tokString[113];
        char newLine[113] = "";
        char *tok;

        fPtr  = fopen(fileName, "r");
        fTemp = fopen("replace.txt", "w");

        while ((fgets(buffer, 113, fPtr)) != NULL)
        {
            strcpy(tokString,buffer);
            tok = strtok(tokString, "/");
            if(strcmp(tok,name) == 0)
            {
                tok = strtok(NULL, "/");
                strcpy(number, tok);
                tok = strtok(NULL, "/");
                strcpy(email, tok);
                printf("%s", email);
                email[strcspn(email, "\n")] = 0;

                printf("\n");
                if(strlen(newName) > 0)
                {
                    strcat(newLine, newName);
                    strcat(newLine, "/");
                }
                else
                {
                    strcat(newLine, name);
                    strcat(newLine, "/");
                }
                if(strlen(newNumber) > 0)
                {
                    strcat(newLine, newNumber);
                    strcat(newLine, "/");
                }
                else
                {
                    strcat(newLine, number);
                    strcat(newLine, "/");
                }
                if(strlen(newEmail) > 0)
                {
                    strcat(newLine, newEmail);
                }
                else
                {
                    strcat(newLine, email);
                }
                strcat(newLine, "\n");
                fputs(newLine, fTemp);
            }
            else
                fputs(buffer, fTemp);
        }
        fclose(fPtr);
        fclose(fTemp);
        remove(fileName);

        /* Rename temporary file as original file */
        rename("replace.txt", fileName);
        printf("[!] Your contact has been modified!\n");
    }
    else
        printf("\n[!] Your contact has not been modified!\n");
    orderContacts(&head);
}
// Deleting a line by rewriting the file and skipping the specific line
void deleteLine(char name[])
{
    FILE * fPtr;
    FILE * fTemp;

    char buffer[113], tokString[113];
    char *tok;

    fPtr  = fopen(fileName, "r");
    fTemp = fopen("replace.txt", "w");

    while ((fgets(buffer, 113, fPtr)) != NULL)
    {
        strcpy(tokString,buffer);
        tok = strtok(tokString, "/");
        if(strcmp(tok,name) != 0)
            fputs(buffer, fTemp);
    }
    fclose(fPtr);
    fclose(fTemp);
    remove(fileName);
    rename("replace.txt", fileName);
}
// Deletes all file
void deleteAllContacts()
{
    FILE *f;
    remove(fileName);
    f = fopen(fileName, "w");
    fclose(f);
}


/// MAIN FUNCTION

int main()
{

    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> PHONE-BOOK <<<<<<<<<<<<<<<<<<<<<<<<<<<<");

    int run = 1, choice;

    while(run)
    {
        FILE *f;
        f = fopen(fileName, "r");
        if(f)
        {
            fclose(f);
        }
        else
        {
            fclose(f);
            fopen(fileName, "w");
        }
        printf("\n\n");
        printf("[1] Display all contacts.\n");
        printf("[2] Search for a contact.\n");
        printf("[3] Add a contact.\n");
        printf("[4] Edit a contact.\n");
        printf("[5] Delete a contact.\n");
        printf("[6] Delete all contacts.\n");
        printf("[7] Exit.\n");
        choice = 0;
        printf("\nYour choice: ");
        scanf("%d", &choice);
        getchar();

        while(choice != 1 && choice != 2 && choice != 3
              && choice != 4 && choice != 5 && choice != 6 && choice != 7)
        {
            printf("\n[WARNING] Please enter a valid choice! ");
            printf("\nPlease enter your choice: ");
            scanf("%d", &choice);
            getchar();
        }


        switch(choice)
        {
        case 1:
            printf("\n--------------------- CONTACT INFO ----------------------");
            allContactDetails();
            break;
        case 2:
            printf("\n--------------------- CONTACT INFO ----------------------");
            printf("\n\nPlease enter a name or phone number to search: ");
            fgets(contact.inputName, 40, stdin);
            contact.inputName[strcspn(contact.inputName, "\n")] = 0;
            contactDetails(contact.inputName);
            break;
        case 3:
            printf("\n------------------- ADDING A CONTACT --------------------\n");
            printf("\nPlease make sure the contact does not exist already!\n");
            printf("\n--------------------- ADDING A NAME ---------------------\n");
            printf("\nNew contact name: ");
            fgets(contact.inputName, 40, stdin);
            contact.inputName[strcspn(contact.inputName, "\n")] = 0;
            while(validName(contact.inputName) || validName2(contact.inputName))
            {
                if(validName(contact.inputName))
                {
                    printf("\n[WARNING] This name is not available!\n[WARNING] Please choose another one!");
                    printf("\n\nNew contact name: ");
                    fgets(contact.inputName, 40, stdin);
                    contact.inputName[strcspn(contact.inputName, "\n")] = 0;
                }
                else
                {
                    if(validName2(contact.inputName))
                    {
                        printf("\n[WARNING] You did not enter a name!");
                        printf("\n\nNew contact name: ");
                        fgets(contact.inputName, 40, stdin);
                        contact.inputName[strcspn(contact.inputName, "\n")] = 0;
                    }
                }

            }
            printf("\n-------------------- ADDING A NUMBER --------------------\n");
            printf("\nNew contact phone number: ");
            fgets(contact.inputNumber, 40, stdin);
            contact.inputNumber[strcspn(contact.inputNumber, "\n")] = 0;
            while(validNumber(contact.inputNumber) || validNumberLength(contact.inputNumber))
            {
                if(validNumber(contact.inputNumber))
                {
                    printf("\n[WARNING] Please enter only digits in your number!");
                    printf("\n\nNew contact phone number: ");
                    fgets(contact.inputNumber, 20, stdin);
                    contact.inputNumber[strcspn(contact.inputNumber, "\n")] = 0;
                }
                else
                {
                    printf("\n[WARNING] You did not enter a valid phone number!");
                    printf("\n\nNew contact phone number: ");
                    fgets(contact.inputNumber, 20, stdin);
                    contact.inputNumber[strcspn(contact.inputNumber, "\n")] = 0;
                }

            }
            printf("\n-------------------- ADDING AN EMAIL --------------------\n");
            printf("\nNew contact email (optional): ");
            fgets(contact.inputEmail, 40, stdin);
            contact.inputEmail[strcspn(contact.inputEmail, "\n")] = 0;
            if(validName2(contact.inputEmail))
                strcpy(contact.inputEmail, "-");
            printf("\n------------------------ SAVING -------------------------\n");
            printf("\nDo you want to save the contact?\n"
                   "[1] Yes.\n[2] No.\n\nYour choice: ");
            scanf("%d", &choice);
            getchar();
            while(choice != 1 && choice != 2)
            {
                printf("\n");
                printf("[WARNING] Please enter a valid choice!\n");
                printf("[1] Yes.\n");
                printf("[2] No.\n");
                printf("\n");
                printf("Your choice: ");
                scanf("%d", &choice);
                getchar();
            }
            if(choice == 1)
            {
                addLine(contact.inputName, contact.inputNumber, contact.inputEmail);
                printf("\n[!] Your contact has been saved.\n");
            }
            else
                printf("\n[!] Your contact has not been saved.\n");
            printf("\n---------------------------------------------------------");
            break;
        case 4:
            printf("\n------------------- EDITING A CONTACT -------------------\n");
            printf("\nEnter the name of the contact you want to edit: ");
            fgets(contact.inputName, 40, stdin);
            contact.inputName[strcspn(contact.inputName, "\n")] = 0;
            if(validName(contact.inputName))
            {
                replaceLine(contact.inputName);
            }
            else
            {
                printf("\n\n[!] I couldn't find any contact with that name!\n");
            }

            printf("\n---------------------------------------------------------");
            break;
        case 5:
            printf("\n------------------ DELETING A CONTACT -------------------");
            printf("\n\nPlease enter the name of the contact you want to delete: ");
            fgets(contact.inputName, 40, stdin);
            contact.inputName[strcspn(contact.inputName, "\n")] = 0;
            if(!validName(contact.inputName))
            {
                printf("\n[WARNING] I could not find any contact with this name!\n\n---------------------------------------------------------");
                break;
            }
            printf("\n\nAre you sure you want to delete '%s' from you contacts?", contact.inputName);
            printf("\n[1] Yes.\n[2] Go back.");
            printf("\n\nPlease enter your choice: ");
            scanf("%d", &choice);
            getchar();
            while(choice != 1 && choice != 2)
            {
                printf("\n");
                printf("[WARNING] Please enter a valid choice!\n");
                printf("[1] Yes.\n");
                printf("[2] No.\n");
                printf("\n");
                printf("Your choice: ");
                scanf("%d", &choice);
                getchar();
            }
            if(choice == 1)
            {
                deleteLine(contact.inputName);
                printf("\n[!] Your contact has been deleted!\n");
            }

            printf("\n---------------------------------------------------------");
            break;
        case 6:
            printf("\n----------------- DELETING ALL CONTACTS -----------------\n");
            printf("\nAre you sure you want to delete all your contacts?\n[1] Yes.\n[2] Go back.\n");
            printf("\nYour choice: ");
            scanf("%d", &choice);
            getchar();
            while(choice != 1 && choice != 2)
            {
                printf("\n");
                printf("[WARNING] Please enter a valid choice!\n");
                printf("[1] Yes.\n");
                printf("[2] No.\n");
                printf("\n");
                printf("Your choice: ");
                scanf("%d", &choice);
                getchar();
            }
            if(choice == 1)
                {
                    deleteAllContacts();
                    printf("\n[!] All contacts have been deleted!\n");
                }
            printf("\n---------------------------------------------------------");
            break;
        case 7:
            run = 0;
            break;
        default:
            printf("Please enter a valid choice [1->7]!");
            break;
        }
        orderContacts(&head);
    }
    printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> THANK YOU <<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    return 0;
}





