#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <math.h>

int main(int argc, char* argv[]) {
  // if(argc != 3){
  //   exit(INCORRRECT_NUM_ARGS);
  // }
  Maze maze;
  IntMaze intMaze;
  init_vector(&maze);
  init_intVector(&intMaze);
  maze = parse_getline(argv[1], maze);
  int r1H = 0, r1W = 0, r2H = 0, r2W = 0, e1H, e1W, e2H, e2W;
  intMaze = convertToInt(maze, intMaze, &r1H, &r1W, &r2H, &r2W, &e1H, &e1W, &e2H, &e2W);
  
  IntMaze bfs1, bfs2, bfs3, bfs4;
  init_intVector(&bfs1);
  init_intVector(&bfs2);
  init_intVector(&bfs3);
  init_intVector(&bfs4);
  
  int i,j;
  for(i = 0; i <= vector_intHeight(&intMaze); i++){
      for(j = 0; j <= vector_intWidth(&intMaze); j++){
          insert_element_intVector(&bfs1, -1, i, j);
          insert_element_intVector(&bfs2, -1, i, j);
          insert_element_intVector(&bfs3, -1, i, j);
          insert_element_intVector(&bfs4, -1, i, j);
      }
  }
  insert_element_intVector(&bfs1, 0, r1H, r1W);
  insert_element_intVector(&bfs2, 0, r2H, r2W);
  insert_element_intVector(&bfs3, 0, e1H, e1W);
  insert_element_intVector(&bfs4, 0, e2H, e2W);
  
  bfs1 = search1(intMaze, bfs1, r1H, r1W, 0); //2D Array of points possible to move to from robot 1 start.  Can't move through robot 2
  bfs2 = search2(intMaze, bfs2, r2H, r2W, 0); //2D Array of points possible to move to from robot 2 start.  Can't move through robot 1
  bfs3 = search3(intMaze, bfs3, e1H, e1W, 0); //2D Array of points possible to move to from exit 1.  Ignores robot 2
  bfs4 = search4(intMaze, bfs4, e2H, e2W, 0); //2D Array of points possible to move to from exit 2.  Ignores robot 1
  
  
  
  
  printf("Character Maze:\n");
  printMaze(maze);
  printf("\n\nInitial int maze:\n");
  printIntMaze(intMaze);
  printf("\n\nBFS1:\n");
  printIntMaze(bfs1);
  printf("\n\nBFS2:\n");
  printIntMaze(bfs2);
  printf("\n\nBFS3:\n");
  printIntMaze(bfs3);
  printf("\n\nBFS4:\n");
  printIntMaze(bfs4);
  printf("\n\n");
  
  printf("\n%d, %d, %d\n", vector_intHeight(&bfs3), vector_intWidth(&bfs3), access_element_intVector(&bfs3, 2, 7));
  
  if(access_element_intVector(&bfs1, e1H, e1W) > -1)
      printf("There is a direct path for robot1 to exit2 of length %d.\n", access_element_intVector(&bfs1, e1H, e1W));
  else
      printf("There is no direct path from robot1 to exit1.\n");
  
  if(access_element_intVector(&bfs2, e2H, e2W) > -1)
      printf("There is a direct path for robot2 to exit2 of length %d.\n", access_element_intVector(&bfs2, e2H, e2W));
  else
      printf("There is no direct path from robot2 to exit2.\n");
  
  if(access_element_intVector(&bfs3, r1H, r1W) > -1)
      printf("There is a direct path (ignoring robot2) for robot1 to exit1 of length %d.\n", access_element_intVector(&bfs3, r1H, r1W));
  else
      printf("There is no direct path no matter what from robot1 to exit1.\n");
  
  if(access_element_intVector(&bfs4, r2H, r2W) > -1)
      printf("There is a direct path (ignoring robot1) for robot2 to exit2 of length %d.\n", access_element_intVector(&bfs4, r2H, r2W));
  else
      printf("There is no direct path no matter what from robot2 to exit2.\n");
  
  
  free_vector(&maze);
  free_intVector(&intMaze);
  return (EXIT_SUCCESS);
}

Maze parse_getline(char* input, Maze maze) {
  int counter = 0;
  int width = 0;
  FILE* fp = fopen(input, "r");
	char* line = NULL;
	size_t nbytes = 0;
  int linelen=0;
	while ((linelen=getline(&line, &nbytes, fp)) != -1) {
		line[linelen-1] = '\0';
    if(width < linelen){
      width = linelen;
    }
    int j = 0;
    for(j = 0; j < linelen; j++){
          insert_element_vector(&maze, line[j], counter, j);
    }
    counter++;
	}
	free(line);
  return(maze);
}

IntMaze convertToInt(Maze maze, IntMaze intMaze, int *r1H, int *r1W, int *r2H, int *r2W, int *e1H, int *e1W, int *e2H, int *e2W){
  int i, j;
  for(i = 0; i <= vector_height(&maze); i++){
    for(j = 0; j <= vector_width(&maze); j++){
      char temp = access_element_vector(&maze, i, j);
      char space = ' ';
      char hash = '#';
      char rob1 = 'S';
      char rob2 = 'F';
      char end1 = 'E';
      char end2 = 'L';
      if(temp == space){
        insert_element_intVector(&intMaze, 0, i, j);
      }
      else if(temp == hash){
        insert_element_intVector(&intMaze, 1, i, j);
      }
      else if(temp == rob1){
        insert_element_intVector(&intMaze, 2, i, j);
      }
      else if(temp == rob2){
        insert_element_intVector(&intMaze, 3, i, j);
      }
      else if(temp == end1){
        insert_element_intVector(&intMaze, 4, i, j);
        *e1H = i;
        *e1W = j;
      }
      else if(temp == end2){
        insert_element_intVector(&intMaze, 5, i, j);
        *e2H = i;
        *e2W = j;
      }
    }
  }
  for(i = 0; i < vector_intHeight(&intMaze); i++){
    for(j = 0; j < vector_intWidth(&intMaze); j++){
        if(access_element_intVector(&intMaze, i, j) == 2){
            *r1H = i;
            *r1W = j;
            if(i != vector_intHeight(&intMaze)){
                if(access_element_intVector(&intMaze, i + 1, j) == 0){
                    insert_element_intVector(&intMaze, 6, i + 1, j);
                }
                if(j != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, i + 1, j + 1) == 0){
                        insert_element_intVector(&intMaze, 6, i + 1, j + 1);
                    }
                }
                if(j != 0){
                    if(access_element_intVector(&intMaze, i + 1, j - 1) == 0){
                        insert_element_intVector(&intMaze, 6, i + 1, j - 1);
                    }
                }
            }
            
            if(i != 0){
                if(access_element_intVector(&intMaze, i - 1, j) == 0){
                    insert_element_intVector(&intMaze, 6, i - 1, j);
                }
                if(j != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, i - 1, j + 1) == 0){
                        insert_element_intVector(&intMaze, 6, i - 1, j + 1);
                    }
                }
                if(j != 0){
                    if(access_element_intVector(&intMaze, i - 1, j - 1) == 0){
                        insert_element_intVector(&intMaze, 6, i - 1, j - 1);
                    }
                }
            }
            
            if(j != vector_intWidth(&intMaze)){
                if(access_element_intVector(&intMaze, i, j + 1) == 0){
                    insert_element_intVector(&intMaze, 6, i, j + 1);
                }
            }
            
            if(j != 0){
                if(access_element_intVector(&intMaze, i, j - 1) == 0){
                    insert_element_intVector(&intMaze, 6, i, j - 1);
                }
            }
        }
        
        if(access_element_intVector(&intMaze, i, j) == 3){
            *r2H = i;
            *r2W = j;
            if(i != vector_intHeight(&intMaze)){
                if(access_element_intVector(&intMaze, i + 1, j) == 0){
                    insert_element_intVector(&intMaze, 7, i + 1, j);
                }
                if(j != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, i + 1, j + 1) == 0){
                        insert_element_intVector(&intMaze, 7, i + 1, j + 1);
                    }
                }
                if(j != 0){
                    if(access_element_intVector(&intMaze, i + 1, j - 1) == 0){
                        insert_element_intVector(&intMaze, 7, i + 1, j - 1);
                    }
                }
            }
            
            if(i != 0){
                if(access_element_intVector(&intMaze, i - 1, j) == 0){
                    insert_element_intVector(&intMaze, 7, i - 1, j);
                }
                if(j != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, i - 1, j + 1) == 0){
                        insert_element_intVector(&intMaze, 7, i - 1, j + 1);
                    }
                }
                if(j != 0){
                    if(access_element_intVector(&intMaze, i - 1, j - 1) == 0){
                        insert_element_intVector(&intMaze, 7, i - 1, j - 1);
                    }
                }
            }
            
            if(j != vector_intWidth(&intMaze)){
                if(access_element_intVector(&intMaze, i, j + 1) == 0){
                    insert_element_intVector(&intMaze, 7, i, j + 1);
                }
            }
            
            if(j != 0){
                if(access_element_intVector(&intMaze, i, j - 1) == 0){
                    insert_element_intVector(&intMaze, 7, i, j - 1);
                }
            }
        }
    }
  }
  return(intMaze);
}

void printMaze(Maze maze){
  int i, j;
  int height = vector_height(&maze);
  int width = vector_width(&maze);
  for(i = 0; i < height + 1; i++){
    for(j = 0; j < width + 1; j++){
      printf("%c", access_element_vector(&maze, i, j));
    }
    printf("\n");
  }
}

void printIntMaze(IntMaze intMaze){
  long i, j, x, count = 0;
  int height = vector_intHeight(&intMaze);
  int width = vector_intWidth(&intMaze);
  for(i = 0; i <= height; i++){
    for(j = 0; j <= width; j++){
        x = access_element_intVector(&intMaze, i, j);
        printf("%d", x);
//        if(x == -1)
//            printf("%s", "xxx");
//        else if(x == 0)
//            printf("%s", "$$$");
//        else if(x/10 == 0)
//            printf("00%d", abs(x));
//        else if(x/10 >= 1 && x/10 <= 9)
//            printf("0%d", abs(x));
//        else if(x/10 >= 10 && x/10 <= 99)
//            printf("%d", abs(x));
//        else{
//            printf("%s", "FUK");
//            count++;
//        }
    }
    printf("\n");
  }
}

IntMaze search1(IntMaze intMaze, IntMaze search, int i, int j, int weight){
    Stack s;
    init_stack(&s);
    Pos p;
    push(&s, i, j, weight);
    while(!isEmpty(&s)){
        p = pop(&s);
            if(getHeight(p) != vector_intHeight(&intMaze)){ //check to make sure not at bottom of map
                if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 4 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 6){ //check 
                    if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) > getWeight(p) + 1){
                        if(getWeight(p) >= 0){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p));
                            push(&s, getHeight(p) + 1, getWidth(p), getWeight(p) + 1);
                        }
                    }
                }
                
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 4 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) > getWeight(p) + 1){
                            if(getWeight(p) >= 0){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) + 1);
                            push(&s, getHeight(p) + 1, getWidth(p) + 1, getWeight(p) + 1);
                            }
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 4 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) > getWeight(p) + 1){
                            if(getWeight(p) >= 0){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) - 1);
                            push(&s, getHeight(p) + 1, getWidth(p) - 1, getWeight(p) + 1);
                            }
                        }
                    }
                }
            }
            
            if(getHeight(p) != 0){ //current position is not along the top of the map
                if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 4 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 6){
                    if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) > getWeight(p) + 1){
                        if(getWeight(p) >= 0){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p));
                        push(&s, getHeight(p) - 1, getWidth(p), getWeight(p) + 1);
                        }
                    }
                }
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 4 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) > getWeight(p) + 1){
                            if(getWeight(p) >= 0){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) + 1);
                            push(&s, getHeight(p) - 1, getWidth(p) + 1, getWeight(p) + 1);
                            }
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 4 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) > getWeight(p) + 1){
                            if(getWeight(p) >= 0){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) - 1);
                            push(&s, getHeight(p) - 1, getWidth(p) - 1, getWeight(p) + 1);
                            }
                        }
                    }
                }
            }
            
            if(getWidth(p) != vector_intWidth(&intMaze)){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 4 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 6){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) + 1) > getWeight(p) + 1){
                        if(getWeight(p) >= 0){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) + 1);
                        push(&s, getHeight(p), getWidth(p) + 1, getWeight(p) + 1);}
                    }
                }
            }
            
            if(getWidth(p) != 0){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 4 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 6){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) - 1) > getWeight(p) + 1){
                        if(getWeight(p) >= 0){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) - 1);
                        push(&s, getHeight(p), getWidth(p) - 1, getWeight(p) + 1);}
                    }
                }
            }
    }
    return search;
}

IntMaze search2(IntMaze intMaze, IntMaze search, int i, int j, int weight){
    Stack s;
    init_stack(&s);
    Pos p;
    push(&s, i, j, weight);
    while(!isEmpty(&s)){
        p = pop(&s);
            if(getHeight(p) != vector_intHeight(&intMaze)){
                if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 5 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 7){
                    if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p));
                        push(&s, getHeight(p) + 1, getWidth(p), getWeight(p) + 1);
                    }
                }
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 5 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) + 1);
                            push(&s, getHeight(p) + 1, getWidth(p) + 1, getWeight(p) + 1);
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 5 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) - 1);
                            push(&s, getHeight(p) + 1, getWidth(p) - 1, getWeight(p) + 1);
                        }
                    }
                }
            }
            
            if(getHeight(p) != 0){
                if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 5 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 7){
                    if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p));
                        push(&s, getHeight(p) - 1, getWidth(p), getWeight(p) + 1);
                    }
                }
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 5 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) + 1);
                            search = search2(intMaze, search, getHeight(p) - 1, getWidth(p) + 1, getWeight(p) + 1);
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 5 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) - 1);
                            push(&s, getHeight(p) - 1, getWidth(p) - 1, getWeight(p) + 1);
                        }
                    }
                }
            }
            
            if(getWidth(p) != vector_intWidth(&intMaze)){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 5 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 7){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) + 1) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) + 1);
                        push(&s, getHeight(p), getWidth(p) + 1, getWeight(p) + 1);
                    }
                }
            }
            
            if(getWidth(p) != 0){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 5 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 7){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) - 1) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) - 1);
                        push(&s, getHeight(p), getWidth(p) - 1, getWeight(p) + 1);
                    }
                }
            }
    }
    return search;
}

IntMaze search3(IntMaze intMaze, IntMaze search, int i, int j, int weight){
    Stack s;
    init_stack(&s);
    Pos p;
    push(&s, i, j, weight);
    while(!isEmpty(&s)){
        p = pop(&s);
            if(getHeight(p) != vector_intHeight(&intMaze)){
                if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 2 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 6 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 3 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 7){
                    if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p));
                        push(&s, getHeight(p) + 1, getWidth(p), getWeight(p) + 1);
                    }
                }
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 2 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 6 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 3 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) + 1);
                            push(&s, getHeight(p) + 1, getWidth(p) + 1, getWeight(p) + 1);
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 2 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 6 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 3 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) - 1);
                            push(&s, getHeight(p) + 1, getWidth(p) - 1, getWeight(p) + 1);
                        }
                    }
                }
            }
            
            if(getHeight(p) != 0){
                if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 2 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 6 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 3 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 7){
                    if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p));
                        push(&s, getHeight(p) - 1, getWidth(p), getWeight(p) + 1);
                    }
                }
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 2 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 6 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 3 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) + 1);
                            push(&s, getHeight(p) - 1, getWidth(p) + 1, getWeight(p) + 1);
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 2 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 6 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 3 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 7){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) - 1);
                            push(&s, getHeight(p) - 1, getWidth(p) - 1, getWeight(p) + 1);
                        }
                    }
                }
            }
            
            if(getWidth(p) != vector_intWidth(&intMaze)){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 2 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 6 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 3 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 7){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) + 1) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) + 1);
                        push(&s, getHeight(p), getWidth(p) + 1, getWeight(p) + 1);
                    }
                }
            }
            
            if(getWidth(p) != 0){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 2 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 6 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 3 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 7){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) - 1) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) - 1);
                        push(&s, getHeight(p), getWidth(p) - 1, getWeight(p) + 1);
                    }
                }
            }
    }
            return search;
}

IntMaze search4(IntMaze intMaze, IntMaze search, int i, int j, int weight){
    Stack s;
    init_stack(&s);
    Pos p;
    push(&s, i, j, weight);
    while(!isEmpty(&s)){
        p = pop(&s);
            if(getHeight(p) != vector_intHeight(&intMaze)){
                if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 3 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 7 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 2 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p)) == 6){
                    if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p)) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p));
                        push(&s, getHeight(p) + 1, getWidth(p), getWeight(p) + 1);
                    }
                }
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 3 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 7 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 2 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) + 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) + 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) + 1);
                            push(&s, getHeight(p) + 1, getWidth(p) + 1, getWeight(p) + 1);
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 3 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 7 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 2 || access_element_intVector(&intMaze, getHeight(p) + 1, getWidth(p) - 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) + 1, getWidth(p) - 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) + 1, getWidth(p) - 1);
                            push(&s, getHeight(p) + 1, getWidth(p) - 1, getWeight(p) + 1);
                        }
                    }
                }
            }
            
            if(getHeight(p) != 0){
                if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 3 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 7 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 2 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p)) == 6){
                    if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p)) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p));
                        push(&s, getHeight(p) - 1, getWidth(p), getWeight(p) + 1);
                    }
                }
                if(getWidth(p) != vector_intWidth(&intMaze)){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 3 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 7 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 2 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) + 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) + 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) + 1);
                            push(&s, getHeight(p) - 1, getWidth(p) + 1, getWeight(p) + 1);
                        }
                    }
                }
                if(getWidth(p) != 0){
                    if(access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 3 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 7 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 2 || access_element_intVector(&intMaze, getHeight(p) - 1, getWidth(p) - 1) == 6){
                        if(access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p) - 1, getWidth(p) - 1) > getWeight(p) + 1){
                            insert_element_intVector(&search, getWeight(p) + 1, getHeight(p) - 1, getWidth(p) - 1);
                            push(&s, getHeight(p) - 1, getWidth(p) - 1, getWeight(p) + 1);
                        }
                    }
                }
            }
            
            if(getWidth(p) != vector_intWidth(&intMaze)){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 3 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 7 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 2 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) + 1) == 6){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) + 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) + 1) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) + 1);
                        push(&s, getHeight(p), getWidth(p) + 1, getWeight(p) + 1);
                    }
                }
            }
            
            if(getWidth(p) != 0){
                if(access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 0 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 3 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 7 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 2 || access_element_intVector(&intMaze, getHeight(p), getWidth(p) - 1) == 6){
                    if(access_element_intVector(&search, getHeight(p), getWidth(p) - 1) == -1 || access_element_intVector(&search, getHeight(p), getWidth(p) - 1) > getWeight(p) + 1){
                        insert_element_intVector(&search, getWeight(p) + 1, getHeight(p), getWidth(p) - 1);
                        push(&s, getHeight(p), getWidth(p) - 1, getWeight(p) + 1);
                    }
                }
            }
    }
            return search;
}