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
    int (* insertX)(char, int);
    char (* removeX)(int);
    char (* getX)(int, struct DynamicArray *, int);
}vector;

typedef struct TextEditor {
    vector * head, * tail;
    int isOpen;
}TEdit;

TEdit files[50];
int MAX_COLS, MAX_LINES, EDITOR_WIDTH, EDITOR_HEIGHT, START_CHAR, CURRENT_CHAR, SCREEN_LINE, noOfChoices;
vector * START_LINE, * CURRENT_LINE;
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
int insertCharX(char ch, int fileNo);
char removeCharX(int fileNo);
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
    mainWindow = createMainWindow();
    addFirstLine(0);
    setLineValues(0);
    fprintf(logFilePointer, "reached");
    insertFileInDS("test", 0, logFilePointer);
    refreshMainWindow(mainWindow, 0, logFilePointer);
    refresh();
    while(1) {
        count++;
        inputCharacter = getch();
        if(inputCharacter == 27) {
            break;
        }
        else if(inputCharacter == (char)KEY_UP) {
            if(CURRENT_CHAR >= (COLS - 2)) {
                CURRENT_CHAR -= (COLS - 2);
                SCREEN_LINE--;
            }
            else {
                if(CURRENT_LINE->prev != NULL) {
                    CURRENT_LINE = CURRENT_LINE->prev;
                    CURRENT_CHAR = (CURRENT_CHAR / (COLS - 2)) * (COLS - 2);
                    SCREEN_LINE--;
                }
            }
            if(SCREEN_LINE < 1) {
                if(START_CHAR >= (COLS - 2)) {
                    START_CHAR -= (COLS - 2);
                    SCREEN_LINE++;
                }
                else {
                    if(START_LINE->prev != NULL) {
                        START_LINE = START_LINE->prev;
                        START_CHAR = (START_CHAR / (COLS - 2)) * (COLS - 2);
                        SCREEN_LINE++;
                    }
                }
            }
            refreshMainWindow(mainWindow, 0, logFilePointer);
        }
        else if(inputCharacter == (char)KEY_DOWN) {
            if(CURRENT_CHAR + (COLS - 2) <= CURRENT_LINE->pos) {
                CURRENT_CHAR += (COLS - 2);
                SCREEN_LINE++;
            }
            else {
                if(CURRENT_LINE->next != NULL) {
                    CURRENT_LINE = CURRENT_LINE->next;
                    CURRENT_CHAR = (CURRENT_CHAR % (COLS - 2) > CURRENT_LINE->pos) ? CURRENT_LINE->pos : (CURRENT_CHAR % (COLS - 2));
                    SCREEN_LINE++;
                }
            }
            if(SCREEN_LINE > EDITOR_HEIGHT) {
                if(START_CHAR + (COLS - 2) <= START_LINE->pos) {
                    START_CHAR += (COLS - 2);
                    SCREEN_LINE--;
                }
                else {
                    if(START_LINE->next != NULL) {
                        START_LINE = START_LINE->next;
                        START_CHAR = 0;
                        SCREEN_LINE--;
                    }
                }
            }
            refreshMainWindow(mainWindow, 0, logFilePointer);
        }
        else if(inputCharacter == (char)KEY_RIGHT) {
            if(CURRENT_CHAR + 1 > CURRENT_LINE->pos) {
                if(CURRENT_LINE->next != NULL) {
                    CURRENT_LINE = CURRENT_LINE->next;
                    CURRENT_CHAR = 0;
                    SCREEN_LINE++;
                }
            }
            else {
                if(CURRENT_CHAR % (COLS - 2) == (COLS - 3)) {
                    SCREEN_LINE++;
                }
                CURRENT_CHAR++;
            }
            if(SCREEN_LINE > (COLS - 2)) {
                if(START_CHAR + (COLS - 2) <= START_LINE->pos)
                    START_CHAR += (COLS - 2);
                else {
                    START_LINE = START_LINE->next;
                    START_CHAR = 0;
                }
                SCREEN_LINE--;

            }
            refreshMainWindow(mainWindow, 0, logFilePointer);
        }
        else if(inputCharacter == (char)KEY_LEFT) {
            if(CURRENT_CHAR == 0) {
                if(CURRENT_LINE->prev != NULL) {
                    CURRENT_LINE = CURRENT_LINE->prev;
                    CURRENT_CHAR = CURRENT_LINE->pos;
                    SCREEN_LINE--;
                }
            }
            else if(CURRENT_CHAR % (COLS - 2) == 0) {
                CURRENT_CHAR--;
                CURRENT_CHAR = (CURRENT_CHAR / (COLS - 2)) * (COLS - 2);
                SCREEN_LINE--;
            }
            else {
                CURRENT_CHAR--;
            }
            if(SCREEN_LINE == 0) {
                SCREEN_LINE = 1;
                if(START_CHAR - (COLS - 2) >= 0)
                    START_CHAR -= (COLS - 2);
                else {
                    if(START_LINE->prev != NULL) {
                        START_LINE = START_LINE->prev;
                        START_CHAR = ((START_LINE->pos) / (COLS - 2)) * (COLS - 2);
                    }
                }
            }
            refreshMainWindow(mainWindow, 0, logFilePointer);
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
            refreshMainWindow(mainWindow, 0, logFilePointer);
        }
        else if(inputCharacter == (char)KEY_BACKSPACE) {
            removeCharX(0);
            refreshMainWindow(mainWindow, 0, logFilePointer);
        }
        else if(inputCharacter == (char)KEY_ENTER) {
            insertCharX('\n', 0);
            refreshMainWindow(mainWindow, 0, logFilePointer);
        }
        else if(inputCharacter >= 32 && inputCharacter <= 126) {
            if(inputCharacter == '\n') {
                fprintf(logFilePointer, "enter pressed : %d\n", (int)inputCharacter);
            }
            insertCharX(inputCharacter, 0);
            refreshMainWindow(mainWindow, 0, logFilePointer);
        }
        else
            continue;
    }
    purgeDS(0);
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

int insertCharX(char ch, int fileNo) {
    int size = CURRENT_LINE->size, tempPos = CURRENT_LINE->pos, pos = CURRENT_CHAR, initLine = SCREEN_LINE;
    if(ch != '\n') {
        if(CURRENT_CHAR % (COLS - 2) == (COLS - 3))
            SCREEN_LINE++;
        if(CURRENT_LINE->pos == size) {
            CURRENT_LINE->container = (char *)realloc(CURRENT_LINE->container, sizeof(char) * size * 2);
            CURRENT_LINE->size = size * 2;
        }
        else if(CURRENT_LINE->pos < (size / 4)) {
            CURRENT_LINE->container = (char *)realloc(CURRENT_LINE->container, sizeof(char) * (size / 2));
            CURRENT_LINE->size = size / 2;
        }
        CURRENT_LINE->pos++;
        while(tempPos != pos) {
            CURRENT_LINE->container[tempPos] = CURRENT_LINE->container[tempPos - 1];
            tempPos--;
        }
        CURRENT_LINE->container[tempPos] = ch;
        CURRENT_CHAR++;
    }
    else {
        addLineInBetween(CURRENT_LINE, fileNo);        
        CURRENT_LINE->pos = pos;
        while(pos != tempPos) {
            insertChar(CURRENT_LINE->container[pos], CURRENT_LINE->next, fileNo);
            pos++;
        }
        CURRENT_LINE = CURRENT_LINE->next;
        CURRENT_CHAR = 0;
        SCREEN_LINE++;
    }
    if(initLine != SCREEN_LINE && SCREEN_LINE > (COLS - 2)) {
        if(START_CHAR + (COLS - 2) <= START_LINE->pos)
            START_CHAR += (COLS - 2);
        else {
            START_LINE = START_LINE->next;
            START_CHAR = 0;
        }
        SCREEN_LINE--;
    }
    return 1;
}

char removeCharX(int fileNo) {
    int tempPos = CURRENT_LINE->pos, tPos;
    vector * toFree = CURRENT_LINE;
    char ch = '\0';
    if(CURRENT_CHAR == 0) {
        if(CURRENT_LINE->prev == NULL) {
            return ch;
        }
        else {
            ch = '\n';
            if(SCREEN_LINE == 1) {
                START_LINE = START_LINE->prev;
                START_CHAR = (START_LINE->pos / (COLS - 2) ) * (COLS - 2);
            }
            else
                SCREEN_LINE--;
            CURRENT_LINE = CURRENT_LINE->prev;
            CURRENT_CHAR = CURRENT_LINE->pos;
            for(tPos = 0;tPos < tempPos;tPos++)
                insertChar(toFree->container[tPos], CURRENT_LINE, fileNo);
            if(toFree->next == NULL)
                files[fileNo].tail = toFree->prev;
            free(toFree->container);
            free(toFree);
        }
    }
    else if(CURRENT_CHAR % (COLS - 2) == 0) {
        ch = CURRENT_LINE->container[CURRENT_CHAR - 1];
        if(SCREEN_LINE == 1) {
            START_CHAR--;
            START_CHAR = (START_LINE->pos / (COLS - 2) ) * (COLS - 2);
        }
        else
            SCREEN_LINE--;
        CURRENT_CHAR--;
        tempPos--;
        for(tPos = CURRENT_CHAR;tPos < tempPos;tPos++)
            CURRENT_LINE->container[tPos] = CURRENT_LINE->container[tPos + 1];
        CURRENT_LINE->pos = tPos;
    }
    else {
        ch = CURRENT_LINE->container[CURRENT_CHAR - 1];
        CURRENT_CHAR--;
        tempPos--;
        for(tPos = CURRENT_CHAR;tPos < tempPos;tPos++)
            CURRENT_LINE->container[tPos] = CURRENT_LINE->container[tPos + 1];
        CURRENT_LINE->pos = tPos;
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
    int current_row = 1, current_col, tempPos, end;
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
                        break;
                    }
                }
                ch = tempLine->container[tempPos++];
                mvwaddch(mainWindow, current_row, current_col, ch);
                current_col++;
            }
            if(tempLine == NULL)
                break;
            current_row++;
        }
    }
    wrefresh(mainWindow);
    move(SCREEN_LINE + 1, CURRENT_CHAR % (COLS - 2) + 1);
    fprintf(lfp, "cursor at %d : %d\n", SCREEN_LINE + 1, CURRENT_CHAR % (COLS - 2) + 1); 
    refresh();
}
void setLineValues(int fileNo) {
    START_LINE = files[fileNo].head;
    CURRENT_LINE = files[fileNo].head;
    START_CHAR = CURRENT_CHAR = 0;
    SCREEN_LINE = 1;
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
    int i, tempW = 1;
    noOfChoices = sizeof(menuOptions) / sizeof(char *);
    if(has_colors() == TRUE) {
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        attron(COLOR_PAIR(2));
        refresh();
    }
    attron(A_BOLD | A_STANDOUT);
    refresh();
    for(i = 0;i < noOfChoices;i++) {
        mvprintw(0, tempW, "%s", menuOptions[i]);
        tempW += strlen(menuOptions[i]) + 1;
    }
    refresh();
    attroff(A_BOLD | A_STANDOUT);
    refresh();
    if(has_colors() == TRUE) {
        attroff(COLOR_PAIR(2));
        refresh();
    }
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
