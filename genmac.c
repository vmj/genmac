/*
 * genmac - Generate a random MAC address.
 *
 * Copyright (C) 2016 Mikko Värri (mikko@varri.fi)
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <sys/types.h>          /* for pid_t */
#include <unistd.h>             /* for getpid() */
#include <stdint.h>             /* for uint8_t */
#include <stdio.h>              /* for printf() */
#include <stdlib.h>             /* for rand()/srand() */
#include <argp.h>               /* for argp stuff */
#include <error.h>              /* for error() */

/* See info libc -> Argp Global Variables */

/**
 * This is what '--version' shows (implemented by argp).
 */
const char *argp_program_version = "genmac 0.4"
        "\nCopyright (C) 2016 Mikko Värri"
        "\nLicense GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>"
        "\nThis is free software: you are free to change and redistribute it."
        "\nThere is NO WARRANTY, to the extent permitted by law.";

/**
 * This is what '--help' shows as bug reporting address (implemented by
 * argp).  We're abusing it to implement the GNU Coding Conventions for
 * '--help' option.
 */
const char *argp_program_bug_address = "mikko@varri.fi"
        "\ngenmac home page: <http://linuxbox.fi/~vmj/genmac/>"
        "\nGeneral help using GNU software: <http://www.gnu.org/gethelp/>";

/* Local types */

typedef uint8_t bool; /**< Boolean type. */
#define TRUE 1        /**< Boolean true. */
#define FALSE 0       /**< Boolean false. */

/**
 * Configuration.  Filled during option parsing.
 *
 * @local #TRUE if locally administered MAC address should be generated.
 *        #FALSE if globally unique MAC address should be generated.
 * @multicast #TRUE if multicast MAC address should be generated. #FALSE
 *            if unicast MAC address should be generated.
 * @count Number of bytes to generate.
 */
typedef struct config_t config_t;
struct config_t {
        bool local;
        bool multicast;
        uint8_t count;
        bool terminate;
        uint8_t terminating_byte;
};

/**
 * Vector of command line options.
 *
 * @name    Long option.
 * @key     Short option (int).
 * @arg     NULL or name of the option argument.
 * @flags   Option flags.
 * @doc     Option documentation or group header.
 * @group   Group identity.
 */
static struct argp_option options[] = {
        {"local", 'l', NULL, 0,
         "Locally administered address (default)"},
        {"global", 'g', NULL, 0,
         "Globally unique (OUI enforced) address"},
        {"unicast", 'u', NULL, 0,
         "Unicast address (default)"},
        {"multicast", 'm', NULL, 0,
         "Multicast address"},
        {"eui48", '6', NULL, 0,
         "EUI-48 address"},
        {"eui64", '8', NULL, 0,
         "EUI-64 address"},
        {"null", '0', NULL, 0,
         "Output a trailing null character"},
        {"none", 'n', NULL, 0,
         "Do not output the trailing newline"},
        {0}
};

/**
 * Command line option handler (callback).
 *
 * @key     Short option or special key.
 * @arg     Option argument or non-option argument or NULL.
 * @state   Argp parsing state.
 */
static error_t
handle_option(int key, char *arg, struct argp_state *state)
{
        error_t err = 0;
        config_t *config = (config_t *)state->input;

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
        case '0':
                config->terminating_byte = '\0';
                break;
        case 'n':
                config->terminate = FALSE;
                break;
        default:
                err = ARGP_ERR_UNKNOWN;
                break;
        }

        return err;
}

/**
 * Argp parser.
 *
 * @options       NULL or pointer to a vector of argp_option structures.
 * @parser        NULL or pointer to a option handing function.
 * @args_doc      NULL or newline separated non-option names.
 * @doc           NULL or vertical tab separated documentation.
 * @children      NULL or pointer to a vector of other parsers.
 * @help_filter   NULL or pointer to a help filter function.
 * @argp_domain   NULL or translation domain string.
 */
static const struct argp argp = { options, handle_option, NULL,
        "genmac - generate MAC address",
        NULL
};

/**
 * Entry point.
 *
 * @argc   Number of command line options and arguments.
 * @argv   Vector of command line options and arguments.
 */
int
main(int argc, char **argv)
{
        error_t err = 0;
        uint8_t byte = 0;
        config_t config = { TRUE, FALSE, 6, TRUE, '\n' };
        int printed = 0;
        char address[23];
        char *p = address;

        /* Parse the command line options and argument */
        err = argp_parse(&argp, argc, argv, 0, NULL, &config);
        if (err)
                error(EXIT_FAILURE, err, "argp_parse");

        /* Seed the random number generator */
        srand((unsigned int)getpid());

        /* Two least significant bits of the first byte have a special
           meaning */
        byte = (uint8_t) rand();

        byte <<= 1;
        if (config.local)
                byte |= 1;

        byte <<= 1;
        if (config.multicast)
                byte |= 1;

        printed = sprintf(p, "%02x", byte);
        if (printed < 0)
                error(EXIT_FAILURE, -printed, "printf");
        p += printed;

        /* Rest of the bytes can be fully random */
        while (--config.count)
        {
                printed = sprintf(p, ":%02x", (uint8_t) rand());
                if (printed < 0)
                        error(EXIT_FAILURE, -printed, "printf");
                p += printed;
        }

        printf("%s", address);
        if (config.terminate)
                printf("%c", config.terminating_byte);
        return 0;
}
