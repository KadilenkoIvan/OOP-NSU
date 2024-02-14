#include <iostream>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>

using namespace std;
//using namespace boost;

int main() {
    typedef boost::adjacency_matrix<boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;

    Graph g(5);

    boost::add_edge(0, 1, 2, g);
    boost::add_edge(1, 2, 1, g);
    boost::add_edge(2, 3, 3, g);
    boost::add_edge(3, 4, 4, g);
    boost::add_edge(0, 4, 5, g);

    vector<vector<int>> distances(boost::num_vertices(g), vector<int>(boost::num_vertices(g)));
    boost::floyd_warshall_all_pairs_shortest_paths(g, distances);

    for (int i = 0; i < boost::num_vertices(g); ++i) {
        for (int j = 0; j < boost::num_vertices(g); ++j) {
            cout << "from " << i << " to " << j << ": ";
            if (distances[i][j] == numeric_limits<int>::max()) {
                cout << "~" << endl;
            }
            else {
                cout << distances[i][j] << endl;
            }
        }
    }

    return 0;
}
