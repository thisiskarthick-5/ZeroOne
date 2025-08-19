team : raven 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <dirent.h>

#define RESET "\033[0m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

typedef struct {
    char name[20];
    double value;
} Variable;

Variable variables[100];
int varCount = 0;

// Trim spaces
void trimSpaces(char *str) {
    char *start = str;
    while (*start == ' ') start++;
    char *end = start + strlen(start) - 1;
    while (end > start && *end == ' ') end--;
    *(end + 1) = '\0';
    memmove(str, start, end - start + 2);
}

// Find variable
int findVariable(char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variables[i].name, name) == 0) return i;
    }
    return -1;
}

// Get variable value
double getVariableValue(char *name) {
    int index = findVariable(name);
    return (index != -1) ? variables[index].value : atof(name);
}

// Set variable
void setVariable(char *name, double value) {
    int index = findVariable(name);
    if (index == -1) {
        strcpy(variables[varCount].name, name);
        variables[varCount].value = value;
        varCount++;
    } else {
        variables[index].value = value;
    }
}

// Execute Mylang commands
void executeMylang(char *command) {
    char var[20], op[3], expr[50], trueCmd[50], falseCmd[50];
    int times;
    double val1, val2;

    // Variable Assignments
    if (sscanf(command, "%s = %[^\n]", var, expr) == 2) {
        double value = atof(expr);
        setVariable(var, value);
        printf(GREEN " ✅ Assigned: %s = %.2f\n" RESET, var, value);
    }
    // Print Command
    else if (strncmp(command, "print ", 6) == 0) {
        sscanf(command + 6, "%s", var);
        printf(GREEN " 🔢 %.2f\n" RESET, getVariableValue(var));
    }
    // Looping
    else if (sscanf(command, "loop %d { %[^\n] }", &times, expr) == 2) {
        for (int i = 0; i < times; i++) executeMylang(expr);
    }
    // If-Else (Short Syntax)
    else if (sscanf(command, "if %s %2s %s : %[^\n] else : %[^\n]", var, op, expr, trueCmd, falseCmd) == 5) {
        val1 = getVariableValue(var);
        val2 = getVariableValue(expr);

        int conditionMet = 0;
        if (strcmp(op, "==") == 0) conditionMet = (val1 == val2);
        else if (strcmp(op, "!=") == 0) conditionMet = (val1 != val2);
        else if (strcmp(op, ">") == 0) conditionMet = (val1 > val2);
        else if (strcmp(op, "<") == 0) conditionMet = (val1 < val2);
        else if (strcmp(op, ">=") == 0) conditionMet = (val1 >= val2);
        else if (strcmp(op, "<=") == 0) conditionMet = (val1 <= val2);

        if (conditionMet) executeMylang(trueCmd);
        else executeMylang(falseCmd);
    }
    else {
        printf(RED " ❌ Unknown command!\n" RESET);
    }
}

// Chatbot Response
void chatbotResponse(char *command) {
    if (strcmp(command, "hello") == 0) printf(CYAN " 🤖 Hello! How can I assist you?\n" RESET);
    else printf(CYAN " 🤖 I don’t understand. Try again!\n" RESET);
}



void fileOperations() {
    int choice;
    char filename[50], newFilename[50], content[500], ch;
    FILE *fptr, *copyPtr;

    while (1) {  
        printf(YELLOW "\n📂 File Operations:\n" RESET);  
        printf("1. Create File\n2. Read File\n3. Append to File\n4. Rename File\n5. Copy File\n6. Delete File\n7. List Files\n8. Back to Main Menu\n");  
        printf("Enter your choice: ");  
        scanf("%d", &choice);  
        getchar();  

        switch (choice) {  
            case 1:  // Create File
                printf("Enter file name: ");  
                scanf("%s", filename);  
                getchar();  
                printf("Enter content: ");  
                fgets(content, sizeof(content), stdin);  
                fptr = fopen(filename, "w");  
                fprintf(fptr, "%s", content);  
                fclose(fptr);  
                printf(GREEN " ✅ File Created Successfully!\n" RESET);  
                break;  

            case 2:  // Read File
                printf("Enter file name: ");  
                scanf("%s", filename);  
                getchar();  
                fptr = fopen(filename, "r");  
                if (fptr == NULL) {  
                    printf(RED " ❌ File Not Found!\n" RESET);  
                } else {  
                    while ((ch = fgetc(fptr)) != EOF) putchar(ch);  
                    fclose(fptr);  
                }  
                break;  

            case 3:  // Append to File
                printf("Enter file name: ");  
                scanf("%s", filename);  
                getchar();  
                fptr = fopen(filename, "a");  
                if (fptr == NULL) {  
                    printf(RED " ❌ File Not Found!\n" RESET);  
                } else {  
                    printf("Enter content to append: ");  
                    fgets(content, sizeof(content), stdin);  
                    fprintf(fptr, "%s", content);  
                    fclose(fptr);  
                    printf(GREEN " ✅ Content Appended!\n" RESET);  
                }  
                break;  

            case 4:  // Rename File
                printf("Enter current file name: ");  
                scanf("%s", filename);  
                printf("Enter new file name: ");  
                scanf("%s", newFilename);  
                if (rename(filename, newFilename) == 0)  
                    printf(GREEN " ✅ File Renamed Successfully!\n" RESET);  
                else  
                    printf(RED " ❌ Failed to Rename File!\n" RESET);  
                break;  

            case 5:  // Copy File
                printf("Enter source file name: ");  
                scanf("%s", filename);  
                printf("Enter destination file name: ");  
                scanf("%s", newFilename);  
                fptr = fopen(filename, "r");  
                copyPtr = fopen(newFilename, "w");  
                if (fptr == NULL || copyPtr == NULL) {  
                    printf(RED " ❌ File Operation Failed!\n" RESET);  
                } else {  
                    while ((ch = fgetc(fptr)) != EOF) fputc(ch, copyPtr);  
                    fclose(fptr);  
                    fclose(copyPtr);  
                    printf(GREEN " ✅ File Copied Successfully!\n" RESET);  
                }  
                break;  

            case 6:  // Delete File
                printf("Enter file name: ");  
                scanf("%s", filename);  
                if (remove(filename) == 0)  
                    printf(GREEN " ✅ File Deleted!\n" RESET);  
                else  
                    printf(RED " ❌ File Not Found!\n" RESET);  
                break;  

            case 7:  // List Files
                printf(GREEN "📂 Files in Current Directory:\n" RESET);
                struct dirent *de;  
                DIR *dr = opendir(".");  
                if (dr == NULL) {  
                    printf(RED " ❌ Could Not Open Directory\n" RESET);  
                } else {  
                    while ((de = readdir(dr)) != NULL)  
                        printf("%s\n", de->d_name);  
                    closedir(dr);  
                }  
                break;  

            case 8:  // Exit
                return;  

            default:  
                printf(RED " ❌ Invalid choice!\n" RESET);  
        }  
    }
}

// C Compiler with Error Checking & Output Storage
void compileAndRunC() {
    char filename[50] = "temp.c";
    char command[200];

    printf("Enter C code (end with 'EOF'):\n");  
    FILE *fptr = fopen(filename, "w");  
    char line[100];  

    while (1) {  
        fgets(line, sizeof(line), stdin);  
        if (strncmp(line, "EOF", 3) == 0) break;  
        fputs(line, fptr);  
    }  
    fclose(fptr);  

    // Compile with error checking
    sprintf(command, "gcc %s -o temp.out 2> errors.txt", filename);
    int compileStatus = system(command);
    
    // Check if there are compilation errors
    FILE *errorFile = fopen("errors.txt", "r");
    fseek(errorFile, 0, SEEK_END);
    if (ftell(errorFile) > 0) {
        printf(RED " ❌ Compilation Failed! See errors below:\n" RESET);
        rewind(errorFile);
        char errorLine[200];
        while (fgets(errorLine, sizeof(errorLine), errorFile))
            printf("%s", errorLine);
        fclose(errorFile);
        remove("errors.txt");
        return;
    }
    fclose(errorFile);
    remove("errors.txt");

    // Execute and save output
    sprintf(command, "./temp.out > output.txt");
    system(command);
    printf(GREEN " ✅ Compilation Successful! Output saved to output.txt\n" RESET);
    remove("temp.out");
}

// Run Python Code
void runPython() {
    char filename[50] = "script.py";
    char command[100];

    printf("Enter Python code (end with 'EOF'):\n");  
    FILE *fptr = fopen(filename, "w");  
    char line[100];  

    while (1) {  
        fgets(line, sizeof(line), stdin);  
        if (strncmp(line, "EOF", 3) == 0) break;  
        fputs(line, fptr);  
    }  
    fclose(fptr);  

    sprintf(command, "python3 %s", filename);  
    system(command);
    remove(filename);
}

// Menu System
void menu() {
    int choice;
    char command[200];

    while (1) {
        printf(YELLOW "\n🚀 Welcome to the Interactive Shell!\n" RESET);
        printf("----------------------------------\n");
        printf("1️⃣ Mylang (Variable, Loop, If-Else)\n");
        printf("2️⃣ Chatbot\n");
        printf("3️⃣ File Operations\n");
        printf("4️⃣ C Compiler\n");
        printf("5️⃣ Exit\n");
        printf("----------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                while (1) {
                    printf(BLUE "\nMylang> " RESET);
                    fgets(command, sizeof(command), stdin);
                    command[strcspn(command, "\n")] = 0;
                    if (strcmp(command, "exit") == 0) break;
                    executeMylang(command);
                }
                break;
            case 2:
                while (1) {
                    printf(CYAN "\nChatbot> " RESET);
                    fgets(command, sizeof(command), stdin);
                    command[strcspn(command, "\n")] = 0;
                    if (strcmp(command, "exit") == 0) break;
                    chatbotResponse(command);
                }
                break;
            case 3:
                fileOperations();
                break;
            case 4:
                compileAndRunC();
                break;
            case 5:
                printf(YELLOW "Goodbye! 👋\n" RESET);
                return;
            default:
                printf(RED " ❌ Invalid choice!\n" RESET);
        }
    }
}

int main() {
    menu();
    return 0;
}
