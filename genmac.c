
#include <sys/types.h> // for pid_t
#include <unistd.h>    // for getpid()
#include <stdint.h>    // for uint8_t
#include <stdio.h>     // for printf()
#include <stdlib.h>    // for rand()/srand()
#include <argp.h>      // for arpg stuff

// See info libc -> Argp Global Variables

const char * argp_program_version = "0.2";
const char * argp_program_bug_address = "vmj@linuxbox.fi";

// Local types

typedef uint8_t bool;
#define TRUE 1
#define FALSE 0

typedef struct config config;
struct config {
        bool local;
        bool multicast;
        uint8_t count;
};

// Option table

static struct argp_option options[] = {
        { "local",     'l', NULL, 0,
          "Locally administered address (default)" },
        { "global",    'g', NULL, 0,
          "Globally unique (OUI enforced) address" },
        { "unicast",   'u', NULL, 0,
          "Unicast address (default)" },
        { "multicast", 'm', NULL, 0,
          "Multicast address" },
        { "eui48",     '6', NULL, 0,
          "EUI-48 address" },
        { "eui64",     '8', NULL, 0,
          "EUI-64 address" },
        { 0 }
};

// Option handler

static error_t
handle_option(int key, char* arg, struct argp_state *state)
{
        error_t err = 0;
        config *config = (struct config*) state->input;

        switch (key)
        {
        case 'l':
                config->local = TRUE;
                break;
        case 'g':
                config->local = FALSE;
                break;
        case 'u':
                config->multicast = FALSE;
                break;
        case 'm':
                config->multicast = TRUE;
                break;
        case '6':
                config->count = 6;
                break;
        case '8':
                config->count = 8;
                break;
        default:
                err = ARGP_ERR_UNKNOWN;
                break;
        }

        return err;
}

// Argument parser

static const struct argp argp = { options, handle_option, NULL,
                                  "genmac - generate MAC address",
                                  NULL
};

// Main

int
main(int argc, char** argv)
{
        int err = 0;
        uint8_t byte = 0;
        config config = { TRUE, FALSE, 6 };

        err = argp_parse(&argp, argc, argv, 0, NULL, &config);
        if (err)
                return err;

        srand((unsigned int)getpid());

        byte = (uint8_t)rand();

        byte <<= 1;
        if (config.local)
                byte |= 1;

        byte <<= 1;
        if (config.multicast)
                byte |= 1;

        printf("%02x", byte);
        for (int i = 1; i < config.count; i++)
                printf(":%02x", (uint8_t)rand());
        printf("\n");

        return 0;
}
