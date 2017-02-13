void ScaleLocal(FullSol* InitialFullSolution, CommonList* CurCommonLists, int generations, float NoOfServers, short int ServerCap, float NoOfLinks, float MaxLinksCap, int w1, int w2, int w3, int w4, int w5,ofstream &myfilenew, bool OldMutation){

	FullSol basesol;
	basesol.CurLists = InitialFullSolution->CurLists;
	basesol.ObjVal = InitialFullSolution->ObjVal;
	basesol.VectorOfPartialSols = InitialFullSolution->VectorOfPartialSols;

	Population CurrentPop;
	CurrentPop.CurPopulaton.push_back(basesol);

	vector<vector <string> > vec;
	for(size_t i = 0; i < 5; i++){			
		vector<string> row;
		vec.push_back(row);
	}

	bool NoEnoughBW = false;
	bool NoSucess = false;

	int f=0;

	while(CurCommonLists->FullChangeListCur.size()>0){

		if( NoEnoughBW == true || NoSucess == true){
			break;
		}

		vector<Policy> OriginalPolicyList = CurCommonLists->PolicyList;

		while(CurrentPop.CurPopulaton.size() > 1){
			CurrentPop.CurPopulaton.pop_back();
		}
			
		for(int g=0; g<CurrentPop.CurPopulaton[0].CurLists.mutsvrchange.size();g++){
			CurrentPop.CurPopulaton[0].CurLists.mutsvrchange[g] = 0;
		}
			
		for(int g=0; g<CurrentPop.CurPopulaton[0].CurLists.mutlinkchange.size();g++){
			CurrentPop.CurPopulaton[0].CurLists.mutlinkchange[g] = 0;
		}

		basesol = CurrentPop.CurPopulaton[0];

		RoundChangeInfo RoundChangeNow;
		RoundChange(CurCommonLists, &RoundChangeNow);

		for(size_t d=0; d<RoundChangeNow.RoundChangeList.size(); d++){
			if( NoEnoughBW == true){
				break;
			}

			int pol = RoundChangeNow.RoundChangeList[d].polToChange;
			int cus = RoundChangeNow.RoundChangeList[d].cus;
			int BWChange = RoundChangeNow.RoundChangeList[d].BWToChange;
			
			for(int k=0; k<CurrentPop.CurPopulaton[0].VectorOfPartialSols[pol].VMsForAPolicy.size(); k++){
				SinglePath P = CurrentPop.CurPopulaton[0].VectorOfPartialSols[pol].VMsForAPolicy[k].path;
				if( P.Path.size() > 1){
					NoEnoughBW = Link_Check(P, &CurrentPop.CurPopulaton[0].CurLists, BWChange);
				}

				if( NoEnoughBW == true){
					break;
				}
			}
		}

		if(NoEnoughBW == false && NoSucess == false){

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
						NoSucess = AddInstance(&CurrentPop, CurCommonLists, pol, changeAdd, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);

						if(NoSucess == true){
							break;
						}
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

					int OriginalInst = OriginalPolicyList[pol].NFIDs[iinst];
					DeleteInstance(&CurrentPop, CurCommonLists, pol, iinst, OriginalInst, NoOfServers, NoOfLinks, MaxLinksCap,w1, w2, w3, w4, w5);
				}
			}

			if(NoSucess == false){

				for(size_t d=0; d<RoundChangeNow.RoundChangeList.size(); d++){

					int pol = RoundChangeNow.RoundChangeList[d].polToChange;
					int cus = RoundChangeNow.RoundChangeList[d].cus;
					int BWChange = RoundChangeNow.RoundChangeList[d].BWToChange;
					string op = RoundChangeNow.RoundChangeList[d].AR;
			
					for(int k=0; k<CurrentPop.CurPopulaton[0].VectorOfPartialSols[pol].VMsForAPolicy.size(); k++){
						SinglePath P = CurrentPop.CurPopulaton[0].VectorOfPartialSols[pol].VMsForAPolicy[k].path;
						if( P.Path.size() > 1){
							if( op == "A" | op == "Q" ){
								Update_LinksMatrixForTraffic(&CurrentPop, 0, P, BWChange,"D"); // add the new BW by deleting BW from relavant links
							}
							if( op == "D" | op == "P"){
								Update_LinksMatrixForTraffic(&CurrentPop, 0, P, BWChange,"A"); // remove the new BW by adding BW to relavant links
							}
						}
					}

					// take care of the out path in last NF
					int lastNF = CurrentPop.CurPopulaton[0].VectorOfPartialSols[pol].VMsForAPolicy.size() - 1;
					int lastNFsvr = CurrentPop.CurPopulaton[0].VectorOfPartialSols[pol].VMsForAPolicy[lastNF].ServerID;

					SinglePath OutPathLastNF = Check_OutPath(lastNFsvr,CurCommonLists);
					if( op == "A" | op == "Q" ){
						Update_LinksMatrixForTraffic(&CurrentPop, 0, OutPathLastNF, BWChange,"D"); // add the new BW by deleting BW from relavant links
					}
					if( op == "D" | op == "P"){
						Update_LinksMatrixForTraffic(&CurrentPop, 0, OutPathLastNF, BWChange,"A"); // remove the new BW by adding BW to relavant links
					}

				}

				ObjectValueCalculation(&CurrentPop, 0, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);

				for(short int i=0; i<10; i++){
					CurrentPop.CurPopulaton.push_back(CurrentPop.CurPopulaton[0]);
				}

                                //PrintSummaryLocal(f, CurrentPop.CurPopulaton[0], myfilenew, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'S', w1, w2, w3, w4, w5);

				GeneticProgramForScale(&CurrentPop, &basesol, RoundChangeNow.RoundChangeList, CurCommonLists, generations, NoOfLinks, NoOfServers, MaxLinksCap, w1, w2, w3, w4, w5, OldMutation);

				PrintSummaryLocal(f, CurrentPop.CurPopulaton[0], myfilenew, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'G', w1, w2, w3, w4, w5);
	 			PrintNewLine(myfilenew);
			}
		}
		f++;
	}

	myfilenew.close();
}
