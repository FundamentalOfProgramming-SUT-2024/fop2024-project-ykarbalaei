#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME_LEN 30
#define MAX_PASS_LEN 30
#define MAX_EMAIL_LEN 50
#define WIDTH 140
#define HEIGHT 40

char password[MAX_PASS_LEN];
char email[MAX_EMAIL_LEN];
char username[MAX_NAME_LEN];

int y = 5;

void generate_random_password(char *password, int length)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int i;
    for (i = 0; i < length - 1; i++)
    {
        password[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    password[length - 1] = '\0';
}

int is_valid_password(char *password)
{
    int has_lower = 0, has_upper = 0, has_digit = 0;
    
    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] >= 'a' && password[i] <= 'z') has_lower = 1;
        if (password[i] >= 'A' && password[i] <= 'Z') has_upper = 1;
        if (password[i] >= '0' && password[i] <= '9') has_digit = 1;
    }

    if (strlen(password) < 7) {
        attron(COLOR_PAIR(2));  
        mvprintw(y += 2, 10, "The password must be at least 7 characters long.");
        attroff(COLOR_PAIR(2));
        return 0;
    } else if (!has_lower) {
        attron(COLOR_PAIR(2));
        mvprintw(y += 2, 10, "Password must contain at least one lowercase letter.");
        attroff(COLOR_PAIR(2));
        return 0;
    } else if (!has_upper) {
        attron(COLOR_PAIR(2));
        mvprintw(y += 2, 10, "Password must contain at least one uppercase letter.");
        attroff(COLOR_PAIR(2));
        return 0;
    } else if (!has_digit) {
        attron(COLOR_PAIR(2));
        mvprintw(y += 2, 10, "Password must contain at least one number.");
        attroff(COLOR_PAIR(2));
        return 0;
    }
    return 1;
}

void generate_password()
{
    char v[150]="Enter your password (at least 7 characters, one lowercase, one uppercase, one number):";
    int p=strlen(v);
    mvprintw(y += 2, 10, "Enter your password (at least 7 characters, one lowercase, one uppercase, one number): ");
    noecho();
    int i = 0;
    while (i < MAX_PASS_LEN - 1)
    {
        char ch = getch();
        if (ch == 10) 
            break;
        else if (ch == 127 && i > 0) { 
            i--;
            mvaddch(y, 10 + i + p, ' ');
            move(y, 10 + i + p);
        }
        else if (ch >= 32 && ch <= 126) { 
            password[i] = ch;
            mvaddch(y, 10 + p + i, '*'); 
            i++;
        }
        refresh();
    }
    password[i] = '\0'; 


    if (!is_valid_password(password)) {
        generate_password(); 
    }
}

void generate_username()
{
    mvprintw(y, 10, "Enter username: ");
    echo();
    getstr(username);
    if (strlen(username) < 1)
    {
        attron(COLOR_PAIR(2));  
        mvprintw(15, 10, "Username cannot be empty.");
        attroff(COLOR_PAIR(2));
        generate_username();
    }
}

int is_valid_email(char *email)
{
    char *at_pos = strchr(email, '@');
    char *dot_pos = strchr(email, '.');

    
    if (at_pos == NULL || dot_pos == NULL) {
        mvprintw(y += 2, 10, "Invalid email format. Please include '@' and '.'.");
        return 0;
    }
    if (at_pos > dot_pos) {
        attron(COLOR_PAIR(2));  
        mvprintw(y += 2, 10, "Invalid email format. Please include '@' and '.'.");
        attroff(COLOR_PAIR(2));
        return 0;
    }
    return 1;
}

void generate_email()
{
    mvprintw(y += 2, 10, "Enter email: ");
    echo();
    getstr(email);
    if (!is_valid_email(email)) {
        generate_email(); 
    }
}

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

    generate_username();

    
    mvprintw(7, 10, "Do you want to generate a random password? (1: Yes, 0: No): ");
    scanw("%d", &option);

    if (option == 1)
    {
        generate_random_password(password, MAX_PASS_LEN);
        attron(COLOR_PAIR(1));
        mvprintw(y += 2, 10, "Your randomly generated password is: %s", password);
        attroff(COLOR_PAIR(1));
    }
    else
    {
        generate_password();
    }

    generate_email(); 

    
    attron(COLOR_PAIR(3));
    mvprintw(y += 2, 10, "User created successfully!");
    attroff(COLOR_PAIR(3));

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
