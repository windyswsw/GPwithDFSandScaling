/*
 * GenLists.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

Config ReadConfig(const char* FILE_NAME){

	Config NewConfig;

	ifstream fileread (FILE_NAME);
	string line;

	while(getline(fileread,line)){
		istringstream iss(line);
		vector<string> tokens;
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));

		stringstream s1;
		int c1;
		s1 << tokens[1];
		s1 >> c1;
		NewConfig.NoOfGenerations = c1;

		stringstream s2;
		int c2;
		s2 << tokens[3];
		s2 >> c2;
		NewConfig.ServerCap = c2;

		stringstream s3;
		int c3;
		s3 << tokens[5];
		s3 >> c3;
		NewConfig.MaxLinksCap = c3;

		stringstream s4;
		bool c4;
		s4 << tokens[7];
		s4 >> c4;
		NewConfig.OldMutation = c4;
	}

	return NewConfig;
}