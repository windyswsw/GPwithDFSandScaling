bool DFSRecursive(FullSol* InitialFullSolution, CommonList* CurCommonList, int policyIndex, PartialSol newVMMap, int CurrentNF){

	bool sol = false;
	short int BW = CurCommonList->PolicyList[policyIndex].BW;
	short int NoOfNFs = CurCommonList->PolicyList[policyIndex].NFIDs.size();

	short int NFID = CurCommonList->PolicyList[policyIndex].NFIDs[CurrentNF];
	short int NFCapacity = CurCommonList->NFCap[NFID];

	vector<int> ServerArr;

	for (std::map<short int,short int>::iterator itlow=  InitialFullSolution->CurLists.ServerCapMap.lower_bound(NFCapacity); itlow!=InitialFullSolution->CurLists.ServerCapMap.end(); ++itlow){
		int FoundCap = itlow->first;
		auto it2 = InitialFullSolution->CurLists.ServerCapHashTable.find(FoundCap);

		for (std::set<int>::iterator itv = it2->second.ServerIndex.begin(); itv != it2->second.ServerIndex.end(); ++itv){
			ServerArr.push_back(*itv);
		}
	}

	bool sucess = false;

	for(int h=0; h< ServerArr.size();h++){

		int next_pos = CurrentNF + 1;
		NFAllocation NewNF;

		bool Assigned = false;

		if(!(CurrentNF == (NoOfNFs-1)) ){ // not the last NF in the chain

			if(CurrentNF == 0 ){ // first NF in the chain
				short int NFSvr = ServerArr[h];

				bool NoInPath = false;

				ReturnPath RPath;
				RPath = Find_InPath(NFSvr,CurCommonList, &InitialFullSolution->CurLists, BW);
				SinglePath InPath = RPath.NewPath;
				NoInPath = RPath.NoPathFound;

				if( NoInPath == false){ // there is an InPath
					Update_LinksMatrix(InitialFullSolution, InPath, BW,"D"); // add the new out path by deleting BW from relavant links

					// update the server

					// First delete the entry for current capacity		
					int CurrentCapOfServer = InitialFullSolution->CurLists.ServerCapArray[NFSvr];
					auto it5 = InitialFullSolution->CurLists.ServerCapHashTable.find(CurrentCapOfServer);
					it5->second.ServerIndex.erase(NFSvr);

					if(it5->second.ServerIndex.size() <= 0){
						InitialFullSolution->CurLists.ServerCapHashTable.erase(CurrentCapOfServer);
						InitialFullSolution->CurLists.ServerCapMap.erase(CurrentCapOfServer);
					}
				
					// update the server with new capacity
					int NewCapOfServer = InitialFullSolution->CurLists.ServerCapArray[NFSvr] - NFCapacity;
					InitialFullSolution->CurLists.ServerCapArray[NFSvr] = NewCapOfServer;
					InitialFullSolution->CurLists.ServerUsedList[NFSvr] = InitialFullSolution->CurLists.ServerUsedList[NFSvr] + 1;

					std::map<short int,short int>::iterator  it3 = InitialFullSolution->CurLists.ServerCapMap.find(NewCapOfServer);
					if( it3 != InitialFullSolution->CurLists.ServerCapMap.end()){
						auto it4 = InitialFullSolution->CurLists.ServerCapHashTable.find(NewCapOfServer);
						it4->second.ServerIndex.insert(NFSvr);
					}
					else{
						InitialFullSolution->CurLists.ServerCapMap.insert(std::pair<short int,short int>(NewCapOfServer,1));
						set<int> ValsNew;
						ValsNew.insert(NFSvr);
						VecForHashTable NewVecForHashTable;
						NewVecForHashTable.ServerIndex = ValsNew;
						InitialFullSolution->CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(NewCapOfServer,NewVecForHashTable));
					}

					NewNF.ServerID = NFSvr;
					NewNF.NFID = NFID;
					NewNF.PolicyID = policyIndex + 1;
					NewNF.path = InPath;
					newVMMap.VMsForAPolicy.insert((newVMMap.VMsForAPolicy.begin()+CurrentNF), NewNF);

					Assigned = true;

					sucess = DFSRecursive(InitialFullSolution, CurCommonList, policyIndex, newVMMap, next_pos); // call to next NF 
				}
			}

			else { // not the first NF or not the last NF in the chain
				
				short int NFSvr = ServerArr[h];
				
				NFAllocation LeftNF = newVMMap.VMsForAPolicy[CurrentNF-1];
				short int LeftNFSvr = LeftNF.ServerID;

				SinglePath NFPath;
				ReturnPath RPath;
				bool NoNFPath = false;

				if(NFSvr != LeftNFSvr){
					RPath = Find_Shortest_Path(LeftNFSvr, NFSvr, CurCommonList, &InitialFullSolution->CurLists, BW);
					NFPath = RPath.NewPath;
					NoNFPath = RPath.NoPathFound;
				}
				else{
					NFPath.Path.push_back(0);
				}

				if(NoNFPath == false){ // there is a path, or NF and left NF are in same server, so continue
					if(NFPath.Path.size() > 1 ){
						Update_LinksMatrix(InitialFullSolution, NFPath, BW,"D"); // add the new out path by deleting BW from relavant links
					}

					// update the server
					
					// First delete the entry for current capacity		
						int CurrentCapOfServer = InitialFullSolution->CurLists.ServerCapArray[NFSvr];
						auto it5 = InitialFullSolution->CurLists.ServerCapHashTable.find(CurrentCapOfServer);
						it5->second.ServerIndex.erase(NFSvr);

						if(it5->second.ServerIndex.size() <= 0){
							InitialFullSolution->CurLists.ServerCapHashTable.erase(CurrentCapOfServer);
							InitialFullSolution->CurLists.ServerCapMap.erase(CurrentCapOfServer);
						}
				
						// update the server with new capacity
						int NewCapOfServer = InitialFullSolution->CurLists.ServerCapArray[NFSvr] - NFCapacity;
						InitialFullSolution->CurLists.ServerCapArray[NFSvr] = NewCapOfServer;
						InitialFullSolution->CurLists.ServerUsedList[NFSvr] = InitialFullSolution->CurLists.ServerUsedList[NFSvr] + 1;

						std::map<short int,short int>::iterator  it3 = InitialFullSolution->CurLists.ServerCapMap.find(NewCapOfServer);
						if( it3 != InitialFullSolution->CurLists.ServerCapMap.end()){
							auto it4 = InitialFullSolution->CurLists.ServerCapHashTable.find(NewCapOfServer);
							it4->second.ServerIndex.insert(NFSvr);
						}
						else{
							InitialFullSolution->CurLists.ServerCapMap.insert(std::pair<short int,short int>(NewCapOfServer,1));
							set<int> ValsNew;
							ValsNew.insert(NFSvr);
							VecForHashTable NewVecForHashTable;
							NewVecForHashTable.ServerIndex = ValsNew;
							InitialFullSolution->CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(NewCapOfServer,NewVecForHashTable));
						}

						NewNF.ServerID = NFSvr;
						NewNF.NFID = NFID;
						NewNF.PolicyID = policyIndex + 1;
						NewNF.path = NFPath;
						newVMMap.VMsForAPolicy.insert((newVMMap.VMsForAPolicy.begin()+CurrentNF), NewNF);

						Assigned = true;

						sucess = DFSRecursive(InitialFullSolution, CurCommonList, policyIndex, newVMMap, next_pos); // call to next NF
					}
				}

				if(sucess == true){
					return sucess;
				}
				else{ // revert back all

					if( Assigned == true){

						NFAllocation NFtoRevert = newVMMap.VMsForAPolicy[CurrentNF];
						short int NFtoRevertID = NFtoRevert.NFID;
						short int NFtoRevertCurSvr = NFtoRevert.ServerID;
						int NFtoRevertPolicyIndex = (NFtoRevert.PolicyID - 1);
						SinglePath NFtoChangePath = NFtoRevert.path;

						short int BW = CurCommonList->PolicyList[NFtoRevertPolicyIndex].BW;
						short int NFtoRevertCapacity = CurCommonList->NFCap[NFtoRevertID];

						Update_LinksMatrix(InitialFullSolution, NFtoChangePath, BW,"D"); // update the path

						// update the server

						// First delete the entry for current capacity		
						int CurrentCapOfNFtoRevertServer = InitialFullSolution->CurLists.ServerCapArray[NFtoRevertCurSvr];
						auto it5 = InitialFullSolution->CurLists.ServerCapHashTable.find(CurrentCapOfNFtoRevertServer);
						it5->second.ServerIndex.erase(NFtoRevertCurSvr);

						if(it5->second.ServerIndex.size() <= 0){
							InitialFullSolution->CurLists.ServerCapHashTable.erase(CurrentCapOfNFtoRevertServer);
							InitialFullSolution->CurLists.ServerCapMap.erase(CurrentCapOfNFtoRevertServer);
						}
				
						// update the server with new capacity
						int NewCapOfNFtoRevertServer = InitialFullSolution->CurLists.ServerCapArray[NFtoRevertCurSvr] + NFtoRevertCapacity;
						InitialFullSolution->CurLists.ServerCapArray[NFtoRevertCurSvr] = NewCapOfNFtoRevertServer;
						InitialFullSolution->CurLists.ServerUsedList[NFtoRevertCurSvr] = InitialFullSolution->CurLists.ServerUsedList[NFtoRevertCurSvr] - 1;

						std::map<short int,short int>::iterator  it3 = InitialFullSolution->CurLists.ServerCapMap.find(NewCapOfNFtoRevertServer);
						if( it3 != InitialFullSolution->CurLists.ServerCapMap.end()){
							auto it4 = InitialFullSolution->CurLists.ServerCapHashTable.find(NewCapOfNFtoRevertServer);
							it4->second.ServerIndex.insert(NFtoRevertCurSvr);
						}
						else{
							InitialFullSolution->CurLists.ServerCapMap.insert(std::pair<short int,short int>(NewCapOfNFtoRevertServer,1));
							set<int> ValsNew;
							ValsNew.insert(NFtoRevertCurSvr);
							VecForHashTable NewVecForHashTable;
							NewVecForHashTable.ServerIndex = ValsNew;
							InitialFullSolution->CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(NewCapOfNFtoRevertServer,NewVecForHashTable));
						}
					}
				}
					
			}

			else { // last NF in the chain

				short int NFSvr = ServerArr[h];
				
				NFAllocation LeftNF = newVMMap.VMsForAPolicy[CurrentNF-1];
				short int LeftNFSvr = LeftNF.ServerID;

				bool NoOutPath = false;
				ReturnPath RPath;
				RPath = Find_OutPath(NFSvr,CurCommonList, &InitialFullSolution->CurLists, BW);
				SinglePath OutPath = RPath.NewPath;
				NoOutPath = RPath.NoPathFound;

				if( NoOutPath == false){
					Update_LinksMatrix(InitialFullSolution, OutPath, BW,"D"); // add the new out path by deleting BW from relavant links
					
					bool NoNFPath = false;
					SinglePath NFPath;

					if(NFSvr != LeftNFSvr){
						RPath = Find_Shortest_Path(LeftNFSvr, NFSvr, CurCommonList, &InitialFullSolution->CurLists, BW);
						NFPath = RPath.NewPath;
						NoNFPath = RPath.NoPathFound;
					}
					else{
						NFPath.Path.push_back(0);
					}

					if(NoNFPath == false){ // there is a path, or NF and left NF are in same server, so continue
				
						if(NFPath.Path.size() > 1 ){
							Update_LinksMatrix(InitialFullSolution, NFPath, BW,"D"); // add the new out path by deleting BW from relavant links
						}

						// update the server

						// First delete the entry for current capacity		
						int CurrentCapOfServer = InitialFullSolution->CurLists.ServerCapArray[NFSvr];
						auto it5 = InitialFullSolution->CurLists.ServerCapHashTable.find(CurrentCapOfServer);
						it5->second.ServerIndex.erase(NFSvr);

						if(it5->second.ServerIndex.size() <= 0){
							InitialFullSolution->CurLists.ServerCapHashTable.erase(CurrentCapOfServer);
							InitialFullSolution->CurLists.ServerCapMap.erase(CurrentCapOfServer);
						}
				
						// update the server with new capacity
						int NewCapOfServer = InitialFullSolution->CurLists.ServerCapArray[NFSvr] - NFCapacity;
						InitialFullSolution->CurLists.ServerCapArray[NFSvr] = NewCapOfServer;
						InitialFullSolution->CurLists.ServerUsedList[NFSvr] = InitialFullSolution->CurLists.ServerUsedList[NFSvr] + 1;

						std::map<short int,short int>::iterator  it3 = InitialFullSolution->CurLists.ServerCapMap.find(NewCapOfServer);
						if( it3 != InitialFullSolution->CurLists.ServerCapMap.end()){
							auto it4 = InitialFullSolution->CurLists.ServerCapHashTable.find(NewCapOfServer);
							it4->second.ServerIndex.insert(NFSvr);
						}
						else{
							InitialFullSolution->CurLists.ServerCapMap.insert(std::pair<short int,short int>(NewCapOfServer,1));
							set<int> ValsNew;
							ValsNew.insert(NFSvr);
							VecForHashTable NewVecForHashTable;
							NewVecForHashTable.ServerIndex = ValsNew;
							InitialFullSolution->CurLists.ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(NewCapOfServer,NewVecForHashTable));
						}

							NewNF.ServerID = NFSvr;
							NewNF.NFID = NFID;
							NewNF.PolicyID = policyIndex + 1;
							NewNF.path = NFPath;
							newVMMap.VMsForAPolicy.insert((newVMMap.VMsForAPolicy.begin()+CurrentNF), NewNF);

							InitialFullSolution->VectorOfPartialSols.push_back(newVMMap); // everything went fine, so save the configurations
							sucess = true;
							return sucess;
					}
					else{ // No Path to left NF, so revert back the OutPath
						Update_LinksMatrix(InitialFullSolution, OutPath, BW,"A"); // delete the new out path by adding BW from relavant links
					}
				}
			}
		}
	sucess = false;
	return sucess;
}
