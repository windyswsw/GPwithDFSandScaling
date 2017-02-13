/*
 * GeneticProgram.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void GeneticProgramForScale(Population* CurrentPop, FullSol* BaseFullSolution, vector<ChangeInfo> RoundChangeList, CommonList* CurCommonList, int NoOfGenerations, float NoOfLinks, float NoOfServers, short int MaxLinksCap, int w1, int w2, int w3, int w4, int w5, bool OldMutation){

	for(int g=0;g < NoOfGenerations;g++){
		int FullSolSelected = 0;
		int popSize = CurrentPop->CurPopulaton.size()-1;

		if(CurrentPop->CurPopulaton.size() > 1){
			FullSolSelected = rand() % popSize+1;
		}

		int polrsize, polrpos, polr =0;
			
		if(RoundChangeList.size() == 1){
			polrpos =  0;					
			polr = RoundChangeList[polrpos].polToChange;
		}
		else{
			polrsize  = RoundChangeList.size() - 1;
			polrpos =  rand() % polrsize + 1;
			polr = RoundChangeList[polrpos].polToChange;
		}

		if(OldMutation == true){
			OldMutationForScale(CurrentPop, BaseFullSolution, CurCommonList, FullSolSelected, polr, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);
		}
		else{
			MutationForScale(CurrentPop, BaseFullSolution, CurCommonList, FullSolSelected, polr, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);
		}
	}

}
