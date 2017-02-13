/*
 * Mutation.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void MutationForScale(Population* CurrentPop, FullSol* BaseFullSolution, CommonList* CurCommonListNew, short int FullSolSelected, int MutationPol, float NoOfServers, float NoOfLinks, short int MaxLinksCap, int w1, int w2, int w3, int w4, int w5){

	short int policyIndex = MutationPol;
	int NoOfMutations = CurCommonListNew->PolicyList[policyIndex].NFIDs.size();

	short int BW = CurCommonListNew->PolicyList[policyIndex].BW;
	short int TotNFs = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy.size();

	FullSol basesol;
	basesol.CurLists = CurrentPop->CurPopulaton[FullSolSelected].CurLists;
	basesol.ObjVal = CurrentPop->CurPopulaton[FullSolSelected].ObjVal;
	basesol.VectorOfPartialSols = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols;

	for(short int i=0; i< NoOfMutations; i++){

		float OldObjVal = CurrentPop->CurPopulaton[FullSolSelected].ObjVal;

		int PosOfChangingNF = i;

		bool NoPathForNF = false;
		bool NoPathForRightNF = false;
		bool NoOutPath = false;
		bool NoInPath = false;

		NFAllocation NFtoChange = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF];
		short int NFtoChangeID = NFtoChange.NFID;
		short int NFtoChangeCurSvr = NFtoChange.ServerID;
		short int NFtoChangePolicyID = NFtoChange.PolicyID;
		SinglePath NFtoChangePath = NFtoChange.path;

		int NFtoChangeCapacity = CurCommonListNew->NFCap[NFtoChangeID];

		short int NFtoChangeNewSvr = 1000; // find a new server for NF

		std::map<short int,short int>::iterator itlow=  CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.lower_bound(NFtoChangeCapacity);
		int FoundCap = itlow->first;
		auto it2 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(FoundCap);
		std::set<int>::iterator itv = it2->second.ServerIndex.begin();
		NFtoChangeNewSvr = *itv;

		if ( NFtoChangeNewSvr != NFtoChangeCurSvr){
			
			if(PosOfChangingNF == (TotNFs-1) ){ // last NF in the chain

				NFAllocation LeftOfNFtoChange = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF-1];
				short int LeftOfNFtoChangeSvr = LeftOfNFtoChange.ServerID;

				if(NFtoChangePath.Path.size() > 1){ // there was a previously assigned path, so remove that path by adding the BW to relavant links, if no path then, NFtoChangePath.size() = 1
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"A");
				}

				SinglePath OutPathOld = Check_OutPath(NFtoChangeCurSvr,CurCommonListNew); // remove the previous out path by adding BW to relavant links
				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, OutPathOld, BW,"A");

				ReturnPath RPathNew;
				RPathNew = Find_OutPath(NFtoChangeNewSvr,CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
				SinglePath OutPathNew = RPathNew.NewPath;
				NoOutPath = RPathNew.NoPathFound; // add the new out path by deleting BW from relavant links

				if( NoOutPath == false){ // there is an out path, so continue

					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, OutPathNew, BW,"D");
					
					NFAllocation NewNF;
					SinglePath NFtoChangeNewPath;

					if(NFtoChangeNewSvr != LeftOfNFtoChangeSvr){
						ReturnPath RPath;
						RPath = Find_Shortest_PathRandom(LeftOfNFtoChangeSvr, NFtoChangeNewSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
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

						// update new server
						it2->second.ServerIndex.erase(NFtoChangeNewSvr);

						if(it2->second.ServerIndex.size() <= 0){
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.erase(FoundCap);
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.erase(FoundCap);
						}

						int NewCapOfNewServer = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeNewSvr] - NFtoChangeCapacity;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeNewSvr] = NewCapOfNewServer;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeNewSvr] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeNewSvr] + 1;

						std::map<short int,short int>::iterator  it3 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.find(NewCapOfNewServer);
						if( it3 != CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.end()){
							auto it4 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(NewCapOfNewServer);
							it4->second.ServerIndex.insert(NFtoChangeNewSvr);
						}
						else{
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.insert(std::pair<short int,short int>(NewCapOfNewServer,1));
							set<int> ValsNew;
							ValsNew.insert(NFtoChangeNewSvr);
							VecForHashTable NewVecForHashTable;
							NewVecForHashTable.ServerIndex = ValsNew;
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(NewCapOfNewServer,NewVecForHashTable));

						}

						// update old server
						int OldServerCap = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr];
						auto  it5 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(OldServerCap);
						it5->second.ServerIndex.erase(NFtoChangeCurSvr);

						if(it5->second.ServerIndex.size() <= 0){
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.erase(OldServerCap);
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.erase(OldServerCap);
						}

						int OldServerNewCap = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr] + NFtoChangeCapacity;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr] = OldServerNewCap;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeCurSvr] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeCurSvr] - 1;
											
						std::map<short int,short int>::iterator  it6 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.find(OldServerNewCap);
						if( it6 != CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.end()){
							auto it7 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(OldServerNewCap);
							it7->second.ServerIndex.insert(NFtoChangeCurSvr);
						}
						else{
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.insert(std::pair<short int,short int>(OldServerNewCap,1));
							set<int> ValsNew1;
							ValsNew1.insert(NFtoChangeCurSvr);
							VecForHashTable NewVecForHashTable1;
							NewVecForHashTable1.ServerIndex = ValsNew1;
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(OldServerNewCap,NewVecForHashTable1));
						}

						NewNF.ServerID = NFtoChangeNewSvr;
						NewNF.NFID = NFtoChangeID;
						NewNF.PolicyID = NFtoChangePolicyID;
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

				NFAllocation NewNF;
				NFAllocation NewNFforRightNF;
				SinglePath RightOfNFtoChangeNewPath;
				SinglePath NFtoChangeNewPath;

				Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"A"); // remove previous In path by adding the BW to relavant links

				ReturnPath RPath;
				RPath = Find_InPath(NFtoChangeNewSvr,CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
				NFtoChangeNewPath = RPath.NewPath;
				NoInPath = RPath.NoPathFound;

				if( NoInPath == false){ // there is an InPath
					
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangeNewPath, BW,"D");

					if(RightOfNFtoChangePath.Path.size() > 1){ // there was a previously assigned path for right side NF, so remove that path by adding the BW to relavant links
						Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangePath, BW,"A");
					}
					if(NFtoChangeNewSvr != RightOfNFtoChangeSvr){
						ReturnPath RPathNew;
						RPathNew = Find_Shortest_PathRandom(NFtoChangeNewSvr,RightOfNFtoChangeSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
						RightOfNFtoChangeNewPath = RPathNew.NewPath;
						NoPathForRightNF = RPathNew.NoPathFound;
					}
					else{
						RightOfNFtoChangeNewPath.Path.push_back(0);
					}

					if( NoPathForRightNF == false){ // there is a path to right NF or they are in same server
						
						if(RightOfNFtoChangeNewPath.Path.size() > 1 ){
							Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangeNewPath, BW,"D");
						}

						// update new server
						it2->second.ServerIndex.erase(NFtoChangeNewSvr);
						
						if(it2->second.ServerIndex.size() <= 0){
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.erase(FoundCap);
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.erase(FoundCap);
						}

						int NewCapOfNewServer = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeNewSvr] - NFtoChangeCapacity;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeNewSvr] = NewCapOfNewServer;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeNewSvr] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeNewSvr] + 1;

						std::map<short int,short int>::iterator  it3 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.find(NewCapOfNewServer);
						if( it3 != CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.end()){
							auto it4 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(NewCapOfNewServer);
							it4->second.ServerIndex.insert(NFtoChangeNewSvr);
						}
						else{
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.insert(std::pair<short int,short int>(NewCapOfNewServer,1));
							set<int> ValsNew;
							ValsNew.insert(NFtoChangeNewSvr);
							VecForHashTable NewVecForHashTable;
							NewVecForHashTable.ServerIndex = ValsNew;
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(NewCapOfNewServer,NewVecForHashTable));
						}

						// update old server
						int OldServerCap = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr];
						auto  it5 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(OldServerCap);
						it5->second.ServerIndex.erase(NFtoChangeCurSvr);
						
						if(it5->second.ServerIndex.size() <= 0){
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.erase(OldServerCap);
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.erase(OldServerCap);
						}

						int OldServerNewCap = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr] + NFtoChangeCapacity;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr] = OldServerNewCap;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeCurSvr] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeCurSvr] - 1;
						
						std::map<short int,short int>::iterator  it6 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.find(OldServerNewCap);
						if( it6 != CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.end()){
							auto it7 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(OldServerNewCap);
							it7->second.ServerIndex.insert(NFtoChangeCurSvr);
						}
						else{
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.insert(std::pair<short int,short int>(OldServerNewCap,1));
							set<int> ValsNew1;
							ValsNew1.insert(NFtoChangeCurSvr);
							VecForHashTable NewVecForHashTable1;
							NewVecForHashTable1.ServerIndex = ValsNew1;
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(OldServerNewCap,NewVecForHashTable1));
						}

						// NF to change
						NewNF.ServerID = NFtoChangeNewSvr;
						NewNF.NFID = NFtoChangeID;
						NewNF.PolicyID = NFtoChangePolicyID;
						NewNF.path = NFtoChangeNewPath;
						CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF] = NewNF;

						// Right of the NF to change
						NewNFforRightNF.ServerID = RightOfNFtoChangeSvr;
						NewNFforRightNF.PolicyID = RightOfNFtoChangePolicyID;
						NewNFforRightNF.NFID = RightOfNFtoChangeNFID;
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

				NFAllocation NewNF;
				NFAllocation NewNFforRightNF;

				if(NFtoChangePath.Path.size() > 1){ // there was a previously assigned path, so remove that path by adding the BW to relavant links, if no path then, NFtoChangePath.size() = 1
					Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangePath, BW,"A");
				}

				SinglePath NFtoChangeNewPath;
				SinglePath RightOfNFtoChangeNewPath;

				if(NFtoChangeNewSvr != LeftOfNFtoChangeSvr){
					ReturnPath RPath;
					RPath = Find_Shortest_PathRandom(LeftOfNFtoChangeSvr, NFtoChangeNewSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
					NFtoChangeNewPath = RPath.NewPath;
					NoPathForNF = RPath.NoPathFound;
				}
				else{
					NFtoChangeNewPath.Path.push_back(0);
				}

				if( NoPathForNF == false){ // there is a path from left NF, or they are in same server

					if(NFtoChangeNewPath.Path.size() > 1 ){
						Update_LinksMatrixMutation(CurrentPop, FullSolSelected, NFtoChangeNewPath, BW,"D");
					}
					
					if(RightOfNFtoChangePath.Path.size() > 1){ // there was a previously assigned path for right side NF, so remove that path by adding the BW to relavant links, if no path then, NFtoChangePath.size() = 1
						Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangePath, BW,"A");
					}

					if(NFtoChangeNewSvr != RightOfNFtoChangeSvr){
						ReturnPath RPathNew;
						RPathNew = Find_Shortest_PathRandom(NFtoChangeNewSvr,RightOfNFtoChangeSvr, CurCommonListNew, &CurrentPop->CurPopulaton[FullSolSelected].CurLists, BW);
						RightOfNFtoChangeNewPath = RPathNew.NewPath;
						NoPathForRightNF = RPathNew.NoPathFound;
					}
					else{
						RightOfNFtoChangeNewPath.Path.push_back(0);
					}

					if(NoPathForRightNF == false){ // there is a path to right NF
						
						if(RightOfNFtoChangeNewPath.Path.size() > 1 ){
							Update_LinksMatrixMutation(CurrentPop, FullSolSelected, RightOfNFtoChangeNewPath, BW,"D");
						}

						// update new server
						
						it2->second.ServerIndex.erase(NFtoChangeNewSvr);

						if(it2->second.ServerIndex.size() <= 0){
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.erase(FoundCap);
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.erase(FoundCap);
						}

						int NewCapOfNewServer = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeNewSvr] - NFtoChangeCapacity;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeNewSvr] = NewCapOfNewServer;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeNewSvr] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeNewSvr] + 1;
						
						std::map<short int,short int>::iterator  it3 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.find(NewCapOfNewServer);
						if( it3 != CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.end()){
							auto it4 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(NewCapOfNewServer);
							it4->second.ServerIndex.insert(NFtoChangeNewSvr);
						}
						else{
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.insert(std::pair<short int,short int>(NewCapOfNewServer,1));
							set<int> ValsNew;
							ValsNew.insert(NFtoChangeNewSvr);
							VecForHashTable NewVecForHashTable;
							NewVecForHashTable.ServerIndex = ValsNew;
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(NewCapOfNewServer,NewVecForHashTable));
						}

						// update old server
						int OldServerCap = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr];
						auto  it5 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(OldServerCap);
						it5->second.ServerIndex.erase(NFtoChangeCurSvr);
						
						if(it5->second.ServerIndex.size() <= 0){
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.erase(OldServerCap);
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.erase(OldServerCap);
						}

						int OldServerNewCap = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr] + NFtoChangeCapacity;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapArray[NFtoChangeCurSvr] = OldServerNewCap;
						CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeCurSvr] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerUsedList[NFtoChangeCurSvr] - 1;
						
						std::map<short int,short int>::iterator  it6 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.find(OldServerNewCap);
						if( it6 != CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.end()){
							auto it7 = CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.find(OldServerNewCap);
							it7->second.ServerIndex.insert(NFtoChangeCurSvr);
						}
						else{
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapMap.insert(std::pair<short int,short int>(OldServerNewCap,1));
							set<int> ValsNew1;
							ValsNew1.insert(NFtoChangeCurSvr);
							VecForHashTable NewVecForHashTable1;
							NewVecForHashTable1.ServerIndex = ValsNew1;
							CurrentPop->CurPopulaton[FullSolSelected].CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(OldServerNewCap,NewVecForHashTable1));
						}

						// NF to change
						NewNF.ServerID = NFtoChangeNewSvr;
						NewNF.NFID = NFtoChangeID;
						NewNF.PolicyID = NFtoChangePolicyID;
						NewNF.path = NFtoChangeNewPath;
						CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[policyIndex].VMsForAPolicy[PosOfChangingNF] = NewNF;
						
						// Right of the NF to change
						NewNFforRightNF.ServerID = RightOfNFtoChangeSvr;
						NewNFforRightNF.PolicyID = RightOfNFtoChangePolicyID;
						NewNFforRightNF.NFID = RightOfNFtoChangeNFID;
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
						if(NFtoChangeNewPath.Path.size() > 1){
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
		}
	}

	CurrentPop->CurPopulaton[FullSolSelected].CurLists.mutsvrchange[MutationPol] = 0;
	CurrentPop->CurPopulaton[FullSolSelected].CurLists.mutlinkchange[MutationPol] = 0;

	for(int j=0; j<basesol.VectorOfPartialSols[MutationPol].VMsForAPolicy.size();j++){

		NFAllocation NF = basesol.VectorOfPartialSols[MutationPol].VMsForAPolicy[j];
		short int NFSvr = NF.ServerID;
		SinglePath NFPath = NF.path;

		NFAllocation NFNew = CurrentPop->CurPopulaton[FullSolSelected].VectorOfPartialSols[MutationPol].VMsForAPolicy[j];
		short int NFNewSvr = NFNew.ServerID;
		SinglePath NFNewPath = NFNew.path;

		if(NFSvr != NFNewSvr){
			CurrentPop->CurPopulaton[FullSolSelected].CurLists.mutsvrchange[MutationPol] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.mutsvrchange[MutationPol] + 1;	
		}
				
		if(NFPath.Path != NFNewPath.Path){
			if(NFNewPath.Path.size() >1){
				CurrentPop->CurPopulaton[FullSolSelected].CurLists.mutlinkchange[MutationPol] = CurrentPop->CurPopulaton[FullSolSelected].CurLists.mutlinkchange[MutationPol] + 1;
			}
		}
	}

	ObjectValueCalculation(CurrentPop, FullSolSelected, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);

}
