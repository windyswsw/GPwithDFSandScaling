population DoLinksChanges(population currentpop,solution basesol,int PolToChange,int popsize,PathsMatrix PathsStr,int linksMaxBw, string OverLink, int BW){
	int pol = PolToChange;
	int newpopsize = popsize;
	
	for(int h=0; h< newpopsize ; h++){
		int forloopsize = currentpop.solpopulaton[h].PartialSol[pol].ParSolutionVMs.SolMap.size();

		for(int k=0; k< forloopsize; k++){
			// check instanes which were not checked from AddIns or DelIns
			MapVal val = currentpop.solpopulaton[h].PartialSol[pol].ParSolutionVMs.SolMap.find(k)->second;
				
			for(size_t f=0; f<val.path.size();f++){
				string path;
				if(val.path.size() > 0){
					path = val.path[f];
				}
				else{
					path = "no";
				}

				if(path != "no" && path != "same"){
					vector<string> links = Get_Links_From_Path(path);					
					TrafficChangeLinks RetTrafficChangeLinksInfo;
					for(size_t i=0; i<links.size();i++){
						if(links[i] == OverLink){
							int svr = val.server;
							string curPath = val.path[0];

							if(curPath != "no" && curPath != "same"){
								vector<string> linksnxt = Get_Links_From_Path(curPath);
								vector<LinksMatrix> RetMatnxt;
								for(size_t i=0; i<linksnxt.size();i++){
										RetMatnxt = Update_LinksMatrix(currentpop.solpopulaton[h].LinksCap, currentpop.solpopulaton[pol].LinksUseList, linksnxt[i], BW, "A");
										currentpop.solpopulaton[h].LinksCap = RetMatnxt[0];
										currentpop.solpopulaton[h].LinksUseList = RetMatnxt[1];
								}
								val.path.pop_back();
							}

							MapVal val2 = currentpop.solpopulaton[h].PartialSol[pol].ParSolutionVMs.SolMap.find(k-1)->second;
							int svrPre = val2.server;

							if(svr != svrPre){
								string ChangePaths = Find_Shortest_Paths(svr, svrPre,PathsStr);
								vector<string> linksnxt = Get_Links_From_Path(ChangePaths);
						
								vector<LinksMatrix> RetMatnxt;
								for(size_t i=0; i<linksnxt.size();i++){
										RetMatnxt = Update_LinksMatrix(currentpop.solpopulaton[h].LinksCap, currentpop.solpopulaton[pol].LinksUseList, linksnxt[i], BW, "D");
										currentpop.solpopulaton[h].LinksCap = RetMatnxt[0];
										currentpop.solpopulaton[h].LinksUseList = RetMatnxt[1];
								}
								val.path.push_back(ChangePaths);
							}
							break;
						}
					}
				}
			}
		}
	}

	return currentpop;
}

