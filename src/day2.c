#include "util.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

void part1(const char** lines, size_t nlines) {
  regex_t start_re;
  if (regcomp(&start_re, "Game ([0-9]+): ", REG_EXTENDED) != 0) {
    fprintf(stderr, "Failed to compile regex\n");
    exit(1);
  }
  regex_t draw_re;
  if (regcomp(&draw_re, "([0-9]+) (red|green|blue)", REG_EXTENDED) != 0) {
    fprintf(stderr, "Failed to compile regex\n");
    exit(1);
  }

  int sum_ids = 0;
  regmatch_t start_matches[2];
  for (size_t l = 0; l < nlines; l++) {
    if (regexec(&start_re, lines[l], 2, start_matches, 0) == REG_NOMATCH) {
      fprintf(stderr, "Couldn't find game ID in `%s'\n", lines[l]);
      exit(1);
    }
    char* id_str = strndup(lines[l] + start_matches[1].rm_so,
                           start_matches[1].rm_eo - start_matches[1].rm_so);
    int id = atoi(id_str);
    free(id_str);

    int possible = 1;
    char* draws_str = strdup(lines[l] + start_matches[0].rm_eo);
    char* draw = strtok(draws_str, ";");
    while (draw != NULL) {
      size_t nmatches;
      regmatch_t* matches =
          re_match_all_subgroups(&draw_re, draw, 3, &nmatches, 0);
      for (size_t i = 0; i < nmatches; i++) {
        char* num_str = re_match_dup(draw, &matches[i * 3 + 1]);
        char* color = re_match_dup(draw, &matches[i * 3 + 2]);

        int num = atoi(num_str);
        if (strcmp(color, "red") == 0 && num > MAX_RED) {
          possible = 0;
        } else if (strcmp(color, "green") == 0 && num > MAX_GREEN) {
          possible = 0;
        } else if (strcmp(color, "blue") == 0 && num > MAX_BLUE) {
          possible = 0;
        }

        free(num_str);
        free(color);
      }
      free(matches);

      if (!possible) {
        break;
      }
      draw = strtok(NULL, ";");
    }
    free(draws_str);

    if (possible) {
      sum_ids += id;
    }
  }

  printf("Sum of possible IDs: %d\n", sum_ids);

  regfree(&start_re);
  regfree(&draw_re);
}

int main(int argc, char** argv) {
  MAIN_BOILERPLATE

  if (part == 1) {
    part1(lines, nlines);
  } else {
    usage_abrt(argv[0]);
  }

  freelines(lines);
  return 0;
}
