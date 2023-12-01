#include<iostream>
#include<string>
#include "utils.hpp"

using namespace std;

int benchmark(string algorithm, string benchmark_file, string output_file) {

    int run;

    if (benchmark_file.length()) {
        problem_instance = (char*)benchmark_file.c_str();
        read_problem(problem_instance);   //Read EVRP from file from EVRP.h
        cout << problem_instance << "\n";
    }
    else {
        cout << "Please specify a problem instance" << endl;
        return 0;
    }
    char* output_path;
    if (output_file.length()) {
        output_path = (char*)output_file.c_str();
    }
    else {
        cout << "Please specify an output path" << endl;
        return 0;
    }
    open_stats(algorithm, output_path);//open text files to store the best values from the 20 runs stats.h
    cout << "Running " << MAX_TRIALS << " times" << endl;

    for (run = 1; run <= MAX_TRIALS; run++) {
        start_run(run);
        if (algorithm == "SACO") {
            SACO* ANTS = new SACO(1, 2, 80, 0.1, 2, 0);
            while (!termination_condition(1)) {
                ANTS->optimize();
            }
            save_solution(output_path, algorithm, problem_instance, run);
            ANTS->free_SACO();
        }
        else if (algorithm == "GS") {
            initialize_GS();
            gs_optimizer.run(gs_optimizer.best_solution);
            save_solution(output_path, algorithm, problem_instance, run);
            free_GS();
        }
        else if (algorithm == "HMAGS") {
            initialize_HMAGS();
            while (!termination_condition(1)) {
                run_HMAGS();
            }
            save_solution(output_path, algorithm, problem_instance, run);
            free_HMAGS();
        }
        else if (algorithm == "SA") {
            initialize_SA();
            SA_optimizer.run(SA_optimizer.cur_sol);
            save_solution(output_path, algorithm, problem_instance, run);
            free_SA();
        }
        else {
            cout << "Algorithm " << algorithm << " not found" << endl;
        }
        end_run(run);  //store the best solution quality for each run
    }
    close_stats();

    return 0;
}

int main(int argc, char* argv[]) {

    string algorithm;
    string output_dir;
    string benchmark_file;
    std::vector<string> benchmark_files;

    if (argc == 1)
    {
        algorithm = "HMAGS";
        
        output_dir = "output_files";
        output_dir += "/" + algorithm;

        benchmark_files =
        {
            "X-n351-k40.evrp",
            "X-n459-k26.evrp",
            "X-n573-k30.evrp",
            "X-n685-k75.evrp",
            "X-n749-k98.evrp",
            "X-n819-k171.evrp",
            "X-n916-k207.evrp",
            "X-n1001-k43.evrp",
            "E-n22-k4.evrp",
            "E-n23-k3.evrp",
            "E-n30-k3.evrp",
            "E-n33-k4.evrp",
            "E-n51-k5.evrp",
            "E-n76-k7.evrp",
            "E-n101-k8.evrp",
            "X-n143-k7.evrp",
            "X-n214-k11.evrp"
        };
    }
    else
    {
        algorithm = argv[1];
        output_dir = argv[2];
        benchmark_files.push_back(argv[3]);
    }

    string benchmark_dir = "../../../../../evrp-benchmark-set/";

    for each (auto benchmark_file in benchmark_files)
    {
        string benchmark_path = benchmark_dir + benchmark_file;
        benchmark(algorithm, benchmark_path, output_dir);
    }

    return 0;
}