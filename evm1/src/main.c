#include "task.h"

#define RUN_TASK104(...)                                            \
    do {                                                            \
        const char *_argv[] = { "task104", __VA_ARGS__, NULL };     \
        int _argc = (int)(sizeof(_argv) / sizeof(_argv[0])) - 1;    \
        task104(_argc, (char **)_argv);                             \
    } while (0)

int main(void) {

#ifdef AUTOTESTING_104
    printf("Test1 (file, n=4, m=4, k=0)\n");
    RUN_TASK104("4", "4", "0", "test/task104/test1.txt");

    printf("Test2 (formula k=1, n=4, m=4)\n");
    RUN_TASK104("4", "4", "1");

    printf("Test3 (formula k=2, n=5, m=5)\n");
    RUN_TASK104("5", "5", "2");

    printf("Test4 (formula k=3, n=6, m=3)\n");
    RUN_TASK104("6", "3", "3");

    printf("Test5 (formula k=4, n=7, m=4)\n");
    RUN_TASK104("7", "4", "4");

    printf("Test6 (formula k=1, n=200, m=6)\n");
    RUN_TASK104("200", "6", "1");
#endif

#ifdef TESTING_104
    char line[512];
    if (fgets(line, sizeof(line), stdin)) {
        const char *argv[8] = { "task104" };
        int argc = 1;
        char *tok = strtok(line, " \t\n");
        while (tok && argc < 7) {
            argv[argc++] = tok;
            tok = strtok(NULL, " \t\n");
        }
        task104(argc,(char **) argv);
    }
#endif

    return 0;
}
