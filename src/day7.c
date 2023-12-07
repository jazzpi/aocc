#include "day7.h"

#include <assert.h>
#include <stdio.h>
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

typedef enum {
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  J,
  QUEEN,
  KING,
  ACE
} card_t;

typedef struct {
  card_t cards[5];
  long bid;
  hand_type type;
} hand;

typedef enum { JACK, JOKER } j_mode_t;
static j_mode_t j_mode;

hand_type get_type_jack(const int* counts) {
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

hand_type get_type_joker(const int* counts) {
  int pairs = 0;
  int threes = 0;
  int fours = 0;
  for (size_t i = 0; i < 13; i++) {
    if (i == J) {
      continue;
    }
    switch (counts[i]) {
    case 2:
      pairs++;
      break;
    case 3:
      threes++;
      break;
    case 4:
      fours++;
      break;
    case 5:
      return FIVE_OF_A_KIND;
    }
  }

  if (threes == 1 && pairs == 1) {
    return FULL_HOUSE;
  } else if (fours == 1) {
    if (counts[J] == 1) {
      return FIVE_OF_A_KIND;
    } else {
      return FOUR_OF_A_KIND;
    }
  } else if (threes == 1) {
    switch (counts[J]) {
    case 0:
      return THREE_OF_A_KIND;
    case 1:
      return FOUR_OF_A_KIND;
    case 2:
      return FIVE_OF_A_KIND;
    }
  } else if (pairs == 2) {
    if (counts[J] == 0) {
      return TWO_PAIR;
    } else {
      return FULL_HOUSE;
    }
  } else if (pairs == 1) {
    switch (counts[J]) {
    case 0:
      return ONE_PAIR;
    case 1:
      return THREE_OF_A_KIND;
    case 2:
      return FOUR_OF_A_KIND;
    case 3:
      return FIVE_OF_A_KIND;
    }
  } else {
    switch (counts[J]) {
    case 0:
      return HIGH_CARD;
    case 1:
      return ONE_PAIR;
    case 2:
      return THREE_OF_A_KIND;
    case 3:
      return FOUR_OF_A_KIND;
    case 4:
    case 5:
      return FIVE_OF_A_KIND;
    }
  }
  assert(0);
}

hand_type get_type(const card_t* cards) {
  int counts[13] = {0};
  for (size_t i = 0; i < 5; i++) {
    counts[cards[i]]++;
  }

  if (j_mode == JACK) {
    return get_type_jack(counts);
  } else {
    return get_type_joker(counts);
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
      int ac = a->cards[i];
      int bc = b->cards[i];
      if (j_mode == JOKER) {
        ac = (ac == J) ? -1 : ac;
        bc = (bc == J) ? -1 : bc;
      }
      return ac - bc;
    }
  }
  // If we have two hands with the same cards, they must also have the same bid
  // -- otherwise we can't rank them.
  assert(a->bid == b->bid);
  return 0;
}

const char* hand_str(const hand* hand) {
  static char buf[1024];
  char* p = buf;
  for (size_t i = 0; i < 5; i++) {
    card_t card = hand->cards[i];
    if (card >= TWO && card <= NINE) {
      *p++ = '2' + card;
    } else if (card == 8) {
      *p++ = 'T';
    } else if (card == 9) {
      *p++ = 'J';
    } else if (card == 10) {
      *p++ = 'Q';
    } else if (card == 11) {
      *p++ = 'K';
    } else if (card == 12) {
      *p++ = 'A';
    } else {
      assert(0);
    }
  }
  const char* type_str;
  switch (hand->type) {
  case HIGH_CARD:
    type_str = "High Card";
    break;
  case ONE_PAIR:
    type_str = "One Pair";
    break;
  case TWO_PAIR:
    type_str = "Two Pair";
    break;
  case THREE_OF_A_KIND:
    type_str = "Three of a Kind";
    break;
  case FULL_HOUSE:
    type_str = "Full House";
    break;
  case FOUR_OF_A_KIND:
    type_str = "Four of a Kind";
    break;
  case FIVE_OF_A_KIND:
    type_str = "Five of a Kind";
    break;
  default:
    assert(0);
  }
  sprintf(p, " (%s)", type_str);
  return buf;
}

int calc(const char** lines, size_t nlines) {
  hand* hands = parse_hands(lines, nlines);
  qsort(hands, nlines, sizeof(*hands), cmp_hands);

  long winnings = 0;
  for (size_t i = 0; i < nlines; i++) {
    printf("[%zu] %s\n", i, hand_str(&hands[i]));
    winnings += hands[i].bid * (i + 1);
  }

  free(hands);

  return winnings;
}

int day7_part1(const char** lines, size_t nlines) {
  j_mode = JACK;
  return calc(lines, nlines);
}

int day7_part2(const char** lines, size_t nlines) {
  j_mode = JOKER;
  return calc(lines, nlines);
}
