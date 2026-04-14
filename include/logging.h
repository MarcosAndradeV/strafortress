#pragma once

#include "nob.h"

#ifdef _WIN32
    #include <io.h>
    #define ISATTY _isatty
    #define FILENO _fileno
#else
    #include <unistd.h>
    #define ISATTY isatty
    #define FILENO fileno
#endif

#define init_logging() if(ISATTY(FILENO(stdout))) nob_set_log_handler(sf_log_handler)

static void sf_log_handler(Nob_Log_Level level, const char *fmt, va_list args)
{
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_RESET   "\x1b[0m"
    if (level < nob_minimal_log_level) return;

    switch (level) {
        case NOB_INFO:
            fprintf(stderr, ANSI_COLOR_GREEN "INFO: " ANSI_COLOR_RESET);
            break;
        case NOB_WARNING:
            fprintf(stderr, ANSI_COLOR_YELLOW "WARNING: " ANSI_COLOR_RESET);
            break;
        case NOB_ERROR:
            fprintf(stderr, ANSI_COLOR_RED "ERROR: " ANSI_COLOR_RESET);
            break;
        case NOB_NO_LOGS:
            return;
        default:
            NOB_UNREACHABLE("Nob_Log_Level");
    }

    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
}
