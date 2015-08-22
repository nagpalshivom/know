#include <ncurses.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct DynamicArray {
    char * container;
    int size, pos;
    struct DynamicArray * next, * prev;
    int (* insert)(char, struct DynamicArray *, int);
    int (* insertX)(char, int, struct DynamicArray *, int);
    char (* removeX)(int, struct DynamicArray *, int);
    char (* getX)(int, struct DynamicArray *, int);
}vector;

typedef struct TextEditor {
    vector * head, * tail;
    int isOpen;
}TEdit;

TEdit files[50];
int MAX_COLS, MAX_LINES, EDITOR_WIDTH, EDITOR_HEIGHT, START_CHAR = 0, CURRENT_CHAR = 0, SCREEN_X = 0, SCREEN_Y = 0, noOfChoices;
vector * START_LINE, * CURRENT_LINE, * LAST_LINE;
char * menuOptions[] = {
    "NEW",
    "OPEN",
    "SAVE",
    "SAVE AS",
    "SEARCH",
    "CLOSE", 
    "EXIT"
};

void set_curses();
void unset_curses();
void handle_window_resizing();
void openLogFile(FILE ** logFilePointer);
void setLineValues(int fileNo);

vector * newVector(int size);
int insertChar(char ch, vector * tempLine, int fileNo);
int insertCharX(char ch, int pos, vector * tempLine, int fileNo);
char removeCharX(int pos, vector * tempLine, int fileNo);
char getCharX(int pos, vector * tempLine, int fileNo);
void addFirstLine(int fileNo);
void addLineInBetween(vector * data, int fileNo);
void insertFileInDS(char * fileName, int fileNo, FILE * lfp);
void writeDSToFile(int fileNo, char * fileName);
void purgeDS(int fileNo);

WINDOW * createMainWindow();
WINDOW * createOptionsWindow();
void refreshMainWindow(WINDOW * mainWindow, int fileNo, FILE * lfp);
void fillOptionsMenu(int fileNo);
void reportChoice(int x, int y, int * mouseChoice, FILE * lfp);

int main() {
    WINDOW * mainWindow;
    MEVENT event;
    int mainWindow_x, mainWindow_y, mainWindow_width, mainWindow_height, mouseChoice, count = 0, pressed_flag = 0;
    char inputCharacter, * copyrightString = "copyright @nagpalshivom - asoc internationals pvt. lmt.";
    FILE * logFilePointer = NULL;
    set_curses();
    memset(files, 0, sizeof(files));
    openLogFile(&logFilePointer);
    signal(SIGWINCH, handle_window_resizing);
    mvaddstr(LINES - 1, COLS / 2 - strlen(copyrightString) / 2, copyrightString);
    fillOptionsMenu(0);
    refresh();
    mainWindow = createMainWindow();
    refresh();
    addFirstLine(0);
    setLineValues(0);
    insertFileInDS("test", 0, logFilePointer);
    refreshMainWindow(mainWindow, 0, logFilePointer);
    refresh();
    while(1) {
        count++;
        inputCharacter = getch();
        fprintf(logFilePointer, "%d : %c\n", count, inputCharacter);
        if(inputCharacter == 27) {
            break;
        }
        else if(inputCharacter == (char)KEY_UP) {
            if(CURRENT_LINE->prev != NULL) {
                if(CURRENT_LINE == START_LINE) {
                    START_LINE = START_LINE->prev;
                    START_CHAR = 0;
                }
                CURRENT_LINE = CURRENT_LINE->prev;
                CURRENT_CHAR = 0;
            }
        }
        else if(inputCharacter == (char)KEY_DOWN) {
            if(CURRENT_LINE->next != NULL) {
                if(CURRENT_LINE == LAST_LINE) {
                    LAST_LINE = CURRENT_LINE = LAST_LINE->next;
                    START_LINE = START_LINE->next;
                    START_CHAR = 0;
                }
                CURRENT_CHAR = 0;
            }
        }
        else if(inputCharacter == (char)KEY_RIGHT) {
        }
        else if(inputCharacter == (char)KEY_LEFT) {
        }
        else if(inputCharacter == (char)KEY_MOUSE) {
            if(getmouse(&event) == OK) {
                if(event.bstate & BUTTON1_PRESSED) {
                    reportChoice(event.x, event.y, &mouseChoice, logFilePointer);
                    if(mouseChoice != -1) {
                        mvwprintw(mainWindow, 30, 50, "choice %s", menuOptions[mouseChoice]);
                        wrefresh(mainWindow);
                        fprintf(logFilePointer, "%s", menuOptions[mouseChoice]);
                    }
                }
            }
        }
        else if(inputCharacter == 8){
            //removeCharX();
            //refreshMainWindow();
        }
        else if(inputCharacter >= 32 && inputCharacter <= 126) {
            //insertChar();
            //refreshMainWindow();
        }
        else
            continue;
    }
    delwin(mainWindow);
    printf("reached\n");
    fclose(logFilePointer);
    unset_curses();
    return 0;
}

void unset_curses() {
    keypad(stdscr, FALSE);
    nocbreak();
    echo();
    endwin();
}

void set_curses() {
    initscr();
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_PRESSED, NULL);
    start_color();
    cbreak();
    noecho();
    refresh();
    getmaxyx(stdscr, MAX_LINES, MAX_COLS);
}

void handle_window_resizing(int sig) {
    char resizeCommand[25];
    if(COLS == MAX_COLS && LINES == MAX_LINES)
        return;
    sprintf(resizeCommand, "resize -s %d %d", MAX_LINES, MAX_COLS);
    system(resizeCommand);
    refresh();
}

void openLogFile(FILE ** logFilePointer) {
    int fileNameSize = 200;
    time_t rawtime;
    struct tm * info;
    time(&rawtime);
    info = localtime(&rawtime);
    *logFilePointer = fopen("editor.logs", "w");
    fprintf(*logFilePointer, "%dHours %dMinutes %dSeconds - DATE : %d:%d:%d\n", info->tm_hour, info->tm_min, info->tm_sec, info->tm_mday, info->tm_mon, info->tm_year);
}

vector * newVector(int size) {
    vector * tempLine = (vector *)malloc(sizeof(vector));
    tempLine->size = size;
    tempLine->pos = 0;
    tempLine->container = (char *)malloc(sizeof(char) * size);
    tempLine->prev = tempLine->next = NULL;
    tempLine->insert = insertChar;
    tempLine->insertX = insertCharX;
    tempLine->removeX = removeCharX;
    tempLine->getX = getCharX;
}
void addFirstLine(int fileNo) {
    vector * tempLine = newVector(50);
    files[fileNo].head = files[fileNo].tail = tempLine;
}
void addLineInBetween(vector * data, int fileNo) {
    vector * temp = newVector(50);
    if(data->next != NULL) {
        temp->next = data->next;
        data->next->prev = temp;
    }
    data->next = temp;
    temp->prev = data;
    if(temp->next == NULL)
        files[fileNo].tail = temp;
}

int insertChar(char ch, vector * tempLine, int fileNo) {
    int size = tempLine->size;
    if(ch == '\n') {
        addLineInBetween(tempLine, fileNo);
        tempLine->next->container[0] = '\n';
        tempLine->next->pos = 1;
        //returning two specifies addition of a new line
        return 2; 
    }
    if(tempLine->pos == size) {
        tempLine->container = (char *)realloc(tempLine->container, sizeof(char) * size * 2);
        tempLine->size = size * 2;
    }
    else if(tempLine->pos < (size / 4)) {
        tempLine->container = (char *)realloc(tempLine->container, sizeof(char) * (size / 2));
        tempLine->size = size / 2;
    }
    tempLine->container[tempLine->pos++] = ch;
    //element successfully added to the current line is specified by returning 1
    return 1;
}

int insertCharX(char ch, int pos, vector * tempLine, int fileNo) {
    int size = tempLine->size, tempPos = tempLine->pos;
    if(pos < 0 || pos >= 32767)
        return 0;
    else if(ch != '\n') {
        if(tempLine->pos == size) {
            tempLine->container = (char *)realloc(tempLine->container, sizeof(char) * size * 2);
            tempLine->size = size * 2;
        }
        else if(tempLine->pos < (size / 4)) {
            tempLine->container = (char *)realloc(tempLine->container, sizeof(char) * (size / 2));
            tempLine->size = size / 2;
        }
        tempLine->pos++;
        while(tempPos != pos) {
            tempLine->container[tempPos] = tempLine->container[tempPos - 1];
            tempPos--;
        }
        tempLine->container[tempPos] = ch;
    }
    else {
        addLineInBetween(tempLine, fileNo);
        insertChar(ch, tempLine->next, fileNo);
        tempLine->pos = pos;
        while(pos != tempPos) {
            insertChar(tempLine->container[pos], tempLine->next, fileNo);
            pos++;
        }
    }
    return 1;
}

char removeCharX(int pos, vector * tempLine, int fileNo) {
    int tempPos = tempLine->pos;
    vector * toFree;
    char ch = '\0';
    if(pos < 0 || pos >= tempPos)
        return ch;
    ch = tempLine->container[pos];
    if(ch == '\n') {
        pos++;
        while(pos < tempPos) {
            insertChar(tempLine->container[pos], tempLine->prev, fileNo);
            pos++;
        }
        tempLine->pos = 0;
        toFree = tempLine;
        if(!(tempLine->prev == NULL && tempLine->next == NULL)) {
            if(tempLine->prev != NULL)
                tempLine->prev->next = tempLine->next;
            if(tempLine->next != NULL)
                tempLine->next->prev = tempLine->prev;
            if(tempLine->next == NULL)
                files[fileNo].tail = tempLine->prev;
            if(tempLine->prev == NULL)
                files[fileNo].head = tempLine->next;
        }
        if(CURRENT_LINE == toFree) {
            CURRENT_LINE = CURRENT_LINE->prev;
            CURRENT_CHAR = 0;
        }
        if(START_LINE == toFree) {
            START_LINE = START_LINE->prev;
            START_CHAR = 0;
        }
        free(toFree->container);
        free(toFree);
    }
    else {
        tempPos -= 1;
        while(tempPos != pos) {
            tempLine->container[pos] = tempLine->container[pos + 1];
            pos++;
        }
        tempLine->pos -= 1;
    }
    return ch;
}

char getCharX(int pos, vector * tempLine, int fileNo) {
    if(pos < 0 || pos >= tempLine->pos)
        return '\0';
    return tempLine->container[pos];
}

WINDOW * createMainWindow() {
    WINDOW * tempWindow = newwin(LINES - 2, COLS, 1, 0);
    EDITOR_WIDTH = COLS - 2;
    EDITOR_HEIGHT = LINES - 4;
    box(tempWindow, 0, 0);
    wrefresh(tempWindow);
    return tempWindow;
}

void refreshMainWindow(WINDOW * mainWindow, int fileNo, FILE * lfp) {
    int current_row = 1, current_col, tempPos, end, cursor_x, cursor_y;
    char ch;
    vector * tempLine;
    wclear(mainWindow);
    box(mainWindow, 0, 0);
    wrefresh(mainWindow);
    tempLine = START_LINE;
    tempPos = START_CHAR;
    end = tempLine->pos;
    if(tempLine != NULL) {
        while(current_row <= EDITOR_HEIGHT) {
            current_col = 1;
            while(current_col <= EDITOR_WIDTH) {
                if(tempPos == end) {
                    tempLine = tempLine->next;
                    if(tempLine == NULL)
                        break;
                    else {
                        tempPos = 0;
                        end = tempLine->pos;
                    }
                }
                if(tempLine == CURRENT_LINE && tempPos == CURRENT_CHAR) {
                    cursor_x = current_col;
                    cursor_y = current_row;
                }
                ch = tempLine->container[tempPos++];
                if(ch == '\n')
                    break;
                mvwaddch(mainWindow, current_row, current_col, ch);
                current_col++;
            }
            if(tempLine == NULL)
                break;
            current_row++;
        }
    }
    LAST_LINE = tempLine;
    wmove(mainWindow, cursor_y, cursor_x);
    wrefresh(mainWindow);
}
void setLineValues(int fileNo) {
    START_LINE = files[fileNo].head;
    CURRENT_LINE = files[fileNo].head;
    START_CHAR = CURRENT_CHAR = 0;
}

void insertFileInDS(char * fileName, int fileNo, FILE * lfp) {
    char ch;
    FILE * tempFilePointer = fopen(fileName, "r");
    vector * tempLine = files[fileNo].head;
    while(!feof(tempFilePointer)) {
        ch = fgetc(tempFilePointer);
        insertChar(ch, tempLine, fileNo);
        if(ch == '\n')
            tempLine = tempLine->next;
    }
    fclose(tempFilePointer);
}

void fillOptionsMenu(int fileNo) {
    int i, tempW = 0;
    noOfChoices = sizeof(menuOptions) / sizeof(char *);
    if(has_colors() == TRUE) {
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        attron(COLOR_PAIR(2));
        refresh();
    }
    attron(A_BOLD | A_STANDOUT);
    for(i = 0;i < noOfChoices;i++) {
        mvprintw(0, tempW, "%s", menuOptions[i]);
        tempW += strlen(menuOptions[i]) + 1;
    }
    attroff(A_BOLD | A_STANDOUT);
    if(has_colors() == TRUE)
        attroff(COLOR_PAIR(2));
    refresh();
}

void reportChoice(int x, int y, int * mouseChoice, FILE * lfp) {
    int i, tempX = 0;
    *mouseChoice = -1;
    if(y == 0) {
        for(i = 0;i < noOfChoices;i++) {
            if(x >= tempX && x < (tempX + strlen(menuOptions[i]))) {
                *mouseChoice = i;
                break;
            }
            tempX += strlen(menuOptions[i]) + 1;
        }
    }
}

void writeDSToFile(int fileNo, char * fileName) {
    vector * tempLine = files[fileNo].head;
    int i, tempPos;
    FILE * tempFilePtr = fopen(fileName, "w");
    while(tempLine != NULL) {
        tempPos = tempLine->pos;
        for(i = 0;i < tempPos;i++)
            fputc(tempLine->container[i],tempFilePtr);
        tempLine = tempLine->next;
    }
    fclose(tempFilePtr);
}

void purgeDS(int fileNo) {
    vector * line = files[fileNo].head, * tempLine;
    while(line != NULL) {
        tempLine = line;
        line = line->next;
        free(tempLine->container);
        free(tempLine);
    }
    files[fileNo].head = files[fileNo].tail = NULL;
}
