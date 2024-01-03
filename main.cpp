#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;
using namespace sf;

int numProcesses;
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int remainingTime;
    int priority;
};

void printHeader() {
    cout << setw(8) << "Process |"
        << setw(15) << "Completion Time |"
        << setw(15) << "Turnaround Time |"
        << setw(15) << "Waiting Time" << endl;
    cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
}

void printProcess(const Process& process) {
    cout << setw(8) << process.id << " |"
        << setw(15) << process.completionTime << " |"
        << setw(15) << process.turnaroundTime << " |"
        << setw(15) << process.waitingTime << endl;
}

void simulateFCFS(vector<Process>& processes) {
    int currentTime = 0;
    float wt = 0, tat = 0;
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b)
        {
            return a.arrivalTime < b.arrivalTime;
        });
    for (Process& process : processes) {
        currentTime = max(currentTime, process.arrivalTime);
        process.completionTime = currentTime + process.burstTime;
        process.turnaroundTime = process.completionTime - process.arrivalTime;
        process.waitingTime = process.turnaroundTime - process.burstTime;
        currentTime += process.burstTime;
    }
    for (Process& process : processes) {
        wt += process.waitingTime;
        tat += process.turnaroundTime;
    }

    printHeader();
    for (const Process& process : processes) {
        printProcess(process);
    }

    cout << "\nAverage Waiting time = " << (float)wt / (float)numProcesses;
    cout << "\nAverage Turnaround time = " << (float)tat / (float)numProcesses;
    cout << endl;

}

void simulateSJF(vector<Process>& processes) 
{
    int currentTime = 0;
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b)
        {
            return a.arrivalTime < b.arrivalTime;
        });
    for (Process& process : processes) {
        process.remainingTime = process.burstTime;
    }
    int complete = 0, t = 0;
    bool check = false;

    while (complete != numProcesses) {
        int shortest = -1;
        int minm = INT_MAX;

        for (int j = 0; j < numProcesses; j++) {
            if (processes[j].arrivalTime <= t && processes[j].remainingTime < minm && processes[j].remainingTime>0)
            {
                minm = processes[j].remainingTime;
                shortest = j;
                check = true;
            }
        }

        if (shortest == -1) {
            t++;
            continue;
        }

        processes[shortest].remainingTime--;

        if (processes[shortest].remainingTime == 0) {
            complete++;
            check = false;
            processes[shortest].waitingTime = t + 1 - processes[shortest].burstTime - processes[shortest].arrivalTime;
            if (processes[shortest].waitingTime < 0)
                processes[shortest].waitingTime = 0;
        }

        t++;
    }

    for (Process& process : processes)
    {
        process.turnaroundTime = process.burstTime + process.waitingTime;
    }
    int total_wt = 0, total_tat = 0;
    cout << setw(8) << "Process |"
        << setw(15) << "Burst Time |"
        << setw(15) << "Completion Time |"
        << setw(15) << "Turnaround Time |"
        << setw(15) << "Waiting Time  |" << endl;
    cout << setfill('-') << setw(73) << "-" << setfill(' ') << endl;



    for (int i = 0; i < numProcesses; i++) {
        total_wt += processes[i].waitingTime;
        total_tat += processes[i].turnaroundTime;
        processes[i].completionTime = processes[i].turnaroundTime + processes[i].arrivalTime;
        cout << setw(7) << processes[i].id << " |"
            << setw(13) << processes[i].burstTime << " |"
            << setw(15) << processes[i].completionTime << " |"
            << setw(15) << processes[i].turnaroundTime << " |"
            << setw(13) << processes[i].waitingTime << " |" << endl;
    }

    cout << "\nAverage Waiting time = " << (float)total_wt / (float)numProcesses;
    cout << "\nAverage Turnaround time = " << (float)total_tat / (float)numProcesses;
    cout << endl;

}

void simulateSRTF(int n)
{
    int arrival_time[10], burst_time[10], temp[10];
    int  j, smallest, count = 0, time, limit = n;
    double wait_time = 0, turnaround_time = 0, end;
    float average_waiting_time, average_turnaround_time;

    cout << "Enter Details of " << limit << " Processes" << endl;
    for (int i = 0; i < limit; i++)
    {
        cout << "Enter Arrival Time for Process " << i + 1 << ": ";
        cin >> arrival_time[i];
        cout << "Enter Burst Time for Process " << i + 1 << ": ";
        cin >> burst_time[i];
        temp[i] = burst_time[i];
    }
    for (int i = 0; i < limit - 1; i++) {
        for (int j = 0; j < limit - i - 1; j++) {
            if (arrival_time[j] > arrival_time[j + 1]) {
                swap(arrival_time[j], arrival_time[j + 1]);
                swap(burst_time[j], burst_time[j + 1]);
                swap(temp[j], temp[j + 1]);
            }
        }
    }
    burst_time[9] = 9999;

    cout << "\nProcess\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time" << endl;

    for (time = 0; count < limit; time++)
    {
        smallest = 9;
        for (j = 0; j < limit; j++)
        {
            if (arrival_time[j] <= time && burst_time[j] < burst_time[smallest] && burst_time[j] > 0)
            {
                smallest = j;
            }
        }
        burst_time[smallest]--;
        if (burst_time[smallest] == 0)
        {
            count++;
            end = time + 1;
            wait_time = wait_time + end - arrival_time[smallest] - temp[smallest];
            turnaround_time = turnaround_time + end - arrival_time[smallest];

            cout << smallest + 1 << "\t\t" << arrival_time[smallest] << "\t\t" << temp[smallest] << "\t\t" << end << "\t\t" << (end - arrival_time[smallest]) << "\t\t" << (end - arrival_time[smallest] - temp[smallest]) << endl;
        }
    }

    average_waiting_time = wait_time / limit;
    average_turnaround_time = turnaround_time / limit;

    cout << "\nAverage Waiting Time: " << average_waiting_time << endl;
    cout << "Average Turnaround Time: " << average_turnaround_time << endl;


}

void simulatePriority(vector<Process>& processes,int n) {
    int wt = 0, tat = 0;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].priority > processes[j + 1].priority)
            {
                //Swap the processes
                std::swap(processes[j], processes[j + 1]);
            }
        }
    }

    processes[0].waitingTime = 0;
    processes[0].turnaroundTime = processes[0].burstTime;
    processes[0].completionTime = processes[0].turnaroundTime;

    for (int i = 1; i < n; i++)
    {
        processes[i].waitingTime = processes[i - 1].waitingTime + processes[i - 1].burstTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        processes[i].completionTime = processes[i].turnaroundTime;
    }

    cout << setw(8) << "Priority |"
        << setw(15) << "Brust Time |"
        << setw(15) << "Completion Time |"
        << setw(15) << "Turnaround Time |"
        << setw(15) << "Waiting Time  |" << endl;
    cout << setfill('-') << setw(74) << "-" << setfill(' ') << endl;
    for (int i = 0; i < numProcesses; i++) {
        cout << setw(8) << processes[i].priority << " |"
            << setw(13) << processes[i].burstTime << " |"
            << setw(15) << processes[i].completionTime << " |"
            << setw(15) << processes[i].turnaroundTime << " |"
            << setw(12) << processes[i].waitingTime << "  |" << endl;
    }

    for (Process& process : processes) {
        wt += process.waitingTime;
        tat += process.turnaroundTime;
    }

    cout << "\nAverage Waiting time = " << (float)wt / (float)numProcesses;
    cout << "\nAverage Turnaround time = " << (float)tat / (float)numProcesses;
    cout << endl;

}

void queueUpdation(int queue[], int timer, int arrival[], int n, int maxProccessIndex) {
    int zeroIndex=0;
    for (int i = 0; i < n; i++) {
        if (queue[i] == 0) {
            zeroIndex = i;
            break;
        }
    }
    queue[zeroIndex] = maxProccessIndex + 1;
}

void queueMaintainence(int queue[], int n) {
    for (int i = 0; (i < n - 1) && (queue[i + 1] != 0); i++) {
        int temp = queue[i];
        queue[i] = queue[i + 1];
        queue[i + 1] = temp;
    }
}

void checkNewArrival(int timer, int arrival[], int n, int maxProccessIndex, int queue[]) {
    if (timer <= arrival[n - 1]) {
        bool newArrival = false;
        for (int j = (maxProccessIndex + 1); j < n; j++) {
            if (arrival[j] <= timer) {
                if (maxProccessIndex < j) {
                    maxProccessIndex = j;
                    newArrival = true;
                }
            }
        }
        //adds the incoming process to the ready queue
        //(if any arrives)
        if (newArrival)
            queueUpdation(queue, timer, arrival, n, maxProccessIndex);
    }
}
void simulateRoundRobin(int l) {
    int n = l, tq, timer = 0, maxProccessIndex = 0;
    float avgWait = 0, avgTT = 0;
    cout << "\nEnter the time Quantum : ";
    cin >> tq;
    int arrival[50], burst[50], wait[50], turn[50], queue[50], temp_burst[50];
    bool complete[50];


    for (int i = 0; i < n; i++)
    {
        cout << "\nEnter the arrival time of the processe " << i + 1 << " : ";
        cin >> arrival[i];
        cout << "\nEnter the burst time of the processes " << i + 1 << " : ";
        cin >> burst[i];
        temp_burst[i] = burst[i];
    }
    
    for (int i = 0; i < n; i++) { //Initializing the queue and complete array
        complete[i] = false;
        queue[i] = 0;
    }
    while (timer < arrival[0]) //Incrementing Timer until the first process arrives
        timer++;
    queue[0] = 1;

    while (true) {
        bool flag = true;
        for (int i = 0; i < n; i++) {
            if (temp_burst[i] != 0) {
                flag = false;
                break;
            }
        }
        if (flag)
            break;

        for (int i = 0; (i < n) && (queue[i] != 0); i++) {
            int ctr = 0;
            while ((ctr < tq) && (temp_burst[queue[0] - 1] > 0)) {
                temp_burst[queue[0] - 1] -= 1;
                timer += 1;
                ctr++;

                //Checking and Updating the ready queue until all the processes arrive
                checkNewArrival(timer, arrival, n, maxProccessIndex, queue);
            }
            //If a process is completed then store its exit time
            //and mark it as completed
            if ((temp_burst[queue[0] - 1] == 0) && (complete[queue[0] - 1] == false)) {
                //turn array currently stores the completion time
                turn[queue[0] - 1] = timer;
                complete[queue[0] - 1] = true;
            }

            //checks whether or not CPU is idle
            bool idle = true;
            if (queue[n - 1] == 0) {
                for (int i = 0; i < n && queue[i] != 0; i++) {
                    if (complete[queue[i] - 1] == false) {
                        idle = false;
                    }
                }
            }
            else
                idle = false;

            if (idle) {
                timer++;
                checkNewArrival(timer, arrival, n, maxProccessIndex, queue);
            }

            //Maintaining the entries of processes 
            //after each premption in the ready Queue
            queueMaintainence(queue, n);
        }
    }

    for (int i = 0; i < n; i++) {
        turn[i] = turn[i] - arrival[i];
        wait[i] = turn[i] - burst[i];
    }

    cout << "\nProgram No.\tArrival Time\tBurst Time\tWait Time\tTurnAround Time"
        << endl;
    for (int i = 0; i < n; i++) {
        cout << i + 1 << "\t\t" << arrival[i] << "\t\t"
            << burst[i] << "\t\t" << wait[i] << "\t\t" << turn[i] << endl;
    }
    for (int i = 0; i < n; i++) {
        avgWait += wait[i];
        avgTT += turn[i];
    }
    cout << "\nAverage Waiting time : " << (avgWait / n)
        << "\nAverage Turn Around Time : " << (avgTT / n);

}

int main() {
    try{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "PROCESS SCHEDULING SIMULATOR");

    sf::Font font;
    if (!font.loadFromFile("Roboto-Black.ttf")) {
        std::cerr << "Error loading font file." << std::endl;
        return EXIT_FAILURE;
    }

    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(40);
    menuText.setFillColor(sf::Color::White);
    menuText.setString("PROCESS SCHEDULING SIMULATOR\n\n\n1. FCFS \n2. SJF \n3. ROUND ROBIN \n4. SRTF \n5. PRIORITY \n6. EXIT ");
    menuText.setPosition(200, 200);

    cout << "Enter the number of processes: ";
    cin >> numProcesses;
    if (numProcesses <= 0) {
        throw std::invalid_argument("Number of processes must be greater than zero.");
    }

    vector<Process> processes(numProcesses);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    std::cout << "FCFS selected." << std::endl;
                    for (int i = 0; i < numProcesses; i++) {
                        cout << "Enter details for Process " << i + 1 << ":" << endl;
                        processes[i].id = i + 1;
                        cout << "Arrival Time: ";
                        cin >> processes[i].arrivalTime;
                        cout << "Burst Time: ";
                        cin >> processes[i].burstTime;
                    }
                    cout << "FCFS Scheduling:" << endl;
                    simulateFCFS(processes);
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    std::cout << "SJF selected." << std::endl;
                    for (int i = 0; i < numProcesses; ++i) {
                        cout << "Enter details for Process " << i + 1 << ":" << endl;
                        processes[i].id = i + 1;
                        cout << "Arrival Time: ";
                        cin >> processes[i].arrivalTime;
                        cout << "Burst Time: ";
                        cin >> processes[i].burstTime;
                    }
                    cout << "Shortest Job Next (SJF) Scheduling:" << endl;
                    simulateSJF(processes);
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    std::cout << "ROUND ROBIN selected." << std::endl;
                    cout << "Round Robin (RR) Scheduling:" << endl;
                    simulateRoundRobin(numProcesses);
                }
                else if (event.key.code == sf::Keyboard::Num4) {
                    std::cout << "SRTF selected." << std::endl;
                    cout << "Shortest Remaining Time First (SRTF) Scheduling:" << endl;
                    simulateSRTF(numProcesses);
                }
                else if (event.key.code == sf::Keyboard::Num5) {
                    std::cout << "PRIORITY selected." << std::endl;
                    for (int i = 0; i < numProcesses; ++i) {
                        cout << "Enter details for Process " << i + 1 << ":" << endl;
                        processes[i].id = i + 1;
                        processes[i].arrivalTime = 0;
                        cout << "Burst Time: ";
                        cin >> processes[i].burstTime;
                        cout << "Priority: ";
                        cin >> processes[i].priority;
                    }  cout << "Priority Scheduling:" << endl;
                    simulatePriority(processes, numProcesses);
                }
                else if (event.key.code == sf::Keyboard::Num6) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Cyan);
        window.draw(menuText);
        window.display();
    }
}
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    
 system("pause");
        return 0;
}
