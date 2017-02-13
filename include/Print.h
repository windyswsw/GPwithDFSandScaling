/*
 * Print.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void PrintSummary(int exp, FullSol FinalSolution, ofstream &myfile, float totsvrs, float totlinks, short int MaxLinksCap, int Policy, int NF, char Op){

	float Obj = 0;
	int NoOfServersUsed = 0;
	int NoOfLinksUsed = 0;
	float totlinkscap = 0;

	for(int f=0; f<=totsvrs;f++){
		if(FinalSolution.CurLists.ServerUsedList[f] > 0){
			NoOfServersUsed = NoOfServersUsed + 1;
		}
	}

	int n = 0;
	for(int f=0; f<=totsvrs;f++){
		if(f != 0 && FinalSolution.CurLists.ServerCapArray[f] < 1000){
			n = n + 1;
		}
	}

	int a  = sizeof(FinalSolution.CurLists.LinksLists.LinksUse)/sizeof(FinalSolution.CurLists.LinksLists.LinksUse[0]);
	float LinksUsedPercent = 0;

	for(int i=0; i < a; ++i ){
		for(int j = 0; j < a; ++j){
			if(FinalSolution.CurLists.LinksLists.LinksUse[i][j] > 0){
				NoOfLinksUsed = NoOfLinksUsed + 1;
				LinksUsedPercent = LinksUsedPercent + (float) ((float) (MaxLinksCap - FinalSolution.CurLists.LinksLists.LinksBW[i][j])/ (float) MaxLinksCap);
				float usedBW = MaxLinksCap-FinalSolution.CurLists.LinksLists.LinksBW[i][j];
				totlinkscap = totlinkscap + usedBW;
			}
		}
	}

	Obj = (NoOfServersUsed/totsvrs) + (1 - (float) (NoOfLinksUsed/(totlinks * 2))) + (LinksUsedPercent/ (float) (totlinks*2));

	//myfile << " " << "," << exp  << "," << Op << "," << NoOfServersUsed << "," << (NoOfLinksUsed/2) << "," << (totlinkscap/2) << "," << LinksUsedPercent << "," << Obj << "\n";

	//Obj = (NoOfServersUsed/totsvrs) +  (float) (NoOfLinksUsed/(totlinks * 2)) + (LinksUsedPercent/ (float) (totlinks*2));

	myfile << " " << "," << exp  << "," << Op << "," << NoOfServersUsed << "," << (NoOfLinksUsed/2) << "," << (totlinkscap/2) << "," << (LinksUsedPercent/2) << "," << Obj << "\n";

}

void PrintSummaryLocal(int exp, FullSol FinalSolution, ofstream &myfile, float totsvrs, float totlinks, short int MaxLinksCap, int Policy, int NF, char Op, int w1, int w2, int w3, int w4, int w5){

	float Obj = 0;
	int NoOfServersUsed = 0;
	int NoOfLinksUsed = 0;
	int svrChange = 0;
	int linksChange = 0;
	float totlinkscap = 0;

	for(int f=0; f<=totsvrs;f++){
		if(FinalSolution.CurLists.ServerUsedList[f] > 0){
			NoOfServersUsed = NoOfServersUsed + 1;
		}
	}

	int a  = sizeof(FinalSolution.CurLists.LinksLists.LinksUse)/sizeof(FinalSolution.CurLists.LinksLists.LinksUse[0]);
	float LinksUsedPercent = 0;

	for(int i=0; i < a; ++i ){
		for(int j = 0; j < a; ++j){
			if(FinalSolution.CurLists.LinksLists.LinksUse[i][j] > 0){
				NoOfLinksUsed = NoOfLinksUsed + 1;
				LinksUsedPercent = LinksUsedPercent + (float) ((float)(MaxLinksCap - (FinalSolution.CurLists.LinksLists.LinksBW[i][j]))/ (float) MaxLinksCap);
				float usedBW = MaxLinksCap-(FinalSolution.CurLists.LinksLists.LinksBW[i][j]);
				totlinkscap = totlinkscap + usedBW;
			}
		}
	}

	float p1 = (NoOfServersUsed/totsvrs);
	float p2 = (1 - (float)(NoOfLinksUsed/(totlinks * 2)));
	float p3 = (LinksUsedPercent/ (float) (totlinks*2));

	for(size_t b=0; b<FinalSolution.CurLists.mutlinkchange.size(); b++){
		svrChange = svrChange + (FinalSolution.CurLists.mutsvrchange[b]);
	}

	for(size_t a=0; a<FinalSolution.CurLists.mutlinkchange.size(); a++){
		linksChange = linksChange + (FinalSolution.CurLists.mutlinkchange[a]);
	}

	float p5 = svrChange / totsvrs;
	float p6 = linksChange / totlinks;

	Obj = (w1*p1) + (w2*p2) + (w3*p3) + (w4*p5) + (w5*p6);

	myfile << " " << "," << exp  << "," << Op << "," << NoOfServersUsed << "," << (NoOfLinksUsed/2) << "," << (LinksUsedPercent/2) << "," << svrChange << "," << linksChange << "," << Obj ;

}

void PrintSummaryGlobal(int exp, FullSol FinalSolution, ofstream &myfile, float totsvrs, float totlinks, short int MaxLinksCap, int Policy, int NF, char Op, int w1, int w2, int w3, int w4, int w5, int SvrChanges, int LinksChanges){

	float Obj = 0;
	int NoOfServersUsed = 0;
	int NoOfLinksUsed = 0;
	int svrChange = 0;
	int linksChange = 0;
	float totlinkscap = 0;

	for(int f=0; f<=totsvrs;f++){
		if(FinalSolution.CurLists.ServerUsedList[f] > 0){
			NoOfServersUsed = NoOfServersUsed + 1;
		}
	}

	int a  = sizeof(FinalSolution.CurLists.LinksLists.LinksUse)/sizeof(FinalSolution.CurLists.LinksLists.LinksUse[0]);
	float LinksUsedPercent = 0;

	for(int i=0; i < a; ++i ){
		for(int j = 0; j < a; ++j){
			if(FinalSolution.CurLists.LinksLists.LinksUse[i][j] > 0){
				NoOfLinksUsed = NoOfLinksUsed + 1;
				LinksUsedPercent = LinksUsedPercent + (float) ((float)(MaxLinksCap - (FinalSolution.CurLists.LinksLists.LinksBW[i][j]))/ (float) MaxLinksCap);
				float usedBW = MaxLinksCap-(FinalSolution.CurLists.LinksLists.LinksBW[i][j]);
				totlinkscap = totlinkscap + usedBW;
			}
		}
	}

	float p1 = (NoOfServersUsed/totsvrs);
	float p2 = (1 - (float)(NoOfLinksUsed/(totlinks * 2)));
	float p3 = (LinksUsedPercent/ (float) (totlinks*2));

	for(size_t b=0; b<FinalSolution.CurLists.mutlinkchange.size(); b++){
		svrChange = svrChange + (FinalSolution.CurLists.mutsvrchange[b]);
	}

	for(size_t a=0; a<FinalSolution.CurLists.mutlinkchange.size(); a++){
		linksChange = linksChange + (FinalSolution.CurLists.mutlinkchange[a]);
	}

	float p5 = svrChange / totsvrs;
	float p6 = linksChange / totlinks;

	Obj = (w1*p1) + (w2*p2) + (w3*p3) + (w4*p5) + (w5*p6);

	myfile << " " << "," << exp  << "," << Op << "," << NoOfServersUsed << "," << (NoOfLinksUsed/2) << "," << (LinksUsedPercent/2) << "," << SvrChanges << "," << LinksChanges << "," << Obj ;

}

void PrintAllocation(int exp, FullSol FinalSolution, ofstream &myfile, float totsvrs, float totlinks, int Policy, int NF, char Op, int maxBW){

	myfile << exp << "," << Op << "\n";

	for(int i=0; i<FinalSolution.VectorOfPartialSols.size();i++){
		for(int j=0; j<FinalSolution.VectorOfPartialSols[i].VMsForAPolicy.size();j++){
			NFAllocation valnew = FinalSolution.VectorOfPartialSols[i].VMsForAPolicy[j];
			myfile << valnew.PolicyID << " ";
			myfile << valnew.ServerID << " ";
			for(int g=0; g< valnew.path.Path.size(); g++){
				myfile << valnew.path.Path[g] << "-";
			}
			myfile << "\n";
		}
	}

//	std::multimap<short int,short int>::iterator ita = FinalSolution.CurLists.ServerCapList.begin();
//	while(ita != FinalSolution.CurLists.ServerCapList.end()){
//		myfile << ita->second << " " << ita->first << " " ;
//		ita ++;
//	}
//	myfile << "\n";

	int a  = sizeof(FinalSolution.CurLists.LinksLists.LinksUse)/sizeof(FinalSolution.CurLists.LinksLists.LinksUse[0]);

	for(int i=0; i < a; ++i ){
		for(int j = 0; j < a; ++j){
			if(FinalSolution.CurLists.LinksLists.LinksUse[i][j] > 0){
				float usedBW = maxBW-FinalSolution.CurLists.LinksLists.LinksBW[i][j];
				myfile << i << "-" << j << " " << usedBW;
				myfile << "\n";
			}
		}
	}
	myfile << "\n";
}

void PrintNew(int p, int NF){
	ofstream myfile3;
	string filen = "rand.txt";
    myfile3.open (filen, ios::out | ios::app);
	myfile3 << p << "-" << NF << "\n";
	myfile3.close();

}

void printServerList(Population* CurrentPop, int NoOfServers, int gen){
	ofstream myfile;
    myfile.open ("ServerList.csv", ios::out | ios::app);

    myfile << gen << ", " ;
	for(size_t i=0; i<NoOfServers;i++){
		myfile << CurrentPop->CurPopulaton[0].CurLists.ServerCapArray[i] << ",";
	}
	myfile << "\n";
	myfile.close();
}
