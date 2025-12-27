#include <bits/stdc++.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

struct User {
    int id;
    string name;
};

vector<User> users;
int next_id = 1;

/* ---------- Utilities ---------- */

string http_response(const string& body) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain\r\n"
           "Content-Length: " + to_string(body.size()) + "\r\n\r\n" +
           body;
}

map<string,string> parse_query(const string& s) {
    map<string,string> q;
    size_t pos = s.find('?');
    if (pos == string::npos) return q;

    string query = s.substr(pos + 1);
    stringstream ss(query);
    string pair;

    while (getline(ss, pair, '&')) {
        auto eq = pair.find('=');
        if (eq != string::npos)
            q[pair.substr(0, eq)] = pair.substr(eq + 1);
    }
    return q;
}

/* ---------- CRUD Handlers ---------- */

string create_user(const map<string,string>& q) {
    auto it = q.find("name");
    if (it == q.end()) return "Missing name";

    users.push_back({next_id++, it->second});
    return "User created";
}

string list_users() {
    string out;
    for (auto& u : users)
        out += to_string(u.id) + " " + u.name + "\n";
    return out.empty() ? "No users" : out;
}

string get_user(int id) {
    for (auto& u : users)
        if (u.id == id)
            return to_string(u.id) + " " + u.name;
    return "User not found";
}

string update_user(int id, const map<string,string>& q) {
    auto it = q.find("name");
    if (it == q.end()) return "Missing name";

    for (auto& u : users)
        if (u.id == id) {
            u.name = it->second;
            return "User updated";
        }
    return "User not found";
}

string delete_user(int id) {
    auto it = remove_if(users.begin(), users.end(),
                        [&](auto& u){ return u.id == id; });
    if (it == users.end()) return "User not found";

    users.erase(it, users.end());
    return "User deleted";
}

/* ---------- Main Server ---------- */

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 10);

    cout << "Server running on port 8080\n";

    while (true) {
        int client = accept(server_fd, nullptr, nullptr);
        char buffer[4096]{};
        read(client, buffer, sizeof(buffer));

        string req(buffer);
        stringstream ss(req);
        string method, path;
        ss >> method >> path;

        auto q = parse_query(path);
        string response;

        if (path.find("/users") == 0) {
            if (method == "POST")
                response = create_user(q);
            else if (method == "GET") {
                if (q.count("id"))
                    response = get_user(stoi(q["id"]));
                else
                    response = list_users();
            }
            else if (method == "PUT")
                response = update_user(stoi(q["id"]), q);
            else if (method == "DELETE")
                response = delete_user(stoi(q["id"]));
            else
                response = "Unsupported method";
        } else {
            response = "Not found";
        }

        string http = http_response(response);
        write(client, http.c_str(), http.size());
        close(client);
    }
}
