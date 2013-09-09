#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

int moveCount = 0;
char **moves;
int k, n;

static int getLine (char *prmpt, char *buff, size_t sz) {
  int ch, extra;
  
  // Get line with buffer overrun protection.
  if (prmpt != NULL) {
    printf ("%s", prmpt);
    fflush (stdout);
  }
  if (fgets (buff, sz, stdin) == NULL)
    return NO_INPUT;

  // If it was too long, there'll be no newline. In that case, we
  // flush to end of line so that excuss doesn't affect the next call.
  if(buff[strlen(buff)-1] != '\n') {
    extra = 0;
    while ((ch=getchar() != '\n') && (ch != EOF))
      extra = 1;
    return (extra == 1) ? TOO_LONG : OK;
  }

  // Otherwise remove newline and give string back to caller.
  buff[strlen(buff)-1] = '\0';
  return OK;
}

int getIntFromString(char * str, int index){
  int ret = 0;
  ret = str[index] - '0';
  return ret;
}

static void getNandK (){
  char *buff = (char*) malloc(100 * sizeof(char));

  getLine("Enter N K>", buff, sizeof(buff));
  n = getIntFromString(buff, 0);
  k = getIntFromString(buff, 2);

  free(buff);
}

static void getState (char* name, int* state, int length) {
  int i = 0;
  int temp = 0;
  
  char *buffer = (char*) malloc( (length * 2) * sizeof(char) );
  char prmt[30];
  
  sprintf(prmt, "Enter state %s>", name);
  int rc = getLine(prmt, buffer, (sizeof(char) * (length * 2)));

  if(rc == TOO_LONG)
    printf("input was too long\n");

  for(i; i < length; i ++) {
    state[i] = getIntFromString(buffer, i*2);
  }
  free(buffer);
}

int addMove(int from, int to){
  moveCount ++;
  moves = (char**) realloc(moves, moveCount * sizeof(char *));
  moves[moveCount-1] = (char *) malloc(5 * sizeof(char *));
  sprintf(moves[moveCount-1], "%d %d", from, to);
}

int moveDisk(int disk, int destination, int*state) {
  printf("moving disk %d\n", disk);
  int i = 0, j = 0;
  int calculatedDestination = 0;

  for(j = 0; j < disk; j ++){

  // check if the disk your trying to move is on top
    printf("disk: %d isn't on top shifting\n", disk);
    for(i = disk-1; i >= 0; i --) {
      if(state[disk] == state[i]){
	calculatedDestination = findDestination(disk, destination, state, i);
	moveDisk(i, calculatedDestination, state);
      }
    }
  // ensure that there are no smaller disks where you want to move to
  for(i = disk-1; i >= 0; i --) {
    if(state[i] == destination) {
      printf("disk %d is in the road of disk %d, shifting\n", i, disk);
      calculatedDestination = findDestination(disk, destination, state, i);
      moveDisk(i, calculatedDestination, state);
    }
  }
  }

  addMove(state[disk], destination);
  state[disk] = destination;
  printf("moved %d -> %d\n", disk+1, destination);
  printf("new state:\n");
  for(i = 0; i < n; i++){
    printf("%d ", state[i]);
  }
  printf("\n");
}

int isTop(int disk, int * state) {
  int i;
  for(i = disk-1; i >= 0; i --){
    if(state[disk] == state[i]){
      return 0;
    }
  }
  return 1;
}

int findDestination(int parentId, int parentDestination, int *state, int disk) {
  
  int l = 1;
  while(l <= k){
    if(l != parentDestination && diskOnPeg(l, state) == -1 && l != state[disk]) {
    //first check for empty peg
    return l;
    }
    l++;
    }

  // if not empty peg ensure that disk on peg is bigger
  for(l = 1; l <= k; l++) {
    if(l != parentDestination && l != state[parentId] && l != state[disk])
      return l;
  }
  return -1;
}

int diskOnPeg(int peg, int *state) {
  int i = n-1;
  for(i; 0 <= i; i--)
    if(state[i] == peg)
      return i;
  return -1;
}

int main() {
  int i = 0, j=0, m = 0;
  moveCount = 0;

  getNandK();

  int *currentState = malloc( sizeof(int) * n );
  getState("inital", currentState,  n);
  int *finalState = malloc( sizeof(int) * n );
  getState("final", finalState,  n);

  // loop through each disk starting at the largest i.e n - 1
  for(i = (n-1); i >= 0; i--) {
    if(currentState[i] != finalState[i]) {
      moveDisk(i, finalState[i], currentState);
    }
  }

  printf("%d\n", moveCount);
  for(i = 0; i < moveCount; i++){
    printf("%s\n", moves[i]);
  }

  return 0;
}
