/* Problem Statement: Given a transit system, determine fastest route from stop A to stop B.


Design Decisions:
input read from .txt file in GTFS format (eg. https://gtfs.org/schedule/reference/#stopstxt)

1. Map #1: Routes represented as an Adjacency List Data Structure
Transit System => map<string, set<pair<string, int>>>
Stops => string (stop_id)
Route Time => int (seconds)
- Chosen for ideal time and space efficiency
- Set was chosen over vector to avoid duplicates in added routes, a set fixes this elegantly.
  The reasoning is explained in more detail in Transit.cpp -> insertRoute()

2. Map #2: Stops data (stop_id, name)
stop_id (string) => name (string)
- A separate map will be created to index names to stop_ids. This makes insertion fast and easy.
  (Creating a Node class with stop_id/stop_name makes it more tedious to perform insertion due to
  having to create a Node each time before a uniqueness check).

3. Map #3: Stops data flipped (name, stop_id)
name (string) => stop_id (string)
- Again, a separate map is necessary for fast and easy access to existing stops from the CLI string input.

4. All transit functionality within Transit class
- insertRoute, getAdjacent, getStopName, getStopID, shortest path algorithms (Dijkstra, A* Search)
- constructor Transit(filepath) handles GTFS parsing and map populating

5. CLI menu
This will remain rooted in the main function.

- Design change: map -> unordered_map
  Removing the unneeded ordered map should improve performance.
  Unordered sets cause errors, so omitting that.

*/

#include "Transit.cpp"


string secondsToHMSFormat(float seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds - hours * 3600) / 60;
    int secs = seconds - (hours * 3600 + minutes * 60);

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hours << " H " << ": "
        << std::setw(2) << minutes << " M " << ": "
        << std::setw(2) << secs << " S ";
    
    return oss.str();
}

int main() {
    // string STOPS_FILE = "mock_data/stops.txt"; // default: "transit_data/stops.txt"
    string STOPS_FILE = "transit_data/stops.txt";
    // string STOP_TIMES_FILE = "mock_data/stop_times_debug.txt"; // default: "transit_data/stop_times.txt"
    string STOP_TIMES_FILE = "transit_data/stop_times.txt";

    Transit transit(STOPS_FILE, STOP_TIMES_FILE);

    // debugging
    transit.printRoutes();
    cout << endl;

    string stopA_name, stopB_name, stopA_id, stopB_id, exit;

    // Menu CLI
    cout << "Welcome to the NYC Subway Pathfinder!" << endl;
    cout << endl;

    while (true) {
        // User Input
        while (true) {
            cout << "Enter your destination:" << endl;
            getline(cin, stopA_name);

            if (transit.stopExists(stopA_name)) {
                // stopA_id = transit.getStopID(stopA_name);
                break;
            } else {
                cout << "ERROR: This stop does not exist." << endl;
            }
        }
        
        while (true) {
            cout << "Enter your current location:" << endl;
            getline(cin, stopB_name);

            if (transit.stopExists(stopB_name)) {
                // stopB_id = transit.getStopID(stopB_name);
                break;
            } else {
                cout << "ERROR: This stop does not exist." << endl;
            }
        }

        // A* Calculation
        // returns pair<path, time>
        
        tuple<string, float, chrono::microseconds> a_star_calc = transit.shortest_path_a_star(stopA_name, stopB_name);
        tuple<string, float, chrono::microseconds> dijkstrasCalc = transit.shortest_path_dijkstra(stopA_name, stopB_name);

        
        // Calculation & Output
        cout << endl;
        cout << "A* Fastest pathing: ";
        cout << get<0>(a_star_calc) << endl;
        cout << "A* Estimated Route Time: " << secondsToHMSFormat(get<1>(a_star_calc)) << endl;
        cout << "A* Search Algorithm runtime: " << get<2>(a_star_calc).count() << " microseconds" << endl;
        cout << endl;
        cout << "Dijkstra's Fastest pathing: ";
        cout << get<0>(dijkstrasCalc) << endl;
        cout << "Dijkstra's Estimated Route Time: " << secondsToHMSFormat(get<1>(dijkstrasCalc)) << endl;
        cout << "Dijkstra's Algorithm runtime: " << get<2>(dijkstrasCalc).count() << " microseconds" << endl;
        cout << endl;


        cout << "Exit application? (y to exit, any other character to find new route)" << endl;
        getline(cin, exit);

        // Termination
        if (exit == "y") {
            cout << "Exiting application..." << endl; 
            break;
        }
    }
    
    return 0;
}