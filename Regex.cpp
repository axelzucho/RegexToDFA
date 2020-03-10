//
// Created by axelzucho on 10/03/20.
//

#include <fstream>
#include <iostream>
#include <stack>
#include "Regex.h"

string insertConcatOp(const string& s){
    string result;

    for(int i = 0; i < s.length()-1; i++){
       const char& current = s[i];
       result += current;

       if(current == '(' || current == '|') continue;

       const char& next = s[i+1];
       if(next != '*' && next != '|' && next != ')') {
           result += '.';
       }
    }

    result += s.back();
    return result;
}

string convertToPostfix(const string& s, const map<char, short int>& precedence){
    string output;
    stack<char> ops;
   for (const char& c:s){
      if (c == '.' || c == '|' || c == '*') {
          while(!ops.empty() && precedence.at(ops.top()) >= precedence.at(c)) {
              output += ops.top();
              ops.pop();
          }
          ops.push(c);
      } else if(c == '(') {
          ops.push(c);
      } else if(c == ')') {
          while(!ops.empty() && ops.top() != '(') {
              output += ops.top();
              ops.pop();
          }
          // Getting rid of the '('
          ops.pop();
      } else {
          output += c;
      }
   }

   while(!ops.empty()){
       output += ops.top();
       ops.pop();
   }

   return output;
}

Regex::Regex(const string& filepath) {
    file.open(filepath);
    // Check if we could open the provided filepath.
    if (!file.is_open()) {
        opened_file = false;
        return;
    }
    // We could open the file.
    opened_file = true;

    file >> prefix_form;
    this->prefix_form = insertConcatOp(this->prefix_form);

    map<char, short int> precedence = {
            {'(', 0},
            {'.', 1},
            {'|', 2},
            {'*', 3}};

    this->postfix_form = convertToPostfix(this->prefix_form, precedence);
    cout << "Prefix: " << this->prefix_form << "\n";
    cout << "Postfix: " << this->postfix_form << "\n";
}
