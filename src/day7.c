#include "day7.h"

#include <assert.h>
#include <stdlib.h>

typedef enum {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND,
} hand_type;

typedef struct {
  int cards[5];
  long bid;
  hand_type type;
} hand;

hand_type get_type(const int* cards) {
  int counts[13] = {0};
  for (size_t i = 0; i < 5; i++) {
    counts[cards[i]]++;
  }
  int pairs = 0;
  int threes = 0;
  for (size_t i = 0; i < 13; i++) {
    switch (counts[i]) {
    case 2:
      pairs++;
      break;
    case 3:
      threes++;
      break;
    case 4:
      return FOUR_OF_A_KIND;
    case 5:
      return FIVE_OF_A_KIND;
    }
  }

  if (threes == 1 && pairs == 1) {
    return FULL_HOUSE;
  } else if (threes == 1) {
    return THREE_OF_A_KIND;
  } else if (pairs == 2) {
    return TWO_PAIR;
  } else if (pairs == 1) {
    return ONE_PAIR;
  } else {
    return HIGH_CARD;
  }
}

hand* parse_hands(const char** lines, size_t nlines) {
  hand* hands = malloc(sizeof(*hands) * nlines);
  for (size_t i = 0; i < nlines; i++) {
    const char* line = lines[i];
    assert(line[5] == ' ');
    hand* hand = &hands[i];

    for (size_t j = 0; j < 5; j++) {
      char c = line[j];
      if (c >= '2' && c <= '9') {
        hand->cards[j] = c - '2';
      } else if (c == 'T') {
        hand->cards[j] = 8;
      } else if (c == 'J') {
        hand->cards[j] = 9;
      } else if (c == 'Q') {
        hand->cards[j] = 10;
      } else if (c == 'K') {
        hand->cards[j] = 11;
      } else if (c == 'A') {
        hand->cards[j] = 12;
      } else {
        assert(0);
      }
    }

    hand->bid = atol(line + 6);
    hand->type = get_type(hand->cards);
  }
  return hands;
}

int cmp_hands(const void* avoid, const void* bvoid) {
  const hand* a = avoid;
  const hand* b = bvoid;
  if (a->type != b->type) {
    return a->type - b->type;
  }
  for (size_t i = 0; i < 5; i++) {
    if (a->cards[i] != b->cards[i]) {
      return a->cards[i] - b->cards[i];
    }
  }
  // If we have two hands with the same cards, they must also have the same bid
  // -- otherwise we can't rank them.
  assert(a->bid == b->bid);
  return 0;
}

int day7_part1(const char** lines, size_t nlines) {
  hand* hands = parse_hands(lines, nlines);
  qsort(hands, nlines, sizeof(*hands), cmp_hands);

  long winnings = 0;
  for (size_t i = 0; i < nlines; i++) {
    winnings += hands[i].bid * (i + 1);
  }

  free(hands);

  return winnings;
}
