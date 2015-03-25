/************************************************* 
Author: Hao Li(haolichina1991@gmail.com)
Date:2015-03-25 
Description: The basic solution is following.
1.find all holes in the labyrinth schema
2.for each hole, find the largest pathway by using a varietal dfs arithmetic.
  Different from dfs arithmetic, the arithmetic I used will remove the mark of the node that the code have passed when it return to a previous node.  
3.find the largest pathway from the largest pathways of all holes
**************************************************/ 

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <stack> 

using namespace std;

/*The struct to store location info of each element in the labyrinth*/ 
typedef struct LocationStruct{
	int x;//index of row
	int y;//index of column
	int flag;//the flag that records the direction that we can go from a hole; 0:up, left, down, right; 1:left, down, right; 2:down, right; 3.right; 4.no direction
}Location;

/************************************************* 
Function: readLabyrinthSchema 
Description: read labyrinth schema from a file
Param: 
1.file name to read from
2.two-dimensional vector to store labyrinth
*************************************************/  
void readLabyrinthSchema(const char *fileName, vector< vector<char> > &map){
	std::ifstream input(fileName);
	if(input.fail()) {
		std::cout<<"could not open file!"<< std::endl;
		return;
	}
	int count = 0;

	while (!input.eof() )  
	{  
		char buffer[1024];
		
		input.getline(buffer,1024);  

		vector<char> temp;
		for(int i=0; i<strlen(buffer); i++){
			temp.push_back(buffer[i]);
		}

		map.push_back(temp);
	}  
}

/************************************************* 
Function: writeResult 
Description: write result to a file
Param: 
1.file name to write to 
2.length of pathway
3.two-dimensional vector that stores result schema
*************************************************/  
void writeResult(const char *fileName, int step_num, const vector< vector<char> > &map){
	std::ofstream output(fileName) ;
	if(output.fail()) {
		std::cout<<"could not open file!"<< std::endl;
		return;
	}

	output << step_num << "\n";

	for(int i=0; i<map.size(); i++){
		vector<char> temp = map[i];
		for(int j=0; j<temp.size(); j++){
			output << temp[j];
		}
		output << "\n";
	}

	output.close();  
}

/************************************************* 
Function: findHoles 
Description: find all holes in the labyrinth
Param: 
1.two-dimensional vector that stores labyrinth 
2.vector to store holes' location
*************************************************/  
void findHoles(const vector< vector<char> > &map, vector<Location> &holes_location){
		for(int i=0; i<map.size(); i++){
			vector<char> temp = map[i];
			for(int j=0; j<temp.size(); j++){
				if(temp[j] == '.'){
					Location location = {i, j, 0};
					holes_location.push_back(location);
				}
			}
		}
}

/************************************************* 
Function: getLargestPathwayFromAHole 
Description: get the largest pathway from a hole
Param: 
1.two-dimensional vector that stores labyrinth 
2.location of the start hole
3.vector to store the path
*************************************************/  
void getLargestPathwayFromAHole(const vector< vector<char> > &map, const Location startHole, vector<Location> &path){
    vector< vector<char> > flags_tem = map;
	stack<Location> path_tem;

	stack<Location> s;  
	s.push(startHole);  
	flags_tem[startHole.x][startHole.y]='*';  

	//Location hole_tem = startHole;

	while(!s.empty())  
	{  
		Location hole_tem = s.top(); 

		if(hole_tem.x>0 && flags_tem[hole_tem.x-1][hole_tem.y]=='.' && hole_tem.flag==0){
			hole_tem.flag=1;
			s.pop();
			s.push(hole_tem);

			Location l_tem = {hole_tem.x-1, hole_tem.y, 0};
			s.push(l_tem);
			flags_tem[l_tem.x][l_tem.y]='*';  
		}
		else if(hole_tem.y>0 && flags_tem[hole_tem.x][hole_tem.y-1]=='.' && hole_tem.flag<=1){
			hole_tem.flag=2;
			s.pop();
			s.push(hole_tem);

			Location l_tem = {hole_tem.x, hole_tem.y-1, 0};
			s.push(l_tem);
			flags_tem[l_tem.x][l_tem.y]='*';
		}
		else if(hole_tem.x<map.size()-1 && flags_tem[hole_tem.x+1][hole_tem.y]=='.' && hole_tem.flag<=2){
			hole_tem.flag=3;
			s.pop();
			s.push(hole_tem);

			Location l_tem = {hole_tem.x+1, hole_tem.y, 0};
			s.push(l_tem);
			flags_tem[l_tem.x][l_tem.y]='*';
		}
		else if(hole_tem.y<map[0].size()-1 && flags_tem[hole_tem.x][hole_tem.y+1]=='.' && hole_tem.flag<=3){
			hole_tem.flag=4;
			s.pop();
			s.push(hole_tem);

			Location l_tem = {hole_tem.x, hole_tem.y+1, 0};
			s.push(l_tem);
			flags_tem[l_tem.x][l_tem.y]='*';
		}
		else{
			if(path_tem.size()<s.size()){
				path_tem=s;
			}

			flags_tem[hole_tem.x][hole_tem.y]=='.';
			s.pop();
		}
	}

	while(!path_tem.empty()){
		Location tem = path_tem.top(); 
		path.push_back(tem);
		path_tem.pop();
	}
}

/************************************************* 
Function: getLargestPathway 
Description: get the largest pathway of the labyrinth
Param: 
1.two-dimensional vector that stores labyrinth 
2.length of pathway
3.two-dimensional vector to store result schema
*************************************************/
void getLargestPathway(const vector< vector<char> > &map, int &step_num, vector< vector<char> > &result){
	vector<Location> holes_location;
	findHoles(map, holes_location);

	vector<Location> largest_path;

	for(int i=0; i<holes_location.size(); i++){
		vector<Location> path_tem;
		getLargestPathwayFromAHole(map, holes_location[i], path_tem);

		if(largest_path.size() < path_tem.size()){
			largest_path = path_tem;
		}
	}

	result = map;

	for(int i=0; i<largest_path.size(); i++){
		result[largest_path[i].x][largest_path[i].y] = '0'+i;
	}

	step_num = largest_path.size();

}

int main (int argc, char *argv[])
{
	const char *inFileName = "./input.txt";
    vector< vector<char> > map;
    vector< vector<char> > result;
	int step_num = 0;

	const char *outFileName = "./output.txt";

	readLabyrinthSchema(inFileName, map);
	getLargestPathway(map, step_num, result);
	writeResult(outFileName, step_num, result);

	return 0;
}
