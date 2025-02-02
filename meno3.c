#include <ncurses.h>
#include <string.h>

#define WIDTH 120
#define HEIGHT 30

int difficulty = 1; 
int color_scheme = 1; 
int music_choice = 0; 


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


void show_settings_menu()
{
    int choice;
    int ch;

    
    initscr();
    cbreak();
    noecho();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);  
    init_pair(2, COLOR_RED, COLOR_BLACK);    
    init_pair(3, COLOR_BLUE, COLOR_BLACK);   


    draw_rect();

    
    attron(COLOR_PAIR(3) | A_BOLD);
    mvprintw(3, 10, "Game Settings Menu");
    attroff(COLOR_PAIR(3) | A_BOLD);

    char *choices[] = {
        "Difficulty: Easy",
        "Difficulty: Medium",
        "Difficulty: Hard",
        "Character Color: Default",
        "Character Color: Red",
        "Character Color: Blue",
        "Music: None",
        "Music: Music 1",
        "Music: Music 2",
        "Back to Main Menu"
    };

    int n_choices = sizeof(choices) / sizeof(char *);
    int current_choice = 0;


    while (1) {
        for (int i = 0; i < n_choices; i++) {
            if (i == current_choice)
                attron(A_REVERSE);  
            mvprintw(5 + i, 10, "%s", choices[i]);
            if (i == current_choice)
                attroff(A_REVERSE);
        }

        // دریافت ورودی
        ch = getch();
        switch (ch) {
            case KEY_UP:
                if (current_choice > 0) {
                    current_choice--;
                }
                break;
            case KEY_DOWN:
                if (current_choice < n_choices - 1) {
                    current_choice++;
                }
                break;
            case 10:  
                if (current_choice == 0) {  
                    difficulty = 0;
                } else if (current_choice == 1) {  
                    difficulty = 1;
                } else if (current_choice == 2) {  
                    difficulty = 2;
                } else if (current_choice == 3) {  
                    color_scheme = 1;
                } else if (current_choice == 4) {  
                    color_scheme = 2;
                } else if (current_choice == 5) {  
                    color_scheme = 3;
                } else if (current_choice == 6) {  
                    music_choice = 0;
                } else if (current_choice == 7) {  
                    music_choice = 1;
                } else if (current_choice == 8) {  
                    music_choice = 2;
                } else if (current_choice == 9) {  
                    endwin();  
                    return;
                }
                clear();
                draw_rect();
                attron(COLOR_PAIR(3) | A_BOLD);
                mvprintw(3, 10, "Game Settings Updated!");
                attroff(COLOR_PAIR(3) | A_BOLD);
                refresh();
                getch();  
                break;
        }
    }
}


int main()
{
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);  

    show_settings_menu();

    endwin(); 
    return 0;
}
