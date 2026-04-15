
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

bool src_walk_dir(Walk_Entry entry) {
    if(entry.type == FILE_REGULAR) {
        String_View path = sv_from_cstr(entry.path);
        if(!sv_ends_with_cstr(path, ".cpp")) return true;
        Cmd *cmd = entry.data;
        cmd_append(cmd, temp_strdup(entry.path));
    }
    return true;
}

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

    size_t mark = temp_save(); {
        walk_dir(SRC_FOLDER, src_walk_dir, .data = &cmd);
    } temp_rewind(mark);

    cmd_append(&cmd, LIBS);
    if(!cmd_run(&cmd)) return 1;

    if(*run) {
        cmd_append(&cmd, BIN_PATH);
        if (!cmd_run(&cmd)) return 1;
    }

    return 0;
}
