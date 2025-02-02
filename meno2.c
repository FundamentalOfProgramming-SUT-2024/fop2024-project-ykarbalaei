#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME_LEN 30
#define MAX_PASS_LEN 30
#define MAX_EMAIL_LEN 50
#define WIDTH 120
#define HEIGHT 30

char password[MAX_PASS_LEN];
char email[MAX_EMAIL_LEN];
char username[MAX_NAME_LEN];

int y = 5;

void draw_rect()
{
    for (int y = 2; y < HEIGHT; y++)
    {
        for (int x = 7; x < WIDTH; x++)
        {
            if (y == 2 || y == HEIGHT - 1)
                mvaddch(y, x, '-');
            else if (x == 7 || x == WIDTH - 1)
                mvaddch(y, x, '|');
            else
                mvaddch(y, x, ' ');
        }
    }
    refresh();
}


int check_login(char *input_username, char *input_password)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL) {
        attron(COLOR_PAIR(2)); 
        mvprintw(y += 2, 10, "No user data file found.");
        attroff(COLOR_PAIR(2));
        return 0;
    }

    char stored_username[MAX_NAME_LEN];
    char stored_password[MAX_PASS_LEN];
    char stored_email[MAX_EMAIL_LEN];

    
    while (fscanf(file, "Username: %s\nPassword: %s\nEmail: %s\n\n", stored_username, stored_password, stored_email) != EOF)
    {
        if (strcmp(input_username, stored_username) == 0) {
            
            if (strcmp(input_password, stored_password) == 0) {
                fclose(file);
                return 1;  
            } else {
                fclose(file);
                return -1;  
            }
        }
    }

    fclose(file);
    return 0;  
}


void login_user()
{
    char input_username[MAX_NAME_LEN];
    char input_password[MAX_PASS_LEN];
    int login_result;

    mvprintw(y += 2, 10, "Enter your username: ");
    echo();
    getstr(input_username);
    char a[50]="Enter your password:";
    int b=strlen(a);
    mvprintw(y += 2, 10, "Enter your password: ");
    noecho();
    int i = 0;
    while (i < MAX_PASS_LEN - 1)
    {
        char ch = getch();
        if (ch == 10) 
            break;
        else if (ch == 127 && i > 0) { 
            i--;
            mvaddch(y, 10 + i, ' ');
            move(y, 10 + i+b);
        }
        else if (ch >= 32 && ch <= 126) { 
            input_password[i] = ch;
            mvaddch(y, 10 + i+b, '*'); 
            i++;
        }
        refresh();
    }
    input_password[i] = '\0'; 

    login_result = check_login(input_username, input_password);

    if (login_result == 1) {
        attron(COLOR_PAIR(3));  
        mvprintw(y += 2, 10, "Login successful!");
        attroff(COLOR_PAIR(3));
    } else if (login_result == -1) {
        attron(COLOR_PAIR(2));  
        mvprintw(y += 2, 10, "Incorrect password. Try again.");
        attroff(COLOR_PAIR(2));
    } else {
        attron(COLOR_PAIR(2));  
        mvprintw(y += 2, 10, "Username not found. Please check and try again.");
        attroff(COLOR_PAIR(2));
    }

    refresh();
    getch();
}

void create_user()
{
    int option;

    initscr();
    cbreak();
    noecho();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  
    init_pair(2, COLOR_RED, COLOR_BLACK);    
    init_pair(3, COLOR_GREEN, COLOR_BLACK);  
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));  

    draw_rect();

    
    attron(COLOR_PAIR(4) | A_BOLD); 
    mvprintw(3, 10, "Welcome to Regue");
    attroff(COLOR_PAIR(4) | A_BOLD);

    
    mvprintw(y += 2, 10, "Do you want to login? (1: Yes, 0: No): ");
    scanw("%d", &option);

    if (option == 1) {
        login_user();
    }

    refresh();
    getch();
    endwin(); 
}

int main()
{
    srand(time(NULL)); 
    create_user();
    return 0;
}
