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
 *     eurovisionCreate - makes a eurovision competition.
 *     eurovisionDestroy - destroys the whole eurovision.
 *     eurovisionAddJudge - adds a judge
 *     eurovisionRemoveJudge - removes a judge
 *     eurovisionAddState - adds a state
 *     eurovisionRemoveState - removes a state
 *     eurovisionAddVote - adds a single vote
 *     eurovisionRemoveVote - removes a single vote
 *     eurovisionRunContest - returns a list of who got most points to who got the least point
 *     eurovisionRunAudienceFavorite - returns a list of most to least popular states
 *     eurovisionRunFriendlyStates - returns a list of which state was most popular in which state
 *
 * if we need more stuff we will add them
*/

/* ADT for judges part of the eurovision */

typedef struct judges {
    struct  judges *previous;
    int id;
    const char* name;
    int *vote;
    struct judges *next;
}judges;

/* ADT for states of the eurovision */

typedef struct states {
    int id;
    const char* name;
    const char* songName;
    int *vote;
    struct states *next;
}states;



/* ADT of the Eurovision struct */
struct eurovision_t {
    struct judges* Judge;
    struct states* State;
};




/* eurovision possible results*/


/**
 * eurovisionDestroy: deletes everything in a eurovision
 * @param 8urovision   the eurovision which is deleted
 */
void eurovisionDestroy(Eurovision eurovision){
    listDestroy((List)(eurovision->Judge));
    listDestroy((List)(eurovision->State));
    free(eurovision);
}
/**
 * eurovisionCreate: creates a new eurovision
 *
 * returns NULL in case of failiure
 *         ,or a eurovision in case of success.
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

static bool checkStateValid(Eurovision eurovision, int state_id){
    struct states* temp_state = eurovision->State;
    while(temp_state != NULL){
        if (temp_state->id == state_id)
            return true;
        else
            temp_state = eurovision->State->next;
    }
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

static bool checkName (const char* judgeName){
    const char* tempName = judgeName;
    while(tempName){
        if ((*tempName >= 'a' && *tempName <= 'z') || *tempName == ' ') {
            tempName++;
        }
        else
            return false;
    }
    return true;
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
    int* tempResult = judgeResults;
    bool flag;
    while(tempResult != NULL){
        flag = checkStateValid(eurovision, *tempResult);
        if (flag)
            break;
        else
            tempResult++;
    }
    if (!tempResult)
        return EUROVISION_STATE_NOT_EXIST;

    //check judge name:
    if (!checkName(judgeName))
        return EUROVISION_INVALID_NAME;
    //Creating the judge:
    //not sure how to work with judge->next
    eurovision->Judge = malloc(sizeof(eurovision->Judge));
    if (eurovision->Judge == NULL) {
        eurovisionDestroy(eurovision);
        return EUROVISION_OUT_OF_MEMORY;
    }
    eurovision->Judge->id = judgeId;
    eurovision->Judge->name = judgeName;
    eurovision->Judge->vote = judgeResults;
    eurovision->Judge->next = NULL;
    return EUROVISION_SUCCESS;
}

/**
 * eurovisionAddState: adds a country to the eurovision
 * @param eurovision   the eurovision which the country is added to
 * @param stateName  the name of the country          CREATES A COPY OF THE ARRAYS
 * @param songName  the song of the country
 * @param stateId    they asked for this?
 * @return returns EUROVISION_STATE_ALREADY_EXIST in case the country is already in the eaurovision
 *         or EUROVISION_INVALID_ID in case of an invalid id
 *         or EUROVISION_INVALID_NAME in case of an invalid name
 *         or EUROVISION_SUCCESS in case of success
 */
 EurovisionResult eurovisionAddState(Eurovision eurovision,int stateId, const char* stateName,
         const char* songName)
{
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
 EurovisionResult eurovisionRemoveState(Eurovision eurovision, int stateId){
     return EUROVISION_SUCCESS;
 }

/** eurovisionRemoveJudge: removes a judge from the eurovision
 *  @param eurovision the eurovision the judge is getting removed from
 *  @param judgeId the id of the judge that is getting removed
 *  @return EUROVISION_INVALID_ID in case there is a problem with the id
 *          or EUROVISION_JUDGE_NOT_EXIST in case the judge is not the eurovision
 *          or EUROVISION_SUCCESS in case or success
 */
 EurovisionResult eurovisionRemoveJudge(Eurovision eurovision, int judgeId){
     return EUROVISION_SUCCESS;
 }

/**
 * eurovisionAddVote: adds a civilian's vote in the country poles
 * @param eurovision the eurovision that the vote takes place in
 * @param stateGiver the id of the state in which the vote takes place in
 * @param stateTaker the id or the state which is getting voted
 * @return returns EUROVISION_INVALID_ID if there is a problem with one of the id
 *         or EUROVISION_STATE_NOT_EXIST in case one of the ids do not exist in the eurovision
 *         or EUROVISION_SAME_STATE in case they tried to vote for themselfs (<--it is a word and it IS spelt like that)
 *         or EUROVISION_SUCCESS in case of success
 */
EurovisionResult eurovisionAddVote(Eurovision eurovision, int stateGiver, int stateTaker){
    return EUROVISION_SUCCESS;
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
