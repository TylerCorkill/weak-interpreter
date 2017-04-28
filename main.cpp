#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <typeinfo>

using namespace std;

char start_scopes[3] = {'(', '[', '{'};
char end_scopes[3] = {')', ']', '}'};
char string_id[2] = {'\"', '\''};
char operators[7] = {'=', '+', '-', '/', '*', '<', '>'};
char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

// typedef int (*IntFunctionWithOneParameter) (int a);

// int test(int a)
// {
//   return a;
// }
// IntFunctionWithOneParameter dict[] = {test};
// unordered_map<string, IntFunctionWithOneParameter> dict = {{'asdf', test}};

// const string functions[1] = {'log'};

void print(int var)
{
  cout << var << ' ';
}
void print(bool var)
{
  cout << var << ' ';
}
void print(char var)
{
  cout << var << ' ';
}
void print(string var)
{
  cout << var << ' ';
}


void print_strings(vector<string> lines)
{
  for (vector<string>::const_iterator i = lines.begin(); i != lines.end(); ++i)
    cout << *i << '~';
}



// vector<string> generate_lines(string filename)
string generate_text(string filename)
{
  // vector<string> lines;
  ifstream infile(filename);
  string text;
  char current;
  // while (getline(infile, line))
  while (infile.get(current))
  {
    text.push_back(current);
  }
  return text;
}

bool is_in_array(char character, char *arr, int size)
{
  for (int i = 0; i < size; i++)
  {
    if (character == arr[i]) return true;
  }
  return false;
}

int char_type(char character)
{
  if (character == ' ') return 0; // White Space
  if (character == ';') return 1; // End of Line
  if (character == '.') return 2; // period


  if ( is_in_array(character, start_scopes, 3) ) return 3;
  if ( is_in_array(character, end_scopes, 3) ) return 4;
  if ( is_in_array(character, string_id, 2) ) return 5;
  if ( is_in_array(character, operators, 7) ) return 6;
  if ( is_in_array(character, numbers, 10) ) return 7;
  if ( is_in_array(character, alphabet, 26) ) return 8;

  if (character == ',') return 9;

  return 0;
}

bool same_type(char a, char b)
{
  int a_type = char_type(a);
  int b_type = char_type(b);
  if (a_type == b_type) return true;
  return false;
}


vector<string> get_tokens(string line)
{
  vector<string> tokens;

  bool is_string = false;
  char previous_type = 0;
  string current_token;
  for (int i = 0; i < line.length() + 1; i++)
  {
    int current_type = char_type(line[i]);
    // cout << current_type;
    if (is_string || current_type == 5) {
      if (current_type == 5) is_string = !is_string;
      current_token.push_back(line[i]);
    }
    else if ((current_type == previous_type && current_type) ||
      (previous_type == 7 && current_type == 8) ||
      (previous_type == 8 && current_type == 7))
    {
      current_token.push_back(line[i]);
    }
    else
    {
      if (previous_type) tokens.push_back(current_token);
      if (current_type)
        current_token = line[i];
      else
        current_token = "";
    }
    // cout << line[i];
    previous_type = current_type;
  }
  // cout << '\n';
  return tokens;
}




// ------------------------------ JavaScript Functions ------------------------------



// ---------------------- Misc Helpers ----------------------


bool contains_chars(string token)
{
  for (int i = 0; i < token.length(); i++)
  {
    if (char_type(token[i]) != 7)
      return true;
  }
  return false;
}





// ---------------------- Expressions ----------------------

string js_expression(vector<string> tokens)
{
  int size = tokens.size();
  string result = "";
  for (int i = 0; i < size; i++)
  {
    if (tokens[i] == "+")
    {
      string first = tokens[i-1], second = tokens[i+1];
      // cout << first << second;
      if ( (first.length() == 1 && second.length() == 1) &&
      (char_type(first[0]) == 7 && char_type(second[0]) == 7) ) // both numbers
      // if ( first.length() == 1 && second.length() == 1 )

      {
        int first_int = first[0] - '0', second_int = second[0] - '0';
        // cout << first_int + second_int;
        // int total = first_int + second_int;
        result = to_string(first_int + second_int);
        // result.push_back(x);
        // print(result);
      }
      else
      {
        if (contains_chars(first))
          first = first.substr(1, first.length() - 2);
        if (contains_chars(second))
          second = second.substr(1, second.length() - 2);
        result = first + second;
      }
    }
  }
  return result;
}




// ---------------------- Variables ----------------------

unordered_map<string, string> variables;

int js_set_var(vector<string> tokens, int i, int size)
{
  string name = tokens[i+1];
  i += 2;
  bool set_value = false, is_string = false;
  string current;
  vector<string> expression;
  while (i < size)
  {
    if (tokens[i] == ";")
    {
      // cout << expression;
      // print_strings(expression);
      // cout << js_expression(expression);
      // cout << '\n';
      variables[name] = js_expression(expression);
      expression.clear();
      break;
    }

    else if (tokens[i] == "=")
      set_value = true;

    else if (set_value)
    {
      expression.push_back(tokens[i]);
      // if (tokens[i][0] == '\'')
      //   variables[name] = tokens[i].substr(1, tokens[i].length() - 2);
      // else
      //   variables[name] = tokens[i];
      // break;
    }
    i++;
  }
  return i;
}

string js_get_var(string name) {
  unordered_map<string,string>::const_iterator got = variables.find(name);

  if ( got == variables.end() )
  {
    return "not found";
  }
  else
  {
    // cout << got->first << " is " << got->second << endl;
    return got -> second;
  }
}







// ---------------------- JS Keywords ----------------------


int js_log(vector<string> tokens, int i, int size)
{
  i += 2;
  bool is_string = false;
  while (i < size)
  {
    if (tokens[i] == ")")
    {
      cout << '\n';
      break;
    }
    else if (tokens[i] != ",")
    {
      if (tokens[i][0] == '\'')
        print(tokens[i].substr(1, tokens[i].length() - 2));
      else if (!contains_chars(tokens[i]))
        print(tokens[i]);
      else
        print(js_get_var(tokens[i]));
    }
    i++;
  }
  return i;
}






// ------------------------------------- Interpreter -------------------------------------



void interpret_tokens(vector<string> tokens)
{
  // current_func
  int size = tokens.size();
  for (int i = 0; i < size; i++)
  {
    // string current = tokens[i];
    if (tokens[i] == "log" && tokens[i+1] == "(") // log function
    {
      i = js_log(tokens, i, size);
    }
    else if (tokens[i] == "var")
    {
      i = js_set_var(tokens, i, size);
    }
    // else if


  }
}

// ------------------------------------- Main -------------------------------------


int main(int argc, char *argv[])
{
  if (argv[1]) {
    // vector<string> lines = generate_text(argv[1]);
    string text = generate_text(argv[1]);
    // for (int i = 0; i != text.length(); i++)
    // {
    // print_strings(get_tokens(text));
    // cout << endl;
    interpret_tokens(get_tokens(text));
      // cout << *i << '\n';
    // }

    // print_strings(get_tokens(lines[0]));
    // print_strings( lines );
  }



  return 0;
}




