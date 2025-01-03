
#include "fetalib/cli/arguments.hpp"

void feta::ArgumentParser::add(feta::detail::Argument arg)
{
  args.push_back(arg);
}

bool feta::ArgumentParser::arg_exists(std::string key)
{
  for (feta::detail::Argument arg : args) {
    if (arg.key == key || arg.alternate_key == key) {
      return true;
    }
  }
  return false;
}

feta::detail::Argument feta::ArgumentParser::get_arg(std::string key)
{
  for (feta::detail::Argument arg : args) {
    if (arg.key == key || arg.alternate_key == key) {
      return arg;
    }
  }
  throw std::invalid_argument("argument not found.");
}

bool feta::ArgumentParser::dependency_check(std::vector<feta::detail::ArgumentDependency> deps) {
  if (deps.size() == 0) return true;
  for (feta::detail::ArgumentDependency dep : deps) {
    for (std::string arg : argv) {
      if (dep.key == arg) return true;
    }
  }
  return false;
}

feta::Validation feta::ArgumentParser::validate()
{
  // ensure all required values accounted for.
  for (feta::detail::Argument arg : args) {
    if (arg.is_optional) continue;
    if (!dependency_check(arg.dependencies)) continue;
    bool found = false;
    for (std::string str : argv) {
      if (str.find(arg.key) != std::string::npos
          || str.find(arg.key) != std::string::npos)
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
      feta::detail::Argument arg = get_arg(argv[i]);
      if (!arg.is_flag) {
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

std::string feta::ArgumentParser::extract_help_string(detail::Argument arg, int i_tab, int max_chars, int override_out) {
  std::string ret = "";
  for (int i = 0; i < i_tab; i++) ret += std::string("\t");
  ret += arg.key;
  int i_out = arg.key.length();
  if (arg.alternate_key != "") {
    ret += std::string(", ") + arg.alternate_key;
    i_out += 2 + arg.alternate_key.length();
  }
  if (arg.help_message != "") {
    i_out += 4;
    if (override_out > -1) {
      int rem = override_out - i_out; // fixme: am pretending like override out is always greater or eq.
      for (int i = 0; i < rem; i++) ret += std::string(" ");
      i_out = override_out;
    }
    ret += std::string(" :- ");
    std::string current_line = "";
    std::string word = ""; 
    for (int i = 0; i < arg.help_message.length(); i++) {
      if (arg.help_message.substr(i, 1) == " ") {
        if (i_out + current_line.length() + word.length() > max_chars) {
          current_line += std::string("\n");
          ret += current_line;
          for (int i = 0; i < i_tab; i++) ret += std::string("\t");
          for (int i = 0; i < i_out; i++) ret += std::string(" ");
          current_line = word;
          word = "";
        } else {
          if (current_line == "") current_line += word;
          else current_line += std::string(" ") + word;
          word = "";
        }
      } else {
        word += arg.help_message.substr(i, 1);
        if (i+1 == arg.help_message.length()) {
          if (current_line != "") current_line += std::string(" ");
          current_line += word;
        }
      }
    }
    ret += current_line;
  }
  return ret;
}

std::string feta::ArgumentParser::extract_help_string(detail::ArgumentDependency arg, int i_tab, int max_chars, int override_out) {
  std::string ret = "";
  for (int i = 0; i < i_tab; i++) ret += std::string("\t");
  ret += arg.key;
  int i_out = arg.key.length();
  if (arg.help_message != "") {
    i_out += 4;
    if (override_out > -1) {
      int rem = override_out - i_out; // fixme: am pretending like override out is always greater or eq.
      for (int i = 0; i < rem; i++) ret += std::string(" ");
      i_out = override_out;
    }
    ret += std::string(" :- ");
    std::string current_line = "";
    std::string word = "";
    for (int i = 0; i < arg.help_message.length(); i++) {
      if (arg.help_message.substr(i, 1) == " ") {
        if (i_out + current_line.length() + word.length() > max_chars) {
          ret += current_line + std::string("\n");
          for (int i = 0; i < i_tab; i++) ret += std::string("\t");
          for (int i = 0; i < i_out; i++) ret += std::string(" ");
          current_line = word;
          word = "";
        } else {
          if (current_line == "") current_line += word;
          else current_line += std::string(" ") + word;
          word = "";
        }
      } else {
        word += arg.help_message.substr(i, 1);
        if (i+1 == arg.help_message.length()) {
          if (current_line != "") current_line += std::string(" ");
          current_line += word;
        }
      }
    }
    ret += current_line;
  }
  return ret;
}

int feta::ArgumentParser::find_largest_hs_offset(std::vector<feta::detail::Argument> args) {
  int max_off = -1;
  for (int i = 0; i < args.size(); i++) {
    int off = args[i].key.length();
    if (args[i].alternate_key != "") off += 2 + args[i].alternate_key.length(); // "[key], [alternate]"
    off += 4; // " :- "
    if (off > max_off) max_off = off;
  }
  return max_off;
}

std::vector<std::string> feta::ArgumentParser::get_help_message(std::string app_name, bool should_align_levels) {
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

  std::vector<feta::detail::Argument> general_args;
  for (feta::detail::Argument arg : args) {
    if (arg.dependencies.size() == 0) general_args.push_back(arg);
  }
  lines.push_back("general arguments:");
  int ovr_off = should_align_levels ? find_largest_hs_offset(general_args) : -1;
  for (feta::detail::Argument arg : general_args) {
      lines.push_back(extract_help_string(arg, 1, 60, ovr_off));
  }
   
  return lines;
} 

