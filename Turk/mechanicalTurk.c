/*
 *  Mr Thd.  Completely useless
 *  This AI makes really bad moves!
 *  When they have enough BPS or BQN students they convert them to THDs.
 *  Otherwise they pass
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Game.h"
#include "mechanicalTurk.h"

typedef struct _resource {
    int numTHD;
    int numBPS;
    int numBQN;
    int numMJ;
    int numMTV;
    int numMMONEY;
} resource;

static int isValid(Game g, int code, path p, int from, int to);
static action mrPassRetrain(Game g, resource r);
static action GO8Retrain(Game g, resource r);

action decideAction(Game g) {

    action nextAction;
    nextAction.actionCode = PASS;
    int badLuck = FALSE;

    //Who am I? Who's turn it is when I am called?
    int currentPlayer = getWhoseTurn(g);
    if (currentPlayer != NO_ONE) {
        resource resources;
        resources.numTHD = getStudents(g, currentPlayer, STUDENT_THD);
        resources.numBPS = getStudents(g, currentPlayer, STUDENT_BPS);
        resources.numBQN = getStudents(g, currentPlayer, STUDENT_BQN);
        resources.numMJ = getStudents(g, currentPlayer, STUDENT_MJ);
        resources.numMTV = getStudents(g, currentPlayer, STUDENT_MTV);
        resources.numMMONEY = getStudents(g, currentPlayer, STUDENT_MMONEY);

        //This checks you have all the necessary conditions
        //then creates the action
        //Alternately you could create the action and then just check it is legal if that is easier
        //some simple hard coded features to beat Mr.PASSes
        if (currentPlayer == UNI_A) {
            //retrain for GO8
            if (getCampus(g, "RL") != CAMPUS_A && getCampus(g, "RL") != GO8_A) {
                badLuck = TRUE;
            }
            if (badLuck == FALSE
                    && (getGO8s(g, UNI_A) + getGO8s(g, UNI_B)
                            + getGO8s(g, UNI_C)) < 8) {
                if (GO8Retrain(g, resources).disciplineTo != STUDENT_THD
                        && getGO8s(g, getWhoseTurn(g)) < 3
                        && getTurnNumber(g) > 0) {
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom =
                            GO8Retrain(g, resources).disciplineFrom;
                    nextAction.disciplineTo =
                            GO8Retrain(g, resources).disciplineTo;

                    if (isLegalAction(g, nextAction)) {
                        return nextAction;
                    }
                }
                //Build GO8
                if (getTurnNumber(g) > 0 && resources.numMJ >= 2
                        && resources.numMMONEY >= 3
                        && getGO8s(g, getWhoseTurn(g)) < 3) {
                    nextAction.actionCode = BUILD_GO8;
                    if (getGO8s(g, getWhoseTurn(g)) == 0) {
                        strncpy(nextAction.destination, "", 1);
                    } else if (getGO8s(g, getWhoseTurn(g)) == 1) {
                        strncpy(nextAction.destination, "RLRLRLRLLRR", 12);
                    } else if (getGO8s(g, getWhoseTurn(g))
                            == 2&& badLuck == FALSE) {
                        strncpy(nextAction.destination, "RL", 3);
                    }
                    printf("GO8: %d\n", getGO8s(g, getWhoseTurn(g)));

                    //   if (isLegalAction(g, nextAction)) {
                    // }
                }
            }

            if (getTurnNumber(g) != 0) {
                //retrain for spinoff
                if (mrPassRetrain(g, resources).disciplineTo != STUDENT_THD
                        && (badLuck == TRUE
                                || (badLuck == FALSE
                                        && getGO8s(g, getWhoseTurn(g)) == 3))) {
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom =
                            mrPassRetrain(g, resources).disciplineFrom;
                    nextAction.disciplineTo =
                            mrPassRetrain(g, resources).disciplineTo;

                    if (isLegalAction(g, nextAction)) {
                        return nextAction;
                    }

                }
                //Do spin off
                if (resources.numMJ > 0 && resources.numMTV > 0
                        && resources.numMMONEY > 0
                        && (badLuck == TRUE
                                || (badLuck == FALSE
                                        && getGO8s(g, getWhoseTurn(g)) == 3))) {
                    nextAction.actionCode = START_SPINOFF;
                }
            }
            //first turn strats
            if (getTurnNumber(g) == 0) {
                if (getARC(g, "R") == VACANT_ARC) {
                    if (isValid(g, OBTAIN_ARC, "R", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = OBTAIN_ARC;
                        strncpy(nextAction.destination, "R", 2);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                } else if (getARC(g, "RL") == VACANT_ARC) {
                    if (isValid(g, OBTAIN_ARC, "RL", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = OBTAIN_ARC;
                        strncpy(nextAction.destination, "RL", 3);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                } else if (getCampus(g, "RL") == VACANT_VERTEX) {
                    if (isValid(g, BUILD_CAMPUS, "RL", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = BUILD_CAMPUS;
                        strncpy(nextAction.destination, "RL", 3);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                }
            }
        } else if (currentPlayer == UNI_B) {
            //retrain for GO8
            if (getCampus(g, "LRLRLRRLRLBL") != CAMPUS_B
                    && getCampus(g, "LRLRLRRLRLBL") != GO8_B) {
                badLuck = TRUE;
            }
            if (badLuck == FALSE
                    && (getGO8s(g, UNI_A) + getGO8s(g, UNI_B)
                            + getGO8s(g, UNI_C)) < 8) {
                if (GO8Retrain(g, resources).disciplineTo != STUDENT_THD
                        && getGO8s(g, getWhoseTurn(g)) < 3
                        && getTurnNumber(g) > 0) {
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom =
                            GO8Retrain(g, resources).disciplineFrom;
                    nextAction.disciplineTo =
                            GO8Retrain(g, resources).disciplineTo;

                    if (isLegalAction(g, nextAction)) {
                        return nextAction;
                    }
                }
                //Build GO8
                if (getTurnNumber(g) > 0 && resources.numMJ >= 2
                        && resources.numMMONEY >= 3
                        && getGO8s(g, getWhoseTurn(g)) < 3) {
                    nextAction.actionCode = BUILD_GO8;
                    if (getGO8s(g, getWhoseTurn(g)) == 0) {
                        strncpy(nextAction.destination, "LRLRLRRLRL", 12);
                    } else if (getGO8s(g, getWhoseTurn(g)) == 1) {
                        strncpy(nextAction.destination, "RRLRL", 6);
                    } else if (getGO8s(g, getWhoseTurn(g))
                            == 2&& badLuck == FALSE) {
                        strncpy(nextAction.destination, "LRLRLRRLRLBL", 14);
                    }
                    printf("GO8: %d\n", getGO8s(g, getWhoseTurn(g)));

                    //   if (isLegalAction(g, nextAction)) {
                    // }
                }
            }

            if (getTurnNumber(g) != 1) {
                //retrain for spinoff
                if (mrPassRetrain(g, resources).disciplineTo != STUDENT_THD
                        && (badLuck == TRUE
                                || (badLuck == FALSE
                                        && getGO8s(g, getWhoseTurn(g)) == 3))) {
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom =
                            mrPassRetrain(g, resources).disciplineFrom;
                    nextAction.disciplineTo =
                            mrPassRetrain(g, resources).disciplineTo;

                    if (isLegalAction(g, nextAction)) {
                        return nextAction;
                    }
                }
                //Do spin off
                if (resources.numMJ > 0 && resources.numMTV > 0
                        && resources.numMMONEY > 0
                        && (badLuck == TRUE
                                || (badLuck == FALSE
                                        && getGO8s(g, getWhoseTurn(g)) == 3))) {
                    nextAction.actionCode = START_SPINOFF;
                }
            }
            //first turn strats
            if (getTurnNumber(g) == 1) {
                if (getARC(g, "LRLRLRRLRLB") == VACANT_ARC) {
                    if (isValid(g, OBTAIN_ARC, "LRLRLRRLRLB", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = OBTAIN_ARC;
                        strncpy(nextAction.destination, "LRLRLRRLRLB", 13);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                } else if (getARC(g, "LRLRLRRLRLBL") == VACANT_ARC) {
                    if (isValid(g, OBTAIN_ARC, "LRLRLRRLRLBL", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = OBTAIN_ARC;
                        strncpy(nextAction.destination, "LRLRLRRLRLBL", 14);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                } else if (getCampus(g, "LRLRLRRLRLBL") == VACANT_VERTEX) {
                    if (isValid(g, BUILD_CAMPUS, "LRLRLRRLRLBL", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = BUILD_CAMPUS;
                        strncpy(nextAction.destination, "LRLRLRRLRLBL", 14);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                }
            }

        } else if (currentPlayer == UNI_C) {
            if (getCampus(g, "RRLRLLRLRLLL") != CAMPUS_C
                    && getCampus(g, "RRLRLLRLRLLL") != GO8_C) {
                badLuck = TRUE;
            }
            if (badLuck == FALSE
                    && (getGO8s(g, UNI_A) + getGO8s(g, UNI_B)
                            + getGO8s(g, UNI_C)) < 8) {
                if (GO8Retrain(g, resources).disciplineTo != STUDENT_THD
                        && getGO8s(g, getWhoseTurn(g)) < 3
                        && getTurnNumber(g) > 0) {
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom =
                            GO8Retrain(g, resources).disciplineFrom;
                    nextAction.disciplineTo =
                            GO8Retrain(g, resources).disciplineTo;
                    if (isLegalAction(g, nextAction)) {
                        return nextAction;
                    }
                }
                //Build GO8
                if (getTurnNumber(g) > 0 && resources.numMJ >= 2
                        && resources.numMMONEY >= 3
                        && getGO8s(g, getWhoseTurn(g)) < 3) {
                    nextAction.actionCode = BUILD_GO8;
                    if (getGO8s(g, getWhoseTurn(g)) == 0) {
                        strncpy(nextAction.destination, "RRLRLLRLRL", 11);
                    } else if (getGO8s(g, getWhoseTurn(g)) == 1) {
                        strncpy(nextAction.destination, "LRLRL", 6);
                    } else if (getGO8s(g, getWhoseTurn(g))
                            == 2&& badLuck == FALSE) {
                        strncpy(nextAction.destination, "RRLRLLRLRLLL", 13);
                    }
                    printf("GO8: %d\n", getGO8s(g, getWhoseTurn(g)));

                    //   if (isLegalAction(g, nextAction)) {
                    // }
                }
            }

            if (getTurnNumber(g) != 2) {
                //retrain for spinoff
                if (mrPassRetrain(g, resources).disciplineTo != STUDENT_THD
                        && (badLuck == TRUE
                                || (badLuck == FALSE
                                        && getGO8s(g, getWhoseTurn(g)) == 3))) {
                    nextAction.actionCode = RETRAIN_STUDENTS;
                    nextAction.disciplineFrom =
                            mrPassRetrain(g, resources).disciplineFrom;
                    nextAction.disciplineTo =
                            mrPassRetrain(g, resources).disciplineTo;

                    if (isLegalAction(g, nextAction)) {
                        return nextAction;
                    }

                }
                //Do spin off
                if (resources.numMJ > 0 && resources.numMTV > 0
                        && resources.numMMONEY > 0
                        && (badLuck == TRUE
                                || (badLuck == FALSE
                                        && getGO8s(g, getWhoseTurn(g)) == 3))) {
                    nextAction.actionCode = START_SPINOFF;
                }
            }
            //first turn strats
            if (getTurnNumber(g) == 2) {
                if (getARC(g, "RRLRLLRLRLL") == VACANT_ARC) {
                    if (isValid(g, OBTAIN_ARC, "RRLRLLRLRLL", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = OBTAIN_ARC;
                        strncpy(nextAction.destination, "RRLRLLRLRLL", 12);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                } else if (getARC(g, "RRLRLLRLRLLL") == VACANT_ARC) {
                    if (isValid(g, OBTAIN_ARC, "RRLRLLRLRLLL", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = OBTAIN_ARC;
                        strncpy(nextAction.destination, "RRLRLLRLRLLL", 13);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                } else if (getCampus(g, "RRLRLLRLRLLL") == VACANT_VERTEX) {
                    if (isValid(g, BUILD_CAMPUS, "RRLRLLRLRLLL", STUDENT_BPS,
                    STUDENT_BQN) == TRUE) {
                        nextAction.actionCode = BUILD_CAMPUS;
                        strncpy(nextAction.destination, "RRLRLLRLRLLL", 13);
                        nextAction.disciplineFrom = STUDENT_BPS;
                        nextAction.disciplineTo = STUDENT_BQN;
                    }
                }
            }
        }
    }
    if (!isLegalAction(g, nextAction)) {
        nextAction.actionCode = PASS;
    }
    return nextAction;
}

static int isValid(Game g, int code, path p, int from, int to) {
    action a;
    a.actionCode = code;
    strncpy(a.destination, p, sizeof(a.destination));
    a.disciplineFrom = from;
    a.disciplineTo = to;
    return isLegalAction(g, a);
}

static action mrPassRetrain(Game g, resource r) {
    action a;
    a.actionCode = PASS;
    a.disciplineTo = STUDENT_THD;
    if (r.numMJ == 0) {
        if (r.numMMONEY >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MMONEY,
        STUDENT_MJ) + 1) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MMONEY;
            a.disciplineTo = STUDENT_MJ;
        } else if (r.numMTV >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MTV,
        STUDENT_MJ) + 1) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MTV;
            a.disciplineTo = STUDENT_MJ;
        } else if (r.numBPS >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BPS,
        STUDENT_MJ)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_MJ;
        } else if (r.numBQN >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BQN,
        STUDENT_MJ)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BQN;
            a.disciplineTo = STUDENT_MJ;
        }
    } else if (r.numMTV == 0) {
        if (r.numMMONEY >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MMONEY,
        STUDENT_MTV) + 1) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MMONEY;
            a.disciplineTo = STUDENT_MTV;
        } else if (r.numMJ >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MJ,
        STUDENT_MTV) + 1) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MJ;
            a.disciplineTo = STUDENT_MTV;
        } else if (r.numBPS >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BPS,
        STUDENT_MTV)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_MTV;
        } else if (r.numBQN >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BQN,
        STUDENT_MTV)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BQN;
            a.disciplineTo = STUDENT_MTV;
        }
    } else if (r.numMMONEY == 0) {
        if (r.numMTV >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MTV,
        STUDENT_MMONEY) + 1) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MTV;
            a.disciplineTo = STUDENT_MMONEY;
        } else if (r.numMJ >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MJ,
        STUDENT_MMONEY) + 1) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MJ;
            a.disciplineTo = STUDENT_MMONEY;
        } else if (r.numBPS >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BPS,
        STUDENT_MMONEY)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_MMONEY;
        } else if (r.numBQN >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BQN,
        STUDENT_MMONEY)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BQN;
            a.disciplineTo = STUDENT_MMONEY;
        }
    }

    return a;
}

static action GO8Retrain(Game g, resource r) {
    action a;
    a.actionCode = PASS;
    a.disciplineTo = STUDENT_THD;
    if (r.numMJ < 2) {
        if (r.numMMONEY >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MMONEY,
        STUDENT_MJ) + 3) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MMONEY;
            a.disciplineTo = STUDENT_MJ;
        } else if (r.numMTV >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MTV,
        STUDENT_MJ)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MTV;
            a.disciplineTo = STUDENT_MJ;
        } else if (r.numBPS >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BPS,
        STUDENT_MJ)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_MJ;
        } else if (r.numBQN >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BQN,
        STUDENT_MJ)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BQN;
            a.disciplineTo = STUDENT_MJ;
        }
    } else if (r.numMMONEY < 3) {
        if (r.numMTV >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MTV,
        STUDENT_MMONEY)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MTV;
            a.disciplineTo = STUDENT_MMONEY;
        } else if (r.numMJ >= getExchangeRate(g, getWhoseTurn(g), STUDENT_MJ,
        STUDENT_MMONEY) + 2) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_MJ;
            a.disciplineTo = STUDENT_MMONEY;
        } else if (r.numBPS >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BPS,
        STUDENT_MMONEY)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BPS;
            a.disciplineTo = STUDENT_MMONEY;
        } else if (r.numBQN >= getExchangeRate(g, getWhoseTurn(g), STUDENT_BQN,
        STUDENT_MMONEY)) {
            a.actionCode = RETRAIN_STUDENTS;
            strncpy(a.destination, "L", sizeof(a.destination));
            a.disciplineFrom = STUDENT_BQN;
            a.disciplineTo = STUDENT_MMONEY;
        }
    }

    return a;
}
