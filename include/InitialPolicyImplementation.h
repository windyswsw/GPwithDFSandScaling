/*
 * InitialPolicyImplementation.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */


bool InitialPolicyImplementation(FullSol* InitialFullSolution, CommonList* CurCommonLists, float totlinks, float totsvrs, float MaxLinksCap){
	bool sucess = false;
	for(int p=0; p<=CurCommonLists->PolicyList.size()-1; p++){
		PartialSol newVMMap;
		int CurrentNF = 0;
		sucess = false;
		sucess = DFSRecursive(InitialFullSolution, CurCommonLists, p, newVMMap, CurrentNF);
		if(sucess == false){
			break;
		}
	}

	return sucess;
}
