
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 100 // Maximum string length this program can handle

struct Games
{
    char title[MAX_BUFFER], platform[MAX_BUFFER];
    int release_year, score;
};

// The CSV parser
int next_field(FILE *f, char *buf, int max)
{
    int i = 0, end = 0, quoted = 0;

    for (;;)
    {
        // fetch the next character from file
        buf[i] = fgetc(f);
        // if we encounter quotes then flip our state and immediately fetch next char
        if (buf[i] == '"')
        {
            quoted = !quoted;
            buf[i] = fgetc(f);
        }
        // end of field on comma if we're not inside quotes
        if (buf[i] == ',' && !quoted)
        {
            break;
        }
        // end record on newline or end of file
        if (feof(f) || buf[i] == '\n')
        {
            end = 1;
            break;
        }
        // truncate fields that would overflow the buffer
        if (i < max - 1)
        {
            ++i;
        }
    }

    buf[i] = 0; // null terminate the string
    return end; // flag stating whether or not this is end of the line
}

// Stuff to make life a bit neater in main
void fetch_games(FILE *csv, struct Games *p)
{
    char buf[MAX_BUFFER];

    next_field(csv, p->title, MAX_BUFFER);    // name and type are just strings so read
    next_field(csv, p->platform, MAX_BUFFER); // those directly into the struct

    next_field(csv, buf, MAX_BUFFER); // load score into buffer as string
    p->score = atoi(buf);             // then parse to integer

    next_field(csv, buf, MAX_BUFFER); // load score into buffer as string
    p->release_year = atoi(buf);      // then parse to integer
}

void print_games(struct Games *p, int SIZE_games)
{
    for (int i = 0; i < SIZE_games; i++)
    {
        printf("| %s | %s | %d | %d |\n",
               p[i].title, p[i].platform, p[i].score, p[i].release_year);
        printf(" --------------------------------------------------------------\n");
    }
    printf("hi");
}
void printtest(struct Games *p)
{
      printf("| %s | %s | %d | %d |\n",
               p->title, p->platform, p->score, p->release_year);
        printf(" --------------------------------------------------------------\n");
    
}

void swap(struct Games *g1, struct Games *g2)
{
    struct Games temp;
    temp = *g1;
    *g1 = *g2;
    *g2 = temp;
}


void insertionsort(struct Games *g, int SIZE_games)
{
    for (int curr = 0; curr < SIZE_games - 1; curr++) // first to last element of array
    {
        for (int next = curr + 1; next > 0; next--) // element after current and current back until current = 0
        {
            if (g[next - 1].score > g[next].score) // if current(i.e. element after current - 1) greater than element after(i.e. current)
            {
                swap(&g[next - 1], &g[next]);

            }

            else
            {
                break; // leave loop
            }
        }
    }
}
void quicksort(struct Games *g, int start_index, int end_index)
{
    int key, i, j, pivot;
    if (start_index < end_index) // handle recursion
    {
        pivot = end_index;
        swap(&g[start_index], &g[pivot]);
        key = g[start_index].score;
        i = start_index + 1;
        j = end_index;
        while (i <= j)
        {
            while ((i <= end_index) && (g[i].score <= key))
                i++;
            while ((j >= start_index) && (g[j].score > key))
                j--;
            if (i < j)
                swap(&g[i], &g[j]);
        }
        swap(&g[start_index], &g[j]);

        // recursion
        quicksort(g, start_index, j - 1); // start to before pivot
        quicksort(g, j + 1, end_index);   // after pivot to end
    }
}

int main()
{
    int SIZE_games = 18000; // update to exct value in file
    FILE *f;
    struct Games p;
    struct Games g[SIZE_games];

    // Try to open the input file. If there is a problem, report failure and quit
    f = fopen("ign.csv", "r");
    if (!f)
    {
        printf("unable to open");
        return EXIT_FAILURE;
    }
    
    //fetch_games(f, &p); // discard the header data in the first line

    // Now read and print pokemon until the end of the file
    int i = 0;
    while (!feof(f) && i < SIZE_games)
    {
        
        fetch_games(f, &p);
        printtest(&p);

        g[i] = p;
       // printf("\n");

        i++;
    }
    printf("before");
   // quicksort(g, 0,SIZE_games -1 );
    insertionsort(g, SIZE_games);
    print_games(g, SIZE_games);
    printf("after");

    fclose(f); // Always remember to close the file
    return EXIT_SUCCESS;
}
