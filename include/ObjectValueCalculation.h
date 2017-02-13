/*
 * ObjectValueCalculation.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

struct less_than_key{
	inline bool operator() (const FullSol &s1, const FullSol &s2){
		return (s1.ObjVal < s2.ObjVal);
	}
};

void sortSol(Population* CurrentPop){
	sort(CurrentPop->CurPopulaton.begin(),CurrentPop->CurPopulaton.end(),less_than_key());
	while(CurrentPop->CurPopulaton.size() > 10){
		CurrentPop->CurPopulaton.pop_back();
	}
}

void ObjectValueCalculation(Population* CurrentPop, short int FullSolChanged, float totsvrs, float totlinks, short int MaxLinksCap, int w1, int w2, int w3, int w4, int w5){
	float Obj = 0;
	int NoOfServersUsed = 0;
	int NoOfLinksUsed = 0;
	int svrChange = 0;
	int linksChange = 0;

	for(int f=0; f<=totsvrs;f++){
		if(CurrentPop->CurPopulaton[FullSolChanged].CurLists.ServerUsedList[f] > 0){
			NoOfServersUsed = NoOfServersUsed + 1;
		}
	}

	int a  = sizeof(CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksUse)/sizeof(CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksUse[0]);
	float LinksUsedPercent = 0;

	for(int i=0; i < a; ++i ){
		for(int j = 0; j < a; ++j){
			if(CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksUse[i][j] > 0){
				NoOfLinksUsed = NoOfLinksUsed + 1;
				LinksUsedPercent = LinksUsedPercent + (float) ((float)(MaxLinksCap - CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksBW[i][j])/ (float) MaxLinksCap);
			}
		}
	}

	float p1 = (NoOfServersUsed/totsvrs);
	float p2 = (1 - (float)(NoOfLinksUsed/(totlinks * 2)));
	float p3 = (LinksUsedPercent/ (float) (totlinks*2));

	for(size_t b=0; b<CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutlinkchange.size(); b++){
		svrChange = svrChange + CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutsvrchange[b];
	}

	for(size_t a=0; a<CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutlinkchange.size(); a++){
		linksChange = linksChange + CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutlinkchange[a];
	}

	float p5 = svrChange / totsvrs;
	float p6 = linksChange / totlinks;

	Obj = (w1*p1) + (w2*p2) + (w3*p3) + (w4*p5) + (w5*p6);

	if(CurrentPop->CurPopulaton[FullSolChanged].ObjVal != Obj){
		CurrentPop->CurPopulaton[FullSolChanged].ObjVal = Obj;
		if((CurrentPop->CurPopulaton.size()) > 1){
			sortSol(CurrentPop);
		}
	}

}

void ObjectValueCalculationWithOutSort(Population* CurrentPop, short int FullSolChanged, float totsvrs, float totlinks, short int MaxLinksCap, int w1, int w2, int w3, int w4, int w5){
	float Obj = 0;
	int NoOfServersUsed = 0;
	int NoOfLinksUsed = 0;
	int svrChange = 0;
	int linksChange = 0;

	for(int f=0; f<=totsvrs;f++){
		if(CurrentPop->CurPopulaton[FullSolChanged].CurLists.ServerUsedList[f] > 0){
			NoOfServersUsed = NoOfServersUsed + 1;
		}
	}

	int a  = sizeof(CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksUse)/sizeof(CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksUse[0]);
	float LinksUsedPercent = 0;

	for(int i=0; i < a; ++i ){
		for(int j = 0; j < a; ++j){
			if(CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksUse[i][j] > 0){
				NoOfLinksUsed = NoOfLinksUsed + 1;
				LinksUsedPercent = LinksUsedPercent + (float) ((float)(MaxLinksCap - CurrentPop->CurPopulaton[FullSolChanged].CurLists.LinksLists.LinksBW[i][j])/ (float) MaxLinksCap);
			}
		}
	}

	float p1 = (NoOfServersUsed/totsvrs);
	float p2 = (1 - (float)(NoOfLinksUsed/(totlinks * 2)));
	float p3 = (LinksUsedPercent/ (float) (totlinks*2));

	for(size_t b=0; b<CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutlinkchange.size(); b++){
		svrChange = svrChange + CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutsvrchange[b];
	}

	for(size_t a=0; a<CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutlinkchange.size(); a++){
		linksChange = linksChange + CurrentPop->CurPopulaton[FullSolChanged].CurLists.mutlinkchange[a];
	}

	float p5 = svrChange / totsvrs;
	float p6 = linksChange / totlinks;

	Obj = (w1*p1) + (w2*p2) + (w3*p3) + (w4*p5) + (w5*p6);

	if(CurrentPop->CurPopulaton[FullSolChanged].ObjVal != Obj){
		CurrentPop->CurPopulaton[FullSolChanged].ObjVal = Obj;
	}

}

