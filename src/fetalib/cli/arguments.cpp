
#include "fetalib/cli/arguments.hpp"
#include "fetalib/cli/colors.hpp"

#include <unordered_map>

void feta::ArgumentParser::add(feta::detail::Argument *arg, feta::detail::ArgumentDependency *command)
{
  if (command != nullptr) {
    command->arguments.push_back(arg);
    arg->dependencies.push_back(command);
  }
  args.push_back(arg);
}

void feta::ArgumentParser::add(feta::detail::ArgumentDependency *command) {
  commands.push_back(command);
}

bool feta::ArgumentParser::arg_exists(std::string key)
{
  for (feta::detail::Argument *arg : args) {
    if (arg->key == key || arg->alternate_key == key) {
      return true;
    }
  }
  return false;
}

feta::detail::Argument *feta::ArgumentParser::get_arg(std::string key)
{
  for (feta::detail::Argument *arg : args) {
    if (arg->key == key || arg->alternate_key == key) {
      return arg;
    }
  }
  throw std::invalid_argument("argument not found.");
}

feta::detail::ArgumentDependency *feta::ArgumentParser::get_command() {
  for (std::string str : argv) {
    for (detail::ArgumentDependency *command : commands) {
      if (str == command->key) return command;
    }
  }
}

bool feta::ArgumentParser::dependency_check(std::vector<feta::detail::ArgumentDependency*> deps) {
  if (deps.size() == 0) return true;
  for (feta::detail::ArgumentDependency *dep : deps) {
    for (std::string arg : argv) {
      if (dep->key == arg) return true;
    }
  }
  return false;
}

feta::Validation feta::ArgumentParser::validate()
{

  // ensure all required values accounted for.
  for (feta::detail::Argument *arg : args) {
    if (arg->is_optional) continue;
    if (!dependency_check(arg->dependencies)) continue;
    bool found = false;
    for (std::string str : argv) {
      if (str == arg->key
          || str == arg->alternate_key)
      {
        found = true;
      }
    }
    if (!found)
      return Validation {false, "required argument not found."};
  }

  // ensure non-flag arg not followed by arg (or vec-end) (no value provided)
  // ensure single definition of arguments
  bool last_arg = false;
  for (int i = 0; i < argv.size(); i++) {
    if (arg_exists(argv[i])) {
      if (last_arg) {
        return Validation {false, "argument not provided a value."};
      }
      feta::detail::Argument *arg = get_arg(argv[i]);
      if (!arg->is_flag) {
        if (i == argv.size() - 1)
          return Validation {false, "argument not provided a value."};
        last_arg = true;
      }
    } else {
      last_arg = false;
    }
    for (int j = i + 1; j < argv.size(); j++) {  // >1 def check
      if (argv[i] == argv[j])
        return Validation
        {
          false, "argument defined more than once."
        };
    }
  }

  return Validation {true, ""};
}

std::string feta::ArgumentParser::extract_help_string(detail::Argument *arg, int a_off, int max_char_width, int ovr_b_off) {
  std::string ret = "";
  for (int i = 0; i < a_off; i++) ret += std::string(" ");
  ret += arg->key;
  int b_off = arg->key.length();
  if (arg->alternate_key != "") {
    ret += std::string(", ") + arg->alternate_key;
    b_off += 2 + arg->alternate_key.length();
  }
  ret = feta::colorize(ret, feta::Color::PURPLE);
  if (arg->help_message != "") {
    b_off += 4;
    if (ovr_b_off > -1) {
      int rem = ovr_b_off - b_off; // fixme: am pretending like override out is always greater or eq.
      for (int i = 0; i < rem; i++) ret += std::string(" ");
      b_off = ovr_b_off;
    }
    ret += std::string(" :- ");
    std::string current_line = "";
    std::string word = ""; 
    int local_max_char_width = max_char_width-a_off-b_off;
    for (int i = 0; i < arg->help_message.length(); i++) {
      if (arg->help_message.substr(i, 1) == " ") {
        if (current_line.length() + word.length() > local_max_char_width) {
          current_line += std::string("\n");
          ret += current_line;
          for (int i = 0; i < a_off + b_off; i++) ret += std::string(" ");
          current_line = word;
          word = "";
        } else {
          if (current_line == "") current_line += word;
          else current_line += std::string(" ") + word;
          word = "";
        }
      } else {
        word += arg->help_message.substr(i, 1);
        if (i+1 == arg->help_message.length()) {
          if (current_line != "") current_line += std::string(" ");
          current_line += word;
        }
      }
    }
    ret += current_line;
  }
  return ret;
}

std::string feta::ArgumentParser::extract_help_string(detail::ArgumentDependency *arg, int a_off, int max_char_width, int ovr_b_off) {
  std::string ret = "";
  for (int i = 0; i < a_off; i++) ret += std::string(" ");
  ret += arg->key;
  int b_off = arg->key.length();
  if (arg->help_message != "") {
    b_off += 4;
    if (ovr_b_off > -1) {
      int rem = ovr_b_off - b_off; // fixme: am pretending like override out is always greater or eq.
      for (int i = 0; i < rem; i++) ret += std::string(" ");
      b_off = ovr_b_off;
    }
    ret = feta::colorize(ret, feta::Color::CYAN);
    ret += std::string(" :- ");
    std::string current_line = "";
    std::string word = "";
    int local_max_char_width = max_char_width-a_off-b_off;
    for (int i = 0; i < arg->help_message.length(); i++) {
      if (arg->help_message.substr(i, 1) == " ") {
        if (current_line.length() + word.length() > local_max_char_width) {
          ret += current_line + std::string("\n");
          for (int i = 0; i < a_off; i++) ret += std::string(" ");
          for (int i = 0; i < b_off; i++) ret += std::string(" ");
          current_line = word;
          word = "";
        } else {
          if (current_line == "") current_line += word;
          else current_line += std::string(" ") + word;
          word = "";
        }
      } else {
        word += arg->help_message.substr(i, 1);
        if (i+1 == arg->help_message.length()) {
          if (current_line != "") current_line += std::string(" ");
          current_line += word;
        }
      }
    }
    ret += current_line;
  }
  return ret;
}

int feta::ArgumentParser::get_largest_b_off(std::vector<feta::detail::Argument*> args) {
  int max_off = -1;
  for (int i = 0; i < args.size(); i++) {
    int off = args[i]->key.length();
    if (args[i]->alternate_key != "") off += 2 + args[i]->alternate_key.length(); // "[key], [alternate]"
    off += 4; // " :- "
    if (off > max_off) max_off = off;
  }
  return max_off;
}

std::vector<std::string> feta::ArgumentParser::get_help_message(std::string app_name, bool should_align_levels, int max_char_width) {
  // usage: [app_name] [arguments] [command]
  // arguments:
  //  ~~ - 
  //  ~~~~ - 
  //  ~~ - 
  //
  // commands:
  //  ~~~~~ - 
  //    -~ - 
  //  ~~~~~ - 
  //  ~~~~~ - 
  //
  // max of 60 chars in a line

  std::vector<std::string> lines;
  lines.push_back(std::string("usage: ") + std::string(app_name) + std::string(" [arguments]") + std::string(" [command]"));
  lines.push_back("");

  // -- gen args --

  if (args.size() > 0) {
    std::vector<detail::Argument*> genargs;
    for (feta::detail::Argument *arg : args) {
      if (arg->dependencies.size() == 0) genargs.push_back(arg);
    }
    lines.push_back(feta::colorize("general arguments:", feta::Color::BLUE));
    int ovr_off = should_align_levels ? get_largest_b_off(genargs) : -1;
    for (feta::detail::Argument *arg : genargs) {
      lines.push_back(extract_help_string(arg, 2, max_char_width, ovr_off));
    }
    lines.push_back("");
  }

  // -- command dep args --

  lines.push_back(feta::colorize("commands:", feta::Color::BLUE));
  for (detail::ArgumentDependency *command : commands) {
    lines.push_back(extract_help_string(command, 0, max_char_width, -1));
    int ovr_off = should_align_levels ? get_largest_b_off(command->arguments) : -1;
    int a_off = command->key.length() + 4 + 2; // temporary, surely
    for (feta::detail::Argument *arg : command->arguments) {  
      lines.push_back("");
      lines.push_back(extract_help_string(arg, a_off, max_char_width, ovr_off));
    } 
  }

  return lines;
} 

