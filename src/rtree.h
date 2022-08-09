#ifndef RTREE_H_
#define RTREE_H_

#include <iostream>
#include <list>
#include <climits>
#include <fstream>
#include <cfloat>
#include <vector>
#include <map>
#include <cmath>

#define M 20
#define m 10
#define DIM 2 

#define nullptr NULL

// Geolocation vector type definition
typedef std::vector<float> GeoLoc;

// Forward declaration of RTree
class RTree;

/**
 * @brief Bounding box
 * 		top right and bottom left locations
 */
struct BoundingBox {
	GeoLoc top_right_, bottom_left_;
};

/**
 * @brief RTree node structure
 * 
 */
class RTreeNode {
    public:
		int id_; // Node ID

		/**
		 * @brief Geolocation of data stored
		 * 		= valid location, for nodes with data
		 * 		= nullptr, otherwise 
		*/
		GeoLoc loc_;

		/**
		 * @brief Number of children under this node
		 * 		= valid number, for internal node
		 *		= 0, for leaf node or data node
		 */
        int n_children_;

        bool is_leaf_; // denotes if node is leaf        

		/**
		 * @brief Bounding box for internal node
		 * 		= valid boundary, for internal nodes
		 * 		= nullptr, for leaf nodes
		 */
        BoundingBox* bb_;

        std::list<RTreeNode*> children_; // Inner node children
        
		RTreeNode* parent_; // Parent node

		RTreeNode(int id, GeoLoc &loc);
		RTreeNode();
		~RTreeNode();

		// Adjust bounding boxes
		void AdjustBB(GeoLoc &loc);
		void AdjustBB(BoundingBox &bb);
		BoundingBox* AdjustedBB(GeoLoc &loc);

		// Area modifications
		float ExpandedArea(BoundingBox* bb);
		float BoundedArea();
		float CombinedAreaLoc(GeoLoc &loc);
		float CombinedBoundingBoxArea(GeoLoc &loc);
		float CombinedBoundingBoxArea(BoundingBox* bb);
		float IncreasedArea(RTreeNode* entry);

		// Check if internal node is full
		bool FullChildren();

		// Remove child entry from an internal node
		void RemoveChildEntry(RTreeNode* child, bool change_bb = true);

		// Pick next child for processing
		RTreeNode* PickNext(RTreeNode* s_node);

		// Pick seeds for splitting an internal node
		void PickSeeds(RTreeNode* new_entry, RTreeNode** first_seed, RTreeNode** second_seed);

		// Split internal node into two
		RTreeNode* SplitNode(RTreeNode* new_entry);

		// Adjust tree bottom up to accomodate any downstream changes
		void AdjustTree(RTree *rtree, RTreeNode* snode_1, RTreeNode* snode_2);

		// Add node to internal node
		void AddChild(RTree *rtree, RTreeNode* new_entry);

		// Split and add new entry
		void SplitAddChild(RTree *rtree, RTreeNode* new_entry);

		// Choose internal leaf node to add new entry
		RTreeNode* ChooseLeaf(RTreeNode* new_entry);

		// Insert a new entry into RTree
		void Insert(RTree *rtree, RTreeNode* new_node);

		// Search for a query in RTree
		bool IsLocInRegion(GeoLoc &loc);
		void SearchNodes(GeoLoc &loc, std::map<int, std::vector<RTreeNode*>> &results);

		// Get neighborhood locations of a query
		float LatLonDisplacement(GeoLoc &loc1, GeoLoc &loc2);
		float GetMinDist(GeoLoc &loc);
		void SearchNearestBB(GeoLoc &loc, float& min_dist, std::map<int, std::vector<RTreeNode*>> &results, float lb_dist);
};

class RTree {
	private:
		RTreeNode* rtree_root_;
	public:
		RTree();
		~RTree();

		// Set and Get RTree root
		void SetRoot(RTreeNode* root) {rtree_root_ = root;}
		RTreeNode* GetRoot() { return rtree_root_;}

		void InsertNewNode(int id, GeoLoc &loc);
};

#endif
