/*
 * InitialListsGeneration.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

UniqueList GenUniqueList(float NoOfServers, short int ServerCap, float NoOfLinks, short int BW, short int NoOfPolicies){

	UniqueList NewAllList;

	gen_ServerCapacityList(&NewAllList, NoOfServers, ServerCap);
	gen_LinksList(&NewAllList,"Links.txt", BW);

	for(int i=0; i<=NoOfServers;i++){
		NewAllList.ServerUsedList.push_back(0);
	}

	for(int i=0; i<NoOfPolicies; i++){
		NewAllList.mutsvrchange.push_back(0);
		NewAllList.mutlinkchange.push_back(0);
	}

	return NewAllList;
}


CommonList GenCommonList(float NoOfServers, short int ServerCap, float NoOfLinks, short int BW){

	CommonList NewAllList;

	gen_PolicyList(&NewAllList,"Policy.txt");
	GenAllPathsLists(&NewAllList,"Paths.txt", "InPaths.txt", "OutPaths.txt");
	gen_NFCapList(&NewAllList,"NFs.txt");
	gen_PolicyChangeList(&NewAllList, "ChangeInfo.txt");

	return NewAllList;

}
