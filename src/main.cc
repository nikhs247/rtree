#include "rtree.h"

void Tokenize(std::string line, std::string delim, std::vector<std::string> &tokens) {
    int start = 0;
    int end = line.find(delim);
    while (end != -1) {
        tokens.push_back(line.substr(start, end - start));
        start = end + delim.size();
        end = line.find(delim, start);
    }
    tokens.push_back(line.substr(start, end - start));
}


int main(int argc, char *argv[]){
	RTree *rtree = new RTree();

	std::string data_file = std::string(argv[1]);
	std::fstream fin;
    fin.open(data_file, std::ios::in);
	std::string line;

	GeoLoc loc(DIM, 0.0);
	while(fin >> line){
		std::vector<std::string> tokens;
		Tokenize(line, ",", tokens);

		int id = std::stoi(tokens[0]);

		// TODO: Accomodate any dimension
		loc[0] = std::stof(tokens[1]); // lat, x
		loc[1] = std::stof(tokens[2]); // lon, y

		rtree->InsertNewNode(id, loc);
		std::cout << "Inserted: " << id << std::endl;
	}
	fin.close();

	std::ofstream bb_file_handle;
	bb_file_handle.open(std::string(argv[2]), std::ios::out);
	rtree->PrintRTreeBB(bb_file_handle);
	bb_file_handle.close();

	return 0;
}