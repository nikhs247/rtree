#include "rtree.h"
#include <chrono>
typedef std::chrono::duration<int,std::milli> msec;

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

bool cmp(std::pair<int, float> c1, std::pair<int, float> c2){
	return c1.second < c2.second;
}

int main(int argc, char *argv[]){
	RTree *rtree = new RTree();

	std::string data_file = std::string(argv[1]);
	int dp_threshold = std::stoi(std::string(argv[2]));
	std::fstream fin;
    fin.open(data_file, std::ios::in);
	std::string line;

	GeoLoc loc(DIM, 0.0);
	int n_locs = 0;

	auto start = std::chrono::high_resolution_clock::now();
	while(fin >> line){
		std::vector<std::string> tokens;
		Tokenize(line, ",", tokens);

		int id = std::stoi(tokens[0]);

		// TODO: Accomodate any dimension

		loc[0] = std::stof(tokens[1]); // lat, x
		loc[1] = std::stof(tokens[2]); // lon, y

		rtree->InsertNewNode(id, loc);
		n_locs++;
		if(n_locs == dp_threshold) break;
		std::cout << "Inserted: " << id << std::endl;
	}
	fin.close();

	auto stop = std::chrono::high_resolution_clock::now();
	auto insert_duration = std::chrono::duration_cast<msec>(stop - start);
	double get_insert_latency = std::chrono::duration_cast<msec>(insert_duration).count();
	std::cout << "Number of data points:  " << n_locs << std::endl;
	std::cout << "Overall insertion time: " << get_insert_latency << std::endl;
	std::cout << "Average insertion time: " << get_insert_latency / (float)n_locs << std::endl;

	GeoLoc search_loc = {0.2, 0.4};
	
	/**************RTree*************/
	std::map<int, GeoLoc *> concentric_space;
	// // Search for neighbors of a point
	start = std::chrono::high_resolution_clock::now();
	rtree->SearchConcentricNeighbors(search_loc, concentric_space);
	std::vector<std::pair<int, float>> dist_hosts;
	for (auto it = concentric_space.begin(); it != concentric_space.end(); ++it){
		GeoLoc *comp_loc = it->second;
		float x = (search_loc[0] - comp_loc->at(0)) * (search_loc[0] - comp_loc->at(0));
		float y = (search_loc[1] - comp_loc->at(1)) * (search_loc[1] - comp_loc->at(1));

		float dist = x + y;
		dist_hosts.push_back(std::make_pair(it->first, dist));
	}
	sort(dist_hosts.begin(), dist_hosts.end(), cmp);
	stop = std::chrono::high_resolution_clock::now();
	auto search_duration = std::chrono::duration_cast<msec>(stop - start);
	double get_search_latency = std::chrono::duration_cast<msec>(search_duration).count();

	std::cout << "Number of enitities to search: " << concentric_space.size() << std::endl;
	std::cout << "Search latency: " << get_search_latency << std::endl;

	/**************Map*************/
	// std::vector<std::pair<int, float>> dist_hosts;
	// for (auto it = dp_map.begin(); it != dp_map.end(); ++it){
	// 	GeoLoc comp_loc = it->second;
	// 	float x = (search_loc[0] - comp_loc[0]) * (search_loc[0] - comp_loc[0]);
	// 	float y = (search_loc[1] - comp_loc[1]) * (search_loc[1] - comp_loc[1]);

	// 	float dist = x + y;
	// 	dist_hosts.push_back(std::make_pair(it->first, dist));
	// }
	// sort(dist_hosts.begin(), dist_hosts.end(), cmp);
	// stop = std::chrono::high_resolution_clock::now();
	// auto search_duration = std::chrono::duration_cast<msec>(stop - start);
	// double get_search_latency = std::chrono::duration_cast<msec>(search_duration).count();

	// std::cout << "Number of enitities to search: " << dp_map.size() << std::endl;
	// std::cout << "Search latency: " << get_search_latency << std::endl;

	// std::ofstream bb_file_handle;
	// bb_file_handle.open(std::string(argv[2]), std::ios::out);
	// rtree->PrintRTree(bb_file_handle, "BB");
	// bb_file_handle.close();

	// std::ofstream data_bb_file_handle;
	// data_bb_file_handle.open(std::string(argv[3]), std::ios::out);
	// rtree->PrintRTree(data_bb_file_handle, "DATABB");
	// data_bb_file_handle.close();

	return 0;
}