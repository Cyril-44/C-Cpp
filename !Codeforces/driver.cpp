#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
    if (argc != 3) return puts("Usage: ./driver [graderExePath] [ProgramExePath]"), -1;

    int i2c[2]; // interactor → contestant
    int c2i[2]; // contestant → interactor

    if (pipe(i2c) == -1) perror("pipe"), exit(2);
    if (pipe(c2i) == -1) perror("pipe"), exit(2);

    pid_t pid_interactor = fork();
    if (pid_interactor == 0) {
        // --- Interactor child ---
        dup2(c2i[0], STDIN_FILENO);   // read from contestant
        dup2(i2c[1], STDOUT_FILENO);  // write to contestant

        close(i2c[0]); close(i2c[1]);
        close(c2i[0]); close(c2i[1]);

        execl(argv[1], argv[1], NULL);
        perror("exec");
        exit(1);
    }

    pid_t pid_contestant = fork();
    if (pid_contestant == 0) {
        // --- Contestant child ---
        dup2(i2c[0], STDIN_FILENO);   // read from interactor
        dup2(c2i[1], STDOUT_FILENO);  // write to interactor

        close(i2c[0]); close(i2c[1]);
        close(c2i[0]); close(c2i[1]);

        execl(argv[2], argv[2], NULL);
        perror("exec");
        exit(1);
    }

    // --- Parent ---
    close(i2c[0]); close(i2c[1]);
    close(c2i[0]); close(c2i[1]);

    waitpid(pid_interactor, NULL, 0);
    waitpid(pid_contestant, NULL, 0);
    return 0;
}
