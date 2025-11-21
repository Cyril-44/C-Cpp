#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

/*
用法：
    ./driver ./CF1599_re ./CF1599_grader
*/

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <solver> <grader>\n";
        return 1;
    }

    string solver = argv[1];
    string grader = argv[2];

    // 建立两对管道：solver->grader 和 grader->solver
    int to_grader[2], to_solver[2];
    pipe(to_grader);
    pipe(to_solver);

    pid_t pid_solver = fork();
    if (pid_solver == 0) {
        // 子进程：运行 solver
        dup2(to_solver[0], STDIN_FILENO);   // grader->solver 输入
        dup2(to_grader[1], STDOUT_FILENO);  // solver->grader 输出
        close(to_solver[1]);
        close(to_grader[0]);
        execl(solver.c_str(), solver.c_str(), (char*)NULL);
        perror("exec solver failed");
        exit(1);
    }

    pid_t pid_grader = fork();
    if (pid_grader == 0) {
        // 子进程：运行 grader
        dup2(to_grader[0], STDIN_FILENO);   // solver->grader 输入
        dup2(to_solver[1], STDOUT_FILENO);  // grader->solver 输出
        close(to_solver[0]);
        close(to_grader[1]);
        execl(grader.c_str(), grader.c_str(), (char*)NULL);
        perror("exec grader failed");
        exit(1);
    }

    // 父进程关闭所有管道端口
    close(to_grader[0]);
    close(to_grader[1]);
    close(to_solver[0]);
    close(to_solver[1]);

    // 等待两个子进程结束
    int status;
    waitpid(pid_solver, &status, 0);
    waitpid(pid_grader, &status, 0);

    cerr << "Interaction finished.\n";
    return 0;
}
