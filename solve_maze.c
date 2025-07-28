#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define EMPTY ' '

struct cell {
    char ch;
    bool vis;
    bool path;
    bool start;
};
typedef  struct cell cell;

void fill_maze(FILE* fp, cell* maze, int area);
void print_maze(cell* maze, int width, int height);
void get_dimensions(FILE* fp, int* width, int* height);
void check_pointer(cell* pointer);
int find_start(cell* maze, int width, int height);
void init_maze(cell* maze, int area);
bool find_end(cell* maze, int i, int j, int width, int height);
void fill_path(cell* maze, int area);

int main(int argc, char* argv[]) {

    FILE* fp = fopen(argv[argc-1], "r");

    if (fp == NULL) {
        fprintf(stderr, "Unable to open file");
        exit(EXIT_FAILURE);
    }
    int width = 0, height = 0;
    get_dimensions(fp, &width, &height);

    int area = width * height;
    cell* maze = malloc((2*area) * sizeof(cell));
    fill_maze(fp, maze, area);
    fclose(fp);

    print_maze(maze, width, height);

    init_maze(maze, area);
    int start_row = find_start(maze, width, height);
    maze[start_row*width].start = true;
    //maze[start_row*width].ch = '.';
    find_end(maze, start_row, 0, width, height);
    fill_path(maze, area);

    print_maze(maze, width, height);



    free(maze);
    return 0;
}

int find_start(cell* maze, int width, int height) {

    check_pointer(maze);

    for (int i=0; i<height; i++) {

        if (maze[i*width].ch == EMPTY) {
            return i;
        }
    }
    return -1;
}

bool find_end(cell* maze, int i, int j, int width, int height) {

    if (j < 0 || j >= width || maze[i*width+j].ch == '#' || maze[i*width+j].vis == true) {
        return false;
    }

    maze[i*width+j].vis = true;
    //maze[i*width+j].ch = '.';

    if (!maze[i*width+j].start && (i==0 || i==height-1 || j==0 || j==width-1)) {
        maze[i*width+j].path = true;
        return true;
    }

    bool down = find_end(maze, i+1, j, width, height);
    bool up = find_end(maze, i-1, j, width, height);
    bool right = find_end(maze, i, j+1, width, height);
    bool left = find_end(maze, i, j-1, width, height);

    if (down || up || right || left) {
        maze[i*width+j].path = true;
        return true;
    }

    return false;
}

void fill_maze(FILE* fp, cell* maze, int area) {

    check_pointer(maze);

    char ch = 0;
    int i = 0;
    while ((ch = fgetc(fp)) != '#' && ch != EOF);
    maze[i++].ch = ch;

    while (i < area && (ch = fgetc(fp)) != EOF) {

        if (ch != '\n') {
            maze[i++].ch = ch;
        }
    }
}

void print_maze(cell* maze, int width, int height) {

    printf("\n");

    check_pointer(maze);

    for (int row=0; row<height; row++) {
        for (int col=0; col<width; col++) {
            // ReSharper disable once CppDFANullDereference
            if (maze[row*width + col].ch == '.') {
                printf("\033[101m  \033[0m");
            }
            else if (maze[row*width + col].ch == '#') {
                printf("\033[47m  \033[0m");
            }
            else {
                printf("  ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void get_dimensions(FILE* fp, int* width, int* height) {

    if (fscanf(fp, "%i,%i", width, height) != 2) {
        fprintf(stderr, "Error reading maze dimensions\n");
        exit(EXIT_FAILURE);
    }
    //printf("Width = %i, Height = %i\n", *width, *height);
}

void check_pointer(cell* pointer) {

    if (pointer == NULL) {
        fprintf(stderr, "ERROR\n");
        exit(EXIT_FAILURE);
    }
}

void init_maze(cell* maze, int area) {

    for (int i=0; i<area; i++) {
        maze[i].vis = false;
        maze[i].path = false;
        maze[i].start = false;
    }
}

void fill_path(cell* maze, int area) {

    for (int i=0; i<area; i++) {

        if (maze[i].path == true) {
            maze[i].ch = '.';
        }
    }
}
