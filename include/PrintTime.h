/*
 * PrintTime.h
 *
 *  Created on: Oct 19, 2015
 *      Author: u99319
 */

void PrintTime(int s,int time1, int time2){
	ofstream myfile;
    myfile.open ("Time.csv", ios::out | ios::app);
	myfile <<  s << "," <<  time1 << "," << time2 << "\n";
	myfile.close();
}

void PrintDis(string info){
	ofstream myfile;
    myfile.open ("Time.csv", ios::out | ios::app);
	myfile <<  info << "\n";
	myfile.close();
}

void PrintBothTime(int exp, double time1, double time2, ofstream &myfile){
	myfile <<  exp << "," <<  time1 << "," << time2 << "\n";

}

void PrintNewLine(ofstream &myfile){
	myfile <<  "\n";
}