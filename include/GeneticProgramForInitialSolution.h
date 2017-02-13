/*
 * GeneticProgram.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void GeneticProgramForInitialSolution(Population* CurrentPop, CommonList* CurCommonList, int NoOfGenerations, float NoOfLinks, float NoOfServers, short int MaxLinksCap, int w1, int w2, int w3, int w4, int w5, bool OldMutation){

	for(int g=0;g < NoOfGenerations;g++){
		int FullSolSelected = 0;
		int popSize = CurrentPop->CurPopulaton.size()-1;

		if(CurrentPop->CurPopulaton.size() > 1){
			FullSolSelected = rand() % popSize+1;
		}

		int polrsize = 0;
		int polr = 0;

		polrsize  = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols.size() - 1;
		polr =  rand() % polrsize + 1;

		if(OldMutation == true){
			OldMutationForInitialSolution(CurrentPop, CurCommonList, FullSolSelected, polr, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);
		}
		else{
			MutationForInitialSolution(CurrentPop, CurCommonList, FullSolSelected, polr, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);
		}
	}

}
