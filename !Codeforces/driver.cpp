#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <string>
#include <vector>

static void write_all(int fd, const char *buf, ssize_t len) {
    ssize_t written = 0;
    while (written < len) {
        ssize_t w = write(fd, buf + written, len - written);
        if (w == -1) {
            if (errno == EINTR) continue;
            perror("write");
            break;
        }
        written += w;
    }
}

struct Args {
    bool testlib = false;
    std::string interactor;
    std::string contestant;
    std::string input;
    std::string ans;
    std::string tout;
};

static void print_usage() {
    puts("\e[36;1mUsage:\e[0m\n"
         "  ./driver <interactor> <contestant> [- | <inputFile>]\n"
         "  ./driver --testlib <interactor> <contestant> [<inputFile> [<ansFile> [<toutFile>]]]\n"
         "\n"
         "Non-testlib:\n"
         "  input from stdin (with \e[4m-\e[0m) or file is fed to interactor stdin.\n"
         "\n"
         "Testlib mode:\n"
         "  input=\e[4m-\e[0m → read stdin as input\n"
         "  tout omitted → no log for registerInteraction\n"
         "  interactor receives: input tout ans (if given)");
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage();
        return -1;
    }

    Args args;
    int idx = 1;
    if (strcmp(argv[1], "--testlib") == 0) {
        args.testlib = true;
        idx++;
        if (argc - idx < 2) {
            print_usage();
            return -1;
        }
        args.interactor = argv[idx++];
        args.contestant = argv[idx++];
        if (idx < argc) args.input = argv[idx++];
        if (idx < argc) args.ans = argv[idx++];
        if (idx < argc) args.tout = argv[idx++];
        if (idx < argc) {
            print_usage();
            return -1;
        }
    } else {
        args.interactor = argv[1];
        args.contestant = argv[2];
        if (argc == 4) args.input = argv[3];
        else if (argc > 4) {
            print_usage();
            return -1;
        }
    }

    std::string real_input, real_tout;
    bool created_input = false, created_tout = false;

    if (args.testlib) {
        if (!args.input.empty() && args.input == "-") {
            char tmpl[] = "/tmp/driver_input_XXXXXX";
            int fd = mkstemp(tmpl);
            if (fd == -1) { perror("mkstemp input"); exit(2); }
            char buf[4096];
            ssize_t r;
            while ((r = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
                write_all(fd, buf, r);
            }
            close(fd);
            real_input = tmpl;
            created_input = true;
        } else if (!args.input.empty()) {
            real_input = args.input;
        } else {
            char tmpl[] = "/tmp/driver_input_XXXXXX";
            int fd = mkstemp(tmpl);
            close(fd);
            real_input = tmpl;
            created_input = true;
        }

        if (!args.tout.empty()) {
            real_tout = args.tout;
        } else {
            char tmpl[] = "/tmp/driver_tout_XXXXXX";
            int fd = mkstemp(tmpl);
            close(fd);
            real_tout = tmpl;
            created_tout = true;
        }
    }

    int i2c[2], c2i[2];
    if (pipe(i2c) == -1) { perror("pipe i2c"); exit(2); }
    if (pipe(c2i) == -1) { perror("pipe c2i"); exit(2); }

    pid_t pid_interactor = fork();
    if (pid_interactor == -1) { perror("fork"); exit(2); }

    if (pid_interactor == 0) {
        if (dup2(c2i[0], STDIN_FILENO) == -1) { perror("dup2 interactor stdin"); exit(1); }
        if (dup2(i2c[1], STDOUT_FILENO) == -1) { perror("dup2 interactor stdout"); exit(1); }
        close(i2c[0]); close(i2c[1]);
        close(c2i[0]); close(c2i[1]);

        if (!args.testlib) {
            execl(args.interactor.c_str(), args.interactor.c_str(), nullptr);
        } else {
            std::vector<std::string> iargs = {args.interactor, real_input, real_tout};
            if (!args.ans.empty()) iargs.push_back(args.ans);
            std::vector<char*> exec_args;
            for (auto& s : iargs) exec_args.push_back(const_cast<char*>(s.c_str()));
            exec_args.push_back(nullptr);
            execv(args.interactor.c_str(), exec_args.data());
        }
        perror("exec interactor");
        exit(1);
    }

    pid_t pid_contestant = fork();
    if (pid_contestant == -1) { perror("fork"); exit(2); }

    if (pid_contestant == 0) {
        if (dup2(i2c[0], STDIN_FILENO) == -1) { perror("dup2 contestant stdin"); exit(1); }
        if (dup2(c2i[1], STDOUT_FILENO) == -1) { perror("dup2 contestant stdout"); exit(1); }
        close(i2c[0]); close(i2c[1]);
        close(c2i[0]); close(c2i[1]);

        execl(args.contestant.c_str(), args.contestant.c_str(), nullptr);
        perror("exec contestant");
        exit(1);
    }

    close(i2c[0]); close(i2c[1]);
    close(c2i[0]);
    int parent_write_fd = c2i[1];

    if (!args.testlib && !args.input.empty()) {
        const char *src = args.input.c_str();
        int fd = -1;
        if (args.input == "-") {
            fd = STDIN_FILENO;
        } else {
            fd = open(src, O_RDONLY);
            if (fd == -1) {
                fprintf(stderr, "\e[31;1mFailed to open %s: %s\e[0m\n\e[36mUsing no input file instead...\e[0m\n", src, strerror(errno));
                close(parent_write_fd);
                waitpid(pid_interactor, nullptr, 0);
                waitpid(pid_contestant, nullptr, 0);
                return 2;
            }
        }

        char buf[4096];
        ssize_t r;
        while ((r = read(fd, buf, sizeof(buf))) > 0) {
            write_all(parent_write_fd, buf, r);
        }
        if (r == -1 && errno != EINTR) perror("read input");

        if (fd != STDIN_FILENO) close(fd);
        close(parent_write_fd);
    } else {
        close(parent_write_fd);
    }

    int si, sc;
    waitpid(pid_interactor, &si, 0);
    if (!WIFEXITED(si) || WEXITSTATUS(si) != 0) {
        fprintf(stderr, "\e[31mInteractor failed (did not exit with 0)\e[0m\n");
        kill(pid_contestant, SIGKILL);   // testlib 的 FAIL/WA 会让 interactor 非0退出，但 contestant 可能卡在 read
        waitpid(pid_contestant, &sc, 0); // 强制杀死，避免整个 driver 卡住
        return 1;
    }

    waitpid(pid_contestant, &sc, 0);

    if (!WIFEXITED(sc) || WEXITSTATUS(sc) != 0) {
        fprintf(stderr, "\e[31mContestant failed (did not exit with 0)\e[0m\n");
        return 1;
    }

    if (args.testlib) {
        if (created_input) unlink(real_input.c_str());
        if (created_tout) unlink(real_tout.c_str());
    }

    return 0;
}