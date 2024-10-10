// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <thread>
#include <map>

using namespace std;

string read_line_from(string file) {
    string line;
    ifstream myfile(file);
    if (myfile.is_open())
    {
        getline(myfile, line);
        myfile.close();
    }
    else cout << "Unable to open file"; 
    return line;
}

void print_load_avg() {
    string line = read_line_from("/proc/loadavg");
    int start = -1;
    for (int i = 0; i < 3; ++i) {
        start = line.find_first_of(' ', start + 1);
    }
    string load = line.substr(0, start);
    cout << "󰻠 " << load << " ";
}

void print_mem_info() {
    ifstream myfile("/proc/meminfo");
    if (myfile.is_open())
    {
        map<string, unsigned long> m;
        string line;
        for (int i = 0; i < 3; ++i) {
            getline(myfile, line);
            int key_end = line.find_first_of(':', 0);
            string key = line.substr(0, key_end);
            int value_start = line.find_first_not_of(' ', key_end+1);
            int value_end = line.find_first_of(' ', value_start);
            string str = line.substr(value_start, value_end - value_start);
            unsigned long value = std::stoul(str, nullptr);
            m[key] = value / 1000000;
        }
        myfile.close();
        cout << "󰍛 " << m["MemAvailable"] << '/' << m["MemTotal"] << '(' << m["MemFree"] << ')' << " ";
    }
    else cout << "Unable to open file"; 
}

void print_battery_info(string battery_id) {
    string charge_full = read_line_from("/sys/class/power_supply/" + battery_id + "/charge_full");
    string charge_now = read_line_from("/sys/class/power_supply/" + battery_id + "/charge_now");
    unsigned long charge_percentage = std::stoul(charge_now, nullptr) * 100 / std::stoul(charge_full, nullptr);
    string status = read_line_from("/sys/class/power_supply/" + battery_id + "/status");
    string battery_status;
    if (status == "Full") {
        battery_status = "󰂅 ";
    } else if (status == "Charging") {
        if (charge_percentage < 33) {
            battery_status = "󱊤 ";
        } else if (charge_percentage < 66) {
            battery_status = "󱊥 ";
        } else {
            battery_status = "󱊦 ";
        }
    } else if (status == "Discharging") {
        if (charge_percentage < 33) {
            battery_status = "󱊡 ";
        } else if (charge_percentage < 66) {
            battery_status = "󱊢 ";
        } else {
            battery_status = "󱊣 ";
        }
    }
    cout << battery_status << charge_percentage << '%' << " ";
}

void print_datetime() {
    time_t now = time(0);
    struct tm * timeinfo = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "%a,%d-%b-%Y %H:%M", timeinfo);
    cout << " " << buffer << " ";
}

int main (int argc, char *argv[]) {
    while (true) {
        print_load_avg();
        print_mem_info();
        if (argc > 1) {
            print_battery_info(argv[1]);
        }
        print_datetime();
        cout << std::flush;
        this_thread::sleep_for(chrono::seconds(5));
    }

    return 0;
}
