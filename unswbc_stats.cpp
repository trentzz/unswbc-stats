#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

typedef std::vector<std::string> Team;
typedef std::vector<std::pair<std::string, int>> StringIntVec;

class Player {
private:
  bool member;
  std::set<std::string> sessions_attended;

public:
  Player() { member = false; }
  int games_played = 0;
  void add_game(std::string date) {
    sessions_attended.insert(date);
    games_played++;
  }
  int get_sessions_attended() { return sessions_attended.size(); }
};

class Line {
public:
  std::string date;
  std::string court_no;
  std::vector<std::string> team;
};

class Session {
private:
  std::unordered_map<std::string, int> player_games_played;
  std::unordered_map<std::string, int> team_games_played;
  std::unordered_map<std::string, int> courts_used;

public:
  int total_games_played = 0;

  void add_team(Team team) {
    total_games_played++;

    std::string team_name = "";
    bool first_name = true;
    for (std::string name : team) {
      player_games_played[name]++;
      if (!first_name) {
        team_name += ", ";
      }
      team_name += name;
      first_name = false;
    }

    team_games_played[team_name]++;
  }

  void add_court(std::string num) { courts_used[num]++; }

  StringIntVec get_team_games_played() {
    std::cout << "Teams that have played more than once:" << std::endl;
    StringIntVec team_int_vec(team_games_played.begin(),
                              team_games_played.end());
    std::sort(team_int_vec.begin(), team_int_vec.end(),
              [](const auto &a, const auto &b) {
                if (a.second == b.second) {
                  return a.first < b.first;
                }
                return a.second > b.second;
              });
    return team_int_vec;
    std::cout << std::endl;
  }

  void print_team_games_played() {
    StringIntVec team_int_vec = get_team_games_played();
    for (auto t : team_int_vec) {
      if (t.second > 1) {

        std::cout << t.first << ": " << t.second << std::endl;
      }
    }
  }

  void print_session_court_stats() {
    std::cout << "Court Usage Stats:" << std::endl;
    StringIntVec court_vec(courts_used.begin(), courts_used.end());
    std::sort(court_vec.begin(), court_vec.end(),
              [](const auto &a, const auto &b) {
                if (a.second == b.second) {
                  return a.first < b.first;
                }
                return a.second > b.second;
              });
    for (auto c : court_vec) {
      std::cout << c.first << ": " << c.second << std::endl;
    }
  }
};

typedef std::vector<std::pair<std::string, Player>> PlayerVec;
typedef std::vector<std::pair<std::string, Session>> SessionVec;

class Stats {
private:
  std::unordered_map<std::string, Session> session_data;
  std::unordered_map<std::string, Player> player_data;

  int games_played;
  std::string folder_path;

public:
  std::vector<std::string> split_string(std::string str, char splitter) {
    std::vector<std::string> result;
    std::string current = "";
    for (int i = 0; i < str.size(); i++) {
      if (str[i] == splitter) {
        if (current != "") {
          result.push_back(current);
          current = "";
        }
        continue;
      }
      current += str[i];
    }
    if (current.size() != 0)
      result.push_back(current);
    return result;
  }

  void add_line(Line line) {
    session_data[line.date].add_team(line.team);
    session_data[line.date].add_court(line.court_no);
    for (std::string name : line.team) {
      player_data[name].add_game(line.date);
    }
  }

  std::string clean_str(std::string str) {
    // Remove leading spaces
    size_t first = str.find_first_not_of(" ");
    if (first != std::string::npos) {
      str = str.substr(first);
    }

    // Remove trailing spaces
    size_t last = str.find_last_not_of(" ");
    if (last != std::string::npos) {
      str = str.substr(0, last + 1);
    }

    return str;
  }

  void get_data() {
    for (const auto &file : fs::directory_iterator(folder_path)) {
      if (file.path().extension() == ".csv") {
        std::ifstream csv_file(file.path());
        if (csv_file.is_open()) {
          // prints the csv, for debugging purposes
          // std::cout << "Opened " << file.path() << std::endl;

          // get date
          fs::path pathname = file.path();
          std::string date = pathname.string();
          date = date.substr(10, 8);

          std::string line;

          bool first_line = true;
          while (std::getline(csv_file, line)) {
            // ignore the first row of the spreadsheet
            if (first_line) {
              first_line = false;
              continue;
            }

            std::stringstream ss(line);
            std::vector<std::string> line_vec = split_string(line, ',');

            std::string court_no = line_vec.at(0);

            // if there is no assigned court_no, then it is the end of the list
            if (court_no == "")
              break;

            // getting a vector of names that form a team
            std::vector<std::string> team(line_vec.begin() + 1,
                                          line_vec.begin() + 5);

            // making all names lower case
            for (auto &str : team) {
              str = clean_str(str);
              std::transform(str.begin(), str.end(), str.begin(),
                             [](unsigned char c) { return std::tolower(c); });
            }

            // sorting names alphabetically
            std::sort(team.begin(), team.end());

            // adding line to data set
            Line line;
            line.date = date;
            line.court_no = court_no;
            line.team = team;

            add_line(line);
          }
        } else {
          std::cerr << "Failed to open " << file.path() << std::endl;
        }
      }
    }
  }

  SessionVec get_session_vec() {
    SessionVec session_vec(session_data.begin(), session_data.end());
    std::sort(session_vec.begin(), session_vec.end(),
              [](const auto &a, const auto &b) {
                return a.second.total_games_played >
                       b.second.total_games_played;
              });
    return session_vec;
  }

  PlayerVec get_player_vec() {
    PlayerVec player_vec(player_data.begin(), player_data.end());
    std::sort(player_vec.begin(), player_vec.end(),
              [](const auto &a, const auto &b) {
                return a.second.games_played > b.second.games_played;
              });
    return player_vec;
  }

  void print_stats() {
    SessionVec session_vec = get_session_vec();
    for (auto s : session_vec) {
      std::cout << "Session Date: " << s.first << std::endl;
      std::cout << "Games played this session: " << s.second.total_games_played
                << "\n\n";
      s.second.print_team_games_played();
      std::cout << std::endl;
      s.second.print_session_court_stats();
    }

    std::cout << std::endl;
    PlayerVec player_vec = get_player_vec();
    std::cout << "Games played:" << std::endl;
    for (auto p : player_vec) {
      std::cout << p.first << ": " << p.second.games_played << std::endl;
    }
  }

  void get_stats(std::string in_folder_path) {
    folder_path = in_folder_path;
    get_data();
    print_stats();
  }
};

int main() {
  // change this to the folder containing CSV files
  const std::string folder_path = "csv_files/";
  Stats stats;
  stats.get_stats(folder_path);

  return 0;
}
