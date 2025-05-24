#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void validate_user() {
    char username[100];
    char password[100];
    int loginAttempt = 1;

    while (loginAttempt < 4) {
        printf("Enter your username: ");
        scanf("%99s", username);
        printf("Enter your password: ");
        scanf("%99s", password);

        if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
            printf("Welcome Admin!\n");
            break;
        } else {
            printf("Invalid login attempt: %d. Please try again.\n", loginAttempt);
            loginAttempt++;
        }

        if (loginAttempt == 4) {
            printf("Too many login attempts! The program will now terminate.\n");
            exit(0);
        }
    }
}

void add_entry() {
    FILE *file = fopen("Diary_2024.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Error in opening the file!\n");
        return;
    }

    char date[20];
    printf("Enter the date (YYYY-MM-DD): ");
    scanf("%19s", date);
    getchar(); // Consume newline left in buffer

    char entry[100];
    printf("Enter entry to be written for %s: ", date);
    fgets(entry, sizeof(entry), stdin);
    entry[strcspn(entry, "\n")] = '\0';

    fprintf(file, "%s - %s\n", date, entry);
    fclose(file);
}

void view_entry() {
    FILE *file = fopen("Diary_2024.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error in opening the file!\n");
        return;
    }

    char date[20];
    printf("Enter the date (YYYY-MM-DD): ");
    scanf("%19s", date);
    getchar();

    char entry[100];
    int found = 0;

    while (fgets(entry, sizeof(entry), file) != NULL) {
        if (strncmp(entry, date, strlen(date)) == 0) {
            printf("Entry for %s:\n%s", date, entry);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No entry found for the given date.\n");
    }

    fclose(file);
}

void edit_entry() {
    FILE *file = fopen("Diary_2024.txt", "r+");
    if (file == NULL) {
        fprintf(stderr, "Error in opening the file!\n");
        return;
    }

    char date[20];
    printf("Enter the date of the entry to edit (YYYY-MM-DD): ");
    scanf("%19s", date);
    getchar();

    char new_entry[100];
    int found = 0;
    char entry[100];
    long pos;

    while ((pos = ftell(file)) != -1L && fgets(entry, sizeof(entry), file) != NULL) {
        if (strncmp(entry, date, strlen(date)) == 0) {
            printf("Current entry for %s: %s", date, entry);
            printf("Enter the new entry: ");
            fgets(new_entry, sizeof(new_entry), stdin);
            new_entry[strcspn(new_entry, "\n")] = '\0';

            fseek(file, pos, SEEK_SET);
            fprintf(file, "%s - %s\n", date, new_entry);
            found = 1;
            printf("Entry got updated in the Diary.\n");
            break;
        }
    }

    if (!found) {
        printf("No entry found for the given date.\n");
    }

    fclose(file);
}

void delete_entry() {
    FILE *file = fopen("Diary_2024.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error in opening the file!\n");
        return;
    }

    char date[20];
    printf("Enter the date of the entry to delete (YYYY-MM-DD): ");
    scanf("%19s", date);
    getchar();

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        fprintf(stderr, "Error in opening the temporary file!\n");
        fclose(file);
        return;
    }

    char entry[100];
    int found = 0;

    while (fgets(entry, sizeof(entry), file) != NULL) {
        if (strncmp(entry, date, strlen(date)) != 0) {
            fputs(entry, temp);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        printf("No entry found for the given date.\n");
        remove("temp.txt");
    } else {
        remove("Diary_2024.txt");
        rename("temp.txt", "Diary_2024.txt");
        printf("Entry deleted successfully.\n");
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n1. Adding Entry\n2. Viewing Entry\n3. Editing Entry\n4. Deleting Entry\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                validate_user();
                add_entry();
                break;
            case 2:
                validate_user();
                view_entry();
                break;
            case 3:
                validate_user();
                edit_entry();
                break;
            case 4:
                validate_user();
                delete_entry();
                break;
            case 5:
                printf("\nThank you. Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}
