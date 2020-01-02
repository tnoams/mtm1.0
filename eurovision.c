#include "list.h"
#include <stdlib.h>
#include <stdbool.h>
#include "eurovision.h"
/**
 * Implements the type eurovision used for the contest.
 * The header's point is for
 * It contains the actions which can be used for and on the type eurovision
 * ------------------place some tips about the use of eurovision----
 *
 * the following functions are available:
 *
 *   ~  eurovisionCreate - makes a eurovision competition.
 *   ~  eurovisionDestroy - destroys the whole eurovision.
 *   ~  eurovisionAddJudge - adds a judge
 *   ~  eurovisionRemoveJudge - removes a judge
 *   ~  eurovisionAddState - adds a state
 *   ~  eurovisionRemoveState - removes a state
 *     eurovisionAddVote - adds a single vote
 *     eurovisionRemoveVote - removes a single vote
 *     eurovisionRunContest - returns a list of who got most points to who got the least point
 *     eurovisionRunAudienceFavorite - returns a list of most to least popular states
 *     eurovisionRunFriendlyStates - returns a list of which state was most popular in which state
 *
*/

/* ADT for judges part of the eurovision */

typedef struct judges {
    int id;
    const char* name;
    int *vote;
    struct judges *next;
}judges;

/* ADT for votes of states*/

typedef struct vote {
    int stateGiver;
    int votes; // a counter
    struct vote* next_vote;
}vote;


/* ADT for states of the eurovision */

typedef struct states {
    int id;
    const char* name;
    const char* songName;
    vote *vote;
    struct states *next;
}states;



/* ADT of the Eurovision struct */
struct eurovision_t {
    struct judges* Judge;
    struct states* State;
};

/*
 * eurovisionDestroy: deletes everything in a eurovision
 * @param eurovision   the eurovision which is deleted
 */
void eurovisionDestroy(Eurovision eurovision){
    listDestroy((List)(eurovision->Judge));
    listDestroy((List)(eurovision->State));
    free(eurovision);
}
/**
 * eurovisionCreate: creates a new eurovision
 *
 * returns NULL in case of failure, or a eurovision in case of success.
 */
Eurovision eurovisionCreate(){
    Eurovision eurovision = malloc(sizeof(Eurovision));
    if (eurovision == NULL) {
        eurovisionDestroy(eurovision);
        return NULL;
    }
    eurovision->Judge = NULL;
    eurovision->State = NULL;
    return eurovision;
}

//
static int findState(Eurovision eurovision , int stateId){
    struct states* temp_state = eurovision->State;
    int index=0;
    while(temp_state != NULL){
        if (temp_state->id == stateId)
            return index;
        else {
            temp_state = temp_state->next;
            index++;
        }
    }
    return -1;
}


static int findJudge(Eurovision eurovision , int JudgeId){
    judges* temp_Judge = eurovision->Judge;
    int index=0;
    while(temp_Judge != NULL){
        if (temp_Judge->id == JudgeId)
            return index;
        else {
            temp_Judge = temp_Judge->next;
            index++;
        }
    }
    return -1;
}

static bool checkStateExists(Eurovision eurovision, int state_id){
    if (findState(eurovision,state_id) != -1)
        return true;
    else
        return false;
}

static bool checkJudgeExists (Eurovision eurovision, int judge_id){
    struct judges* temp_judge = eurovision->Judge;
    while (temp_judge != NULL){
        if(temp_judge->id == judge_id)
            return true;
        else
            temp_judge = eurovision->Judge->next;
    }
    return false;
}

static bool checkName (const char* Name){
    const char* tempName = Name;
    while(*tempName){
        if ((*tempName >= 'a' && *tempName <= 'z') || *tempName == ' ') {
            tempName++;
        }
        else
            return false;
    }
    return true;
}

static judges* getToJudge(Eurovision eurovision, int judgeId){
    judges* next_judge = eurovision->Judge;
    while(next_judge){
        if (next_judge->id == judgeId)
            return next_judge;
        else
            next_judge = next_judge->next;
    }
    return NULL;
}

static states* getToState(Eurovision eurovision, int stateId){
    states* next_state = eurovision->State;
    while(next_state){
        if (next_state->id == stateId)
            return next_state;
        else
            next_state = next_state->next;
    }
    return NULL;
}

static vote* getToVote(states* gettingState, int givingState){
    vote* next_vote = gettingState->vote;
    while(next_vote) {
        if (next_vote->stateGiver == givingState)
            return next_vote;

        else
            next_vote = next_vote->next_vote;
    }
    return  NULL;
}

static judges* placeToPutJudge (judges* judge){
    if (judge == NULL)
        return judge;
    else
        placeToPutJudge(judge->next);

    return judge;

}

static states* placeToPutState (states* state){
    if (state == NULL)
        return state;
    else
        placeToPutState(state->next);

    return state;

}


/**
 * eurovisionAddJudge: adds a Judge to the eurovision
 * @param 8urovision   the eurovision which will be destroyed
 * @param judgeName the name of the judge
 * @param judgeResults a pointer to an array that lists the judge's votes
 * @param judge_id the id of the judge
 * @return EUROVISION_IDVALID_ID in case of a problem with the id
 *         or EUROVISION_STATE_NOT_EXIST in case there is a problem with their votes
 *         or EUROVISION_JUDGE_ALREADY_EXIST in case there is already a judge with the same id
 *         or EUROVISION_INVALID_NAME in case there is a problem with the judge's name
 *         or EUROVISION_SUCCESS in case of success
 *
 */

EurovisionResult eurovisionAddJudge(Eurovision eurovision, int judgeId,const char* judgeName, int *judgeResults){
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;

    //check id of judge:
    if (judgeId <= 0)
        return EUROVISION_INVALID_ID;

    //check if judge exists:
    if (checkJudgeExists(eurovision, judgeId))
        return EUROVISION_JUDGE_ALREADY_EXIST;

    //check the votes of the judge:
    if(checkStateExists(eurovision, *judgeResults))
        return EUROVISION_STATE_NOT_EXIST;

    //check judge name:
    if (!checkName(judgeName))
        return EUROVISION_INVALID_NAME;

    //Creating the judge:
    judges* Judge = placeToPutJudge(eurovision->Judge);
    Judge = malloc(sizeof(eurovision->Judge));
    if (Judge == NULL) {
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    Judge->id = judgeId;
    Judge->name = judgeName;
    Judge->vote = judgeResults;
    Judge->next = NULL;
    return EUROVISION_SUCCESS;
}

/**
 * eurovisionAddState: adds a country to the eurovision
 * @param eurovision   the eurovision which the country is added to
 * @param stateName  the name of the country          CREATES A COPY OF THE ARRAYS
 * @param songName  the song of the country
 * @param stateId    they asked for this?
 * @return returns EUROVISION_STATE_ALREADY_EXIST in case the country is already in the eurovision
 *         or EUROVISION_INVALID_ID in case of an invalid id
 *         or EUROVISION_INVALID_NAME in case of an invalid name
 *         or EUROVISION_SUCCESS in case of success
 */
 EurovisionResult eurovisionAddState(Eurovision eurovision,int stateId, const char* stateName, const char* songName){

     if (eurovision == NULL)
         return EUROVISION_NULL_ARGUMENT;
     if (stateId <= 0)
         return EUROVISION_INVALID_ID;
     if (checkStateExists(eurovision, stateId))
         return EUROVISION_STATE_ALREADY_EXIST;
     if (!checkName(stateName))
         return EUROVISION_INVALID_NAME;

    states* State = placeToPutState(eurovision->State);
    State = malloc(sizeof(eurovision->State));
    if (State == NULL){
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    State->id = stateId;
    State->name = stateName;
    State->songName = songName;
    State->next = NULL;
    return  EUROVISION_SUCCESS;
}

 /**
  * eurovisionRemoveState: removes a country from the eurovision
  * @param eurovision the eurovision the state gets removed from
  * @param stateId the id of the state that is getting removed
  * @return returns EUROVISION_INVALID_ID in case there is a problem with the id
  *         or EUROVISION_STATE_NOT_EXIST in case there is no state with said id
  *         or EUROVISION_SUCCESS in case of success
  */

 /*
  * Noam's version
 EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (stateId <= 0)
        return EUROVISION_INVALID_ID;
     if(!checkStateExists(eurovision, stateId))
         return EUROVISION_STATE_NOT_EXIST;
     states* State = getToState(eurovision, stateId);
     listDestroy((List)State);
     return EUROVISION_SUCCESS;
 }
*/

EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (eurovision->State == NULL)
        return EUROVISION_STATE_NOT_EXIST;
    if (stateId <= 0)
        return EUROVISION_INVALID_ID;
    int index = findState(eurovision,stateId);
    states *previous_state = eurovision->State;
    if (index == -1)
        return EUROVISION_STATE_NOT_EXIST;
    for (int i=0;i<index-1;i++){
        previous_state = previous_state->next;
    }
    free(previous_state->next->vote);
    free(previous_state->next);
    states *temp_state = previous_state->next;
    previous_state->next=temp_state->next;
    free(temp_state);
    return EUROVISION_SUCCESS;
}



/** eurovisionRemoveJudge: removes a judge from the eurovision
 *  @param eurovision the eurovision the judge is getting removed from
 *  @param judgeId the id of the judge that is getting removed
 *  @return EUROVISION_INVALID_ID in case there is a problem with the id
 *          or EUROVISION_JUDGE_NOT_EXIST in case the judge is not the eurovision
 *          or EUROVISION_SUCCESS in case or success
 */

/*
 * Noam's version:
 EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId){
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (judgeId <= 0)
        return EUROVISION_INVALID_ID;
     if (!checkJudgeExists(eurovision, judgeId))
         return EUROVISION_JUDGE_NOT_EXIST;
     judges* Judge = getToJudge(eurovision, judgeId);
     listDestroy((List)Judge);
    return EUROVISION_SUCCESS;
 }

*/

EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId){
    if (eurovision == NULL)
        return EUROVISION_NULL_ARGUMENT;
    if (eurovision->Judge == NULL)
        return EUROVISION_JUDGE_NOT_EXIST;
    if (judgeId <= 0)
        return EUROVISION_INVALID_ID;
    int index = findJudge(eurovision,judgeId);
    judges *previous_judge = eurovision->Judge;
    if (index == -1)
        return EUROVISION_JUDGE_NOT_EXIST;
    for (int i=0;i<index-1;i++){
        previous_judge = previous_judge->next;
    }
    free(previous_judge->next->vote);
    free(previous_judge->next);
    judges *temp_judge = previous_judge->next;
    previous_judge->next=temp_judge->next;
    free(temp_judge);
    return EUROVISION_SUCCESS;
}

/**
 * eurovisionAddVote: adds a civilian's vote in the country poles
 * @param eurovision the eurovision that the vote takes place in
 * @param stateGiver the id of the state in which the vote takes place in
 * @param stateTaker the id or the state which is getting voted
 * @return returns EUROVISION_INVALID_ID if there is a problem with one of the id
 *         or EUROVISION_STATE_NOT_EXIST in case one of the ids do not exist in the eurovision
 *         or EUROVISION_SAME_STATE in case they tried to vote for themselves (<--it is a word and it IS spelt like that)
 *         or EUROVISION_SUCCESS in case of success
 */
EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver, int stateTaker){
    //check all necessities as usual
    states* taker = getToState(eurovision, stateTaker);
    taker->vote = getToVote(taker, stateGiver);
    if (taker->vote == NULL)
    if(taker->vote->votes >= 1 ) {
        taker->vote->votes++;
        return EUROVISION_SUCCESS;
    }
}

/**
 * eurovisionRemoveVote: removes a single vote in a state
 * @param eurovision the eurovision that the vote took place in
 * @param stateGiver the id of the state that the removal takes place in
 * @param stateTaker the id of the state that the dude regretted voting for
 * @return EUROVISION_INVALID_ID in case there was a problem with the ids
 *         or EUROVISION_STATE_NOT_EXIST if the state is not in the eurovision
 *         or EUROVISION_SAME_STATE in case the vote was for the same country
 *         or EUROVISION_SUCCESS in case of success
 */
EurovisionResult eurovisionRemoveVote(Eurovision eurovision, int stateGiver,
                                      int stateTaker){
    return EUROVISION_SUCCESS;
}

/**
 * eurovisionRunContest: returns a list organized by most total points to least
 * @param eurovision the eurovision it runs
 * @param audiencePrecent the worth of the votes of states
 * @returns returns NULL if there was a problem while running
 *          or the List-head if there was a success
 */
List eurovisionRunContest(Eurovision eurovision, int audiencePrecent){
    return NULL;
}

/**
 * eurovisionRunAudienceFavorite : returns a list that is ordered from the most voted for(civilian wise) to least
 * @param eurovision the eurovision it checks in
 * @return returns NULL if there was a problem while running
 *         or the list-head if there was no problem what so ever
 */
List eurovisionRunAudienceFavorite(Eurovision eurovision){
    return NULL;
}

/**
 * eurovisionRunGetFriendlyStates: gives you a list of countries which gave 12 points to others
 * @param eurovision the eurovision it happens in
 * @return returns NULL if there a problem while running
 *         or a list of strings that looks like "*voter_COUNTRY_NAME - *voted_for_COUNTRY_NAME , ..."
 *         * the ',' represents the next list on the list
 */
List eurovisionRunGetFriendlyStates(Eurovision eurovision){
    return NULL;
}
