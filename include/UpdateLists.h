/*
 * UpdateLists.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void Update_LinksMatrix(FullSol* InitialFullSolution, SinglePath path, int BW, string mode){
	if(path.Path.size() > 1){
		if(mode == "D"){
			for(int l=0; l<path.Path.size()-1;l++){
				int newBW = InitialFullSolution->CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] - BW;

				InitialFullSolution->CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] = newBW;
				InitialFullSolution->CurLists.LinksLists.LinksBW[path.Path[l+1]][path.Path[l]] = newBW;

				InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]] = ++ InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]];
				InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]] = ++ InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]];
			}
		}

		if(mode == "A"){
			for(int l=0; l<path.Path.size()-1;l++){
				int newBW = InitialFullSolution->CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] + BW;

				InitialFullSolution->CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] = newBW;
				InitialFullSolution->CurLists.LinksLists.LinksBW[path.Path[l+1]][path.Path[l]] = newBW;

				InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]] = -- InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]];
				InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]] = -- InitialFullSolution->CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]];
			}
		}
	}

}

void Update_LinksMatrixMutation(Population* CurrentPop, short int FullSolSelected, SinglePath path, int BW, string mode){
	if(path.Path.size() > 1){
		if(mode == "D"){
			for(int l=0; l<path.Path.size()-1;l++){
				int newBW = CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] - BW;

				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] = newBW;
				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l+1]][path.Path[l]] = newBW;

				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]] = ++ CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]];
				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]] = ++ CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]];
			}
		}

		if(mode == "A"){
			for(int l=0; l<path.Path.size()-1;l++){
				int newBW = CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] + BW;

				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] = newBW;
				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l+1]][path.Path[l]] = newBW;

				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]] = -- CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l]][path.Path[l+1]];
				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]] = -- CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksUse[path.Path[l+1]][path.Path[l]];
			}
		}
	}

}

void Update_LinksMatrixForTraffic(Population* CurrentPop, short int FullSolSelected, SinglePath path, int BW, string mode){
	if(path.Path.size() > 1){
		if(mode == "D"){
			for(int l=0; l<path.Path.size()-1;l++){
				int newBW = CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] - BW;

				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] = newBW;
				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l+1]][path.Path[l]] = newBW;
			}
		}

		if(mode == "A"){
			for(int l=0; l<path.Path.size()-1;l++){
				int newBW = CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] + BW;

				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l]][path.Path[l+1]] = newBW;
				CurrentPop->CurPopulaton[FullSolSelected].CurLists.LinksLists.LinksBW[path.Path[l+1]][path.Path[l]] = newBW;
			}
		}
	}

}

UniqueList Update_ServerList(UniqueList List, short int Server, short int Capacity, string mode){

	/*if(mode == "A"){
		std::multimap<short int,short int>::iterator  it = List.ServerCapList.begin();
		while(it != List.ServerCapList.end()){
			if((it->second == Server)){
				int NewCapacityOfTheNewServer = it->first + Capacity;
				List.ServerCapList.erase(it);
				List.ServerCapList.insert(std::pair<short int,short int>(NewCapacityOfTheNewServer,Server));
				List.ServerUsedList[Server] = List.ServerUsedList[Server] - 1;
				break;
			}
			++it;
		}
	}

	if(mode == "D"){
		std::multimap<short int,short int>::iterator  it = List.ServerCapList.begin();
		while(it != List.ServerCapList.end()){
			if((it->second == Server)){
				int NewCapacityOfTheNewServer = it->first - Capacity;
				List.ServerCapList.erase(it);
				List.ServerCapList.insert(std::pair<short int,short int>(NewCapacityOfTheNewServer,Server));
				List.ServerUsedList[Server] = List.ServerUsedList[Server] + 1;
				break;
			}
			++it;
		}
	}*/


	return List;
}
