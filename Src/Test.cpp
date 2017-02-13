/*
 * Main.cpp
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

/*
 * Test.cpp
 *
 *  Created on: 25 June 2014
 *      Author: Windhya Rankothge
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iterator>
#include <map>
#include <time.h>
#include <algorithm>
#include <stdio.h>
#include <cstring>
#include <iomanip>
#include <stdarg.h>
#include <cstdarg>
#include <ctime>
#include <unordered_map>
#include <set>

#include "StructsUse.h"
#include "ReadConfig.h"
#include "FormatTime.h"
#include "GenLists.h"
#include "InitialListsGeneration.h"
#include "LinksPathsCheck.h"
#include "UpdateLists.h"
#include "ObjectValueCalculation.h"
#include "Print.h"
#include "PrintTime.h"
#include "DFS.h"
#include "InitialPolicyImplementation.h"
#include "Rewire.h"
#include "MutationForInitialSolution.h"
#include "OldMutationForInitialSolution.h"
#include "MutationForScale.h"
#include "OldMutationForScale.h"
#include "GeneticProgramForInitialSolution.h"
#include "GeneticProgramForScale.h"
#include "PolicyChangeListForRound.h"
#include "AddInstance.h"
#include "DeleteInstance.h"
#include "ScaleGlobal.h"
#include "ScaleLocal.h"

 int main(){

		Config ExperimentConfig = ReadConfig("ConfigFile.txt"); 
		int NoOfGenerations = ExperimentConfig.NoOfGenerations;
		short int ServerCap = ExperimentConfig.ServerCap;
		short int MaxLinksCap = ExperimentConfig.MaxLinksCap;
		bool OldMutation = ExperimentConfig.OldMutation;

		int experiments = 1;
		float NoOfServers = 64;
		float NoOfLinks = 88;
		bool initial = true;
		bool scaleglobal = true;
		bool scalelocal1 = true;
		bool scalelocal2 = true;
    
			// weights to use for initial policy implementation (Here we use only first 3 parameters) 
		int w1 = 1;
		int w2 = 1;
		int w3 = 1;
		int w4 = 0;
		int w5 = 0;

			// weights to use for local1 (All for full eq, last two when changes eq) 
		int sw1 = 1;
		int sw2 = 1;
		int sw3 = 1;
		int sw4 = 1;
		int sw5 = 1;

		// weights to use for local2 (All for full eq, last two when changes eq) 
		int lw1 = 0;
		int lw2 = 0;
		int lw3 = 0;
		int lw4 = 1;
		int lw5 = 1;

			// weights to use for global
		int gw1 = 1;
		int gw2 = 1;
		int gw3 = 1;
		int gw4 = 0;
		int gw5 = 0;

		time_t currentTime;
	    struct tm *currentDate;

	    time(&currentTime);
	    currentDate = localtime(&currentTime);

		char filename1[256] = {0};
		strcpy(filename1,"SummaryScaleLocal1");
		strcat(filename1, fmt("-%d-%d-%d@%d.%d.%d.csv", currentDate->tm_mday, currentDate->tm_mon+1, currentDate->tm_year+1900, currentDate->tm_hour, currentDate->tm_min, currentDate->tm_sec).c_str());
		ofstream myfile1;
		myfile1.open (filename1, ios::out | ios::app);

		/*char filename2[256] = {0};
		strcpy(filename2,"SummaryScaleLocal2");
		strcat(filename2, fmt("-%d-%d-%d@%d.%d.%d.csv", currentDate->tm_mday, currentDate->tm_mon+1, currentDate->tm_year+1900, currentDate->tm_hour, currentDate->tm_min, currentDate->tm_sec).c_str());
		ofstream myfile2;
		myfile2.open (filename2, ios::out | ios::app);*/

		char filename3[256] = {0};
		strcpy(filename3,"SummaryScaleGlobal");
		strcat(filename3, fmt("-%d-%d-%d@%d.%d.%d.csv", currentDate->tm_mday, currentDate->tm_mon+1, currentDate->tm_year+1900, currentDate->tm_hour, currentDate->tm_min, currentDate->tm_sec).c_str());
		ofstream myfile3;
		myfile3.open (filename3, ios::out | ios::app);

		srand((unsigned int) time(0));

		for(int r=0; r<experiments;r++){

			char filename1[256] = {0};
			char filename2[256] = {0};

			CommonList NewCommonList = GenCommonList(NoOfServers, ServerCap, NoOfLinks, MaxLinksCap);

			short int NoOfPolicies = NewCommonList.PolicyList.size();

			FullSol InitialFullSolution;
			InitialFullSolution.ObjVal = 0;
			InitialFullSolution.CurLists = GenUniqueList(NoOfServers, ServerCap, NoOfLinks, MaxLinksCap, NoOfPolicies);

			bool sucess = false;

			sucess = InitialPolicyImplementation(&InitialFullSolution, &NewCommonList, NoOfLinks, NoOfServers, MaxLinksCap);

			Population CurrentPop;
			CurrentPop.CurPopulaton.push_back(InitialFullSolution);

	 		if(sucess == true){
				//PrintSummaryLocal(r, InitialFullSolution, myfile1, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'D', w1, w2, w3, w4, w5);
				//PrintSummaryLocal(r, InitialFullSolution, myfile2, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'D', w1, w2, w3, w4, w5);
				//PrintSummaryLocal(r, InitialFullSolution, myfile3, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'D', w1, w2, w3, w4, w5);

				ObjectValueCalculation(&CurrentPop, 0, NoOfServers, NoOfLinks, MaxLinksCap, w1, w2, w3, w4, w5);

				for(short int i=0; i<10; i++){
					CurrentPop.CurPopulaton.push_back(CurrentPop.CurPopulaton[0]);
				}

				GeneticProgramForInitialSolution(&CurrentPop, &NewCommonList, NoOfGenerations, NoOfLinks, NoOfServers, MaxLinksCap, w1, w2, w3, w4, w5, OldMutation);

				PrintSummaryLocal(r, CurrentPop.CurPopulaton[0], myfile1, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'G',w1, w2, w3, w4, w5);
				//PrintSummaryLocal(r, CurrentPop.CurPopulaton[0], myfile2, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'G', w1, w2, w3, w4, w5);
				PrintSummaryLocal(r, CurrentPop.CurPopulaton[0], myfile3, NoOfServers, NoOfLinks, MaxLinksCap, 0,0,'G', w1, w2, w3, w4, w5);

	 			PrintNewLine(myfile1);
				//PrintNewLine(myfile2);
				PrintNewLine(myfile3);

				if(scaleglobal == true && initial == true){
					vector<ChangeInfo> FullChangeListCur3;
					gen_PolicyChangeListSpecial(&FullChangeListCur3, "ChangeInfo.txt");
					NewCommonList.FullChangeListCur = FullChangeListCur3;
					Population CurrentPop3 = CurrentPop;
					ScaleGlobal(&CurrentPop3.CurPopulaton[0], &NewCommonList, NoOfGenerations, NoOfServers, ServerCap, NoOfLinks, MaxLinksCap, gw1, gw2, gw3, gw4, gw5, myfile3, OldMutation);
				}

				if(scalelocal1 == true && initial == true){
					vector<ChangeInfo> FullChangeListCur1;
					gen_PolicyChangeListSpecial(&FullChangeListCur1, "ChangeInfo.txt");
					NewCommonList.FullChangeListCur = FullChangeListCur1;
					Population CurrentPop1 = CurrentPop;
					ScaleLocal(&CurrentPop1.CurPopulaton[0], &NewCommonList, 2000, NoOfServers, ServerCap, NoOfLinks, MaxLinksCap, sw1, sw2, sw3, sw4, sw5, myfile1, OldMutation);
				}

				/*if(scalelocal2 == true && initial == true){
					vector<ChangeInfo> FullChangeListCur2;
					gen_PolicyChangeListSpecial(&FullChangeListCur2, "ChangeInfo.txt");
					NewCommonList.FullChangeListCur = FullChangeListCur2;
					Population CurrentPop2 = CurrentPop;
					ScaleLocal(&CurrentPop2.CurPopulaton[0], &NewCommonList, 2000, NoOfServers, ServerCap, NoOfLinks, MaxLinksCap, lw1, lw2, lw3, lw4, lw5, myfile2, OldMutation);
				}*/

			}
		}

		myfile1.close();
		//myfile2.close();
		myfile3.close();

	return 0;
}



