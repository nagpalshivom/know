#include <ncurses.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int MAX_COLS, MAX_LINES, FILE_NO = 0;

typedef struct DynamicArray {
    char * container;
    int size, pos;
    struct DynamicArray * next, * prev;
    int (* insert)(char, struct DynamicArray *);
    int (* insertX)(char, int, struct DynamicArray *);
    char (* removeX)(int, struct DynamicArray *);
    char (* getX)(int, struct DynamicArray *);
}vector;

typedef struct TextEditor {
    vector * data, * head, * tail;
    int currentLine, noOfLines, isOpen;
}TEdit;

void set_curses();
void unset_curses();
void handle_window_resizing();
void openLogFile(FILE ** logFilePointer);

vector * newVector(int size);
int insertChar(char ch, vector * tempLine);
int insertCharX(char ch, int pos, vector * tempLine);
char removeCharX(int pos, vector * tempLine);
char getCharX(int pos, vector * tempLine);
void addNewLine(TEdit * tempFile);
void addLineInBetween(vector * data);

WINDOW * createMainWindow();
WINDOW * createOptionsWindow();

int main() {
    WINDOW * mainWindow, * optionsWindow;
    TEdit files[50];
    int mainWindow_x, mainWindow_y, mainWindow_width, mainWindow_height;
    char inputCharacter;
    FILE * logFilePointer = NULL;
    set_curses();
    memset(files, 0, sizeof(files));
    openLogFile(&logFilePointer);
    signal(SIGWINCH, handle_window_resizing);
    mainWindow = createMainWindow();
    optionsWindow = createOptionsWindow();
    refresh();
    while(1) {
        inputCharacter = getch();
        if(inputCharacter == 27)
            break;
        else {

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
    char * logFileCaption;
    int fileNameSize = 200;
    time_t rawtime;
    struct tm * info;
    logFileCaption = (char *)malloc(sizeof(char) * fileNameSize);
    time(&rawtime);
    info = localtime(&rawtime);
    snprintf(logFileCaption, fileNameSize - 5, "%dhours %dminutes %dseconds - DATE : %d:%d:%d", info->tm_hour, info->tm_min, info->tm_sec, info->tm_mday, info->tm_mon, info->tm_year);
    (*logFilePointer) = fopen("editor.logs", "a");;
    fprintf(*logFilePointer, logFileCaption);
    free(logFileCaption);
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

void addNewLine(TEdit * tempFile) {
    vector * temp;
    if(tempFile->noOfLines) {
        tempFile->currentLine = 1;
        tempFile->data = tempFile->head = tempFile->tail = newVector(50);
    }
    else {
        temp = tempFile->data;
        while(temp->next != NULL) {
            temp = temp->next;
            tempFile->currentLine++;
        }
        temp->next = newVector(50);
        temp->next->prev = temp;
        tempFile->data = temp->next;
        tempFile->currentLine++;
    }
    tempFile->noOfLines++;
}

void addLineInBetween(vector * data) {
    vector * temp = newVector(50);
    if(data->next != NULL) {
        temp->next = data->next;
        data->next->prev = temp;
    }
    data->next = temp;
    temp->prev = data;
}

int insertChar(char ch, vector * tempLine) {
    int size = tempLine->size;
    if(ch == '\n') {
        addLineInBetween(tempLine);
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

int insertCharX(char ch, int pos, vector * tempLine) {
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
        addLineInBetween(tempLine);
        tempLine->pos = pos;
        while(pos != tempPos) {
            insertChar(tempLine->container[pos], tempLine->next);
            pos++;
        }
    }
    return 1;
}

char removeCharX(int pos, vector * tempLine) {
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

char getCharX(int pos, vector * tempLine) {
    if(pos < 0 || pos >= tempLine->pos)
        return '\0';
    return tempLine->container[pos];
}

WINDOW * createMainWindow() {
    WINDOW * tempWindow = newwin(LINES, COLS - 25, 0, 0);
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
