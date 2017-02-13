void RoundChange(CommonList* CurCommonLists, RoundChangeInfo* RoundChangeNow){

	int s = CurCommonLists->FullChangeListCur.size();

	for(size_t h=0; h<s; h++){
		if(CurCommonLists->FullChangeListCur[0].AR == "0" ){
			CurCommonLists->FullChangeListCur.erase(CurCommonLists->FullChangeListCur.begin());
			break;
		}
		else{
			RoundChangeNow->RoundChangeList.push_back(CurCommonLists->FullChangeListCur[0]);
			CurCommonLists->FullChangeListCur.erase(CurCommonLists->FullChangeListCur.begin());
		}
	}
}
