void ScaleGlobal(FullSol* InitialFullSolution, CommonList* CurCommonLists, int generations, float NoOfServers, short int ServerCap, float NoOfLinks, float MaxLinksCap, int w1, int w2, int w3, int w4, int w5,ofstream &myfilenew, bool OldMutation){

	FullSol basesol;
	basesol.CurLists = InitialFullSolution->CurLists;
	basesol.ObjVal = InitialFullSolution->ObjVal;
	basesol.VectorOfPartialSols = InitialFullSolution->VectorOfPartialSols;

	int popsize = 1;

	vector<vector <string> > vec;
	for(size_t i = 0; i < 5; i++){			
		vector<string> row;
		vec.push_back(row);
	}

	int f=0;

	Population CurrentPop;

	while(CurCommonLists->FullChangeListCur.size()>0){

		RoundChangeInfo RoundChangeNow;
		RoundChange(CurCommonLists, &RoundChangeNow);

		int newpopsize = 1;

		for(size_t d=0; d<RoundChangeNow.RoundChangeList.size(); d++){
			int pol = RoundChangeNow.RoundChangeList[d].polToChange;
			int BWChange = RoundChangeNow.RoundChangeList[d].BWToChange;
			string op = RoundChangeNow.RoundChangeList[d].AR;

			if(op == "A"){
				CurCommonLists->PolicyList[pol].BW = CurCommonLists->PolicyList[pol].BW + BWChange;
			}
			if(op == "D" || op == "P"){
				CurCommonLists->PolicyList[pol].BW = CurCommonLists->PolicyList[pol].BW - BWChange;
			}
		}

		for(size_t d=0; d<RoundChangeNow.RoundChangeList.size(); d++){
			int pol = RoundChangeNow.RoundChangeList[d].polToChange;
			int cus = RoundChangeNow.RoundChangeList[d].cus;

			string op = RoundChangeNow.RoundChangeList[d].AR;

			int changeIns = 0;
			string changePolIns;
			string delimiter;
			string token;
			string n1, n2;
			int ipol, iinst; 
			size_t posn = 0;
			vector<string> wordArr;

			if( op == "A"){ 
				int ChangeSize = 0;
				int changeAdd = 0;

				if(CurCommonLists->PolicyList[pol].NFIDs.size() > 1){
					ChangeSize  = CurCommonLists->PolicyList[pol].NFIDs.size() - 1;
					if(ChangeSize > 0){
						changeAdd =  rand() % ChangeSize + 1;
					}
					CurCommonLists->PolicyList[pol].NFIDs[changeAdd] = 8;  // instance is doubled now
				}

				stringstream ss;
				ss << pol << '-' << changeAdd;
				changePolIns = ss.str();
				vec[cus].push_back(changePolIns);
			}

			else if( op == "D"){
				changePolIns = vec[cus].back();
				vec[cus].pop_back();
				delimiter = "-";

				while ((posn = changePolIns.find(delimiter)) != std::string::npos) {
					 token = changePolIns.substr(0, posn);
					 changePolIns.erase(0, posn + delimiter.length());
				}

				n1 = token;
				n2 = changePolIns;

				istringstream ss1(n1) ;
				ss1 >> ipol;
				istringstream ss2(n2) ;
				ss2 >> iinst;

				CurCommonLists->PolicyList[pol].NFIDs[iinst] = 1; // original instance is assigned
			}
		}

		short int NoOfPolicies = CurCommonLists->PolicyList.size();

		FullSol NewInitialFullSolution;
		NewInitialFullSolution.ObjVal = 0;

		NewInitialFullSolution.CurLists = GenUniqueList(NoOfServers, ServerCap, NoOfLinks, MaxLinksCap, NoOfPolicies);

		bool sucess = false;

		sucess = InitialPolicyImplementation(&NewInitialFullSolution, CurCommonLists, NoOfLinks, NoOfServers, MaxLinksCap);

		//Population CurrentPop;
		CurrentPop.CurPopulaton.push_back(NewInitialFullSolution);

	 	if(sucess == true){
			
			ObjectValueCalculation(&CurrentPop, 0, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);

			for(short int i=0; i<10; i++){
				CurrentPop.CurPopulaton.push_back(CurrentPop.CurPopulaton[0]);
			}

			//PrintSummaryLocal(f, CurrentPop.CurPopulaton[0], myfilenew, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'S', w1, w2, w3, w4, w5);

			GeneticProgramForInitialSolution(&CurrentPop, CurCommonLists, generations, NoOfLinks, NoOfServers, MaxLinksCap, w1, w2, w3, w4, w5, OldMutation);

			int newsvrChanges = 0;
			int newlinkChanges = 0;

			for(int i=0; i<basesol.VectorOfPartialSols.size();i++){
				for(int j=0; j<basesol.VectorOfPartialSols[i].VMsForAPolicy.size();j++){

					NFAllocation NF = basesol.VectorOfPartialSols[i].VMsForAPolicy[j];
					short int NFSvr = NF.ServerID;
					SinglePath NFPath = NF.path;

					NFAllocation NFNew = CurrentPop.CurPopulaton[0].VectorOfPartialSols[i].VMsForAPolicy[j];
					short int NFNewSvr = NFNew.ServerID;
					SinglePath NFNewPath = NFNew.path;

					if(NFSvr != NFNewSvr){
						newsvrChanges = newsvrChanges + 1;		
					}
				
					if(NFPath.Path != NFNewPath.Path){
						if(NFNewPath.Path.size() >1){
							newlinkChanges =  newlinkChanges + (NFNewPath.Path.size()-1);
						}
					}
				}
			}

			PrintSummaryGlobal(f, CurrentPop.CurPopulaton[0], myfilenew, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'G',  w1, w2, w3, w4, w5,newsvrChanges,newlinkChanges );
	 		PrintNewLine(myfilenew);

			basesol = CurrentPop.CurPopulaton[0];
		}
		else{
			break;
		}

		while(CurrentPop.CurPopulaton.size() !=  0){
			CurrentPop.CurPopulaton.pop_back();
		}

		f++;
	}

	myfilenew.close();
}
