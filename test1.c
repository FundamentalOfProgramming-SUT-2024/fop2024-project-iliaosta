#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define MAX_LEN 100

// پروتوتایپ توابع
void showMainMenu(void);
void loginMenu(void);
void createUserMenu(void);
int displayMenu(char *menu[], int menuSize);
void is_it_in(char *username, char *password, int *tr);
void auth(void);
void forgotten_password(void) {
    // اینجا باید منطق بازیابی رمز عبور را پیاده‌سازی کنید
    mvprintw(5, 10, "Forgot password logic goes here.");
}

// تابع loginMenu
void loginMenu() {
    char *menu[] = {
        "Enter Username, Password",
        "Forgot Password"
    };
    int menuSize = sizeof(menu) / sizeof(menu[0]);
    int choice = displayMenu(menu, menuSize);
    if (choice == 0) {
        auth();
    } else if (choice == 1) {
        forgotten_password();  // فراخوانی تابع بازیابی رمز عبور
    }
}

// تابع is_it_in
void is_it_in(char *username, char *password, int *tr) {
    FILE *file1 = fopen("usernames.txt", "r");
    FILE *file2 = fopen("passwords.txt", "r");
    char line[1024];
    int found = 0;

    if (file1 == NULL || file2 == NULL) {
        printf("Error opening files.\n");
        return;
    }

    while (fgets(line, sizeof(line), file1)) {
        char *word = strtok(line, ";");
        while (word != NULL) {
            if (strcmp(word, username) == 0) {
                found = 1;
                break;
            }
            word = strtok(NULL, ";");
        }
        if (found) break;
    }

    if (found) {
        found = 0;
        while (fgets(line, sizeof(line), file2)) {
            char *word = strtok(line, ";");
            while (word != NULL) {
                if (strcmp(word, password) == 0) {
                    *tr = 1;
                    found = 1;
                    break;
                }
                word = strtok(NULL, ";");
            }
            if (found) break;
        }
    }

    fclose(file1);
    fclose(file2);
}

// تابع auth
void auth() {
    clear();
    initscr();
    cbreak();
    noecho();  
    keypad(stdscr, TRUE);
    int tr = 0;

    int height, width;
    getmaxyx(stdscr, height, width);

    int x = width / 2 - 9;
    int y = height / 2;

    char username[100];
    echo();
    mvprintw(y + 1, x, "Enter Username: ");
    getstr(username);
    noecho();

    mvprintw(y + 2, x, "Enter Password: ");
    echo();
    char password[100];
    getstr(password);
    noecho();

    is_it_in(username, password, &tr);

    if (tr == 0) {
        mvprintw(y + 4, x, "Invalid username or password. Try again.");
        getch();
        auth();  // دوباره درخواست ورود
    } else {
        mvprintw(y + 4, x, "Login successful!");
        getch();
    }

    getch();
    endwin();
}

// تابع نمایش منو
int displayMenu(char *menu[], int menuSize) {
    int choice = 0;
    int ch;
    clear();
    while (1) {
        for (int i = 0; i < menuSize; i++) {
            if (i == choice) {
                attron(A_REVERSE);
            }
            mvprintw(i + 1, 10, "%s", menu[i]);
            if (i == choice) {
                attroff(A_REVERSE);
            }
        }

        ch = getch();
        switch (ch) {
            case KEY_UP:
                choice = (choice > 0) ? choice - 1 : menuSize - 1;
                break;
            case KEY_DOWN:
                choice = (choice < menuSize - 1) ? choice + 1 : 0;
                break;
            case 10:
                return choice;
            default:
                break;
        }
    }
}

// تعریف منوی اصلی
void showMainMenu(void) {
    char *menu[] = {
        "Login",
        "Create New User",
        "Guest",
        "Exit"
    };
    int menuSize = sizeof(menu) / sizeof(menu[0]);

    int choice = displayMenu(menu, menuSize);

    if (choice == 0) {
        loginMenu();
    } else if (choice == 1) {
        createUserMenu();
    } else if (choice == 2) {
        printw("You entered as a Guest.\n");
        getch();
    } else if (choice == 3) {
        endwin();
        return;
    } else {
        // Default case if needed
    }
}

// تابع ذخیره‌سازی در دیتابیس
void put_in_database(char *username, char *password, char *email) {
    FILE *file1 = fopen("usernames.txt", "a");
    FILE *file2 = fopen("passwords.txt", "a");
    FILE *file3 = fopen("emails.txt", "a");

    if (file1 == NULL || file2 == NULL || file3 == NULL) {
        printf("Error opening files.\n");
        return;
    }

    fprintf(file1, "%s;\n", username);
    fprintf(file2, "%s;\n", password);
    fprintf(file3, "%s;\n", email);

    fclose(file1);
    fclose(file2);
    fclose(file3);
}

// تابع بررسی ایمیل
void email_checker(char *email, int *tr) {
    int count1 = 0;

    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '.') {
            count1++;
        }
    }

    if (count1 >= 1 && strchr(email, '@') != NULL) {
        *tr = 1;
    } else {
        *tr = 0;
    }
}

// تابع ایجاد کاربر جدید
void create(){
    clear();
    initscr();
    cbreak();
    noecho();  
    keypad(stdscr, TRUE);
    int tr = 0;

    int height, width;
    getmaxyx(stdscr, height, width);

    int x = width / 2 - 14;
    int y = height / 2;

    char username[100];
    echo();
    mvprintw(y + 1, x, "Enter Username: ");
    getstr(username);
    noecho();

    mvprintw(y + 2, x, "Enter Password: ");
    echo();
    char password[100];
    getstr(password);
    noecho();
    
    // بررسی طول رمز عبور
    while (strlen(password) < 7){

        mvprintw(y + 2, x, "Enter Password: ");
        echo();
        getstr(password);
        noecho();
        mvprintw(y + 3, x, "Password must be at least 7 characters.");
        getch();

        move(y + 2, x);
        clrtoeol();
        refresh();

    }
    mvprintw(y + 3, x, "your Password is good");

    move(y + 3, x);
    clrtoeol();
    refresh();
    mvprintw(y + 2, x, "Enter Password: %s", password);

    char email[100];
    while(tr != 1){
        mvprintw(y + 3, x, "Enter email: ");
        echo();
        getstr(email);
        noecho();
        email_checker(email, &tr);
        if(tr == 1){
            mvprintw(y + 4, x, "Your email is ok.");
            y += 1;
        }
        else{
            mvprintw(y + 4, x, "Your email is not valid. Please enter it again.");
        }
    }

    is_it_in(username, password, &tr);

    if (tr == 1) {
        mvprintw(y + 4, x, "Your user has been created successfully.");
        put_in_database(username, password, email);
        getch();
    } else {
        mvprintw(y + 4, x, "Your username or password are in our database.");
        getch();
        create(); // دوباره فراخوانی تابع create()
    }

    getch();
    showMainMenu();
}

// منوی ایجاد کاربر جدید
void createUserMenu(){
    char *menu[] = {
        "Create New User",
        "Back"
    };
    int menuSize = sizeof(menu) / sizeof(menu[0]);

    int choice = displayMenu(menu, menuSize);
    if (choice == 0) {
        create();
    } else if (choice == 1) {
        showMainMenu();
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    showMainMenu();

    endwin();
    return 0;
}
