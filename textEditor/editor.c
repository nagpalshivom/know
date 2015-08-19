#include <ncurses.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int MAX_COLS, MAX_LINES, FILE_NO = 0, EDITOR_LEFT, EDITOR_RIGHT, EDITOR_TOP, EDITOR_BOTTOM, start_pos_line = 0, start_pos_char = 0, current_pos_line = 0, current_pos_char = 0;

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
    vector * data, * head, * tail;
    int currentLine, noOfLines, isOpen;
    FILE * fileInStorage;
}TEdit;

TEdit files[50];

void set_curses();
void unset_curses();
void handle_window_resizing();
void openLogFile(FILE ** logFilePointer);

vector * newVector(int size);
int insertChar(char ch, vector * tempLine, int fileNo);
int insertCharX(char ch, int pos, vector * tempLine, int fileNo);
char removeCharX(int pos, vector * tempLine, int fileNo);
char getCharX(int pos, vector * tempLine, int fileNo);
void addNewLine(int fileNo);
void addLineInBetween(vector * data, int fileNo);

WINDOW * createMainWindow();
WINDOW * createOptionsWindow();

void refreshMainWindow(WINDOW * mainWindow, int fileNo);

int main() {
    WINDOW * mainWindow, * optionsWindow;
    int mainWindow_x, mainWindow_y, mainWindow_width, mainWindow_height, start_pos_line = 0, start_pos_char = 0, current_pos_line = 0, current_pos_char = 0;
    char inputCharacter, * copyrightString = "copyright @nagpalshivom - asoc internationals pvt. lmt.";
    FILE * logFilePointer = NULL;
    set_curses();
    memset(files, 0, sizeof(files));
    openLogFile(&logFilePointer);
    signal(SIGWINCH, handle_window_resizing);
    mainWindow = createMainWindow();
    optionsWindow = createOptionsWindow();
    mvaddstr(LINES - 1, COLS / 2 - strlen(copyrightString) / 2, copyrightString);
    refresh();
    refreshMainWindow(mainWindow, 1);
    while(1) {
        inputCharacter = getch();
        if(inputCharacter == 27)
            break;
        else if(inputCharacter == 127)
            continue;
        else if(inputCharacter == 8){
        }
    }
    delwin(mainWindow);
    delwin(optionsWindow);
    printf("reached\n");
    fclose(logFilePointer);
    unset_curses();
    return 0;
}

void unset_curses() {
    keypad(stdscr, false);
    nocbreak();
    endwin();
}

void set_curses() {
    initscr();
    cbreak();
    keypad(stdscr, true);
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
    *logFilePointer = fopen("editor.logs", "a");
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

void addNewLine(int fileNo) {
    vector * temp;
    if(files[fileNo].noOfLines) {
        files[fileNo].currentLine = 1;
        files[fileNo].data = files[fileNo].head = files[fileNo].tail = newVector(50);
    }
    else {
        temp = files[fileNo].data;
        while(temp->next != NULL) {
            temp = temp->next;
            files[fileNo].currentLine++;
        }
        temp->next = newVector(50);
        temp->next->prev = temp;
        files[fileNo].data = temp->next;
        files[fileNo].currentLine++;
    }
    files[fileNo].noOfLines++;
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
    char ch = '\0';
    if(pos < 0 || pos >= tempPos)
        return ch;
    ch = tempLine->container[pos];
    tempPos -= 1;
    while(tempPos != pos) {
        tempLine->container[pos] = tempLine->container[pos + 1];
        pos++;
    }
    tempLine->pos -= 1;
    return ch;
}

char getCharX(int pos, vector * tempLine, int fileNo) {
    if(pos < 0 || pos >= tempLine->pos)
        return '\0';
    return tempLine->container[pos];
}

WINDOW * createMainWindow() {
    WINDOW * tempWindow = newwin(LINES - 1, COLS - 25, 0, 0);
    EDITOR_LEFT = EDITOR_TOP = 1;
    EDITOR_BOTTOM = LINES - 2;
    EDITOR_RIGHT = COLS - 26;
    box(tempWindow, 0, 0);
    wrefresh(tempWindow);
    return tempWindow;
}

WINDOW * createOptionsWindow() {
    WINDOW * tempWindow = newwin(LINES / 2, 25, 0, COLS - 25);
    box(tempWindow, 0, 0);
    wrefresh(tempWindow);
    return tempWindow;
}

void refreshMainWindow(WINDOW * mainWindow, int fileNo) {
    int current_row, current_col, tempPos, end, currentLine = 0;
    char ch;
    vector * tempLine;
    tempLine = files[fileNo].data;
    while(tempLine->next != NULL && currentLine != start_pos_line) {
        currentLine++;
        tempLine = tempLine->next;
    }
    tempPos = start_pos_char;
    end = tempLine->size;
    if(tempLine != NULL) {
        while(current_row <= EDITOR_BOTTOM) {
            while(current_col <= EDITOR_RIGHT) {
                if(tempPos == end) {
                    tempLine = tempLine->next;
                    if(tempLine == NULL)
                        break;
                    else {
                        tempPos = 0;
                        end = tempLine->size;
                    }
                }
                ch = tempLine->container[tempPos++];
                mvaddch(current_row, current_col, ch);
                current_col++;
            }
            if(tempLine == NULL)
                break;
            current_row++;
        }
    }
    current_pos_line = currentLine;
    current_pos_char = tempPos;
    refresh();
}
