// MVSC, c++ 20
#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <future>
#include <chrono>
#include <syncstream>

using namespace std;
using namespace std::chrono;

void slow(const string& name) {
    this_thread::sleep_for(seconds(7));
    osyncstream(cout) << name << endl;
}

void quick(const string& name) {
    this_thread::sleep_for(seconds(1));
    osyncstream(cout) << name << endl;
}

void work() {
    auto start_time = steady_clock::now();

    auto fut_branch1 = async(launch::async, []() {
        slow("C2");
        quick("D2");
        });

    auto fut_branch2 = async(launch::async, []() {
        quick("A2");
        quick("C1");
        });

    slow("A1");

    fut_branch2.get();

    quick("B");

    quick("D1");

    fut_branch1.get();

    auto end_time = steady_clock::now();
    double elapsed = duration_cast<milliseconds>(end_time - start_time).count() / 1000.0;

    {
        osyncstream out(cout);
        out << fixed << setprecision(2);
        out << "Execution time: " << elapsed << " s" << endl;
        out << "Work is done!" << endl;
    }
}

int main() {
    work();
    return 0;
}
