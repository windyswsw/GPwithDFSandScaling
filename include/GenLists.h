/*
 * GenLists.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void gen_ServerCapacityList(UniqueList* NewAllList, float NoOfServers, short int Cap){

	set<int> ValsNew;
	for(short int i=0; i<= NoOfServers; i++){
		if( i!=0){
	   		ValsNew.insert(i);
		}
		NewAllList->ServerCapArray.push_back(Cap);
	}

	VecForHashTable FirstVecForHashTable;
	FirstVecForHashTable.ServerIndex = ValsNew;

	NewAllList->ServerCapMap.insert(std::pair<short int,short int>(Cap,1));
	NewAllList->ServerCapHashTable.insert(std::pair<short int,VecForHashTable>(Cap,FirstVecForHashTable));

}

void gen_LinksList(UniqueList* NewAllList, const char* FILE_NAME, short int BW ){

	ifstream fileread (FILE_NAME);
	string line;

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));

		string mystr = tokens[0];
		string newword;
		stringstream stream(mystr);
		vector<string> wordArr;
		while(getline(stream, newword, '-')){
		   wordArr.push_back(newword);
		}
		int i1, i2;
		istringstream (wordArr[0]) >> i1;
		istringstream (wordArr[1]) >> i2;

		NewAllList->LinksLists.LinksBW[i1][i2] = BW;
		NewAllList->LinksLists.LinksBW[i2][i1] = BW;

		NewAllList->LinksLists.LinksUse[i1][i2] = 0;
		NewAllList->LinksLists.LinksUse[i2][i1] = 0;
	}

}

void gen_NFCapList(CommonList* NewAllList,const char* FILE_NAME){

	ifstream fileread (FILE_NAME);
	string line;

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
		stringstream s;
		int c;
		s << tokens[1];
		s >> c;
		NewAllList->NFCap.push_back(c);
	}

}

void gen_PathsList(CommonList* NewAllList,const char* FILE_NAME){

	ifstream fileread (FILE_NAME);
	string line;

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));

		string mystr = tokens[0];
		string newword;
		stringstream stream(mystr);
		vector<string> wordArr;
		while(getline(stream, newword, '-')){
		   wordArr.push_back(newword);
		}
		int i1, i2;
		istringstream (wordArr[0]) >> i1;
		istringstream (wordArr[1]) >> i2;

		SinglePath PathNew;
		string mystr2 = tokens[1];
		string newword2;
		stringstream stream2(mystr2);
		vector<int> NodeArr;
		while(getline(stream2, newword2, ',')){
			string mystr1 = newword2;
			string newword3;
			stringstream stream(mystr1);
			while(getline(stream, newword3, '-')){
				int c;
				stringstream s;
				s << newword3;
				s >> c;

				int NodeArrSize = NodeArr.size()-1;
				if(NodeArrSize < 0 ){
					NodeArr.push_back(c);
				}
				else{
					if(NodeArr[NodeArrSize] != c){
						NodeArr.push_back(c);
					}
				}
			}
		}

		PathNew.Path = NodeArr;
		NewAllList->PathsList.PathsList[i1][i2].push_back(PathNew);
	}

}

void gen_InPathsVector(CommonList* NewAllList, const char* FILE_NAME){

	ifstream fileread (FILE_NAME);
	string line;

	SinglePath PathNewNull;
	vector<int> NodeArrNull;
	NodeArrNull.push_back(0);
	PathNewNull.Path = NodeArrNull;
	NewAllList->PathsList.InPaths.push_back(PathNewNull); // server 0 NULL

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));

		int Pos;
		stringstream PosStr;
		PosStr << tokens[0];
		PosStr >> Pos;

		SinglePath PathNew;
		string mystr2 = tokens[1];
		string newword2;
		stringstream stream2(mystr2);
		vector<int> NodeArr;
		while(getline(stream2, newword2, ',')){
			string mystr1 = newword2;
			string newword3;
			stringstream stream(mystr1);
			while(getline(stream, newword3, '-')){
				int c;
				stringstream s;
				s << newword3;
				s >> c;

				int NodeArrSize = NodeArr.size()-1;
				if(NodeArrSize < 0 ){
					NodeArr.push_back(c);
				}
				else{
					if(NodeArr[NodeArrSize] != c){
						NodeArr.push_back(c);
					}
				}
			}
		}

		PathNew.Path = NodeArr;
		NewAllList->PathsList.InPaths.push_back(PathNew);
	}

}

void gen_OutPathsVector(CommonList* NewAllList, const char* FILE_NAME){


	ifstream fileread (FILE_NAME);
	string line;

	SinglePath PathNewNull;
	vector<int> NodeArrNull;
	NodeArrNull.push_back(0);
	PathNewNull.Path = NodeArrNull;
	NewAllList->PathsList.OutPaths.push_back(PathNewNull); // server 0 NULL

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));

		int Pos;
		stringstream PosStr;
		PosStr << tokens[0];
		PosStr >> Pos;

		SinglePath PathNew;
		string mystr2 = tokens[1];
		string newword2;
		stringstream stream2(mystr2);
		vector<int> NodeArr;
		while(getline(stream2, newword2, ',')){
			string mystr1 = newword2;
			string newword3;
			stringstream stream(mystr1);
			while(getline(stream, newword3, '-')){
				int c;
				stringstream s;
				s << newword3;
				s >> c;

				int NodeArrSize = NodeArr.size()-1;
				if(NodeArrSize < 0 ){
					NodeArr.push_back(c);
				}
				else{
					if(NodeArr[NodeArrSize] != c){
						NodeArr.push_back(c);
					}
				}
			}
		}

		PathNew.Path = NodeArr;
		NewAllList->PathsList.OutPaths.push_back(PathNew);
	}

}

void GenAllPathsLists(CommonList* NewAllList, const char* Paths, const char* InPaths, const char* OutPaths){
	gen_PathsList(NewAllList, Paths);
	gen_InPathsVector(NewAllList, InPaths);
	gen_OutPathsVector(NewAllList, OutPaths);
}

void gen_PolicyList(CommonList* NewAllList, const char* FILE_NAME){

	ifstream fileread (FILE_NAME);
	string line;

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
		Policy NewPolicy;

		stringstream s;
		char c;
		s << tokens[1];
		s >> c;
		NewPolicy.ART = c;

		stringstream s1;
		int c1;
		s1 << tokens[2];
		s1 >> c1;
		NewPolicy.PolicyID = c1;

		stringstream s3;
		int c3;
		s3 << tokens[4];
		s3 >> c3;
		NewPolicy.BW = c3;

		for(size_t i=6;i<tokens.size();i++){
			stringstream ss;
			int cc;
			ss << tokens[i];
			ss >> cc;
			NewPolicy.NFIDs.push_back(cc);
		}

		NewAllList->PolicyList.push_back(NewPolicy);
	}
}

void gen_PolicyChangeList(CommonList* NewAllList, const char* FILE_NAME){

	ifstream fileread (FILE_NAME);
	string line;

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
		ChangeInfo NewChange;
		stringstream s1;
		int c1;
		s1 << tokens[0];
		s1 >> c1;
		NewChange.cus = c1;
		stringstream s2;
		int c2;
		s2 << tokens[1];
		s2 >> c2;
		NewChange.polToChange = c2;

		NewChange.AR = tokens[2];
		stringstream s;
		int c;
		s << tokens[3];
		s >> c;
		NewChange.BWToChange = c;

		NewAllList->FullChangeListCur.push_back(NewChange);
	}
}

void gen_PolicyChangeListSpecial(vector<ChangeInfo>* FullChangeListCur, const char* FILE_NAME){

	ifstream fileread (FILE_NAME);
	string line;

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
		ChangeInfo NewChange;
		stringstream s1;
		int c1;
		s1 << tokens[0];
		s1 >> c1;
		NewChange.cus = c1;
		stringstream s2;
		int c2;
		s2 << tokens[1];
		s2 >> c2;
		NewChange.polToChange = c2;

		NewChange.AR = tokens[2];
		stringstream s;
		int c;
		s << tokens[3];
		s >> c;
		NewChange.BWToChange = c;

		FullChangeListCur->push_back(NewChange);
	}
}
