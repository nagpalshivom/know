#include <ncurses.h>
#include <malloc.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int MAX_COLS, MAX_LINES;

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
    vector * data;
    int lineNo;
}TEdit;

vector * newVector(int size) {
    vector * tempVector = (vector *)malloc(sizeof(vector));
    tempVector->size = size;
    tempVector->pos = 0;
    tempVector->container = (char *)malloc(sizeof(char) * size);
    tempVector->prev = tempVector->next = NULL;
}

void addNewLine(TEdit * tempFile) {
    vector * temp;
    if(tempFile->lineNo == -1) {
        tempFile->lineNo++;
        tempFile->data = newVector(50);
    }
    else {
        temp = tempFile->data;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newVector(50);
        temp->next->prev = temp;
    }
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

int insertChar(char ch, vector * tempVector) {
    int size = tempVector->size;
    if(ch == '\n') {
        addLineInBetween(tempVector);
        //returning two specifies addition of a new line
        return 2; 
    }
    if(tempVector->pos == size) {
        tempVector->container = (char *)realloc(tempVector->container, sizeof(char) * size * 2);
        tempVector->size = size * 2;
    }
    else if(tempVector->pos < (size / 4)) {
        tempVector->container = (char *)realloc(tempVector->container, sizeof(char) * (size / 2));
        tempVector->size = size / 2;
    }
    tempVector->container[tempVector->pos++] = ch;
    //element successfully added to the current line is specified by returning 1
    return 1;
}

int insertCharX(char ch, int pos, vector * tempVector) {
    int size = tempVector->size, tempPos = tempVector->pos;
    if(pos < 0 || pos >= 32767)
        return 0;
    else if(ch != '\n') {
        if(tempVector->pos == size) {
            tempVector->container = (char *)realloc(tempVector->container, sizeof(char) * size * 2);
            tempVector->size = size * 2;
        }
        else if(tempVector->pos < (size / 4)) {
            tempVector->container = (char *)realloc(tempVector->container, sizeof(char) * (size / 2));
            tempVector->size = size / 2;
        }
        tempVector->pos += 1;
        while(tempPos != pos) {
            tempVector->container[tempPos] = tempVector->container[tempPos - 1];
            tempPos--;
        }
        tempVector->container[tempPos] = ch;
    }
    else {
        addLineInBetween(tempVector);
        tempVector->pos = pos;
        while(pos != tempPos) {
            insertChar(tempVector->container[pos], tempVector);
            pos++;
        }
    }
    return 1;
}

char removeCharX(int pos, vector * tempVector) {
    int tempPos = tempVector->pos;
    char ch = '\0';
    if(pos < 0 || pos >= tempPos)
        return ch;
    ch = tempVector->container[pos];
    tempPos -= 1;
    while(tempPos != pos) {
        tempVector->container[pos] = tempVector->container[pos + 1];
        pos++;
    }
    tempVector->pos -= 1;
    return ch;
}

char getCharX(int pos, vector * tempVector) {
    if(pos < 0 || pos >= tempVector->pos)
        return '\0';
    return tempVector->container[pos];
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

void handle_window_resizing(int sig) {
    char resizeCommand[25];
    if(COLS == MAX_COLS && LINES == MAX_LINES)
        return;
    sprintf(resizeCommand, "resize -s %d %d", MAX_LINES, MAX_COLS);
    system(resizeCommand);
    refresh();
}

void openLogFile(FILE ** logFilePointer) {
    char logFileName[200];
    time_t rawtime;
    struct tm * info;
    time(&rawtime);
    info = localtime(&rawtime);
    sprintf(logFileName, "%d-%d-%d-logs.txt", info->tm_mday, info->tm_mon, info->tm_year);
    *logFilePointer = fopen("editor_logs.txt", "w");
    fprintf(*logFilePointer, "WELCOME-SLAVES");
}

int main() {
    WINDOW * mainWindow, * optionsWindow;
    int mainWindow_x, mainWindow_y, mainWindow_width, mainWindow_height;
    char inputCharacter;
    FILE * logFilePointer = NULL;
    initscr();
    openLogFile(&logFilePointer);
    getmaxyx(stdscr, MAX_LINES, MAX_COLS);
    refresh();
    signal(SIGWINCH, handle_window_resizing);
    cbreak();
    keypad(stdscr, TRUE);
    refresh();
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
    endwin();
    return 0;
}
