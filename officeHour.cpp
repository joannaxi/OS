//
//  main.cpp
//  OfficeHour
//
//  Created by WENXI LU on 2/5/19.
//  Copyright © 2019 WENXI LU. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>
#include <chrono>
#include <stdlib.h>
#include <queue>
#include <stdio.h>
#include <time.h>
#include <algorithm>
#include<set>
using namespace std;

// global variables:
bool ready = false;
const int max_students_wait = 3;


// lock, conditional variables;
//queue<int> chairs;
set<int> st;
mutex mtx;
condition_variable call_ta;
condition_variable call_waiting_students;


// threads of students' function
void student_seek_help(int id) {
    unique_lock<std::mutex> lck(mtx);
    while (true) {
//        cout << "student gets lock" << endl;
        if (st.size() < max_students_wait)  {
//            cout << "student in loop" << endl;
            if(!st.count(id)) {
                st.insert(id);
            }
            cout << st.size() << endl;
            call_ta.notify_all();
            call_waiting_students.wait(lck, [&] {
                return (!st.count(id) && ready);
            });
        }
//        cout << st.size() << endl;
//        cout << "All chairs are occupied." << endl;
//        cout << "thread " << id << " is programming now." << endl;
        srand ((unsigned)time(NULL));
        unsigned sleep_time = rand() % 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));

    }
}


// thread of TA's function
void ta_help_student () {
    unique_lock<std::mutex> lck(mtx);
    while (true) {
        cout << "ta gets lock" << endl;
        while(!st.empty()) {
            st.erase(st.begin());
            srand ((unsigned)time(0));
            unsigned sleep_time = rand() % 100;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
            ready = true; // <- useless
            call_waiting_students.notify_all();
    }
    call_ta.wait(lck, [&]{
        if (!st.empty()) {
            cout << "TA is awaken." << endl;
        return  true;
    } else {
        cout << "TA is sleeping." << endl;
        ready = true;
        return false;
    }
    });
    }
}


int main(int argc, const char * argv[]) {
    int num = stoi(argv[1]);
    thread threads[num];
    for(int i = 0; i < num; ++i) {
        threads[i] = thread(student_seek_help, i);
    }
    thread ta(ta_help_student);
    for (int i = 0; i < num; ++i) threads[i].join();
    ta.join();
}
