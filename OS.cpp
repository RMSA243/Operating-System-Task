#include <iostream>
#include <vector>

#include <algorithm>

using namespace std;

// Process structure
struct Process
{

    int id;
    int exec_time;
    int period;

    int deadline;

};



bool cmp(Process a, Process b) //comparator function for sorting processes based on their deadline
{

    return a.deadline < b.deadline;

}

bool RM_compare(Process a, Process b)
{
    return a.period < b.period;
}

// Function to check if the processes are schedulable or not (Rate Monotonic)
bool is_schedulable_rm(vector<Process>& processes)
{
    double util_factor = 0;

    int n = processes.size();


    for (int i = 0; i < n; i++)
    {

        util_factor += (double)processes[i].exec_time / processes[i].period;


    }


    double bound = n * (pow(2, 1.0 / n) - 1);


    if (util_factor <= bound)
    {


        return true;

    }


    return false;

}




bool is_schedulable_edf(vector<Process>& processes)//Checking to see if the processes are schedulable or not (Earliest Deadline First)
{
    double util_factor = 0;

    int n = processes.size();



    for (int i = 0; i < n; i++)
    {

        util_factor += (double)processes[i].exec_time / min(processes[i].deadline, processes[i].period);



    }


    if (util_factor <= 1)
    {

        return true;


    }


    return false;
}



int LCM(vector<Process> processes, int num) //LCM founder
{

    int counter, least_common_multiple = 0;

    for (int i = 1;; i++)
    {

        counter = 0;

        for (int j = 0; j < num; j++)
        {


            if (i % processes[j].period == 0)
            {

                counter++;

            }

        }

        if (counter == num)
        {

            least_common_multiple = i;
            break;

        }
    }

    return least_common_multiple;
}


int get_highest_priority_task(const vector<Process>& tasks, const vector<int>& running_tasks, int current_time)
{

    int highest_priority = INT_MAX;
    int selected_task = -1;



    for (int i = 0; i < tasks.size(); i++)
    {

        if (running_tasks[i] == 0)
        {
            // Check if task is ready to run
            if ((current_time % tasks[i].period) == 0)
            {

                if (tasks[i].period <= highest_priority)
                {

                    highest_priority = tasks[i].period;
                    selected_task = i;
                }
            }
        }
        else
        {

            if (tasks[i].period <= highest_priority)
            {

                highest_priority = tasks[i].period;
                selected_task = i;
            }


        }
    }


    return selected_task;
}



void generate_gantt_chart_rm(vector<Process>& processes)
{
    int total_time = LCM(processes, processes.size());



    vector<int> exec_remaining(processes.size());
    vector<int> periods(processes.size());

    sort(processes.begin(), processes.end(), RM_compare);

    for (int i = 0; i < processes.size(); i++)
    {
        exec_remaining[i] = processes[i].exec_time;
        periods[i] = processes[i].period;

        cout << "Process are: " << processes[i].id << "per: " << processes[i].period << endl;
    }

    cout << "| ";
    for (int i = 0; i < total_time; i++)
    {
        int min_idx = get_highest_priority_task(processes, exec_remaining, i);



        if (min_idx != -1)
        {
            exec_remaining[min_idx]--;

            if (exec_remaining[min_idx] == -1)
            {
                exec_remaining[min_idx] = processes[min_idx].exec_time - 1;
                periods[min_idx] += processes[min_idx].period;
                cout << "P" << processes[min_idx].id << " | ";

                for (int k = 0; k < processes.size(); k++)
                {
                    if ((i % processes[k].period) == 0 && k != min_idx)
                    {
                        exec_remaining[k] = processes[k].exec_time;
                        periods[k] += processes[k].period;

                    }

                }

            }
            else
            {
                cout << "P" << processes[min_idx].id << " | ";
            }
        }
        else
        {
            cout << "IDLE | ";
        }
    }

    cout << endl;

    cout << endl;
    cout << endl;

    //cout << "0 ";
    //for (int i = 1; i <= total_time; i++)
    //{
    //    int min_idx = get_highest_priority_task(processes, exec_remaining, i - 1);

    //    if (min_idx != -1)
    //    {
    //        exec_remaining[min_idx]--;

    //        if (exec_remaining[min_idx] == -1)
    //        {
    //            exec_remaining[min_idx] = processes[min_idx].exec_time - 1;
    //            periods[min_idx] += processes[min_idx].period;

    //            for (int k = 0; k < processes.size(); k++)
    //            {
    //                if ((i % processes[k].period) == 0 && k != min_idx)
    //                {
    //                    exec_remaining[k] = processes[k].exec_time;
    //                    periods[k] += processes[k].period;

    //                }

    //            }

    //        }
    //    }

    //    int current_process_idx = -1;
    //    for (int j = 0; j < processes.size(); j++)
    //    {
    //        if (i == periods[j])
    //        {
    //            current_process_idx = j;
    //            break;
    //        }
    //    }

    //    if (current_process_idx != -1)
    //    {
    //        cout << i << " ";
    //    }
    //    else
    //    {
    //        cout << "  ";
    //    }
    //}

    //cout << total_time << endl;
}


int get_earliest_deadline_task(const vector<Process>& tasks, const vector<int>& running_tasks, int current_time)
{

    int earliest_deadline = INT_MAX;
    int selected_task = -1;


    for (int i = 0; i < tasks.size(); i++)
    {


        if (running_tasks[i] == 0)//c1 simpler condition
        {

            // Check if task is ready to run
            if ((current_time % tasks[i].period) == 0)
            {

                if (tasks[i].deadline <= earliest_deadline)
                {

                    earliest_deadline = tasks[i].deadline;
                    selected_task = i;

                }
            }

        }
        else //c2 'copy from above'
        {

            if (tasks[i].deadline <= earliest_deadline)
            {

                earliest_deadline = tasks[i].deadline;
                selected_task = i;

            }

        }
    }


    return selected_task;

}



void generate_gantt_chart_edf(vector<Process>& processes)
{
    int total_time = LCM(processes, processes.size());

    vector<int> exec_remaining(processes.size());
    vector<int> deadlines(processes.size());


    sort(processes.begin(), processes.end(), cmp);


    for (int i = 0; i < processes.size(); i++)
    {

        exec_remaining[i] = processes[i].exec_time;
        deadlines[i] = processes[i].deadline;

    }

    cout << "| ";
    for (int i = 0; i < total_time; i++)
    {
        int min_idx = get_earliest_deadline_task(processes, exec_remaining, i);

        if (min_idx != -1)
        {
            exec_remaining[min_idx]--;

            if (exec_remaining[min_idx] == -1)//c3 condition changed from 0 to -1
            {
                exec_remaining[min_idx] = processes[min_idx].exec_time - 1; //C3.2 -1 added
                deadlines[min_idx] += processes[min_idx].period;
                cout << "P" << processes[min_idx].id << " | ";

                for (int k = 0; k < processes.size(); k++) //checking for others... C4
                {
                    if ((i % processes[k].period) == 0 && k != min_idx)
                    {
                        exec_remaining[k] = processes[k].exec_time;
                        deadlines[k] += processes[k].period;

                    }

                }

            }
            else
            {
                cout << "P" << processes[min_idx].id << " | ";

            }

        }
        else
        {
            cout << "IDLE | ";
        }
    }

    cout << endl;

    cout << endl;
    cout << endl;

    //cout << "0 ";
    //for (int i = 1; i <= total_time; i++)
    //{

    //    int min_idx = get_earliest_deadline_task(processes, exec_remaining, i - 1);

    //    if (exec_remaining[min_idx] == -1)//c3 condition changed from 0 to -1 "copy from above"
    //    {
    //        exec_remaining[min_idx] = processes[min_idx].exec_time - 1; //C3.2 -1 added
    //        deadlines[min_idx] += processes[min_idx].period;

    //        for (int k = 0; k < processes.size(); k++) //checking for others... C4
    //        {
    //            if ((i % processes[k].period) == 0 && k != min_idx)
    //            {
    //                exec_remaining[k] = processes[k].exec_time;
    //                deadlines[k] += processes[k].period;

    //            }

    //        }

    //    }

    //    if (i == deadlines[min_idx])
    //    {
    //        cout << i << " ";
    //    }
    //    else
    //    {
    //        cout << "  ";
    //    }
    //}

    //cout << total_time << endl;
}



int main()
{

    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    // Read input for Rate Monotonic scheduling
    vector<Process> processes_rm;
    cout << "Rate Monotonic Scheduling:" << endl;
    for (int i = 0; i < n; i++) {
        Process p;
        cout << "Process ID: ";
        cin >> p.id;
        cout << "Execution Time: ";
        cin >> p.exec_time;
        cout << "Period: ";
        cin >> p.period;
        processes_rm.push_back(p);
    }

    // Check if processes are schedulable
    if (is_schedulable_rm(processes_rm)) {
        cout << "Processes are schedulable." << endl;
        cout << "Gantt Chart: ";
        generate_gantt_chart_rm(processes_rm);
    }
    else
    {

        cout << "Processes are not schedulable." << endl;

    }



    // Read input for Earliest Deadline First scheduling
    vector<Process> processes_edf;
    cout << "Earliest Deadline First Scheduling:" << endl;
    for (int i = 0; i < n; i++) {
        Process p;
        cout << "Process ID: ";
        cin >> p.id;
        cout << "Execution Time: ";
        cin >> p.exec_time;
        cout << "Period: ";
        cin >> p.period;
        cout << "Deadline: ";
        cin >> p.deadline;
        processes_edf.push_back(p);
    }

    // Check if processes are schedulable
    if (is_schedulable_edf(processes_edf))
    {


        cout << "Processes are schedulable." << endl;
        cout << "Gantt Chart: ";
        generate_gantt_chart_edf(processes_edf);


    }
    else
    {

        cout << "Processes are not schedulable." << endl;

    }

    return 0;
}
