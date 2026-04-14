
#define NOB_IMPLEMENTATION
#include "include/nob.h"
#define FLAG_IMPLEMENTATION
#include "include/flag.h"

#define BUILD_FOLDER "build"
#define BIN_NAME "strafortress"
#define BIN_PATH BUILD_FOLDER"/"BIN_NAME
#define SRC_FOLDER "src"

#define CXX "g++"
#define LIBS "-lraylib", "-lm"

static Cmd cmd = {0};

int main(int argc, char** argv)
{
    GO_REBUILD_URSELF(argc, argv);
    if(!mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    bool *run = flag_bool("run", false, "Run after build.");
    bool *help = flag_bool("help", false, "Prints help message.");

    if(!flag_parse(argc, argv)) {
        flag_print_error(stderr);
        return 1;
    }

    if(*help) {
        flag_print_options(stdout);
        return 0;
    }

    cmd_append(&cmd, CXX);
    cmd_append(&cmd, "-o", BIN_PATH);
    cmd_append(&cmd, SRC_FOLDER"/main.cpp");
    cmd_append(&cmd, SRC_FOLDER"/world.cpp");
    cmd_append(&cmd, SRC_FOLDER"/simulation.cpp");
    cmd_append(&cmd, LIBS);
    if(!cmd_run(&cmd)) return 1;

    if(*run) {
        cmd_append(&cmd, BIN_PATH);
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
