#include <stdio.h>
#include <stdlib.h>
#include "Game.h"

#define ROW_VERTICES 13
#define COLUMN_VERTICES 8
//This is arcs connecting all adjacent vertices in the 2D array,
//which means this number includes invalid ones.
#define NUM_ARCS 187
#define NUM_DISCIPLINES 6
#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3
#define VALUE_CAMPUS 10
#define VALUE_ARC 2
#define VALUE_GO8 20
#define VALUE_IP 10
#define VALUE_MOSTARC 10
#define VALUE_MOSTPUB 10
#define INITIAL_STUDENTS {0,5,5,3,3,1}

typedef struct _player {
    int playerID;
    int KPI;
    int numCampuses;
    int numARCs;
    int numGO8s;
    int numIPs;
    int numPublications;
    int numStudents[NUM_DISCIPLINES];

} player;
typedef struct _vertex {
    int row;
    int column;
    int facing; //this is for routing
    int content;
} vertex;

typedef struct _arc {
    vertex v1;
    vertex v2;
    int content;
} arc;

typedef struct _game {
    int currentTurn;
    int whosTurn;
    int mostARCs;
    int mostPublications;
    int diceValues[NUM_REGIONS];
    int disciplines[NUM_REGIONS];
    arc arcs[NUM_ARCS];
    vertex vertices[ROW_VERTICES][COLUMN_VERTICES];
    player players[NUM_UNIS];
} game;

/**************Helper functions*******************/
static vertex findVertex(path pathToVertex);
static vertex walkPath(int row, int column, int facing, char nextMove);
static int getVertexOwner(Game g, int row, int column);
static player initializePlayer(int playerID);
static int findARCIndex(Game g, vertex v1, vertex v2);
/**************updater functions*******************/
static void buildCampus(Game g, int row, int column, int playerID);
static void buildGO8(Game g, int row, int column, int playerID);
static void buildARC(Game g, int arcIndex, int playerID);
static void buildPublication(Game g, int playerID);
static void buildIP(Game g, int playerID);
static void generateStudent(Game g, int regionID, int desciplin);
/**************validator functions*******************/
static int isVertexOnMap(vertex v);
static int isPathValid(path p);

/* ****Setter functions**** */

// make a new game, given the disciplines produced by each
// region, and the value on the dice discs in each region.
// note: each array must be NUM_REGIONS long
// eg if you are using my sample game struct above this function
// would need to set the field currentTurn to -1.  (because the turn
// number is -1 at the start of the game)
// the ordering of the regions is column by column left to right,
// going from the top of each column to the bottom before moving
// to the next column to the right.
//
// so to create the default game as shown on the badly drawn map:
//
Game newGame(int discipline[], int dice[]) {

    Game g = malloc(sizeof(game));
    g->currentTurn = -1;
    g->whosTurn = NO_ONE;
    g->mostARCs = NO_ONE;
    g->mostPublications = NO_ONE;
    int i = 0;
    while (i < NUM_REGIONS) {
        g->diceValues[i] = dice[i];
        g->disciplines[i] = discipline[i];
        i++;
    }
    //initializing vertices
    int r = 0;
    int c = 0;
    while (r < ROW_VERTICES) {
        while (c < COLUMN_VERTICES) {
            g->vertices[r][c].row = r;
            g->vertices[r][c].column = c;
            g->vertices[r][c].facing = DOWN;
            g->vertices[r][c].content = VACANT_VERTEX;

            c++;
        }
        r++;
    }
    //start from [0][0]and [1][0] of the 2D array,
    //NOT the actual map.
    //initializing vertical arcs
    i = 0;
    int r1 = 0;
    int c1 = 0;
    int r2 = 1;
    int c2 = 0;
    while (c1 < COLUMN_VERTICES) {
        r1 = 0;
        r2 = 1;
        while (r2 < ROW_VERTICES) {
            g->arcs[i].v1.row = r1;
            g->arcs[i].v1.column = c1;
            g->arcs[i].v2.row = r2;
            g->arcs[i].v2.column = c2;
            g->arcs[i].content = VACANT_ARC;
            r1++;
            r2++;
            i++;
        }
        c1++;
        c2++;
    }
    //start from [0][0]and [0][1] of the 2D array,
    //NOT the actual map.
    //initialize horizontal arcs
    r1 = 0;
    c1 = 0;
    r2 = 0;
    c2 = 1;
    while (r1 < ROW_VERTICES) {
        c1 = 0;
        c2 = 1;
        while (c2 < COLUMN_VERTICES) {
            g->arcs[i].v1.row = r1;
            g->arcs[i].v1.column = c1;
            g->arcs[i].v2.row = r2;
            g->arcs[i].v2.column = c2;
            g->arcs[i].content = VACANT_ARC;
            c1++;
            c2++;
            i++;
        }
        r1++;
        r2++;
    }
    g->players[0] = initializePlayer(UNI_A);
    g->players[1] = initializePlayer(UNI_B);
    g->players[2] = initializePlayer(UNI_C);
    buildCampus(g, 1, 3, UNI_A);
    buildCampus(g, 11, 4, UNI_A);
    buildCampus(g, 4, 1, UNI_B);
    buildCampus(g, 8, 6, UNI_B);
    buildCampus(g, 9, 1, UNI_C);
    buildCampus(g, 3, 6, UNI_C);

    return g;
}

// free all the memory malloced for the game
void disposeGame(Game g) {
    free(g);
}
// make the specified action for the current player and update the
// game state accordingly.
// The function may assume that the action requested is legal.
// START_SPINOFF is not a legal action here
/*
 *#define PASS 0
 *#define BUILD_CAMPUS 1
 *#define BUILD_GO8 2
 *#define OBTAIN_ARC 3
 *#define START_SPINOFF 4
 *#define OBTAIN_PUBLICATION 5
 *#define OBTAIN_IP_PATENT 6
 *#define RETRAIN_STUDENTS 7
 *typedef struct _action {
 int actionCode;  // see #defines above
 path destination; // if the action operates on a vertex or ARC this
 // specifies *which* vertex or path.  unused
 // otherwise
 int disciplineFrom;  // used for the retrain students action
 int disciplineTo;    // used for the retrain students action
 } action;
 */
void makeAction(Game g, action a) {
    if (a.actionCode == PASS) {

    } else if (a.actionCode == BUILD_CAMPUS) {
        int row = findVertex(a.destination).row;
        int column = findVertex(a.destination).column;
        buildCampus(g, row, column, g->whosTurn);
    } else if (a.actionCode == BUILD_GO8) {
        int row = findVertex(a.destination).row;
        int column = findVertex(a.destination).column;
        buildGO8(g, row, column, g->whosTurn);
    } else if (a.actionCode == OBTAIN_ARC) {
        vertex v1 = findVertex(a.destination);
        vertex v2 = walkPath(v1.row, v1.column, v1.facing, 'B');
        buildARC(g, findARCIndex(g, v1, v2), g->whosTurn);
    } else if (a.actionCode == OBTAIN_PUBLICATION) {
        buildPublication(g, g->whosTurn);
    } else if (a.actionCode == OBTAIN_IP_PATENT) {
        buildIP(g, g->whosTurn);
    } else if (a.actionCode == RETRAIN_STUDENTS) {
        int rate = getExchangeRate(g, g->whosTurn, a.disciplineFrom,
                a.disciplineTo);
        g->players[g->whosTurn - 1].numStudents[a.disciplineFrom] -= rate;
        g->players[g->whosTurn - 1].numStudents[a.disciplineTo]++;
    }
}

// advance the game to the next turn,
// assuming that the dice has just been rolled and produced diceScore
// the game starts in turn -1 (we call this state "Terra Nullis") and
// moves to turn 0 as soon as the first dice is thrown.
void throwDice(Game g, int diceScore) {
    g->currentTurn++;
    if (g->whosTurn == UNI_C) {
        g->whosTurn = UNI_A;
    } else {
        g->whosTurn++;
    }
    int i = 0;
    while (i < NUM_REGIONS) {
        if (g->diceValues[i] == diceScore) {
            generateStudent(g, i, g->disciplines[i]);
        }
        i++;
    }
    if (diceScore == 7) {
        i = 0;
        while (i < NUM_UNIS) {
            g->players[i].numStudents[STUDENT_THD] =
                    g->players[i].numStudents[STUDENT_THD]
                            + g->players[i].numStudents[STUDENT_MTV]
                            + g->players[i].numStudents[STUDENT_MMONEY];
            g->players[i].numStudents[STUDENT_MTV] = 0;
            g->players[i].numStudents[STUDENT_MMONEY] = 0;
            i++;
        }
    }
}

/* ****Getter functions**** */

// what type of students are produced by the specified region?
// regionID is the index of the region in the newGame arrays (above)
// see discipline codes above
int getDiscipline(Game g, int regionID) {
    return g->disciplines[regionID];
}

// what dice value produces students in the specified region?
// 2..12
int getDiceValue(Game g, int regionID) {
    return g->diceValues[regionID];
}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started.
int getMostARCs(Game g) {
    return g->mostARCs;
}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first publication is made.
int getMostPublications(Game g) {
    return g->mostPublications;
}
// return the current turn number of the game -1,0,1, ..
int getTurnNumber(Game g) {
    return g->currentTurn;
}

// return the player id of the player whose turn it is
// the result of this function is NO_ONE during Terra Nullis
int getWhoseTurn(Game g) {
    return g->whosTurn;
}

// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
int getCampus(Game g, path pathToVertex) {
    int row = findVertex(pathToVertex).row;
    int column = findVertex(pathToVertex).column;
    return g->vertices[row][column].content;
}

// the contents of the given edge (i.e. ARC code or vacant ARC)
int getARC(Game g, path pathToEdge) {
    int c = VACANT_ARC;
    vertex v1 = findVertex(pathToEdge);
    vertex v2 = walkPath(v1.row, v1.column, v1.facing, 'B');
    c = g->arcs[findARCIndex(g, v1, v2)].content;
    return c;
}

// --- get data about a specified player ---

// return the number of KPI points the specified player currently has
int getKPIpoints(Game g, int player) {
    return g->players[player - 1].KPI;
}

// return the number of ARC grants the specified player currently has
int getARCs(Game g, int player) {
    return g->players[player - 1].numARCs;
}
// return the number of GO8 campuses the specified player currently has
int getGO8s(Game g, int player) {
    return g->players[player - 1].numGO8s;
}

// return the number of normal Campuses the specified player currently has
int getCampuses(Game g, int player) {
    return g->players[player - 1].numCampuses;
}

// return the number of IP Patents the specified player currently has
int getIPs(Game g, int player) {
    return g->players[player - 1].numIPs;
}

// return the number of Publications the specified player currently has
int getPublications(Game g, int player) {
    return g->players[player - 1].numPublications;
}

// return the number of students of the specified discipline type
// the specified player currently has
int getStudents(Game g, int player, int discipline) {
    return g->players[player - 1].numStudents[discipline];
}

// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate(Game g, int player, int disciplineFrom, int disciplineTo) {
    int rate = 3; //Default rate
//decide the discipline.
    if (disciplineFrom == STUDENT_BPS) {
        //these two are campuses with BPS training
        if (getVertexOwner(g, 9, 2) == player
                || getVertexOwner(g, 10, 2) == player) {
            rate--;    //one less with training center
        }
    } else if (disciplineFrom == STUDENT_BQN) {
        //these two are campuses with BQN training
        if (getVertexOwner(g, 6, 6) == player
                || getVertexOwner(g, 7, 6) == player) {
            rate--;    //one less with training center
        }
    } else if (disciplineFrom == STUDENT_MJ) {
        //these two are campuses with MJ training
        if (getVertexOwner(g, 9, 5) == player
                || getVertexOwner(g, 10, 5) == player) {
            rate--;    //one less with training center
        }

    } else if (disciplineFrom == STUDENT_MTV) {
        //these two are campuses with MTV training
        if (getVertexOwner(g, 2, 2) == player
                || getVertexOwner(g, 2, 3) == player) {
            rate--;    //one less with training center
        }

    } else if (disciplineFrom == STUDENT_MMONEY) {
        //these two are campuses with MMONEY training
        if (getVertexOwner(g, 2, 4) == player
                || getVertexOwner(g, 2, 5) == player) {
            rate--;    //one less with training center
        }
    }
    return rate;
}

// returns TRUE if it is legal for the current
// player to make the specified action, FALSE otherwise.
//
// "legal" means everything is legal:
//   * that the action code is a valid action code which is legal to
//     be made at this time
//   * that any path is well formed and legal ie consisting only of
//     the legal direction characters and of a legal length,
//     and which does not leave the island into the sea at any stage.
//   * that disciplines mentioned in any retraining actions are valid
//     discipline numbers, and that the university has sufficient
//     students of the correct type to perform the retraining
//
// eg when placing a campus consider such things as:
//   * is the path a well formed legal path
//   * does it lead to a vacent vertex?
//   * under the rules of the game are they allowed to place a
//     campus at that vertex?  (eg is it adjacent to one of their ARCs?)
//   * does the player have the 4 specific students required to pay for
//     that campus?
// It is not legal to make any action during Terra Nullis ie
// before the game has started.
// It is not legal for a player to make the moves OBTAIN_PUBLICATION
// or OBTAIN_IP_PATENT (they can make the move START_SPINOFF)
// you can assume that any pths passed in are NULL terminated strings.
int isLegalAction(Game g, action a) {
    int isLegal = TRUE;
    if (a.actionCode == PASS) {

    } else if (a.actionCode == BUILD_CAMPUS) {
        //check if path is valid
        vertex v;
        int i = 0;
        while (a.destination[i]) {
            if (!(a.destination[i] == 'L' || a.destination[i] == 'R'
                    || a.destination[i] == 'B')) {
                printf("Bad path\n");
                return FALSE;
            }
            i++;
        }
        isLegal = isPathValid(a.destination);
        //check resources
        if (g->players[g->whosTurn - 1].numStudents[STUDENT_BPS] < 1
                || g->players[g->whosTurn - 1].numStudents[STUDENT_BQN] < 1
                || g->players[g->whosTurn - 1].numStudents[STUDENT_MJ] < 1
                || g->players[g->whosTurn - 1].numStudents[STUDENT_MTV] < 1) {
            printf("Not enough resource\n");
            isLegal = FALSE;
        }
        //check if the vertex and adjacent vertices are vacant
        v = findVertex(a.destination);
        if (g->vertices[v.row][v.column].content
                || g->vertices[walkPath(v.row, v.column, v.facing, 'L').row][walkPath(v.row, v.column, v.facing, 'L').column].content
                || g->vertices[walkPath(v.row, v.column, v.facing, 'R').row][walkPath(v.row, v.column, v.facing, 'R').column].content
                || g->vertices[walkPath(v.row, v.column, v.facing, 'B').row][walkPath(v.row, v.column, v.facing, 'B').column].content) {
            printf("Next to campus\n");
            isLegal = FALSE;
        }
        //check if the vertex is connected with an ARC of the same player
        if (g->arcs[findARCIndex(g, v, walkPath(v.row, v.column, v.facing, 'L'))].content
                != g->whosTurn
                && g->arcs[findARCIndex(g, v,
                        walkPath(v.row, v.column, v.facing, 'R'))].content
                        != g->whosTurn
                && g->arcs[findARCIndex(g, v,
                        walkPath(v.row, v.column, v.facing, 'B'))].content
                        != g->whosTurn) {
            printf("Not connected to ARC\n");
            isLegal = FALSE;
        }

    } else if (a.actionCode == BUILD_GO8) {
        int i = 0;
        //check if path is valid
        while (a.destination[i]) {
            if (!(a.destination[i] == 'L' || a.destination[i] == 'R'
                    || a.destination[i] == 'B')) {
                printf("Bad path\n");
                return FALSE;
            }
            i++;
        }
        isLegal = isPathValid(a.destination);
        //check if the vertex contains a normal campus of the same player.
        vertex v = findVertex(a.destination);
        if (g->vertices[v.row][v.column].content != g->whosTurn) {
            printf("The point [%d, %d] content is %d\n",v.row,v.column,g->vertices[v.row][v.column].content);
            printf("Not on campus\n");
            isLegal = FALSE;
        }
        //check resources
        if (g->players[g->whosTurn - 1].numStudents[STUDENT_MJ] < 2
                || g->players[g->whosTurn - 1].numStudents[STUDENT_MMONEY]
                        < 3) {
            printf("Not enough resource\n");
            isLegal = FALSE;
        }
        //check number of GO8s
        if (g->players[0].numGO8s + g->players[1].numGO8s
                + g->players[2].numGO8s >= 8) {
            printf("Too much GO8\n");
            isLegal = FALSE;
        }

    } else if (a.actionCode == OBTAIN_ARC) {
        int i = 0;
        //check if path is valid
        while (a.destination[i]) {
            if (!(a.destination[i] == 'L' || a.destination[i] == 'R'
                    || a.destination[i] == 'B')) {
                printf("Bad Path\n");
                return FALSE;
            }
            i++;
        }
        isLegal = isPathValid(a.destination);
        //check resources
        if (g->players[g->whosTurn - 1].numStudents[STUDENT_BPS] < 1
                || g->players[g->whosTurn - 1].numStudents[STUDENT_BQN] < 1) {
            isLegal = FALSE;
        }
        //check if the ARC is vacant
        if (getARC(g, a.destination)) {
            printf("ARC is not vacant");
            isLegal = FALSE;
        }
        //check if the ARC is connected to another ARC
        vertex v1 = findVertex(a.destination);
        vertex v2 = walkPath(v1.row, v1.column, v1.facing, 'B');
        i = 0;
        int connected = FALSE;
        while (i<NUM_ARCS){
            if((v1.row == g->arcs[i].v1.row && v1.column == g->arcs[i].v1.column)
                    ||(v1.row == g->arcs[i].v2.row && v1.column == g->arcs[i].v2.column)
                    ||(v2.row == g->arcs[i].v1.row && v2.column == g->arcs[i].v1.column)
                    ||(v2.row == g->arcs[i].v2.row && v2.column == g->arcs[i].v2.column)){
                if(g->arcs[i].content == g->whosTurn){
                    connected = TRUE;
                    printf("the ARC is connected to an ARC\n");
                }
            }
            i++;
        }
        if (connected == FALSE) {
            if(getVertexOwner(g,v1.row,v1.column)!=g->whosTurn
                    &&getVertexOwner(g,v2.row,v2.column)!=g->whosTurn){
                isLegal = FALSE;
                printf("the ARC is not connected to a campus either\n");
            }
        }
    } else if (a.actionCode == START_SPINOFF) {
        //check resources
        if (g->players[g->whosTurn - 1].numStudents[STUDENT_MMONEY] < 1
                || g->players[g->whosTurn - 1].numStudents[STUDENT_MJ] < 1
                || g->players[g->whosTurn - 1].numStudents[STUDENT_MTV] < 1) {
            isLegal = FALSE;
        }
    } else if (a.actionCode == OBTAIN_PUBLICATION) {
        isLegal = FALSE;
    } else if (a.actionCode == OBTAIN_IP_PATENT) {
        isLegal = FALSE;
    } else if (a.actionCode == RETRAIN_STUDENTS) {
        if (a.disciplineFrom < 1 || a.disciplineFrom > 5) {
            printf("Bad discipline");
            return FALSE;
        }
        int rate = getExchangeRate(g, g->whosTurn, a.disciplineFrom,
                a.disciplineTo);
        if (g->players[g->whosTurn - 1].numStudents[a.disciplineFrom] < rate) {
            printf("Now enough");
            isLegal = FALSE;
        }
    }
    if (g->currentTurn == -1) {
        isLegal = FALSE;
    }
    return isLegal;
}

/* ****Helper functions**** */
/* ****Helper functions**** */
/* ****Helper functions**** */
static vertex findVertex(path pathToVertex) {
    vertex v;
    int row = 1;
    int column = 3;
    int facing = DOWN;
    v.row =row;
    v.column = column;
    v.facing = facing;
    int i = 0;
    while (pathToVertex[i] != 0) {
        v = walkPath(row, column, facing, pathToVertex[i]);
        row = v.row;
        column = v.column;
        facing = v.facing;
        i++;
    }
    return v;
}

static vertex walkPath(int row, int column, int facing, char nextMove) {
    vertex v;
    v.row = row;
    v.column = column;
    v.facing = facing;
    if (facing == DOWN) { // if facing DOWN
        if ((column + row) % 2 == 0) { //if row and column are both even or odd
            if (nextMove == 'L') {
                v.column++;
                v.facing = RIGHT;
            } else if (nextMove == 'R') {
                v.row++;
                v.facing = DOWN;
            } else {
                v.row--;
                v.facing = UP;
            }
        } else { //if one of row and column is even and the other is odd
            if (nextMove == 'L') {
                v.row++;
                v.facing = DOWN;
            } else if (nextMove == 'R') {
                v.column--;
                v.facing = LEFT;
            } else {
                v.row--;
                v.facing = UP;
            }
        }
    } else if (facing == UP) {
        if ((column + row) % 2 == 0) { //if row and column are both even or odd
            if (nextMove == 'L') {
                v.row--;
                v.facing = UP;
            } else if (nextMove == 'R') {
                v.column++;
                v.facing = RIGHT;
            } else {
                v.row++;
                v.facing = DOWN;
            }
        } else { //if one of row and column is even and the other is odd
            if (nextMove == 'L') {
                v.column--;
                v.facing = LEFT;
            } else if (nextMove == 'R') {
                v.row--;
                v.facing = UP;
            } else {
                v.row++;
                v.facing = DOWN;
            }
        }
    } else if (facing == LEFT) {
        if (nextMove == 'L') {
            v.row++;
            v.facing = DOWN;
        } else if (nextMove == 'R') {
            v.row--;
            v.facing = UP;
        } else {
            v.column++;
            v.facing = RIGHT;
        }
    } else if (facing == RIGHT) {
        if (nextMove == 'L') {
            v.row--;
            v.facing = UP;
        } else if (nextMove == 'R') {
            v.row++;
            v.facing = DOWN;
        } else {
            v.column--;
            v.facing = LEFT;
        }
    }
    return v;
}
//get content of an vertex by the index instead of a path
static int getVertexOwner(Game g, int row, int column) {
    int playerID;
    playerID = g->vertices[row][column].content;
    if (playerID > 3) {
        playerID = playerID - 3;
    }
    return playerID;
}

static player initializePlayer(int playerID) {
    player p;
    int stds[] = INITIAL_STUDENTS;
    p.KPI = 0;
    p.numARCs = 0;
    p.numCampuses = 0;
    p.numGO8s = 0;
    p.numIPs = 0;
    p.numPublications = 0;
    int i = 0;
    while (i < NUM_DISCIPLINES) {
        p.numStudents[i] = stds[i];
        i++;
    }
    p.playerID = playerID;
    return p;
}

static int findARCIndex(Game g, vertex v1, vertex v2) {
    int arcIndex = 0;
    int i = 0;
    while (i < NUM_ARCS) {
        if ((g->arcs[i].v1.row == v1.row && g->arcs[i].v1.column == v1.column
                && g->arcs[i].v2.row == v2.row
                && g->arcs[i].v2.column == v2.column)
                || (g->arcs[i].v1.row == v2.row
                        && g->arcs[i].v1.column == v2.column
                        && g->arcs[i].v2.row == v1.row
                        && g->arcs[i].v2.column == v1.column)) {
            arcIndex = i;
        }
        i++;
    }
    return arcIndex;
}

/**************updater functions*******************/
/**************updater functions*******************/
/**************updater functions*******************/
static void buildCampus(Game g, int row, int column, int playerID) {
    g->vertices[row][column].content = playerID;
    g->players[playerID - 1].numStudents[STUDENT_BPS] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_BQN] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_MJ] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_MTV] -= 1;
    int i = 0;
    while (i < NUM_UNIS) {
        if (g->players[i].playerID == playerID) {
            g->players[i].KPI += VALUE_CAMPUS;
            g->players[i].numCampuses++;
        }
        i++;
    }
}

static void buildGO8(Game g, int row, int column, int playerID) {
    g->vertices[row][column].content = (3 + playerID);
    g->players[playerID - 1].numStudents[STUDENT_MJ] -= 2;
    g->players[playerID - 1].numStudents[STUDENT_MMONEY] -= 3;
    int i = 0;
    while (i < NUM_UNIS) {
        if (g->players[i].playerID == playerID) {
            g->players[i].KPI = g->players[i].KPI - VALUE_CAMPUS + VALUE_GO8;
            g->players[i].numCampuses--;
            g->players[i].numGO8s++;
        }
        i++;
    }
}

static void buildARC(Game g, int arcIndex, int playerID) {
    g->arcs[arcIndex].content = playerID;
    g->players[playerID - 1].numStudents[STUDENT_BQN] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_BPS] -= 1;
    int i = 0;
    while (i < NUM_UNIS) {
        if (g->players[i].playerID == playerID) {
            g->players[i].KPI = g->players[i].KPI + VALUE_ARC;
            g->players[i].numARCs++;
            if (g->mostARCs == 0) {
                g->mostARCs = playerID;
                g->players[g->mostARCs - 1].KPI += VALUE_MOSTARC;
            } else if (g->players[i].numARCs
                    > g->players[g->mostARCs - 1].numARCs) {
                g->players[g->mostARCs - 1].KPI -= VALUE_MOSTARC;
                g->mostARCs = playerID;
                g->players[g->mostARCs - 1].KPI += VALUE_MOSTARC;
            }
        }
        i++;
    }
}

static void buildPublication(Game g, int playerID) {
    int i = 0;
    g->players[playerID - 1].numStudents[STUDENT_MMONEY] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_MJ] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_MTV] -= 1;
    while (i < NUM_UNIS) {
        if (g->players[i].playerID == playerID) {
            g->players[i].numPublications++;
            if (g->mostPublications == 0) {
                g->mostPublications = playerID;
                g->players[g->mostPublications - 1].KPI += VALUE_MOSTPUB;
            } else if (g->players[i].numPublications
                    > g->players[g->mostPublications - 1].numPublications) {
                g->players[g->mostPublications - 1].KPI -= VALUE_MOSTPUB;
                g->mostPublications = playerID;
                g->players[g->mostPublications - 1].KPI += VALUE_MOSTPUB;
            }
        }
        i++;
    }
}

static void buildIP(Game g, int playerID) {
    int i = 0;
    g->players[playerID - 1].numStudents[STUDENT_MMONEY] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_MJ] -= 1;
    g->players[playerID - 1].numStudents[STUDENT_MTV] -= 1;
    while (i < NUM_UNIS) {
        if (g->players[i].playerID == playerID) {
            g->players[i].numIPs++;
            g->players[i].KPI += VALUE_IP;
        }
        i++;
    }
}
static void generateStudent(Game g, int regionID, int desciplin) {
    int row[19] = { 3, 5, 7, 2, 4, 6, 8, 1, 3, 5, 7, 9, 2, 4, 6, 8, 3, 5, 7 };
    int column[19] = { 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5 };
    int r = row[regionID];
    int c = column[regionID];
    int i = 0;
    int j = 0;
    while (j < 2) {
        i = 0;
        while (i < 3) {
            if (getVertexOwner(g, r + i, c + j)) {
                g->players[getVertexOwner(g, r + i, c + j) - 1].numStudents[desciplin]++;
            }
            i++;
        }
        j++;
    }
}
/**************Validator functions*******************/
/**************Validator functions*******************/
/**************Validator functions*******************/
//this function only check if v is on map,
//it does not check the content or any other feature.
static int isVertexOnMap(vertex v) {
    int isValid = FALSE;
    if (v.column == 3 || v.column == 4) {
        if (v.row > 0 && v.row < 12) {
            isValid = TRUE;
        }
    } else if (v.column == 2 || v.column == 5) {
        if (v.row > 1 && v.row < 11) {
            isValid = TRUE;
        }

    } else if (v.column == 1 || v.column == 6) {
        if (v.row > 2 && v.row < 10) {
            isValid = TRUE;
        }
    }
    return isValid;
}

static int isPathValid(path p) {
    int isLegal = TRUE;
    vertex v;
    int row = 1;
    int column = 3;
    int facing = DOWN;
    int i = 0;
    while (p[i]) {
        v = walkPath(row, column, facing, p[i]);
        row = v.row;
        column = v.column;
        facing = v.facing;
        if (isVertexOnMap(v) == FALSE) {
            isLegal = FALSE;
            printf("Good but invalid path\n");
        }
        i++;
    }
    return isLegal;
}

