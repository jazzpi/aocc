#include "re.h"
#include "util.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

long day2_part1(const char** lines, size_t nlines) {
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
      re_match_t* matches = re_match_all(&draw_re, draw, &nmatches, 0);
      for (size_t i = 0; i < nmatches; i++) {
        int num = atoi(matches[i].groups[1].str);
        char* color = matches[i].groups[2].str;

        if (strcmp(color, "red") == 0 && num > MAX_RED) {
          possible = 0;
        } else if (strcmp(color, "green") == 0 && num > MAX_GREEN) {
          possible = 0;
        } else if (strcmp(color, "blue") == 0 && num > MAX_BLUE) {
          possible = 0;
        }
      }
      re_free_matches(matches, nmatches);

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

  regfree(&start_re);
  regfree(&draw_re);

  return sum_ids;
}

long day2_part2(const char** lines, size_t nlines) {
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

  int power_sum = 0;
  regmatch_t start_matches[2];
  for (size_t l = 0; l < nlines; l++) {
    if (regexec(&start_re, lines[l], 2, start_matches, 0) == REG_NOMATCH) {
      fprintf(stderr, "Couldn't find game ID in `%s'\n", lines[l]);
      exit(1);
    }

    char* draws_str = strdup(lines[l] + start_matches[0].rm_eo);
    char* draw = strtok(draws_str, ";");
    int min_red = 0, min_green = 0, min_blue = 0;
    while (draw != NULL) {
      size_t nmatches;
      re_match_t* matches = re_match_all(&draw_re, draw, &nmatches, 0);
      for (size_t i = 0; i < nmatches; i++) {
        int num = atoi(matches[i].groups[1].str);
        char* color = matches[i].groups[2].str;

        if (strcmp(color, "red") == 0) {
          min_red = MAX(min_red, num);
        } else if (strcmp(color, "green") == 0) {
          min_green = MAX(min_green, num);
        } else if (strcmp(color, "blue") == 0) {
          min_blue = MAX(min_blue, num);
        }
      }
      re_free_matches(matches, nmatches);

      draw = strtok(NULL, ";");
    }
    free(draws_str);
    power_sum += min_red * min_green * min_blue;
  }

  regfree(&start_re);
  regfree(&draw_re);

  return power_sum;
}
