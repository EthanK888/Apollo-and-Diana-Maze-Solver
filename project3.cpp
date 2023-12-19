#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <fstream>
#include <string>

//These variables are utilized in the recursive DFS function
bool found = false;
std::string path = "";

//Defining the properties of the vertices and edges of the graph
struct VertexProperties{
    int row, col;   //stores the row and column where the arrow is in the grid
    std::string color, direction;   //stores the color of the arrow and the direction the arrow is facing
    bool discovered;    //used for DFS
};

struct EdgeProperties{
    int distance;   //stores the distance between the 2 connected vertices
};

//Defines the type of graph used in the problem. Uses lists for the vertex and edge lists and is a directed graph
typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS, VertexProperties, EdgeProperties> Graph;

//Defining different graph types for ease of use later
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
typedef boost::graph_traits<Graph>::out_edge_iterator out_edge_iterator;
typedef boost::graph_traits<Graph>::adjacency_iterator adjacency_iterator_t;

//Function definitions
Graph BuildGraph();
void DFS(Graph g, vertex_t v, vertex_t prev_v = boost::graph_traits<Graph>::null_vertex());    //The first call of DFS doesn't have a previous vertex, so null_vertex() is default

int main(){

    Graph g = BuildGraph();

    vertex_iterator v = boost::vertices(g).first;
    DFS(g, *v);

    //When DFS is done, we have our path and output it to the file
    std::ofstream file;
    file.open("output.txt");
    file << path;
    file.close();

    return 0;
}

//Builds all vertices and edges of the graph
Graph BuildGraph(){
    using namespace boost;

    std::ifstream file;
    file.open("input.txt");
    if(!file.is_open()){
        std::cout << "Error opening file\n";
        abort();
    }

    //Getting the total number of rows and columns
    std::string rows;
    std::string columns;
    std::getline(file, rows, ' ');
    std::getline(file, columns, '\n');
    int r = std::stoi(rows);
    int c = std::stoi(columns);

    Graph g;

    //Loop through the entire grid to create the vertices
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            std::string direction;
            char color;
            int row, col;

            file.get(color);
            //The bullseye doesn't have the same format as all other cells, so it is treated differently (O is just saved to color)
            if(color != 'O'){
                file.get();     //discard the hyphen
                if(j == c - 1) std::getline(file, direction, '\n');     //final column has a newline character after direction, all others have a space
                else std::getline(file, direction, ' ');
            
                //Add the vertex with all its properties
                vertex_t v = add_vertex(g);
                g[v].color = color;
                g[v].direction = direction;
                g[v].row = i;
                g[v].col = j;
                g[v].discovered = false;
            }
            else{
                file.get();
                vertex_t v = add_vertex(g);
                g[v].color = color;
                g[v].direction = "No arrow";
                g[v].row = i;
                g[v].col = j;
                g[v].discovered = false;
            }
        }
    }


    //Now that all the vertices are created, we iterate through all of them to create the edges
    std::pair<vertex_iterator, vertex_iterator> V = vertices(g);
    for (vertex_iterator v = V.first; v != V.second; v++){
        
        //Check which direction the arrow is facing, then search all vertices for appropriate connections
        //break statements are used where possible to reduce complexity

        //N, S, E, W just need to look for vertices in the same row/column. Distance is defined in EdgeProperties{}
        if(g[*v].direction == "N"){
            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].row == g[*v].row) break;
                else if(g[*u].col == g[*v].col && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{abs(g[*u].row  - g[*v].row)}, g);
                }
            }
        }

        else if(g[*v].direction == "S"){
            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].row > g[*v].row && g[*u].col == g[*v].col && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{abs(g[*u].row  - g[*v].row)}, g);
                }
            }
        }

        else if(g[*v].direction == "E"){
            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].col > g[*v].col && g[*u].row == g[*v].row && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{abs(g[*u].col  - g[*v].col)}, g);
                }
            }
        }

        else if(g[*v].direction == "W"){
            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].col == g[*v].col && g[*u].row == g[*v].row) break;
                else if(g[*u].row == g[*v].row && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{abs(g[*u].col  - g[*v].col)}, g);
                }
            }
        }


        //NE, NW, SW, SE are more complicated. A count variable is used to keep track of the distance, 
        //and the vertices we're looking for have a row and column position that is +/- count depending on the direction
        else if(g[*v].direction == "NE"){
            int count;

            //For NE and NW, count must start at the max possible distance from v since we have to start iterating through vertices above v
            //In these cases, the starting value of count depends on the row and column of v
            if((c - 1) - g[*v].col >= g[*v].row) count = g[*v].row;
            else count = (c - 1) - g[*v].col;

            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].row == g[*v].row - count && g[*u].col == g[*v].col + count && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{count}, g);
                    count--;
                }
                //count still has to change at the appropriate moment, even if the edge isn't saved (because the color is the same)
                else if(g[*u].row == g[*v].row - count && g[*u].col == g[*v].col + count && g[*u].color == g[*v].color){
                    count--;
                }
                if(count == 0) break;   //stop once we have reached all possible vertices to have an edge with
            }
        }

        else if(g[*v].direction == "NW"){
            int count;

            if(g[*v].col >= g[*v].row) count = g[*v].row;
            else count = g[*v].col;

            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].row == g[*v].row - count && g[*u].col == g[*v].col - count && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{count}, g);
                    count--;
                }
                else if(g[*u].row == g[*v].row - count && g[*u].col == g[*v].col - count && g[*u].color == g[*v].color){
                    count--;
                }
                if(count == 0) break;
            }
        }

        else if(g[*v].direction == "SW"){
            //count can start at 1 for SW and SE since we iterate "downward" through the grid
            int count = 1;

            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].row == g[*v].row + count && g[*u].col == g[*v].col - count && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{count}, g);
                    count++;
                }
                else if(g[*u].row == g[*v].row + count && g[*u].col == g[*v].col - count && g[*u].color == g[*v].color){
                    count++;
                }
            }
        }

        else if(g[*v].direction == "SE"){
            int count = 1;

            for (vertex_iterator u = V.first; u != V.second; u++){
                if(g[*u].row == g[*v].row + count && g[*u].col == g[*v].col + count && g[*u].color != g[*v].color){
                    add_edge(*v, *u, EdgeProperties{count}, g);
                    count++;
                }
                else if(g[*u].row == g[*v].row + count && g[*u].col == g[*v].col + count && g[*u].color == g[*v].color){
                    count++;
                }
            }
        }
    }

    file.close();
    return g;
}

//Mostly standard DFS, with some changes to get the path taken and to stop DFS at the bullseye
//It is similar to the psuedocode in the report
void DFS(Graph g, vertex_t v, vertex_t prev_v){
    using namespace boost;

    g[v].discovered = true;     //mark v as discovered
    
    //if the bullseye is found, DFS changes
    if(g[v].color == "O"){
        found = true;
    }
    
    //Iterate through all neighbors of v
    std::pair<adjacency_iterator_t, adjacency_iterator_t> neighbors = adjacent_vertices(v, g);
    for(adjacency_iterator_t n = neighbors.first; n != neighbors.second; n++){
        if(!g[*n].discovered && !found) DFS(g, *n, v);  //DFS stops when the bullseye is found
    }
    
    //Once the bullseye is found, we start building the path
    if(found && prev_v != graph_traits<Graph>::null_vertex()){
        
        //This is where prev_v comes in. When DFS is finished, we work backwards through the graph and add how we got to v to the front of the path
        //The edges are needed because they store the distance, which is required for output
        std::pair<out_edge_iterator, out_edge_iterator> E = out_edges(prev_v, g);
        for(out_edge_iterator e = E.first; e != E.second; e++){
            //vertex_t source = boost::source(*e, g);
            vertex_t target = boost::target(*e, g);

            if(target == v){
                path = std::to_string(g[*e].distance) + (g[prev_v].direction) + ' ' + path;
                break;
            }
        }
    }
}