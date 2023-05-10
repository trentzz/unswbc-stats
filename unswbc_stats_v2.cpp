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
#include <yaml-cpp/emittermanip.h>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;

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

class Session {
public:
  std::string date;
  std::vector<std::pair<std::string, std::vector<std::string>>> games;

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

        // sorting names alphabetically
        std::sort(team.begin(), team.end());

        // adding line to data set
        std::pair<std::string, std::vector<std::string>> p =
            std::make_pair(court_no, team);
        games.push_back(p);
      }
    } else {
      std::cerr << "Failed to open " << file_path << std::endl;
    }
  }
};

class Player {
public:
  std::string name;
  bool member;
  int games_played;
};

class Court {
public:
  int games_played;
};

class Team {
public:
  std::vector<std::string> names;
};

class PlayerStats {
private:
  std::unordered_map<std::string, Player> data;

public:
  void print_all() { std::cout << "printing all..." << std::endl; }

  void print_one(std::string item) {
    std::cout << "printing one..." << std::endl;
  }
};

class CourtStats {
private:
  std::unordered_map<std::string, Court> data;

public:
  void print_all() { std::cout << "printing all..." << std::endl; }

  void print_one(std::string item) {
    std::cout << "printing one..." << std::endl;
  }
};

class TeamStats {

private:
  std::unordered_map<std::string, Player> data;

public:
  void print_all() { std::cout << "printing all..." << std::endl; }

  void print_one(std::string item) {
    std::cout << "printing one..." << std::endl;
  }
};

class SessionStats {
private:
  PlayerStats player_stats;
  CourtStats court_stats;
  TeamStats team_stats;

  std::unordered_map<std::string, Player> data;

public:
  void print_all() { std::cout << "printing all..." << std::endl; }

  void print_one(std::string item) {
    std::cout << "printing one..." << std::endl;
  }
};

class Stats {
private:
  std::unordered_map<std::string, Session> data;

  PlayerStats player_stats;
  CourtStats court_stats;
  TeamStats team_stats;
  SessionStats session_stats;

  // std::string yaml_path;

public:
  void add_folder(std::string folder_path) {

    for (const auto &file : fs::directory_iterator(folder_path)) {
      if (file.path().extension() == ".csv") {
        // get date
        fs::path pathname = file.path();
        std::string date = pathname.string();
        date = date.substr(10, 8);

        data[date].add_session(pathname);
      }
    }
  }

  void add_file(std::string date) {
    // file path should be in the form "csv_files/<date>_games.csv"
    // then call session.add_session
  }

  // void set_yaml(std::string yaml_path) { yaml_path = yaml_path; }

  void calculate() {
    // calculate
  }

  void print_all() {
    player_stats.print_all();
    court_stats.print_all();
    team_stats.print_all();
    session_stats.print_all();
  }

  void input(std::vector<std::string> inputs) {
    // do stuff
    std::cout << "doing stuff..." << std::endl;
  }
};

void command_line() {
  Stats stats;
  stats.add_folder("csv_files/");
  // stats.set_yaml("data.yaml");
  stats.calculate();
  stats.print_all();
  std::string input;

  while (true) {
    std::cout << "\n> ";
    std::getline(std::cin, input);
    std::vector<std::string> inputs = StringUtils::split_string(input, ' ');

    if (inputs[0] == "exit") {
      break;
    } else {
      stats.input(inputs);
    }
  }
  return;
}

int main() {
  command_line();
  return 0;
}
