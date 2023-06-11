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
// #include <yaml-cpp/emittermanip.h>
// #include <yaml-cpp/yaml.h>

// TODO:
// write name_bank to yaml
// write stats to filesystem

namespace fs = std::filesystem;

class Session;
typedef std::vector<std::pair<std::string, std::vector<std::string>>>
    SessionData;
typedef std::unordered_map<std::string, Session> AllData;

namespace StringUtils {
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

}; // namespace StringUtils

typedef std::unordered_map<std::string, std::string> UMapSS;

class NameBank {
private:
  UMapSS name_bank_;

public:
  UMapSS name_bank() { return name_bank_; }
  void name_bank(std::string nb_file) {
    // read yaml file
  }

  std::string get_full_name(std::string name) {
    if (name_bank_[name] == "") {
      std::string full_name;
      std::cout << "New name \"" << name
                << "\" encountered! Please enter their full name.\n>>> ";
      std::cin.ignore();
      getline(std::cin, full_name, '\n');

      std::cout << std::endl;
      name_bank_[name] = full_name;
    }

    return name_bank_[name];
  }
};

class Player {
public:
  int games_played = 0;
};

class Court {
public:
  int games_played = 0;
};

class Team {
public:
  int games_played = 0;
};

typedef std::unordered_map<std::string, Player> Players;
typedef std::unordered_map<std::string, Court> Courts;
typedef std::unordered_map<std::string, Team> Teams;

class Session {
private:
  std::string stats_path = "stats_files/";
  std::string date;
  SessionData session_data;

  NameBank name_bank_;

  Players players_;
  Courts courts_;
  Teams teams_;

  int games_played = 0;
  int unique_players = 0;

public:
  Players players() { return players_; }
  Courts courts() { return courts_; }
  Teams teams() { return teams_; }

  void calculate() {
    for (auto g : session_data) {
      games_played++;
      std::string court_num = g.first;
      std::vector<std::string> team = g.second;

      courts_[court_num].games_played++;

      std::string team_name = "";
      bool first = true;
      for (auto p : team) {
        if (!players_[p].games_played)
          unique_players++;

        players_[p].games_played++;

        if (!first)
          team_name += ", ";
        team_name += p;
        first = false;
      }

      // note: this relies on every line being a list of sorted players
      teams_[team_name].games_played++;

      // DEBUG
      // std::cout << team_name << std::endl;
    }
  }

  void add_nb(NameBank nb) { name_bank_ = nb; }

  void add_session(std::string file_path) {
    std::ifstream csv_file(file_path);
    if (csv_file.is_open()) {
      // prints the csv, for debugging purposes
      // std::cout << "Opened " << file.path() << std::endl;

      std::string line;

      bool first_line = true;
      while (std::getline(csv_file, line)) {
        // ignore the first row of the spreadsheet
        if (first_line) {
          first_line = false;
          continue;
        }

        std::stringstream ss(line);
        std::vector<std::string> line_vec =
            StringUtils::split_string(line, ',');

        std::string court_no = line_vec.at(0);

        // if there is no assigned court_no, then it is the end of the list
        if (court_no == "")
          break;

        // getting a vector of names that form a team
        std::vector<std::string> team(line_vec.begin() + 1,
                                      line_vec.begin() + 5);

        // making all names lower case
        for (auto &str : team) {
          str = StringUtils::clean_str(str);
          std::transform(str.begin(), str.end(), str.begin(),
                         [](unsigned char c) { return std::tolower(c); });
        }

        // change names to full names
        std::vector<std::string> full_name_team;
        for (auto str : team) {
          std::string full_name = name_bank_.get_full_name(str);
          full_name_team.push_back(full_name);
        }

        // sorting names alphabetically
        std::sort(full_name_team.begin(), full_name_team.end());

        // adding line to data set
        std::pair<std::string, std::vector<std::string>> p =
            std::make_pair(court_no, full_name_team);
        session_data.push_back(p);
      }
    } else {
      std::cerr << "Failed to open " << file_path << std::endl;
    }

    calculate();
  }

  void write_session_stats() {
    // create new file in the form YYYYMMDD.txt
    // write session stats
    std::string file_name = stats_path + date;
    std::ofstream sessionFile(file_name);
    sessionFile << "Session Date: " << date << "\n\n";

    sessionFile << "Player Stats:\n";
    std::vector<std::pair<std::string, Player>> sorted_players;
    for (auto &p : players_) {
      sorted_players.push_back(p);
    }
    sessionFile.close();
  }
};

class AllSessions {
private:
  std::string csv_path = "csv_files/";
  std::string stats_path = "stats_files/";
  std::string nb_file = "name_bank.yaml";
  std::unordered_map<std::string, Session> all_sessions;

  NameBank name_bank_;

  Players players_;
  Courts courts_;
  Teams teams_;
  // std::string yaml_path;

public:
  void run() {
    name_bank_.name_bank(nb_file);
    add_folder(csv_path);
    combine();
    write_all();
    write_sessions();
  }

  void add_folder(std::string csv_path) {

    for (const auto &file : fs::directory_iterator(csv_path)) {
      if (file.path().extension() == ".csv") {
        // get date
        fs::path pathname = file.path();
        std::string date = pathname.string();
        date = date.substr(10, 8);

        all_sessions[date].add_nb(name_bank_);
        all_sessions[date].add_session(pathname);
      }
    }
  }

  void add_file(std::string date) {
    // file path should be in the form "csv_files/<date>_games.csv"
    // then call session.add_session
  }

  // void set_yaml(std::string yaml_path) { yaml_path = yaml_path; }

  // combines player/court stats from every session
  void combine() {
    for (auto s : all_sessions) {
      std::string date = s.first;
      Players sp = all_sessions[date].players();
      Courts sc = all_sessions[date].courts();
      Teams st = all_sessions[date].teams();

      for (const auto &p : sp) {
        players_[p.first].games_played += (p.second).games_played;
      }

      for (const auto &c : sc) {
        courts_[c.first].games_played += (c.second).games_played;
      }

      for (const auto &t : st) {
        teams_[t.first].games_played += (t.second).games_played;
      }
    }
  }

  void print_stats() {
    for (auto p : players_) {
      std::cout << p.first << ' ' << (p.second).games_played << std::endl;
    }

    for (auto c : courts_) {
      std::cout << c.first << ' ' << (c.second).games_played << std::endl;
    }
  }

  // writes stats to folder: stats_files
  //    all.txt     - stats for all sessions
  //    2023-XX-XX  - stats for each session
  void write_all() {
    // write all stats
    std::string file_name = stats_path + "all.txt";
  }

  void write_sessions() {
    // print for debugging
    // print_stats();

    // write all_sessions stats
    // write session stats
    for (auto &s : all_sessions) {
    }
  }
};

int main() {
  AllSessions all_sessions;
  all_sessions.run();
  return 0;
}
