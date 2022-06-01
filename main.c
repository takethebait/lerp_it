#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "main.h"

float get_num()
{
    return 2.4;
}

void print_log(char msg[])
{
    printf("[-] %s\n", msg);
}

int main()
{
    float x0, y0;
    float x_header[] = {1, 2, 4, 7, 9};
    float y_header[] = {10, 20, 40, 70, 90};
    int table[][5] = {
        {20, 30, 40, 50, 60},
        {21, 31, 41, 51, 61},
        {22, 32, 42, 52, 62},
        {23, 33, 43, 53, 63},
        {24, 34, 44, 54, 64}};

    long long counter;

    time_t target_time = time(NULL) + 1;

    for (;;)
    {
        if (time(NULL) > target_time)
        {
            printf("%lli lps\n", counter);
            counter = 0;
            target_time = time(NULL) + 1;
        }

        x0 = float_rand(1, 9);
        y0 = float_rand(10, 90);

        float final_interpolated = lerp_it(x0, y0, x_header, sizeof(x_header) / sizeof(x_header[0]), y_header, sizeof(y_header) / sizeof(y_header[0]), table);
        counter++;
    }

    return 0;
}

float float_rand(float min, float max)
{
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

size_t get_index(float arr[], size_t len, int value)
{
    for (size_t i = 0; i < len; i++)
    {
        if (arr[i] == value)
        {
            return i + 1;
        }
        else if (arr[i] > value)
        {
            return i;
        }
    }
    return len;
}

float lerp_it(float x0, float y0, float x_header[], size_t x_header_size, float y_header[], size_t y_header_size, int table[][5])
{

    //             x_index -1      x0y0    x_index
    //                 |             |        |
    //   y_index - 1 --|-------------*--------|----
    //                 |             |
    //                 |             |
    //                 |             |
    //                 |             ^
    //                 |             |
    //                 |             |
    //                 |             |
    //      y_index  --|-------------|------------
    //                 |             |

    int x_index = get_index(x_header, x_header_size, x0);
    int y_index = get_index(y_header, y_header_size, y0);

    float x_lower_value = x_header[x_index - 1];
    float x_upper_value = x_header[x_index];

    float y_lower_value = y_header[y_index - 1];
    float y_upper_value = y_header[y_index];

    float inter_x1_y1 = ((x_upper_value - x0) / (x_upper_value - x_lower_value)) * table[y_index - 1][x_index - 1];
    float inter_x2_y1 = ((x0 - x_lower_value) / (x_upper_value - x_lower_value)) * table[y_index - 1][x_index];
    float first_interpolated = inter_x1_y1 + inter_x2_y1;

    float inter_x1_y2 = ((x_upper_value - x0) / (x_upper_value - x_lower_value)) * table[y_index][x_index - 1];
    float inter_x2_y2 = ((x0 - x_lower_value) / (x_upper_value - x_lower_value)) * table[y_index][x_index];
    float second_interpolated = inter_x1_y2 + inter_x2_y2;

    float final_interpolated = (((y_upper_value - y0) / (y_upper_value - y_lower_value)) * first_interpolated) + (((y0 - y_lower_value) / (y_upper_value - y_lower_value)) * second_interpolated);
    return final_interpolated;
}
