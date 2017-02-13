/*
 * StructsUse.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

using namespace std;

struct Links{
	short int LinksBW[85][85];
	short int LinksUse[85][85];
};

struct SinglePath{
	vector<int> Path;
};

struct ReturnPath{
	SinglePath NewPath;
	bool NoPathFound;
};

struct Paths{
	vector<SinglePath> PathsList[65][65];
	vector<SinglePath> InPaths;
	vector<SinglePath> OutPaths;
};

struct Policy{
	vector<int> NFIDs;
	short int PolicyID;
	char ART;
	short int BW;
};

struct NFAllocation{
	short int ServerID;
	SinglePath path;
	short int PolicyID;
	short int NFID;
};

struct PartialSol{
	vector<NFAllocation> VMsForAPolicy;
};

struct VecForHashTable{
	set<int> ServerIndex;
};


struct ChangeInfo{
	int cus;
	int polToChange;
	string AR;
	int BWToChange;
};

struct RoundChangeInfo{
	vector<ChangeInfo> RoundChangeList;
};

struct UniqueList{
	map<short int,short int> ServerCapMap;
	unordered_multimap<short int, VecForHashTable> ServerCapHashTable;
	vector<short int> ServerCapArray;
	vector<short int> ServerUsedList;
	vector<int> mutsvrchange;
	vector<int> mutlinkchange;
	Links LinksLists;
};

struct CommonList{
	vector<Policy> PolicyList;
	Paths PathsList;
	vector<short int> NFCap;
	vector<ChangeInfo> FullChangeListCur;
};


struct FullSol{
	vector<PartialSol> VectorOfPartialSols;
	float ObjVal;
	UniqueList CurLists;
};

struct Population{
	vector<FullSol> CurPopulaton;
	CommonList CurCommonList;
};

struct Config{
	int NoOfGenerations;
	short int ServerCap;
	short int MaxLinksCap;
	bool OldMutation;
};
