#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

#define WIDTH 130
#define HEIGHT 35
#define MAX_ROOMS 8
#define MAX_DOORS (MAX_ROOMS * 2)
#define INF INT_MAX
#define OUTER_WIDTH 170
#define OUTER_HEIGHT 42

int level = 1;
int x_end;
int y_end;

typedef struct
{
    int x, y;
    int width, height;
    int door_x1, door_y1;
    int door_x2, door_y2;
    char type;
} Room;

typedef struct
{
    int x, y;
    int health;
    int hunger;
    int stamina;
    int Gold;
    char prev_char;
} Player;

int check_overlap(Room rooms[], int num_rooms, Room new_room)
{
    
    if (new_room.x < 37 || new_room.x + new_room.width >= 37 + WIDTH ||
        new_room.y < 7 || new_room.y + new_room.height >= 7 + HEIGHT)
    {
        return 1;
    }

    for (int i = 0; i < num_rooms; i++)
    {
        if (!(new_room.x + new_room.width < rooms[i].x ||
              new_room.x > rooms[i].x + rooms[i].width ||
              new_room.y + new_room.height < rooms[i].y ||
              new_room.y > rooms[i].y + rooms[i].height))
        {
            return 1;
        }
    }
    return 0;
}

void draw_rect(int x_offset, int y_offset, int width, int height)
{
    for (int y = y_offset; y < y_offset + height; y++)
    {
        for (int x = x_offset; x < x_offset + width; x++)
        {
            if (y == y_offset || y == y_offset + height - 1)
                mvaddch(y, x, '-');
            else if (x == x_offset || x == x_offset + width - 1)
                mvaddch(y, x, '|');
            else
                mvaddch(y, x, ' ');
        }
    }
    refresh();
}

void draw_room(Room r)
{
    for (int y = r.y; y < r.y + r.height; y++)
    {
        for (int x = r.x; x < r.x + r.width; x++)
        {
            if (y == r.y || y == r.y + r.height - 1)
                mvaddch(y, x, '-');
            else if (x == r.x || x == r.x + r.width - 1)
                mvaddch(y, x, '|');
            else
                mvaddch(y, x, '.');
        }
    }
}

void place_doors(Room *r, int i, int num_rooms)
{
    if (i == 0)
    {
        r->door_x2 = r->x + r->width - 1;
        r->door_y2 = (rand() % (r->height - 2)) + r->y + 1;
    }
    else if (i == num_rooms - 1)
    {
        r->door_x1 = r->x;
        r->door_y1 = (rand() % (r->height - 2)) + r->y + 1;
    }
    else
    {
        r->door_x1 = r->x;
        r->door_y1 = (rand() % (r->height - 2)) + r->y + 1;
        r->door_x2 = r->x + r->width - 1;
        r->door_y2 = (rand() % (r->height - 2)) + r->y + 1;
    }
    mvaddch(r->door_y1, r->door_x1, '+');
    if (i >= 0 && i < num_rooms - 1)
        mvaddch(r->door_y2, r->door_x2, '+');
}

void draw_corridor(int sx, int sy, int ex, int ey)
{
    sx += 1;
    ex -= 1;
    int sy_copy = sy;
    int ey_copy = ey;
    while (sx != ex)
    {
        mvaddch(sy, sx, '#');
        sx += (sx < ex) ? 1 : -1;
    }
    while (sy != ey)
    {
        mvaddch(sy, sx, '#');
        sy += (sy < ey) ? 1 : -1;
    }
    if (sy_copy < ey_copy)
    {
        mvaddch(sy, sx, '#');
    }
    if (sy_copy > ey_copy)
    {
        mvaddch(sy, sx, '#');
    }
    char ch = mvinch(sy, sx + 1) & A_CHARTEXT;
    if (ch == ' ')
    {
        mvaddch(sy, sx += 1, '#');
    }
    x_end = sx;
    y_end = sy;
}

void generate_corridors(Room rooms[], int num_rooms)
{
    for (int i = 0; i < num_rooms - 1; i++)
    {
        draw_corridor(rooms[i].door_x2, rooms[i].door_y2, rooms[i + 1].door_x1, rooms[i + 1].door_y1);
    }
    // adding stairs
    char ch1 = mvinch(y_end, x_end - 1) & A_CHARTEXT;
    char ch2 = mvinch(y_end + 1, x_end) & A_CHARTEXT;
    char ch3 = mvinch(y_end - 1, x_end) & A_CHARTEXT;
    char ch4 = mvinch(y_end, x_end + 1) & A_CHARTEXT;
    if (ch1 == ' ')
    {
        mvaddch(y_end, x_end - 1, '^');
    }
    else if (ch2 == ' ')
    {
        mvaddch(y_end + 1, x_end, '^');
    }
    else if (ch3 == ' ')
    {
        mvaddch(y_end - 1, x_end, '^');
    }
    else if (ch4 == ' ')
    {
        mvaddch(y_end, x_end + 1, '^');
    }
}

void place_items(Room *r, int room_index)
{
    
    for (int i = 0; i < 2; i++)
    {
        int gold_x = r->x + 1 + rand() % (r->width - 2);
        int gold_y = r->y + 1 + rand() % (r->height - 2);
        attron(COLOR_PAIR(1));
        mvaddch(gold_y, gold_x, 'G');
        attroff(COLOR_PAIR(1));
    }


    if (room_index == 2 || room_index == 4 || room_index == 5 || room_index == 7)
    {
        char foods[] = {'m', 'T', 'J', 'F'};
        int food_x = r->x + 1 + rand() % (r->width - 2);
        int food_y = r->y + 1 + rand() % (r->height - 2);
        char food_type = foods[rand() % 4];
        attron(COLOR_PAIR(2));
        mvaddch(food_y, food_x, food_type);
        attroff(COLOR_PAIR(2));
    }

    if (room_index == 3 || room_index == 5)
    {
        int trap_x = r->x + 1 + rand() % (r->width - 2);
        int trap_y = r->y + 1 + rand() % (r->height - 2);
        attron(COLOR_PAIR(3));
        mvaddch(trap_y, trap_x, 'Z');
        attroff(COLOR_PAIR(3));
    }
}

void generate_rooms(Room rooms[], int *num_rooms)
{
    *num_rooms = (rand() % 2) ? 6 : 8;
    int section_width = (WIDTH - 40) / *num_rooms;
    int count = 0;

    while (count < *num_rooms)
    {
        Room temp;
        temp.width = 10 + (rand() % 5);
        temp.height = 6 + (rand() % 4);

        if (section_width > temp.width + 1)
        {
            temp.x = 38 + (count * section_width) + (rand() % (section_width - temp.width - 1));
        }
        else
        {
            temp.x = 38 + (count * section_width);
            if (count != 0 && temp.x <= rooms[count - 1].x + rooms[count - 1].width)
            {
                temp.x = (rooms[count - 1].x + rooms[count - 1].width - temp.x) + 2;
            }
        }

        if (HEIGHT - 10 - temp.height > 0)
        {
            temp.y = 8 + (rand() % (HEIGHT - 10 - temp.height));
        }
        else
        {
            temp.y = 8;
        }

        temp.type = (count == 0 || count == *num_rooms - 1) ? 'b' : 'm';

        if (!check_overlap(rooms, count, temp))
        {
            rooms[count] = temp;
            count++;
        }
    }
}

void place_player(Room rooms[], int num_rooms, Player *player)
{
    int room_index = 3; 
    Room *r = &rooms[room_index];
    player->x = r->x + 1 + rand() % (r->width - 2);
    player->y = r->y + 1 + rand() % (r->height - 2);
    player->prev_char = '.';
    attron(COLOR_PAIR(4));
    mvaddch(player->y, player->x, 'P');
    attroff(COLOR_PAIR(4));
}

void init_colors()
{
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  
    init_pair(3, COLOR_BLUE, COLOR_BLACK);    
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    
}
void display_stats(Player *player)
{
    mvprintw(2, 8, "Health: %d", player->health);
    mvprintw(3, 8, "Hunger: %d", player->hunger);
    mvprintw(4, 8, "Stamina: %d", player->stamina);
    mvprintw(5, 8, "Gold: %d", player->Gold);
    refresh();
}

void move_player(Player *player, Room rooms[], int num_rooms, int ch);

void creat_level()
{
    while(level != 5)
    {
        Player player = {0, 0, 100, 50, 100, 0, '.'};

        init_colors();
        draw_rect(7, 1, OUTER_WIDTH, OUTER_HEIGHT);

        mvprintw(2, 8, "Health: %d", player.health);
        mvprintw(3, 8, "Hunger: %d", player.hunger);
        mvprintw(4, 8, "Stamina: %d", player.stamina);
        mvprintw(5, 8, "Gold: %d", player.Gold);

        draw_rect(37, 7, WIDTH, HEIGHT);

        Room rooms[MAX_ROOMS];
        int num_rooms;
        generate_rooms(rooms, &num_rooms);

        for (int i = 0; i < num_rooms; i++)
        {
            if (rooms[i].type == 'b')
            {
                attron(COLOR_PAIR(5));
                draw_room(rooms[i]);
                attroff(COLOR_PAIR(5));
            }
            else
            {
                draw_room(rooms[i]);
            }

            place_doors(&rooms[i], i, num_rooms);
            place_items(&rooms[i], i);
        }

        generate_corridors(rooms, num_rooms);
        place_player(rooms, num_rooms, &player);

        refresh();
        int ch;
        while ((ch = getch()) != 'q')
        {
            move_player(&player, rooms, num_rooms, ch);
            display_stats(&player);
            refresh();
        }
    }
}

void move_player(Player *player, Room rooms[], int num_rooms, int ch)
{
    int new_x = player->x;
    int new_y = player->y;
    char current_char;

    switch (ch)
    {
    case '2':
        new_y++;
        break;
    case '8':
        new_y--;
        break;
    case '6':
        new_x++;
        break;
    case '4':
        new_x--;
        break;
    case '1':
        new_y++;
        new_x--;
        break;
    case '3':
        new_y++;
        new_x++;
        break;
    case '7':
        new_y--;
        new_x--;
        break;
    case '9':
        new_y--;
        new_x++;
        break;
    default:
        return;
    }

    current_char = mvinch(new_y, new_x) & A_CHARTEXT;

    if (current_char != ' ' && current_char != '|' && current_char != '-')
    {
        if (current_char == 'G')
        {
            player->Gold++;
        }
        if (player->prev_char == '#')
            mvaddch(player->y, player->x, '#');
        else if (player->prev_char == 'G')
        {
            mvaddch(player->y, player->x, '.');
        }
        else if (current_char == '^')
        {
            level++;  
            
        }
        else
            mvaddch(player->y, player->x, player->prev_char);

        player->prev_char = (current_char == '#') ? '#' : (current_char == 'G') ? '.'
                                                                                : current_char;
        player->x = new_x;
        player->y = new_y;

        attron(COLOR_PAIR(4));
        mvaddch(player->y, player->x, 'P');
        attroff(COLOR_PAIR(4));
    }
}

int main()
{
    initscr();
    noecho();
    curs_set(FALSE);
    srand(time(0));
    start_color();

    while (1)
    {
        Player player = {0, 0, 100, 50, 100, 0, '.'};

        init_colors();
        draw_rect(7, 1, OUTER_WIDTH, OUTER_HEIGHT);

        mvprintw(2, 8, "Health: %d", player.health);
        mvprintw(3, 8, "Hunger: %d", player.hunger);
        mvprintw(4, 8, "Stamina: %d", player.stamina);
        mvprintw(5, 8, "Gold: %d", player.Gold);

        draw_rect(37, 7, WIDTH, HEIGHT);

        Room rooms[MAX_ROOMS];
        int num_rooms;
        generate_rooms(rooms, &num_rooms);

        for (int i = 0; i < num_rooms; i++)
        {
            if (rooms[i].type == 'b')
            {
                attron(COLOR_PAIR(5));
                draw_room(rooms[i]);
                attroff(COLOR_PAIR(5));
            }
            else
            {
                draw_room(rooms[i]);
            }

            place_doors(&rooms[i], i, num_rooms);
            place_items(&rooms[i], i);
        }

        generate_corridors(rooms, num_rooms);
        place_player(rooms, num_rooms, &player);

        refresh();
        int ch;
        while ((ch = getch()) != 'q')
        {
            move_player(&player, rooms, num_rooms, ch);
            display_stats(&player);
            refresh();

            
            // char current_char = mvinch(player.y, player.x) & A_CHARTEXT;
            // if (current_char == '^')
            // {
            //     level++; 
            //     break;   
            // }
        }
        //   printw("ooooooooooooooooooooooo");
        // clear();
        // refresh();
        // break; 
    }

    // printw("iiiiiiiiiiiiiiiiiiiiiiiiiiiii");
    // refresh();

//     while (2)
//     {
//         Player player = {0, 0, 100, 50, 100, 0, '.'};

//         init_colors();
//         draw_rect(7, 1, OUTER_WIDTH, OUTER_HEIGHT);

//         mvprintw(2, 8, "Health: %d", player.health);
//         mvprintw(3, 8, "Hunger: %d", player.hunger);
//         mvprintw(4, 8, "Stamina: %d", player.stamina);
//         mvprintw(5, 8, "Gold: %d", player.Gold);

//         draw_rect(37, 7, WIDTH, HEIGHT);

//         Room rooms[MAX_ROOMS];
//         int num_rooms;
//         generate_rooms(rooms, &num_rooms);

//         for (int i = 0; i < num_rooms; i++)
//         {
//             if (rooms[i].type == 'b')
//             {
//                 attron(COLOR_PAIR(5));
//                 draw_room(rooms[i]);
//                 attroff(COLOR_PAIR(5));
//             }
//             else
//             {
//                 draw_room(rooms[i]);
//             }

//             place_doors(&rooms[i], i, num_rooms);
//             place_items(&rooms[i], i);
//         }

//         generate_corridors(rooms, num_rooms);
//         place_player(rooms, num_rooms, &player);

//         refresh();
//         int ch;
//         while ((ch = getch()) != 'q')
//         {
//             move_player(&player, rooms, num_rooms, ch);
//             display_stats(&player);
//             refresh();
//         }
//         char current_char = mvinch(player.y, player.x) & A_CHARTEXT;
//         if (current_char == '^')
//         {
//             level++;
//             break;
//         }
//         break;
//     }
//     refresh();
//     while (3)
//     {
//         Player player = {0, 0, 100, 50, 100, 0, '.'};

//         init_colors();
//         draw_rect(7, 1, OUTER_WIDTH, OUTER_HEIGHT);

//         mvprintw(2, 8, "Health: %d", player.health);
//         mvprintw(3, 8, "Hunger: %d", player.hunger);
//         mvprintw(4, 8, "Stamina: %d", player.stamina);
//         mvprintw(5, 8, "Gold: %d", player.Gold);

//         draw_rect(37, 7, WIDTH, HEIGHT);

//         Room rooms[MAX_ROOMS];
//         int num_rooms;
//         generate_rooms(rooms, &num_rooms);

//         for (int i = 0; i < num_rooms; i++)
//         {
//             if (rooms[i].type == 'b')
//             {
//                 attron(COLOR_PAIR(5));
//                 draw_room(rooms[i]);
//                 attroff(COLOR_PAIR(5));
//             }
//             else
//             {
//                 draw_room(rooms[i]);
//             }

//             place_doors(&rooms[i], i, num_rooms);
//             place_items(&rooms[i], i);
//         }

//         generate_corridors(rooms, num_rooms);
//         place_player(rooms, num_rooms, &player);

//         refresh();
//         int ch;
//         while ((ch = getch()) != 'q')
//         {
//             move_player(&player, rooms, num_rooms, ch);
//             display_stats(&player);
//             refresh();
//         }
//         char current_char = mvinch(player.y, player.x) & A_CHARTEXT;
//         if (current_char == '^')
//         {
//             level++;
//             break;
//         }
//         break;
//     }
//     while (4)
//     {
//         Player player = {0, 0, 100, 50, 100, 0, '.'};

//         init_colors();
//         draw_rect(7, 1, OUTER_WIDTH, OUTER_HEIGHT);

//         mvprintw(2, 8, "Health: %d", player.health);
//         mvprintw(3, 8, "Hunger: %d", player.hunger);
//         mvprintw(4, 8, "Stamina: %d", player.stamina);
//         mvprintw(5, 8, "Gold: %d", player.Gold);

//         draw_rect(37, 7, WIDTH, HEIGHT);

//         Room rooms[MAX_ROOMS];
//         int num_rooms;
//         generate_rooms(rooms, &num_rooms);

//         for (int i = 0; i < num_rooms; i++)
//         {
//             if (rooms[i].type == 'b')
//             {
//                 attron(COLOR_PAIR(5));
//                 draw_room(rooms[i]);
//                 attroff(COLOR_PAIR(5));
//             }
//             else
//             {
//                 draw_room(rooms[i]);
//             }

//             place_doors(&rooms[i], i, num_rooms);
//             place_items(&rooms[i], i);
//         }

//         generate_corridors(rooms, num_rooms);
//         place_player(rooms, num_rooms, &player);

//         refresh();
//         int ch;
//         while ((ch = getch()) != 'q')
//         {
//             move_player(&player, rooms, num_rooms, ch);
//             display_stats(&player);
//             refresh();
//         } // avaz shavad!
//         char current_char = mvinch(player.y, player.x) & A_CHARTEXT;
//         if (current_char == '^')
//         {
//             level++;
//             break;
//         }
//     }

    endwin();
    return 0;
}
