#include "Game.h"
#include <stdio.h>
#include <assert.h>

#define DEFAULT_DISCIPLINES {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5}
#define NUMBER_OF_DISCIPLINES 6

int main(int argc, char *argv[]) {
    //Test for initialization
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    printf("********************************************\n");
    printf("**********Testing initialization************\n");
    printf("********************************************\n");
    Game g = newGame(disciplines, dice);
    int i = 0;
    printf("checking regions\n");
    while (i < NUM_REGIONS) {
        assert(getDiscipline(g, i) == disciplines[i]);
        assert(getDiceValue(g, i) == dice[i]);
        i++;
    }
    assert(getTurnNumber(g) == -1);
    assert(getWhoseTurn(g) == NO_ONE);
    assert(getMostARCs(g) == NO_ONE);
    assert(getMostPublications(g) == NO_ONE);
    printf("Passed NO_ONE\n");
    assert(getKPIpoints(g, UNI_A) == 20);
    assert(getKPIpoints(g, UNI_B) == 20);
    assert(getKPIpoints(g, UNI_C) == 20);
    printf("Passed KPI\n");
    assert(getARCs(g, UNI_A) == 0);
    assert(getARCs(g, UNI_B) == 0);
    assert(getARCs(g, UNI_C) == 0);
    assert(getGO8s(g, UNI_A) == 0);
    assert(getGO8s(g, UNI_B) == 0);
    assert(getGO8s(g, UNI_C) == 0);
    assert(getCampuses(g, UNI_A) == 2);
    assert(getCampuses(g, UNI_B) == 2);
    assert(getCampuses(g, UNI_C) == 2);
    assert(getIPs(g, UNI_A) == 0);
    assert(getIPs(g, UNI_B) == 0);
    assert(getIPs(g, UNI_C) == 0);
    printf("Passed IP = 0\n");
    assert(getPublications(g, UNI_A) == 0);
    assert(getStudents(g, UNI_A, STUDENT_THD) == 0);
    assert(getStudents(g, UNI_A, STUDENT_BPS) == 3);
    assert(getStudents(g, UNI_A, STUDENT_BQN) == 3);
    assert(getStudents(g, UNI_A, STUDENT_MJ) == 1);
    assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
    assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 1);
    assert(getStudents(g, UNI_B, STUDENT_THD) == 0);
    assert(getStudents(g, UNI_B, STUDENT_BPS) == 3);
    assert(getStudents(g, UNI_B, STUDENT_BQN) == 3);
    assert(getStudents(g, UNI_B, STUDENT_MJ) == 1);
    assert(getStudents(g, UNI_B, STUDENT_MTV) == 1);
    assert(getStudents(g, UNI_B, STUDENT_MMONEY) == 1);
    assert(getStudents(g, UNI_C, STUDENT_THD) == 0);
    assert(getStudents(g, UNI_C, STUDENT_BPS) == 3);
    assert(getStudents(g, UNI_C, STUDENT_BQN) == 3);
    assert(getStudents(g, UNI_C, STUDENT_MJ) == 1);
    assert(getStudents(g, UNI_C, STUDENT_MTV) == 1);
    assert(getStudents(g, UNI_C, STUDENT_MMONEY) == 1);
    printf("Passed student\n");
    assert(getExchangeRate(g, UNI_A,STUDENT_BPS,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_A,STUDENT_BQN,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_A,STUDENT_MJ,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_A,STUDENT_MTV,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_A,STUDENT_MMONEY,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_B,STUDENT_BPS,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_B,STUDENT_BQN,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_B,STUDENT_MJ,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_B,STUDENT_MTV,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_B,STUDENT_MMONEY,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_C,STUDENT_BPS,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_C,STUDENT_BQN,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_C,STUDENT_MJ,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_C,STUDENT_MTV,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_C,STUDENT_MMONEY,STUDENT_BQN) == 3);
    assert(getExchangeRate(g, UNI_C,STUDENT_MMONEY,STUDENT_BQN) == 3);
    printf("Passed retraining rate\n");
    assert(getCampus(g,"")== CAMPUS_A);
    assert(getCampus(g,"RLRLRLRLLRR")== CAMPUS_A);
    assert(getCampus(g,"RRLRL")== CAMPUS_B);
    assert(getCampus(g,"LRLRLRRLRL")== CAMPUS_B);
    assert(getCampus(g,"LRLRL")== CAMPUS_C);
    assert(getCampus(g,"RRLRLLRLRL")== CAMPUS_C);
    //all actions are invalid in no man's turn
    action a1 = { .actionCode = PASS, .destination = { 'R' }, .disciplineFrom = STUDENT_THD,
            .disciplineTo = STUDENT_THD };
    i = -10;
    while (i <= 10) {
        a1.actionCode = i;
        assert(isLegalAction(g,a1) == FALSE);
        i++;
    }
     printf("Passed No one's land action invalid\n");
     printf("INITIALIZATION PASSED!!!!!!!!!!!!!!\n");
    //Throw the dice for the first time advance the game in to turn 0.
    //According to the set up, 11 will result in UNI_A
    //acquiring a MTV student, Now UNI_A should have 2 MTV students.
     printf("********************************************\n");
     printf("**********Testing Advance into T1***********\n");
     printf("********************************************\n");
    throwDice(g, 6);
    assert(getTurnNumber(g) == 0);    //Turn 0
    assert(getWhoseTurn(g) == UNI_A);    //UNI_A's turn
    assert(getStudents(g, UNI_A, STUDENT_MJ) == 2);
    //This action should be performed by UNI_A.
    //The action is obtaining the arc to the right of upper UNI_A campus.
    a1.actionCode = OBTAIN_ARC;
    a1.disciplineFrom = -1075544808, a1.disciplineTo = 134525371;
    //This action should be valid.
    assert(isLegalAction(g,a1) == TRUE);
    //make action
    makeAction(g, a1);
    //This action should cost 1 BPS and 1 BQN. Now UNI_A should have 2 and 2 respectively.
    assert(getStudents(g, UNI_A, STUDENT_BPS) == 2);
    assert(getStudents(g, UNI_A, STUDENT_BQN) == 2);
    assert(getStudents(g, UNI_A, STUDENT_MJ) == 2);
    assert(getStudents(g, UNI_A, STUDENT_MTV) == 1);
    assert(getStudents(g, UNI_A, STUDENT_MMONEY) == 1);
    //the arc should belong to UNI_A.
    assert(getARC(g, a1.destination) == ARC_A);
    //UNI_A just got the first ARC!
    assert(getARCs(g, UNI_A) == 1);
    //ARC 1:0:0
    assert(getMostARCs(g) == UNI_A);

    printf("All test passed...For now. More coming.");
    disposeGame(g);
    return 0;

}
