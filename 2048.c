#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define COLOR_LIGHT_PINK 10
#define COLOR_MEDIUM_PINK 11
#define COLOR_DARK_PINK 12
#define COLOR_VERY_DARK_PINK 13
#define COLOR_CORAL 14
#define COLOR_LIGHT_PURPLE 15
#define COLOR_MEDIUM_LIGHT_PURPLE 16
#define COLOR_MEDIUM_PURPLE 17
#define COLOR_MEDIUM_DARK_PURPLE 18
#define COLOR_DARK_PURPLE 19

#define MAX_STEPS_BACK 500

// mai sus am definit cateva culori pentru a colora celulele jocului
// am definit si o variabila care reprezinta numarul maxim de pasi in spate pe
// care ii pot face cu UNDO
int mat[4][4] = {0}, a[4], score = 0, highscore = 0, steps = 0;

// functie care muta elementele nenule ale unui vector spre dreapta pana in
// capat
void move_to_end(int k)
{
    int i, j;
    if (k < 3)
    {
        i = k;
        while (i < 4)
        {
            if (a[i] != 0)
            {
                move_to_end(i + 1);

                j = i;
                while (j < 3)
                {
                    if (a[j + 1] == 0)
                    {
                        a[j + 1] = a[j];
                        a[j] = 0;
                    }
                    else
                        break;

                    j++;
                }
            }

            i++;
        }
    }
}

// functie care lipeste 2 celule cu aceeasi valoare, calculeaza scorul si
// highscore-ul si efectueaza miscarea spre margine
void merge_and_move()
{
    int i;

    for (i = 3; i > 0; i--)
    {
        if (a[i] == a[i - 1])
        {
            a[i] = a[i] + a[i - 1];
            a[i - 1] = 0;
            score = score + a[i];

            if (score > highscore)
                highscore = score;
        }

        else if (a[i] != 0 && a[i - 1] == 0)
        {
            a[i - 1] = a[i];
            a[i] = 0;
        }
    }

    move_to_end(0);
}

// functie care genereaza o valoarea aleatorie de 2 sau 4 pe o pozitie aleatorie
void random_gen()
{
    srand(time(NULL));

    int i, j;
    do
    {
        i = rand() % 4;
        j = rand() % 4;
    } while (mat[i][j] != 0);

    int random_value;

    if (rand() % 2 == 0)
        random_value = 2;

    else
        random_value = 4;

    mat[i][j] = random_value;
}

// funtie care verifica daca jocul a fost pierdut sau castigat
int check_status(int mat[4][4])
{
    int i, j, count = 0;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (mat[i][j] == 2048)
                return 1;

            if (mat[i][j] == 0)
                count++;
        }
    }

    int equals = 0;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
          if ((i < 3 && mat[i][j] == mat[i + 1][j]) ||
              (j < 3 && mat[i][j] == mat[i][j + 1]))
            equals++;

    if (count == 0 && equals == 0)
        return 2;

    return 0;
}

// functie care retine intr-un vector de matrici, matricea curenta a jocului la
// fiecare pas, dar si scorul
void previous_steps(int ***prev, int *s)
{
    int i, j, k;

    if (steps == MAX_STEPS_BACK + 1)
    {
        for (i = 0; i < MAX_STEPS_BACK; i++)
        {
            for (j = 0; j < 4; j++)
            {
                for (k = 0; k < 4; k++)
                    prev[i][j][k] = prev[i + 1][j][k];
                s[i] = s[i + 1];
            }
            s[i] = s[i + 1];
        }

        steps = MAX_STEPS_BACK;
    }

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            prev[steps][i][j] = mat[i][j];
        }
    }
    s[steps] = score;
}

// functie pentru implementarea unei miscari inapoi
void take_step_back(int ***prev, int *s)
{
    int i, j;

    steps--;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            mat[i][j] = prev[steps][i][j];
    }

    score = s[steps];
}

// functie care verifica daca o mutare e valida sau nu
int check_if_invalid(int ***prev, int mat[4][4])
{
    int i, j, count = 0;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (prev[steps][i][j] != mat[i][j])
                count++;

    if (count == 0)
        return 1;

    return 0;
}

// functie care returneaza cea mai buna directie pentru miscarea automata
int determine_best_direction()
{
  int aux[4][4] = {0}, i, j, count = 0, max_count = -1,
      best_direction = KEY_LEFT;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) aux[i][j] = mat[i][j];

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) a[j] = mat[i][j];

    merge_and_move();

    for (j = 0; j < 4; j++) mat[i][j] = a[j];
    }

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (mat[i][j] == 0)
                count++;

    if (count > max_count)
    {
        max_count = count;
        best_direction = KEY_RIGHT;
    }

    count = 0;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat[i][j] = aux[i][j];

    for (i = 0; i < 4; i++)
    {
        for (j = 3; j >= 0; j--)
            a[3 - j] = mat[i][j];

        merge_and_move();

        for (j = 0; j < 4; j++)
            mat[i][3 - j] = a[j];
    }

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (mat[i][j] == 0)
                count++;

    if (count > max_count)
    {
        max_count = count;
        best_direction = KEY_LEFT;
    }

    count = 0;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat[i][j] = aux[i][j];

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
            a[j] = mat[j][i];

        merge_and_move();

        for (j = 0; j < 4; j++)
            mat[j][i] = a[j];
    }

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (mat[i][j] == 0)
                count++;

    if (count > max_count)
    {
        max_count = count;
        best_direction = KEY_DOWN;
    }

    count = 0;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat[i][j] = aux[i][j];

    for (i = 0; i < 4; i++)
    {
        for (j = 3; j >= 0; j--)
            a[3 - j] = mat[j][i];

        merge_and_move();

        for (j = 0; j < 4; j++)
            mat[3 - j][i] = a[j];
    }

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            if (mat[i][j] == 0)
                count++;

    if (count > max_count)
    {
        max_count = count;
        best_direction = KEY_UP;
    }

    count = 0;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            mat[i][j] = aux[i][j];

    return best_direction;
}

// functie pentru afisajului meniului
void Menu(int *p)
{
    int xMax, yMax;
    getmaxyx(stdscr, yMax, xMax);

    bkgd(COLOR_PAIR(1));
    refresh();

    WINDOW *menu = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
    refresh();

    box(menu, 0, 0);
    wrefresh(menu);

    wbkgd(menu, COLOR_PAIR(1));
    wrefresh(menu);

    char text2[] = "SELECT OPTION";
    int len2 = strlen(text2);

    int xWin, yWin;
    getmaxyx(menu, yWin, xWin);

    mvwprintw(menu, 1, (xWin / 2) - (len2 / 2), text2);
    wrefresh(menu);

    keypad(menu, true);
    char *options[3] = {"* NEW GAME *", "* RESUME *", "* QUIT *"};
    int choice;
    int highlight = 0;

    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            if (i == highlight)
                wattron(menu, A_REVERSE);

            mvwprintw(menu, i + yWin / 3, i + (xWin / 2) - (len2 / 2),
                      options[i]);
            wattroff(menu, A_REVERSE);
        }

        choice = wgetch(menu);

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight == -1)
                highlight = 0;
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight == 3)
                highlight = 2;
            break;
        default:
            break;
        }

        if (choice == 10)
            break;
    }

    if (highlight == 2)
        *p = 2;

    else if (highlight == 1)
        *p = 1;

    else if (highlight == 0)
        *p = 0;
}

// functie pentru afisajul tablei de joc, a panoului de control si implementarea
// propriu-zisa a jocului
void Game(char *p, int mat[4][4], int ***prev, int *s)
{
    timeout(0);
    noecho();
    int xMax, yMax;
    int ch;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *new_game = newwin(yMax, xMax, 0, 0);
    refresh();

    wbkgd(new_game, COLOR_PAIR(1));
    wrefresh(new_game);
    keypad(new_game, true);

    WINDOW *header = newwin(1, xMax, 0, 0);
    refresh();

    wbkgd(header, COLOR_PAIR(1));
    wrefresh(header);

    char text[] = "2048 GAME";
    int len = strlen(text);

    mvwprintw(header, 0, (xMax / 2) - (len / 2), text);
    wrefresh(header);

    WINDOW *control_panel = newwin(yMax - 1, xMax / 2, 1, 0);
    refresh();

    wbkgd(control_panel, COLOR_PAIR(1));
    wrefresh(control_panel);

    box(control_panel, 0, 0);
    wrefresh(control_panel);

    WINDOW *actual_game = newwin(yMax - 1, xMax / 2, 1, xMax / 2 + 1);
    refresh();

    wbkgd(actual_game, COLOR_PAIR(1));
    wrefresh(actual_game);

    box(actual_game, 0, 0);
    wrefresh(actual_game);

    // mai sus am desenat ferestrele pentru panoul de control si jocul
    // propriu-zis

    int yGame, xGame;
    getmaxyx(actual_game, yGame, xGame);
    int cell_height = yGame / 4;
    int cell_width = xGame / 4;

    // am declarat 2 variabile care retin timpul la momentul curent

    time_t current_time;
    struct tm *local_time;

    int i, j;

    // am generat aleator la inceperea jocului, doar daca jocul inceput este
    // unul nou
    if (score == 0)
    {
        random_gen();
        random_gen();
    }

    // am initializat timpul actual, timpul ultimei apasari a unei taste
    // am setat durata dintre apasarea unei taste si inceperea unei miscari
    // automate la 10 secunde
    clock_t last_keypress_time = clock();
    clock_t time_at_moment = clock();
    clock_t elapsed_time;
    int time_limit = 10000000;
    int best_direction = KEY_LEFT;

    nodelay(new_game, true);

    while (1)
    {
      char text2[] = "CONTROL PANEL";
      int len2 = strlen(text2);

      mvwprintw(control_panel, 1, (xMax / 4) - (len2 / 2), text2);
      wrefresh(control_panel);

      char text3[] = "SCORE:";
      int len3 = strlen(text3);

      mvwprintw(control_panel, yMax / 5, (xMax / 4) - (len3 / 2), text3);
      move(yMax / 5 + 1, (xMax / 4) - (len3 / 2));
      wprintw(control_panel, "%d", score);
      wrefresh(control_panel);

      char text7[] = "HIGHSCORE:";
      int len7 = strlen(text7);

      mvwprintw(control_panel, 2 * yMax / 5, (xMax / 4) - (len7 / 2), text7);
      move(2 * yMax / 5 + 1, (xMax / 4) - (len7 / 2));
      wprintw(control_panel, "%d", highscore);
      wrefresh(control_panel);

      char text5[] = "COMMANDS:";
      int len5 = strlen(text5);
      mvwprintw(control_panel, 4 * yMax / 5, (xMax / 4) - (len5 / 2), text5);
      wrefresh(control_panel);

      char command1[] = "PRESS \x5E TO MOVE THE BLOCKS UPWARDS";
      int lc1 = strlen(command1);
      mvwprintw(control_panel, 4 * yMax / 5 + 1, (xMax / 4) - (lc1 / 2),
                command1);
      wrefresh(control_panel);

      char command2[] = "PRESS \x76 TO MOVE THE BLOCKS DOWNWARDS";
      int lc2 = strlen(command2);
      mvwprintw(control_panel, 4 * yMax / 5 + 2, (xMax / 4) - (lc2 / 2),
                command2);
      wrefresh(control_panel);

      char command3[] = "PRESS \x3C TO MOVE THE BLOCKS LEFT";
      int lc3 = strlen(command3);
      mvwprintw(control_panel, 4 * yMax / 5 + 3, (xMax / 4) - (lc3 / 2),
                command3);
      wrefresh(control_panel);

      char command4[] = "PRESS \x3E TO MOVE THE BLOCKS RIGHT";
      int lc4 = strlen(command4);
      mvwprintw(control_panel, 4 * yMax / 5 + 4, (xMax / 4) - (lc4 / 2),
                command4);
      wrefresh(control_panel);

      char command5[] = "PRESS q OR Q TO EXIT";
      int lc5 = strlen(command5);
      mvwprintw(control_panel, 4 * yMax / 5 + 5, (xMax / 4) - (lc5 / 2),
                command5);

      char command6[] = "PRESS u OR U TO UNDO";
      int lc6 = strlen(command6);
      mvwprintw(control_panel, 4 * yMax / 5 + 6, (xMax / 4) - (lc6 / 2),
                command6);

      wrefresh(control_panel);

      // mai sus am actualizat constant continutul panoului de control pentru
      // a se actualiza permanent si scorul si highscore-ul

      // am efectuat modificarile pentru fiecare tip de mutare in parte

      // in cazul in care mutarea este valida, am generat o noua valoare de 2
      // sau 4

      char valid[] = "YOU PRESSED:";
      int len10 = strlen(valid);

      mvwprintw(control_panel, 3, (xMax / 4) - (len10 / 2), valid);
      wrefresh(control_panel);

      ch = wgetch(new_game);

      if (ch == KEY_RIGHT) {
        last_keypress_time = clock();
        steps++;
        previous_steps(prev, s);

        for (i = 0; i < 4; i++) {
          for (j = 0; j < 4; j++) a[j] = mat[i][j];

          merge_and_move();

          for (j = 0; j < 4; j++) mat[i][j] = a[j];
        }

        if (check_if_invalid(prev, mat) == 0) {
          random_gen();
          mvwprintw(control_panel, 4, (xMax / 4), "\x3E ");
          wrefresh(control_panel);
        }
        }

        else if (ch == KEY_LEFT)
        {
            last_keypress_time = clock();
            steps++;
            previous_steps(prev, s);

            for (i = 0; i < 4; i++)
            {
                for (j = 3; j >= 0; j--)
                    a[3 - j] = mat[i][j];

                merge_and_move();

                for (j = 0; j < 4; j++)
                    mat[i][3 - j] = a[j];
            }

            if (check_if_invalid(prev, mat) == 0)
            {
                random_gen();
                mvwprintw(control_panel, 4, (xMax / 4), "\x3C ");
                wrefresh(control_panel);
            }
        }

        else if (ch == KEY_DOWN)
        {
            last_keypress_time = clock();
            steps++;
            previous_steps(prev, s);

            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                    a[j] = mat[j][i];

                merge_and_move();

                for (j = 0; j < 4; j++)
                    mat[j][i] = a[j];
            }

            if (check_if_invalid(prev, mat) == 0)
            {
                random_gen();
                mvwprintw(control_panel, 4, (xMax / 4), "\x76 ");
                wrefresh(control_panel);
            }
        }

        else if (ch == KEY_UP)
        {
            last_keypress_time = clock();
            steps++;
            previous_steps(prev, s);

            for (i = 0; i < 4; i++)
            {
                for (j = 3; j >= 0; j--)
                    a[3 - j] = mat[j][i];

                merge_and_move();

                for (j = 0; j < 4; j++)
                    mat[3 - j][i] = a[j];
            }

            if (check_if_invalid(prev, mat) == 0)
            {
                random_gen();
                mvwprintw(control_panel, 4, (xMax / 4), "\x5E ");
                wrefresh(control_panel);
            }
        }

        else if ((ch == 'u' || ch == 'U') && steps > 0)
        {
            last_keypress_time = clock();
            take_step_back(prev, s);
        }

        if (check_if_invalid(prev, mat) != 0)
        {
          char valid2[] =
              "THE MOVE IS INVALID, PRESS ANOTHER KEY FOR A VALID MOVE !";
          int len11 = strlen(valid2);

          mvwprintw(control_panel, 5, (xMax / 4) - (len11 / 2), valid2);
          wrefresh(control_panel);
        }

        else
        {
          char valid2[] =
              "                                                         ";
          int len11 = strlen(valid2);

          mvwprintw(control_panel, 5, (xMax / 4) - (len11 / 2), valid2);
          wrefresh(control_panel);
        }

        // am resetat timpul de fiecare data cand se apasa o tasta

        time_at_moment = clock();
        elapsed_time = time_at_moment - last_keypress_time;

        // am efectuat miscarea automata cand au trecut mai mult de 10 secunde
        // de la apasarea unei taste

        if (elapsed_time >= time_limit)
        {

            best_direction = determine_best_direction();

            if (best_direction == KEY_RIGHT)
            {
                steps++;
                previous_steps(prev, s);

                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                        a[j] = mat[i][j];

                    merge_and_move();

                    for (j = 0; j < 4; j++)
                        mat[i][j] = a[j];
                }
                random_gen();
            }

            else if (best_direction == KEY_LEFT)
            {
                steps++;
                previous_steps(prev, s);

                for (i = 0; i < 4; i++)
                {
                    for (j = 3; j >= 0; j--)
                        a[3 - j] = mat[i][j];

                    merge_and_move();

                    for (j = 0; j < 4; j++)
                        mat[i][3 - j] = a[j];
                }
                random_gen();
            }

            else if (best_direction == KEY_DOWN)
            {
                steps++;
                previous_steps(prev, s);

                for (i = 0; i < 4; i++)
                {
                    for (j = 0; j < 4; j++)
                        a[j] = mat[j][i];

                    merge_and_move();

                    for (j = 0; j < 4; j++)
                        mat[j][i] = a[j];
                }
                random_gen();
            }

            else if (best_direction == KEY_UP)
            {
                steps++;
                previous_steps(prev, s);

                for (i = 0; i < 4; i++)
                {
                    for (j = 3; j >= 0; j--)
                        a[3 - j] = mat[j][i];

                    merge_and_move();

                    for (j = 0; j < 4; j++)
                        mat[3 - j][i] = a[j];
                }
                random_gen();
            }
            last_keypress_time = clock();
        }

        // am impartit fereastra jocului in ferestre mai mici pe care le-am
        // colorat in functie de valoare

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                int y = i * cell_height;
                int x = j * cell_width;

                WINDOW *cell =
                    newwin(cell_height, cell_width, 1 + y, xMax / 2 + 1 + x);

                if (mat[i][j] < 256)
                    wbkgd(cell, COLOR_PAIR(mat[i][j]));

                else if (mat[i][j] == 256)
                    wbkgd(cell, COLOR_PAIR(9));

                else if (mat[i][j] == 512)
                    wbkgd(cell, COLOR_PAIR(10));

                else if (mat[i][j] == 1024)
                    wbkgd(cell, COLOR_PAIR(11));

                else if (mat[i][j] == 2048)
                    wbkgd(cell, COLOR_PAIR(12));

                if (mat[i][j] == 0)
                    wbkgd(cell, COLOR_PAIR(1));

                box(cell, 0, 0);
                wrefresh(cell);

                mvwprintw(cell, cell_height / 2, cell_width / 2, "%d",
                          mat[i][j]);
                wrefresh(cell);
                delwin(cell);
            }
        }

        wrefresh(actual_game);

        // am verificat daca jocul a fost castigat sau pierdut si am afisat un
        // mesaj corespunzator intr-o noua fereastra

        if (check_status(mat) == 1 || check_status(mat) == 2)
        {
            WINDOW *message = newwin(yMax, xMax, 0, 0);
            wrefresh(message);

            wbkgd(message, COLOR_PAIR(1));
            wrefresh(message);

            box(message, 0, 0);
            wrefresh(message);

            if (check_status(mat) == 1)
            {
              // timeout(5000);
              char text8[] = "CONGRATS, YOU'RE A JUICY DELICIOS GAMER NOW <3";
              int len8 = strlen(text8);

              mvwprintw(message, yMax / 2, (xMax / 2) - (len8 / 2), text8);
              wrefresh(message);

              *p = 'w';

              wgetch(message);
              break;
            }

            else if (check_status(mat) == 2)
            {
              char text9[] = "OOPS, YOU DID IT AGAIN ... IT'S OVER NOW";
              int len9 = strlen(text9);

              mvwprintw(message, yMax / 2, (xMax / 2) - (len9 / 2), text9);
              wrefresh(message);

              *p = 'l';
              wgetch(message);
              break;
            }
            wgetch(message);
            break;
        }

        // daca s-a apasat tasta q am parasit jocul

        if (ch == 'q' || ch == 'Q')
        {
            *p = 'q';
            break;
        }

        // am afisat permanent data si ora curenta

        time(&current_time);
        local_time = localtime(&current_time);

        char date_and_time[50];
        strftime(date_and_time, sizeof(date_and_time), "%Y-%m-%d %H:%M:%S",
                 local_time);

        char text4[] = "DATE AND TIME:";
        int len4 = strlen(text4);
        mvwprintw(control_panel, 3 * yMax / 5, (xMax / 4) - (len4 / 2), text4);
        mvwprintw(control_panel, 3 * yMax / 5 + 1,
                  (xMax / 4) - (strlen(date_and_time) / 2), date_and_time);
        wrefresh(control_panel);

        wrefresh(control_panel);
    }
}

int main()
{

    // am initialiat ecranul si am oprit afisarea cursorului

    initscr();
    curs_set(0);

    // am creat cateva culori si cateva perechi de culori

    start_color();

    init_color(COLOR_LIGHT_PINK, 1000, 700, 1000);
    init_color(COLOR_MEDIUM_PINK, 900, 500, 900);
    init_color(COLOR_DARK_PINK, 600, 300, 600);
    init_color(COLOR_VERY_DARK_PINK, 400, 100, 400);
    init_color(COLOR_CORAL, 1000, 700, 700);
    init_color(COLOR_LIGHT_PURPLE, 900, 700, 1000);
    init_color(COLOR_MEDIUM_LIGHT_PURPLE, 800, 600, 900);
    init_color(COLOR_MEDIUM_PURPLE, 700, 500, 800);
    init_color(COLOR_MEDIUM_DARK_PURPLE, 600, 400, 700);
    init_color(COLOR_DARK_PURPLE, 500, 300, 600);

    init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_LIGHT_PINK);
    init_pair(4, COLOR_BLACK, COLOR_MEDIUM_PINK);
    init_pair(8, COLOR_BLACK, COLOR_CORAL);
    init_pair(16, COLOR_BLACK, COLOR_LIGHT_PURPLE);
    init_pair(32, COLOR_BLACK, COLOR_MEDIUM_LIGHT_PURPLE);
    init_pair(64, COLOR_WHITE, COLOR_MEDIUM_PURPLE);
    init_pair(128, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(9, COLOR_WHITE, COLOR_MEDIUM_DARK_PURPLE);
    init_pair(10, COLOR_WHITE, COLOR_DARK_PINK);
    init_pair(11, COLOR_WHITE, COLOR_VERY_DARK_PINK);
    init_pair(12, COLOR_WHITE, COLOR_DARK_PURPLE);

    bkgd(COLOR_PAIR(1));
    refresh();

    char text[] = "2048 GAME";
    int len = strlen(text);

    int xMax, i, j;
    xMax = getmaxx(stdscr);
    int cursor = (xMax / 2) - (len / 2);
    move(0, cursor);
    printw("%s", text);
    refresh();

    // am alocat dinamic un tablou tridimensional in care retin matricea de la
    // fiecare mutare in parte pentru a putea reveni la mutarile precedente, dar
    // si pentru a avea acces la joc in cazul optiunii RESUME

    int ***prev;

    prev = malloc((MAX_STEPS_BACK + 1) * sizeof(int **));

    for (i = 0; i < MAX_STEPS_BACK + 1; i++)
        prev[i] = malloc(4 * sizeof(int *));

    for (i = 0; i < MAX_STEPS_BACK + 1; i++)
    {
        for (j = 0; j < 4; j++)
            prev[i][j] = malloc(4 * sizeof(int));
    }

    int *s;

    s = malloc((MAX_STEPS_BACK + 1) * sizeof(int));

    int final_choice;
    char output;

    // in functie de optiunea aleasa, am initilizat un joc
    // daca incep un joc nou, am initializat cu 0 scorul si matricea jocului
    // daca am continuat un joc, am folosit ultima miscare si ultimul scor
    // memorat

    do
    {
        Menu(&final_choice);

        if (final_choice == 0)
        {
            for (i = 0; i < 4; i++)
                for (j = 0; j < 4; j++)
                    mat[i][j] = 0;

            score = 0;
            steps = 0;
            previous_steps(prev, s);

            Game(&output, mat, prev, s);

            if (output == 'q' || output == 'w' || output == 'l')
                refresh();
        }

        else if (final_choice == 1)
        {
            if (output == 'l')
            {
                take_step_back(prev, s);

                Game(&output, mat, prev, s);

                if (output == 'q' || output == 'w' || output == 'l')
                    refresh();
            }

            else if (output == 'q')
            {
                Game(&output, mat, prev, s);

                if (output == 'q' || output == 'w' || output == 'l')
                    refresh();
            }
        }

    } while (final_choice != 2);

    // am inchis ecranul/fereastra la alegerea optiunii QUIT

    if (final_choice == 2)
    {
        endwin();
        return 0;
    }

    getch();
    endwin();

    // am eliberat memoria pentru tabloul tridimensional si vectorul de scoruri

    for (i = 0; i < MAX_STEPS_BACK + 1; i++)
        for (j = 0; j < 4; j++)
            free(prev[i][j]);

    for (i = 0; i < MAX_STEPS_BACK + 1; i++)
        free(prev[i]);

    free(prev);
    free(s);

    return 0;
}