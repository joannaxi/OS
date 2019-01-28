//
//  main.cpp
//  Sudoku
//
//  Created by WENXI LU on 1/27/19.
//  Copyright © 2019 WENXI LU. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <stdlib.h>
#include <set>
using namespace std;
#define NUM_THREADS 9
#define ROW 9
#define COL 9

int valid[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int Sudoku[9][9] = {
    6,2,4,5,3,9,1,8,7,
    5,1,9,7,2,8,6,3,4,
    8,3,7,6,1,4,2,9,5,
    1,4,3,8,6,5,7,2,9,
    9,5,8,2,4,7,3,6,1,
    7,6,2,3,9,1,4,5,8,
    3,7,1,9,5,6,8,4,2,
    4,9,6,1,8,2,5,7,3,
    2,8,5,4,7,3,9,1,6
};

struct data {
    int row;
    int col;
};

void *checkRow(void *threadData) {
    set<int> seen;
    struct data *t_data;
    t_data = (struct data *) threadData;
    for(int i = t_data->row; i < ROW; ++i) {
//        t_data->row = i;
        for(int j = t_data->col; j < COL; ++j) {
//            t_data->col = j;
            if(seen.count(Sudoku[i][j])) {
                return 0;
            } else {
                seen.insert(Sudoku[i][j]);
            }
        }
        seen.clear();
    }
    valid[0] = 1;
    return 0;
    
}
void *checkCol(void *threadData) {
    set<int> seen;
    struct data *t_data;
    t_data = (struct data *) threadData;
    for(int i = t_data->col; i < COL; ++i) {
//        t_data->col = i;
        for(int j = t_data->row; j < ROW; ++j) {
//            t_data->row = j;
            if(seen.count(Sudoku[j][i])) {
                return 0;
            } else {
                seen.insert(Sudoku[j][i]);
            }
        }
        seen.clear();
    }
    valid[1] = 1;
    return 0;
}

void *checkSubGrids(void *threadData) {
    set<int> seen;
    struct data *t_data;
    t_data = (struct data *) threadData;
    for(int i = t_data->row; i < t_data->row + 1; ++i) {
        for(int j = t_data->col; j < t_data->col + 1; ++j) {
            if(seen.count(Sudoku[i][j])) {
                return 0;
            } else {
                seen.insert(Sudoku[i][j]);
            }
        }
    }
    if(t_data->row == 0) {
        if(t_data->col == 0) {
            valid[2] = 1;
        } else if(t_data->col == 3) {
            valid[3] = 1;
        } else {
            valid[4] = 1;
        }
    }
    else if(t_data->row == 3) {
        if(t_data->col == 0) {
            valid[5] = 1;
        } else if(t_data->col == 3) {
            valid[6] = 1;
        } else {
            valid[7] = 1;
        }
    }
    else {
        if(t_data->col == 0) {
            valid[8] = 1;
        } else if(t_data->col == 3) {
            valid[9] = 1;
        } else {
            valid[10] = 1;
        }
    }
//    valid[2] = 1;
    return 0;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    pthread_t p1;
    pthread_t p2;
    pthread_t threads[NUM_THREADS];
    struct data t1;
    struct data t2;
    struct data td[NUM_THREADS];
    t1.row = 0;
    t1.col = 0;
    t2.row = 0;
    t2.col = 0;
    td[0].row = 0;
    td[0].col = 0;
    td[1].row = 0;
    td[1].col = 3;
    td[2].row = 0;
    td[2].col = 6;
    td[3].row = 3;
    td[3].col = 0;
    td[4].row = 3;
    td[4].col = 3;
    td[5].row = 3;
    td[5].col = 6;
    td[6].row = 6;
    td[6].col = 0;
    td[7].row = 6;
    td[7].col = 3;
    td[8].row = 6;
    td[8].col = 6;
    pthread_create(&p1, NULL, checkRow, (void *)&t1);
    pthread_create(&p2, NULL, checkCol, (void *)&t2);
    for(int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, checkSubGrids, (void *)&td[i]);
    }
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    for(int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
    for(int i = 0; i < 11; ++i) {
//        cout << valid[i] << endl;
        if(valid[i] == 0) {
            cout << "the Sudoku puzzle is invalid." << endl;
//            cout << i << endl;
            return 0;
        }
    }
    cout << "the Sudoku puzzle is valid." << endl;
    return 0;
}
