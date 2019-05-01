/* 
 * set_intersect.c - test program for simple set module
 *
 * John Kotz - April 2019, Adapted from set_iterate
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include "memory.h"

static set_t *set_intersect(set_t *setA, set_t *setB);
static void set_intersect_helper(void *arg, const char *key, void *item);
static void itemprint(FILE *fp, const char *key, void *item);
static void itemdelete(void *item);
static int *intsave(int item);

typedef struct set_intersect_data {
  set_t *setA;
  set_t *setB;
  set_t *destination;
} set_intersect_data_t;

int main() 
{
  set_t *setA, *setB, *result;     // three sets

  setA = assertp(set_new(), "setA");
  setB = assertp(set_new(), "setB");
  
  printf("Building set A: ");
  set_insert(setA, "Brown", intsave(10));
  set_insert(setA, "Dartmouth", intsave(20));
  set_insert(setA, "Yale", intsave(15));
  set_insert(setA, "Harvard", intsave(8));
  set_insert(setA, "Princeton", intsave(5));
  set_insert(setA, "Columbia", intsave(1));
  set_print(setA, stdout, itemprint);
  putchar('\n');
  
  printf("Building set B: ");
  set_insert(setB, "Penn", intsave(7));
  set_insert(setB, "Dartmouth", intsave(11));
  set_insert(setB, "Cornell", intsave(9));
  set_insert(setB, "Stanford", intsave(6));
  set_insert(setB, "Princeton", intsave(3));
  set_insert(setB, "Duke", intsave(12));
  set_print(setB, stdout, itemprint);
  putchar('\n');
  
  printf("\nIntersect setA and setB into result: \n");
  result = set_intersect(setB, setA);
  set_print(result, stdout, itemprint);
  putchar('\n');

  printf("\nDelete the sets...\n");
  set_delete(setA, itemdelete);
  set_delete(setB, itemdelete);
  set_delete(result, itemdelete);
}

/* Merge the second set into the first set;
 * the second set is unchanged.
 */
static set_t *set_intersect(set_t *setA, set_t *setB)
{
  set_t *set = set_new();
  set_intersect_data_t *intersect_data = assertp(malloc(sizeof(set_intersect_data_t)), "intersect_data");
  
  intersect_data->setA = setA;
  intersect_data->setB = setB;
  intersect_data->destination = set;

  set_iterate(setA, intersect_data, set_intersect_helper);
  set_iterate(setB, intersect_data, set_intersect_helper);
  
  return set;
}

/* Help the set_intersect function to complete it's opperation
 */
static void set_intersect_helper(void *arg, const char *key, void *item)
{
  set_intersect_data_t *intersect_data = arg;
  set_t *setA = intersect_data->setA;
  set_t *setB = intersect_data->setB;
  set_t *destination = intersect_data->destination;

  int *itemA = set_find(setA, key);
  int *itemB = set_find(setB, key);
  if (itemA != NULL && itemB != NULL)
    set_insert(destination, key, intsave(*itemB));
}

/* print the given item to the given file.
 */
static void 
itemprint(FILE *fp, const char *key, void *item)
{
  int *countp = item;
  int count = *countp;

  if (key == NULL)
    fprintf(fp, "(null), ");
  else 
    fprintf(fp, "%s=%d, ", key, count);
}

static int *
intsave(int item)
{
  int *saved = assertp(malloc(sizeof(int)), "intsave");
  *saved = item;
  return saved;
}

static void 
itemdelete(void *item)
{
  if (item != NULL)
    free(item);
}