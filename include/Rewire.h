/*
 * Mutation.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void Rewire(Population* CurrentPop, CommonList* CurCommonListNew, short int FullSolSelected, int MutationPol, float NoOfServers, float NoOfLinks, short int MaxLinksCap, int w1, int w2, int w3, int w4, int w5){

	short int policyIndex = MutationPol;
	int NoOfMutations = CurCommonListNew->PolicyList[policyIndex].NFIDs.size();

	short int BW = CurCommonListNew->PolicyList[policyIndex].BW;
	short int TotNFs = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy.size();

	int VMPosSize = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy.size() - 1;
	int PosOfChangingNF = rand() % VMPosSize + 1;

	float OldObjVal = CurrentPop->CurPopulaton[FullSolSelected].ObjVal;

	bool NoPathForNF = false;
	bool NoPathForRightNF = false;
	bool NoOutPath = false;
	bool NoInPath = false;

	NFAllocation NFtoChange = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF];
	short int NFtoChangeID = NFtoChange.NFID;
	short int NFtoChangeCurSvr = NFtoChange.ServerID;
	short int NFtoChangePolicyID = NFtoChange.PolicyID;
	SinglePath NFtoChangePath = NFtoChange.path;
			
	if(PosOfChangingNF == (TotNFs-1) ){ // last NF in the chain
		
		NFAllocation LeftOfNFtoChange = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF-1];
		short int LeftOfNFtoChangeSvr = LeftOfNFtoChange.ServerID;

		if(NFtoChangePath.Path.size() > 1){ // there was a previously assigned path, so remove that path by adding the BW to relavant links, if no path then, NFtoChangePath.size() = 1
			Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"A");
		}

		SinglePath OutPathOld = Check_OutPath(NFtoChangeCurSvr,CurCommonListNew); // remove the previous out path by adding BW to relavant links
		Update_LinksMatrixMutation(CurrentPop, FullSolSelected, OutPathOld, BW,"A");
		
		ReturnPath RPathNew;
		RPathNew = Find_OutPath(NFtoChangeCurSvr,CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
		SinglePath OutPathNew = RPathNew.NewPath;
		NoOutPath = RPathNew.NoPathFound; // add the new out path by deleting BW from relavant links

		if( NoOutPath == false){ // there is an out path, so continue

			Update_LinksMatrixMutation(CurrentPop, FullSolSelected, OutPathNew, BW,"D");
					
			NFAllocation NewNF = NFtoChange;
			SinglePath NFtoChangeNewPath;
			bool NoPathForNF;

			if(NFtoChangeCurSvr != LeftOfNFtoChangeSvr){
				ReturnPath RPath;
				RPath = Find_Shortest_PathRandom(LeftOfNFtoChangeSvr, NFtoChangeCurSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
				NFtoChangeNewPath = RPath.NewPath;
				NoPathForNF = RPath.NoPathFound;
			}
			else{
				NFtoChangeNewPath.Path.push_back(0);
			}

			if(NoPathForNF == false){ // there is a path, or NF and left NF are in same server, so continue

				if(NFtoChangeNewPath.Path.size() > 1 ){
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangeNewPath, BW,"D");
				}

				NewNF.path = NFtoChangeNewPath;
				CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF] = NewNF;

			}
			else{ // there is no new path, so revert back
				if(NFtoChangePath.Path.size() > 1){ // add the old path back
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected,  NFtoChangePath, BW,"D");
				}
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, OutPathOld, BW,"D");
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, OutPathNew, BW,"A");
				CurrentPop->CurPopulaton[FullSolSelected].ObjVal = OldObjVal;
			}
		}
		else{ // there is no out path, so revert back
			if(NFtoChangePath.Path.size() > 1){ // add the old path back
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected,  NFtoChangePath, BW,"D");
			}
			Update_LinksMatrixMutation(CurrentPop, FullSolSelected, OutPathOld, BW,"D");
			CurrentPop->CurPopulaton[FullSolSelected].ObjVal = OldObjVal;
		}
	}

	else if(PosOfChangingNF == 0 ){

		NFAllocation RightOfNFtoChange = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF+1];
		short int RightOfNFtoChangeSvr = RightOfNFtoChange.ServerID;
		short int RightOfNFtoChangePolicyID = RightOfNFtoChange.PolicyID;
		short int RightOfNFtoChangeNFID = RightOfNFtoChange.NFID;
		SinglePath RightOfNFtoChangePath = RightOfNFtoChange.path;

		NFAllocation NewNF = NFtoChange;
		NFAllocation NewNFforRightNF = RightOfNFtoChange;
		SinglePath RightOfNFtoChangeNewPath;
		SinglePath NFtoChangeNewPath;

		Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"A"); // remove previous In path by adding the BW to relavant links
			
		ReturnPath RPath;
		RPath = Find_InPath(NFtoChangeCurSvr,CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
		NFtoChangeNewPath = RPath.NewPath;
		NoInPath = RPath.NoPathFound;

		if( NoInPath == false){ // there is an InPath
					
			Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangeNewPath, BW,"D");
			
			if(RightOfNFtoChangePath.Path.size() > 1){ // there was a previously assigned path for right side NF, so remove that path by adding the BW to relavant links
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangePath, BW,"A");
			}
			if(NFtoChangeCurSvr != RightOfNFtoChangeSvr){
				ReturnPath RPathNew;
				RPathNew = Find_Shortest_PathRandom(NFtoChangeCurSvr,RightOfNFtoChangeSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
				RightOfNFtoChangeNewPath = RPathNew.NewPath;
				NoPathForRightNF = RPathNew.NoPathFound;
			}
			else{
				RightOfNFtoChangeNewPath.Path.push_back(0);
			}

			if( NoPathForRightNF == false){ // there is a path to right NF or they are in same server
				
				if(NewNFforRightNF.path.Path.size() > 1 ){
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NewNFforRightNF.path, BW,"D");
				}

				// NF to change
				NewNF.path = NFtoChangeNewPath;
				CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF] = NewNF;

				// Right of the NF to change
				NewNFforRightNF.path = RightOfNFtoChangeNewPath;
				CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF+1] = NewNFforRightNF;

			}
			else{ // there is no path to right NF, so revert back

				if(NFtoChangePath.Path.size() > 1){ //revert back to old InPath
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"D");
				}
				if(RightOfNFtoChangePath.Path.size() > 1){  // revert back to the old path of right NF
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangePath, BW,"D");
				}
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangeNewPath, BW,"A"); // remove the newly assigned In path
				CurrentPop->CurPopulaton[FullSolSelected].ObjVal = OldObjVal;
			}
		}
		else{ //there is no InPath, so revert back
			if(NFtoChangePath.Path.size() > 1){ //revert back to old InPath
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"D");
			}
		}
	}

	else if ((PosOfChangingNF != 0) && (PosOfChangingNF != (TotNFs-1))) {

		NFAllocation RightOfNFtoChange = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF+1];
		short int RightOfNFtoChangeSvr = RightOfNFtoChange.ServerID;
		short int RightOfNFtoChangePolicyID = RightOfNFtoChange.PolicyID;
		short int RightOfNFtoChangeNFID = RightOfNFtoChange.NFID;
		SinglePath RightOfNFtoChangePath = RightOfNFtoChange.path;

		NFAllocation LeftOfNFtoChange = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF-1];
		short int LeftOfNFtoChangeSvr = LeftOfNFtoChange.ServerID;

		NFAllocation NewNF = NFtoChange;
		NFAllocation NewNFforRightNF = RightOfNFtoChange;

		if(NFtoChangePath.Path.size() > 1){ // there was a previously assigned path, so remove that path by adding the BW to relavant links, if no path then, NFtoChangePath.size() = 1
			Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"A");
		}

		SinglePath NFtoChangeNewPath;
		SinglePath RightOfNFtoChangeNewPath;

		if(NFtoChangeCurSvr != LeftOfNFtoChangeSvr){
			ReturnPath RPath;
			RPath = Find_Shortest_PathRandom(LeftOfNFtoChangeSvr, NFtoChangeCurSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
			NFtoChangeNewPath = RPath.NewPath;
			NoPathForNF = RPath.NoPathFound;
		}
		else{
			NFtoChangeNewPath.Path.push_back(0);
		}
		
		if( NoPathForNF == false){ // there is a path from left NF, or they are in same server
			Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangeNewPath, BW,"D");
			
			if(RightOfNFtoChangePath.Path.size() > 1){ // there was a previously assigned path for right side NF, so remove that path by adding the BW to relavant links, if no path then, NFtoChangePath.size() = 1
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangePath, BW,"A");
			}

			if(NFtoChangeCurSvr != RightOfNFtoChangeSvr){
				ReturnPath RPathNew;
				RPathNew = Find_Shortest_PathRandom(NFtoChangeCurSvr,RightOfNFtoChangeSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
				NFtoChangeNewPath = RPathNew.NewPath;
				NoPathForRightNF = RPathNew.NoPathFound;
			}
			else{
				RightOfNFtoChangeNewPath.Path.push_back(0);
			}

			if(NoPathForRightNF == false){ // there is a path to right NF
				NewNFforRightNF.path = RightOfNFtoChangeNewPath;
					
				if(NewNFforRightNF.path.Path.size() > 1 ){
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NewNFforRightNF.path, BW,"D");
				}

				// NF to change
				NewNF.path = NFtoChangePath;
				CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF] = NewNF;
						
				// Right of the NF to change
				NewNFforRightNF.path = RightOfNFtoChangeNewPath;
				CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF+1] = NewNFforRightNF;
			}
			else { // no path to right NF, revert back all
				if(RightOfNFtoChangePath.Path.size() > 1){
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangePath, BW,"D");
				}
				if(NFtoChangePath.Path.size() > 1){
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"D");
				}
				if(NewNF.path.Path.size() > 1){
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangeNewPath, BW,"A");
				}
				CurrentPop->CurPopulaton[FullSolSelected].ObjVal = OldObjVal;
			}
		}
		else { // there is no path to left NF, so revert back
			if(NFtoChangePath.Path.size() > 1){
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"D");
			}
			CurrentPop->CurPopulaton[FullSolSelected].ObjVal = OldObjVal;
		}
	}

	if(PosOfChangingNF == (TotNFs-1)){
		ObjectValueCalculation(CurrentPop, FullSolSelected, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);
	}
	else{
		ObjectValueCalculationWithOutSort(CurrentPop, FullSolSelected, NoOfServers, NoOfLinks, MaxLinksCap,  w1, w2, w3, w4, w5);
	}
	
}
