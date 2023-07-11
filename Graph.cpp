#include <bits/stdc++.h>

using namespace std;

class Graph_M {
public:
    class Vertex {
    public:
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

    int numVetex() {
        return vtces.size();
    }

    bool containsVertex(const string& vname) {
        return vtces.count(vname) != 0;
    }

    void addVertex(const string& vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(const string& vname) {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (const auto& kvp : vtx.nbrs) {
            keys.push_back(kvp.first);
        }

        for (const string& key : keys) {
            Vertex nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges() {
        int count = 0;
        for (const auto& kvp : vtces) {
            const Vertex& vtx = kvp.second;
            count += vtx.nbrs.size();
        }
        return count / 2;
    }

    bool containsEdge(const string& vname1, const string& vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2))
            return false;

        const Vertex& vtx1 = vtces[vname1];
        return vtx1.nbrs.count(vname2) != 0;
    }

    void addEdge(const string& vname1, const string& vname2, int value) {
        if (!containsVertex(vname1) || !containsVertex(vname2) || containsEdge(vname1, vname2))
            return;

        Vertex& vtx1 = vtces[vname1];
        vtx1.nbrs[vname2] = value;

        Vertex& vtx2 = vtces[vname2];
        vtx2.nbrs[vname1] = value;
    }

    void removeEdge(const string& vname1, const string& vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2) || !containsEdge(vname1, vname2))
            return;

        Vertex& vtx1 = vtces[vname1];
        vtx1.nbrs.erase(vname2);

        Vertex& vtx2 = vtces[vname2];
        vtx2.nbrs.erase(vname1);
    }

    void display_Map() {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl << endl;

        for (const auto& kvp : vtces) {
            string key = kvp.first;
            const Vertex& vtx = kvp.second;

            string str = key + " =>" + '\n';
            for (const auto& nbr : vtx.nbrs) {
                str += "\t" + nbr.first + "\t";
                if (nbr.first.length() < 16)
                    str += "\t";
                if (nbr.first.length() < 8)
                    str += "\t";
                str += to_string(nbr.second) + '\n';
            }
            cout << str << endl;
        }

        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl << endl;
    }

    void display_Stations() {
        cout << "\n***********************************************************************\n" << endl;

        int i = 1;
        for (const auto& kvp : vtces) {
            string key = kvp.first;
            cout << i << ". " << key << endl;
            i++;
        }

        cout << "\n***********************************************************************\n" << endl;
    }

    bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2))
            return true;

        processed[vname1] = true;

        const Vertex& vtx = vtces[vname1];
        for (const auto& nbr : vtx.nbrs) {
            if (!processed.count(nbr.first) && hasPath(nbr.first, vname2, processed))
                return true;
        }

        return false;
    }

    class DijkstraPair {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair& other) const {
            return cost > other.cost;
        }
    };
int dijkstra(const string& src, const string& des, bool nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> heap;

        for (const auto& kvp : vtces) {
            const string& key = kvp.first;
            DijkstraPair np;
            // long long int INT_MAX;
            np.vname = key;
            np.cost = INT_MAX;

            if (key == src) {
                np.cost = 0;
                np.psf = key;
            }

            heap.push(np);
            map[key] = np;
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);
            ans.push_back(rp.vname);

            const Vertex& v = vtces[rp.vname];
            for (const auto& nbr : v.nbrs) {
                if (map.count(nbr.first)) {
                    int oc = map[nbr.first].cost;
                    const Vertex& k = vtces[rp.vname];
                    int nc;
                    if (nan)
                        nc = rp.cost + 120 + 40 * k.nbrs.at(nbr.first);
                    else
                        nc = rp.cost + k.nbrs.at(nbr.first);

                    if (nc < oc) {
                        DijkstraPair& gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;

                        heap.push(gp);
                    }
                }
            }
        }
        return val;
    }

private:
    struct Pair {
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };


    

    string Get_Minimum_Distance(const string& src, const string& dst) {
        int min_dis = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;
        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname)) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                int temp = rp.min_dis;
                if (temp < min_dis) {
                    ans = rp.psf;
                    min_dis = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            vector<string> nbrs(rpvtx.nbrs.begin(), rpvtx.nbrs.end());

            for (const string& nbr : nbrs) {
                if (!processed.count(nbr)) {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.nbrs[nbr];
                    stack.push_front(np);
                }
            }
        }

        ans += to_string(min_dis);
        return ans;
    }

    string Get_Minimum_Time(const string& src, const string& dst) {
        int min_time = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        deque<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;
        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname)) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                int temp = rp.min_time;
                if (temp < min_time) {
                    ans = rp.psf;
                    min_time = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            vector<string> nbrs(rpvtx.nbrs.begin(), rpvtx.nbrs.end());

            for (const string& nbr : nbrs) {
                if (!processed.count(nbr)) {
                    Pair np;
                    np.vname = nbr;
                    np.psf = rp.psf + nbr + "  ";
                    np.min_dis = rp.min_dis + rpvtx.nbrs[nbr];
                    np.min_time = rp.min_time + 120 + 40 * rpvtx.nbrs[nbr];
                    stack.push_front(np);
                }
            }
        }

        double minutes = ceil(static_cast<double>(min_time) / 60);
        ans += to_string(minutes);
        return ans;
    }
    vector<string> get_Interchanges(const string& str) {
    vector<string> arr;
    vector<string> res;
    string temp = "";
    
    // Split the input string by "  "
    for (char c : str) {
        if (c == ' ' && temp != "") {
            res.push_back(temp);
            temp = "";
        } else {
            temp += c;
        }
    }
    if (temp != "") {
        res.push_back(temp);
    }

    arr.push_back(res[0]);
    int count = 0;
    
    for (int i = 1; i < res.size() - 1; i++) {
        int index = res[i].find('~');
        string s = res[i].substr(index + 1);
        
        if (s.length() == 2) {
            string prev = res[i - 1].substr(res[i - 1].find('~') + 1);
            string next = res[i + 1].substr(res[i + 1].find('~') + 1);
            
            if (prev == next) {
                arr.push_back(res[i]);
            } else {
                arr.push_back(res[i] + " ==> " + res[i + 1]);
                i++;
                count++;
            }
        } else {
            arr.push_back(res[i]);
        }
    }
    arr.push_back(to_string(count));
    arr.push_back(res[res.size() - 1]);
    
    return arr;
}
void Create_Metro_Map(Graph_M& g) {
    g.addVertex("Noida Sector 62~B");
    g.addVertex("Botanical Garden~B");
    g.addVertex("Yamuna Bank~B");
    g.addVertex("Rajiv Chowk~BY");
    g.addVertex("Vaishali~B");
    g.addVertex("Moti Nagar~B");
    g.addVertex("Janak Puri West~BO");
    g.addVertex("Dwarka Sector 21~B");
    g.addVertex("Huda City Center~Y");
    g.addVertex("Saket~Y");
    g.addVertex("Vishwavidyalaya~Y");
    g.addVertex("Chandni Chowk~Y");
    g.addVertex("New Delhi~YO");
    g.addVertex("AIIMS~Y");
    g.addVertex("Shivaji Stadium~O");
    g.addVertex("DDS Campus~O");
    g.addVertex("IGI Airport~O");
    g.addVertex("Rajouri Garden~BP");
    g.addVertex("Netaji Subhash Place~PR");
    g.addVertex("Punjabi Bagh West~P");

    g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
    g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
    g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
    g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
    g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
    g.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
    g.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
    g.addEdge("Huda City Center~Y", "Saket~Y", 15);
    g.addEdge("Saket~Y", "AIIMS~Y", 6);
    g.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
    g.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
    g.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
    g.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
    g.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
    g.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
    g.addEdge("DDS Campus~O", "IGI Airport~O", 8);
    g.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
    g.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
    g.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);
}

vector<string> printCodelist() {
    cout << "List of station along with their codes:\n";
    vector<string> codes;
    vector<string> keys;

    for (const auto& pair : vtces) {
        keys.push_back(pair.first);
    }

    sort(keys.begin(), keys.end());

    int i = 1, j = 0, m = 1;
    string temp, code;
    char c;

    for (const string& key : keys) {
        temp = "";
        code = "";
        j = 0;

        for (char ch : key) {
            if (ch == ' ') {
                c = temp[0];
                for (char ch2 : temp) {
                    if (ch2 >= '0' && ch2 <= '9') {
                        code += ch2;
                        j++;
                    }
                    else if ((ch2 < '0' || ch2 > '9') && ch2 < 'z') {
                        code += ch2;
                    }
                }

                if (code.length() < 2) {
                    code += toupper(temp[1]);
                }
                break;
            }
            temp += ch;
        }

        codes.push_back(code);

        cout << i << ". " << key << "\t";
        if (key.length() < (22 - m))
            cout << "\t";
        if (key.length() < (14 - m))
            cout << "\t";
        if (key.length() < (6 - m))
            cout << "\t";
        cout << code << endl;

        i++;
        if (i == static_cast<int>(pow(10, m))) {
            m++;
        }
    }

    return codes;
}

int main() {
    Graph_M g;
    Create_Metro_Map(g);
     vector<string> codes;
    cout << "\n\t\t\t****WELCOME TO THE METRO APP*****" << endl;
    while (true) {
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
        cout << "1. LIST ALL THE STATIONS IN THE MAP" << endl;
        cout << "2. SHOW THE METRO MAP" << endl;
        cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION" << endl;
        cout << "7. EXIT THE MENU" << endl;
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";

        int choice = -1;
        cin >> choice;
        cout << "\n***********************************************************\n";

        if (choice == 7) {
            exit(0);
        }

        switch (choice) {
            case 1: {
                g.display_Stations();
                break;
            }
            case 2: {
                g.display_Map();
                break;
            }
            case 3: {
                vector<string> keys = g.printCodelist();
                cout << "\n1. TO ENTER SERIAL NO. OF STATIONS" << endl;
                cout << "2. TO ENTER CODE OF STATIONS" << endl;
                cout << "3. TO ENTER NAME OF STATIONS" << endl;
                cout << "ENTER YOUR CHOICE: ";
                int ch;
                cin >> ch;
                int j;

                string st1 = "", st2 = "";
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
                if (ch == 1) {
                    int s1, s2;
                    cin >> s1 >> s2;
                    st1 = keys[s1 - 1];
                    st2 = keys[s2 - 1];
                } else if (ch == 2) {
                    string a, b;
                    cin >> a >> b;
                    transform(a.begin(), a.end(), a.begin(), ::toupper);
                    transform(b.begin(), b.end(), b.begin(), ::toupper);
                    for (j = 0; j < keys.size(); j++) {
                        if (a == codes[j])
                            break;
                    }
                    st1 = keys[j];
                    for (j = 0; j < keys.size(); j++) {
                        if (b == codes[j])
                            break;
                    }
                    st2 = keys[j];
                } else if (ch == 3) {
                    cin.ignore();
                    getline(cin, st1);
                    getline(cin, st2);
                } else {
                    cout << "Invalid choice" << endl;
                    exit(0);
                }

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed))
                    cout << "THE INPUTS ARE INVALID" << endl;
                else
                    cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << g.dijkstra(st1, st2, false) << "KM" << endl;
                break;
            }
            case 4: {
                string sat1, sat2;
                cout << "ENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                cout << "SHORTEST TIME FROM (" << sat1 << ") TO (" << sat2 << ") IS " << g.dijkstra(sat1, sat2, true) / 60 << " MINUTES" << endl;
                break;
            }
            case 5: {
                string s1, s2;
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
                cin.ignore();
                getline(cin, s1);
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2))
                    cout << "THE INPUTS ARE INVALID" << endl;
                else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    int len = str.size();
                    cout << "SOURCE STATION : " << s1 << endl;
                    cout << "SOURCE STATION : " << s2 << endl;
                                        cout << "DISTANCE: " << str[len - 1] << endl;
                    cout << "NUMBER OF INTERCHANGES: " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~" << endl;
                    cout << "START  ==>  " << str[0] << endl;
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << "   ==>    END" << endl;
                    cout << "\n~~~~~~~~~~~~~" << endl;
                }
                break;
            }
            case 6: {
                string ss1, ss2;
                cout << "ENTER THE SOURCE STATION: ";
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3))
                    cout << "THE INPUTS ARE INVALID" << endl;
                else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                    int len = str.size();
                    cout << "SOURCE STATION : " << ss1 << endl;
                    cout << "DESTINATION STATION : " << ss2 << endl;
                    cout << "TIME : " << str[len - 1] << " MINUTES" << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                    cout << "START  ==>  " << str[0] << " ==>  ";
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << "   ==>    END" << endl;
                    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                }
                break;
            }
            default: {
                cout << "Please enter a valid option! " << endl;
                cout << "The options you can choose are from 1 to 6." << endl;
                break;
            }
        }
    }

    return 0;
}


};
