// $Id: ubigint.cpp,v 1.8 2017-07-09 23:15:28-07 - - $

#include <cctype>
#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

#include "ubigint.h"
#include "debug.h"

ubigint::ubigint (const string& that): ubig_value(ubigvalue_t()) {
   DEBUGF ('~', "that = \"" << that << "\"");
   for (char digit: that) {
      if (not isdigit (digit)) {
         throw invalid_argument ("ubigint::ubigint(" + that + ")");
      }
      ubig_value.push_back(digit);
   }
   int i = 0;
   int j = ubig_value.size()-1;
    while(j > i){
       char temp = ubig_value[i];
       ubig_value[i] = ubig_value[j];
       ubig_value[j] = temp;
       i++;
       j--;
   }
}

ubigint ubigint::operator+ (const ubigint& that) const {
    string ubig_string = "";
    int carry = 0;
    uint i = 0;
    uint j = 0;
    while(i <= that.ubig_value.size() || j <= this->ubig_value.size()){
        int a = 0;
        int b = 0;
        if(j < this->ubig_value.size()){
            a = static_cast<int>(this->ubig_value[i] - '0');
        }
        if(i < that.ubig_value.size()){
            b = static_cast<int>(that.ubig_value[j] - '0');
        }
        int x = a + b + carry;
        if(x > 9){
            carry = 1;
            x = x % 10;
        } else {
            carry = 0;
        }
        if(i >= that.ubig_value.size() &&
           j >= this->ubig_value.size() && x == 0) {
            return ubigint (ubig_string);
        }
        j++;
        i++;
        string temp = to_string(x);
        ubig_string = temp + ubig_string;
    }
    return ubigint (ubig_string);
}

ubigint ubigint::operator- (const ubigint& that) const {
   string ubig_string = "";
   int carry = 0;
   uint j = 0;
   for(uint i = 0; i < this->ubig_value.size(); ++i) {
       int a = static_cast<int>(this->ubig_value[i] - '0');
       int b = 0;
       if(j < that.ubig_value.size()){
          b = static_cast<int>(that.ubig_value[j] - '0');
       }
       int x;
       a = a + carry;
       if(a > b) {
           x =  a - b;
           carry = 0;
       } else if(a < b) {
           carry = -1;
           a = a + 10;
           x = a - b;
       } else if(a == b) {
           x = 0;
           carry = 0;
       }
       if(i >= this->ubig_value.size() &&
          j >= that.ubig_value.size() &&
          x == 0){
           while(ubig_string.front() == '0'){
               ubig_string.erase(0,1);
           }
           return ubigint (ubig_string);
        }
        string temp = to_string(x);
        ubig_string = temp + ubig_string;
        j++;
   }
   while(ubig_string.front() == '0'){
       ubig_string.erase(0,1);
   }
   return ubigint (ubig_string);
}

ubigint ubigint::operator* (const ubigint& that) const {
    ubigvalue_t vector;
    string ubig_string = "";
    int m = this->ubig_value.size();
    int n = that.ubig_value.size();
    for(int i = 0; i < m + n; ++i){
        vector.push_back('0');
    }
    for(uint i = 0; i < this->ubig_value.size(); ++i) {
        int c = 0;
        int d;
        for(uint j = 0; j < that.ubig_value.size(); ++j) {
            d = static_cast<int>(vector[i + j] - '0') +
                static_cast<int>((that.ubig_value[j] - '0')*
                    (this->ubig_value[i] - '0')) + c;
            vector[i + j] = (d % 10) + '0';
            c = d / 10;
            string ubig_string = "";
            for (int z = 0; z < m + n; ++z) {
                ubig_string = ubig_string +
                              static_cast<char>(vector[z]);
            }
        }
        vector[i + n] = c + '0';
    }
    if(vector[m + n - 1] == '0'){
      vector[m + n - 1] = 0;
      for (int i = 0; i < m + n - 1; ++i) {
        ubig_string = static_cast<char>(vector[i]) + ubig_string;
      }
    } else {
      for (int i = 0; i < m + n; ++i) {
        ubig_string = static_cast<char>(vector[i]) + ubig_string;
      }
    }
    return ubigint(ubig_string);
}

void ubigint::multiply_by_2() {
    int c = 0;
    for(uint i = 0; i < this->ubig_value.size(); ++i) {
        int x = static_cast<int>(this->ubig_value[i] - '0');
        x = x * 2;
        x = x + c;
        c = x / 10;
        x = x % 10;
        this->ubig_value[i] = x + '0';
    }
    if(c > 0) {
        this->ubig_value.push_back(c + '0');
    }
}

void ubigint::divide_by_2() {
    int c = 0;
    for(int i = this->ubig_value.size() - 1; i >= 0; --i) {
        int x = static_cast<int>(this->ubig_value[i] - '0' + c);
        c = (x % 2) * 10;
        x = x / 2;
        this->ubig_value[i] = x + '0';
    }
    if(this->ubig_value[this->ubig_value.size() - 1] == '0'){
        this->ubig_value.pop_back();
    }
}


struct quo_rem { ubigint quotient; ubigint remainder; };
quo_rem udivide (const ubigint& dividend, ubigint divisor) {
   // Note: divisor is modified so pass by value (copy).
   ubigint zero = ubigint("0");
   if (divisor == zero) throw domain_error ("udivide by zero");
   ubigint power_of_2 = ubigint("1");
   ubigint quotient = ubigint("0");
   // left operand, dividend
   ubigint remainder = ubigint(dividend.tostring());
   while (divisor < remainder) {
      divisor.multiply_by_2();
      power_of_2.multiply_by_2();
   }
   while (zero < power_of_2) {
      if ((divisor < remainder) || (divisor == remainder)) {
         remainder = remainder - divisor;
         quotient = quotient + power_of_2;
      }
      divisor.divide_by_2();
      power_of_2.divide_by_2();
   }
   return {.quotient = quotient, .remainder = remainder};
}

string ubigint::tostring() const {
   string ubig_string = "";
   for (uint i = 0; i < this->ubig_value.size(); ++i) {
       ubig_string = static_cast<char>(this->ubig_value[i]) +
                     ubig_string;
   }
   return ubig_string;
}

ubigint ubigint::operator/ (const ubigint& that) const {
    ubigint result = ubigint(that.tostring());
   return udivide (*this, result).quotient;
}

ubigint ubigint::operator% (const ubigint& that) const {
   ubigint result = ubigint(that.tostring());
   return udivide (*this, result).remainder;
}

bool ubigint::operator == (const ubigint& that) const {
    if(that.ubig_value.size() > this->ubig_value.size()){
        return false;
    }
    if(that.ubig_value.size() < this->ubig_value.size()){
        return false;
    }
    for(uint i = 0; i < this->ubig_value.size(); ++i){
        int a = static_cast<int>(that.ubig_value[i] - '0');
        int b = static_cast<int>(this->ubig_value[i] - '0');
        if(a > b){
            return false;
        }
        if(a < b){
            return false;
        }
    }
    return true;
}

bool ubigint::operator< (const ubigint& that) const {
    if(that.ubig_value.size() > this->ubig_value.size()){
        return true;
    }
    if(that.ubig_value.size() < this->ubig_value.size()){
        return false;
    } else {
        for(int i = this->ubig_value.size() - 1; i >= 0; --i){
            int a = static_cast<int>(that.ubig_value[i] - '0');
            int b = static_cast<int>(this->ubig_value[i] - '0');
            if(a > b){
                return true;
            }
            if(a < b){
                return false;
            }
        }
    }
    return false;
}

string ubigint::tostringfinal() const{
   string ubig_string = "";
   int x = 0;
   for (int i = this->ubig_value.size() - 1; i >= 0; --i) {
       if(x == 69){
           ubig_string = ubig_string + "\\\n";
           x = 0;
       }
       ++x;
       ubig_string = ubig_string +
                     static_cast<char>(this->ubig_value[i]);
   }
   return ubig_string;
}

ostream& operator<< (ostream& out, const ubigint& that) {    
   return out << that.tostringfinal();
}

