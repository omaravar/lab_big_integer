// =============================================================
//  big_integer.cpp -- BigInteger class implementation
//
//  TASK: Implement all methods declared in big_integer.h
//  This stub file exists only so the project structure is clear.
//  Replace its contents with your implementation.
// =============================================================

#include "big_integer.h"

BigInteger::BigInteger() : negative_(false) {
    digits_.push_back(0);
}
BigInteger::BigInteger(int value) : negative_(false) {
    long long temp = value;
    if (temp < 0){
        negative_ = true;
        temp = -temp;
    }
    if (temp == 0){
        digits_.push_back(0);
        return;
    }
    while (temp > 0){
        digits_.push_back(temp%10);
        temp /=10;
    }
}
BigInteger::BigInteger(long long value) : negative_(false) {
    if (value < 0) {
        negative_ = true;
        value = -value;
    }

    if (value == 0) {
        digits_.push_back(0);
        return;
    }

    while (value > 0) {
        digits_.push_back(value % 10);
        value /= 10;
    }
}

BigInteger::BigInteger(const std::string& str):negative_(false){
    if (str.empty()){
        digits_.push_back(0);
        return;
    }
    size_t pos = 0;
    if (str[0]=='-'){
        pos = 1;
        negative_ = true;
    }
    while (pos < str.size() && str[pos]=='0'){
        pos++;
    }
    if (pos == str.size()) {
        digits_.push_back(0);
        negative_ = false;
        return;
    }
    for (size_t i = str.size(); i >pos; --i){
        digits_.push_back(str[i-1]-'0');
    }
}

bool BigInteger::isZero() const {
    return digits_[0] == 0 && digits_.size() ==1;
}

void BigInteger::normalize(){
    while(digits_.size()>1 && digits_.back()==0){
        digits_.pop_back();
    }
    if(isZero()){
        negative_ = false;
    }
}

int BigInteger::compareAbs(const BigInteger &a, const BigInteger &b) {
    if (a.digits_.size()>b.digits_.size()){
        return 1;
    }
    if (a.digits_.size()< b.digits_.size()){
        return -1;
    }
    for (size_t i = a.digits_.size(); i >0; --i){
        if (a.digits_[i-1]>b.digits_[i-1]){
            return 1;
        }
        if (a.digits_[i-1]<b.digits_[i-1]){
            return -1;
        }
    }
    return 0;
}

std::vector<int> BigInteger::addAbs(const std::vector<int> &a, const std::vector<int> &b) {
    std::vector<int> result;
    int carry = 0;
    size_t n = std::max(a.size(), b.size());

    for (size_t i = 0; i < n || carry != 0; ++i){
        int sum = carry;
        if(i < a.size()){
            sum+=a[i];
        }
        if (i < b.size()){
            sum += b[i];
        }
        result.push_back(sum%10);
        carry = sum/10;
    }
    return result;
}

std::vector<int> BigInteger::subAbs(const std::vector<int>& a, const std::vector<int>& b){
    std::vector<int> result;
    int borrow = 0;
    for (size_t i = 0; i < a.size(); i ++){
        int diff =a[i] - borrow;
        if (i < b.size()){
            diff-= b[i];
        }
        if (diff <0){
            diff+=10;
            borrow =1;
        }
        else{
            borrow = 0;
        }
        result.push_back(diff);
    }
    while (result.size() > 1 && result.back() ==0){
        result.pop_back();
    }
    return result;
}

BigInteger& BigInteger::operator+=(const BigInteger& rhs){
    if (negative_ == rhs.negative_){
        digits_= addAbs(digits_, rhs.digits_);
    }
    else{
        int cmp = compareAbs(*this, rhs);
        if (cmp == 0 ){
            digits_ = {0};
            negative_ = false;
        }
        else if (cmp>0){
            digits_ = subAbs(digits_, rhs.digits_);
        }
        else{
            digits_ = subAbs(rhs.digits_, digits_);
            negative_ = rhs.negative_;
        }
    }
    normalize();
    return *this;
}
BigInteger BigInteger::operator-() const {
    BigInteger result(*this);
    if (!result.isZero()) {
        result.negative_ = !result.negative_;
    }
    return result;
}

BigInteger BigInteger::operator+(const BigInteger& rhs) const {
    BigInteger result(*this);
    result += rhs;
    return result;
}
BigInteger& BigInteger::operator-=(const BigInteger& rhs) {
    *this += (-rhs);
    return *this;
}

BigInteger BigInteger::operator-(const BigInteger& rhs) const {
    BigInteger result(*this);
    result -= rhs;
    return result;
}

bool BigInteger::operator==(const BigInteger& rhs) const {
    return negative_ == rhs.negative_ && digits_ == rhs.digits_;
}

bool BigInteger::operator!=(const BigInteger& rhs) const {
    return !(*this == rhs);
}

bool BigInteger::operator<(const BigInteger& rhs) const {
    if (negative_ != rhs.negative_) {
        return negative_;
    }

    int cmp = compareAbs(*this, rhs);

    if (!negative_) {
        return cmp < 0;
    } else {
        return cmp > 0;
    }
}

bool BigInteger::operator>(const BigInteger& rhs) const {
    return rhs < *this;
}

bool BigInteger::operator<=(const BigInteger& rhs) const {
    return !(*this > rhs);
}

bool BigInteger::operator>=(const BigInteger& rhs) const {
    return !(*this < rhs);
}
BigInteger& BigInteger::operator*=(const BigInteger& rhs) {
    if (is_zero() || rhs.is_zero()) {
        digits_ = {0};
        negative_ = false;
        return *this;
    }

    std::vector<int> result(digits_.size() + rhs.digits_.size(), 0);

    for (size_t i = 0; i < digits_.size(); ++i) {
        for (size_t j = 0; j < rhs.digits_.size(); ++j) {
            result[i + j] += digits_[i] * rhs.digits_[j];
        }
    }

    for (size_t i = 0; i + 1 < result.size(); ++i) {
        result[i + 1] += result[i] / 10;
        result[i] %= 10;
    }

    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }

    digits_ = result;
    negative_ = (negative_ != rhs.negative_);
    normalize();
    return *this;
}

BigInteger BigInteger::operator*(const BigInteger& rhs) const {
    BigInteger result(*this);
    result *= rhs;
    return result;
}

BigInteger& BigInteger::operator/=(const BigInteger& rhs) {
    // Деление на ноль
    if (rhs.isZero()) {
        throw std::runtime_error("division by zero");
    }

    // Если делимое равно нулю, результат сразу ноль
    if (isZero()) {
        return *this;
    }

    // Запоминаем знак результата
    bool resultNegative = (negative_ != rhs.negative_);

    // Работаем с модулями
    BigInteger dividend(*this);
    dividend.negative_ = false;

    BigInteger divisor(rhs);
    divisor.negative_ = false;

    // Если |dividend| < |divisor|, результат 0
    if (dividend < divisor) {
        digits_ = {0};
        negative_ = false;
        return *this;
    }

    BigInteger current;
    current.digits_ = {0};
    current.negative_ = false;

    std::vector<int> quotient;

    // Идём от старших цифр к младшим
    for (size_t i = dividend.digits_.size(); i > 0; --i) {
        // current = current * 10 + очередная цифра
        if (!(current.digits_.size() == 1 && current.digits_[0] == 0)) {
            current.digits_.insert(current.digits_.begin(), 0);
        }
        current.digits_[0] = dividend.digits_[i - 1];
        current.normalize();

        // Подбираем цифру частного от 0 до 9
        int digit = 0;
        while (current >= divisor) {
            current -= divisor;
            ++digit;
        }

        quotient.push_back(digit);
    }

    // quotient сейчас в прямом порядке: от старших к младшим
    // а нам нужно хранить в обратном
    digits_.clear();

    size_t pos = 0;
    while (pos < quotient.size() && quotient[pos] == 0) {
        ++pos;
    }

    if (pos == quotient.size()) {
        digits_ = {0};
        negative_ = false;
        return *this;
    }

    for (size_t i = quotient.size(); i > pos; --i) {
        digits_.push_back(quotient[i - 1]);
    }

    negative_ = resultNegative;
    normalize();
    return *this;
}
BigInteger BigInteger::operator/(const BigInteger& rhs) const {
    BigInteger result(*this);
    result /= rhs;
    return result;
}


BigInteger& BigInteger::operator%=(const BigInteger& rhs) {
    BigInteger q = *this / rhs;
    *this -= q * rhs;
    return *this;
}

BigInteger BigInteger::operator%(const BigInteger& rhs) const {
    BigInteger result(*this);
    result %= rhs;
    return result;
}

bool BigInteger::is_zero() const {
    return digits_.size() == 1 && digits_[0] == 0;
}

bool BigInteger::is_negative() const {
    return negative_;
}

BigInteger::operator bool() const {
    return !is_zero();
}

std::string BigInteger::to_string() const {
    std::string result;
    if (negative_) {
        result += '-';
    }
    for (size_t i = digits_.size(); i > 0; --i) {
        result += char('0' + digits_[i - 1]);
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& value) {
    os << value.to_string();
    return os;
}

std::istream& operator>>(std::istream& is, BigInteger& value) {
    std::string str;
    is >> str;
    value = BigInteger(str);
    return is;
}

BigInteger& BigInteger::operator++() {
    *this += BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp(*this);
    ++(*this);
    return temp;
}

BigInteger& BigInteger::operator--() {
    *this -= BigInteger(1);
    return *this;
}
BigInteger BigInteger::operator--(int) {
    BigInteger temp(*this);
    --(*this);
    return temp;
}
