// $Id: bigint.cpp,v 1.4 2017-07-09 23:16:48-07 - - $

#include <cstdlib>
#include <exception>
#include <stack>
#include <stdexcept>
using namespace std;

#include "bigint.h"
#include "debug.h"
#include "relops.h"

bigint::bigint (const ubigint& uvalue, bool is_negative):
                uvalue(uvalue), is_negative(is_negative) {
}

bigint::bigint (const string& that) {
   is_negative = that.size() > 0 and that[0] == '_';
   uvalue = ubigint (that.substr (is_negative ? 1 : 0));
}

bigint bigint::operator+ () const {
   return *this;
}

bigint bigint::operator- () const {
   if(is_negative) {
     return bigint(this->uvalue, false);
   } else {
     return bigint(this->uvalue, true);
   }
}

bigint bigint::operator+ (const bigint& that) const {
    bigint result;
    string zero = "0";
    if(this->is_negative == false && that.is_negative == true){
        if(this->uvalue < that.uvalue){
            result = bigint(that.uvalue - this->uvalue, true);
        }
        else if(that.uvalue < this->uvalue){
            result = bigint(this->uvalue - that.uvalue, false);
        }
        else if(this->uvalue == that.uvalue){
            result = bigint(zero,false);
        }
    }
    else if(this->is_negative == true && that.is_negative == false){
        if(this->uvalue < that.uvalue){
            result = bigint(that.uvalue - this->uvalue, false);
        }
        else if(that.uvalue < this->uvalue){
            result = bigint(this->uvalue - that.uvalue, true);
        }
        else if(this->uvalue == that.uvalue){
            result = bigint(zero,false);
        }
    }
    else if(this->is_negative == true && that.is_negative == true){
        result = bigint(uvalue + that.uvalue, true);
    }
    else if(this->is_negative == false && that.is_negative == false){
        result = bigint(uvalue + that.uvalue, false);
    }
    return result;
}

bigint bigint::operator- (const bigint& that) const {
    bigint result;
    string zero = "0";
    if(this->is_negative == false && that.is_negative == true){
        result = bigint(this->uvalue + that.uvalue, false);
    }
    else if(this->is_negative == true && that.is_negative == false){
        result = bigint(this->uvalue + that.uvalue, true);
    }
    else if(this->is_negative == true && that.is_negative == true){
        if(this->uvalue < that.uvalue){
            result = bigint(that.uvalue - this->uvalue, false);
        }
        else if(that.uvalue < this->uvalue){
            result = bigint(this->uvalue - that.uvalue, true);
        }
        else if(this->uvalue == that.uvalue){
            result = bigint(zero,false);
        }
    }
    else if(this->is_negative == false && that.is_negative == false){
        if(this->uvalue < that.uvalue){
            result = bigint(that.uvalue - this->uvalue, true);
        }
        else if(that.uvalue < this->uvalue){
            result = bigint(this->uvalue - that.uvalue, false);
        }
        else if(this->uvalue == that.uvalue){
            result = bigint(zero,false);
        }
    }
    return result;
}

bigint bigint::operator* (const bigint& that) const {
    bigint result;
    if(this->is_negative == false && that.is_negative == true){
        result = bigint(this->uvalue * that.uvalue, true);
    }
    else if(this->is_negative == true && that.is_negative == false){
        result = bigint(this->uvalue * that.uvalue, true);
    }
    else if(this->is_negative == false && that.is_negative == false){
        result = bigint(this->uvalue * that.uvalue, false);
    }
    else if(this->is_negative == true && that.is_negative == true){
        result = bigint(this->uvalue * that.uvalue, false);
    }
   return result;
}

bigint bigint::operator/ (const bigint& that) const {
    bigint result = this->uvalue / that.uvalue;
   return result;
}

bigint bigint::operator% (const bigint& that) const {
    bigint result = this->uvalue % that.uvalue;
   return result;
}

bool bigint::operator== (const bigint& that) const {
   return this->uvalue == that.uvalue;
}

bool bigint::operator< (const bigint& that) const {
   return this->uvalue < that.uvalue;
}

ostream& operator<< (ostream& out, const bigint& that) {
   return out << (that.is_negative ? "-" : "") << that.uvalue;
}

