#include "spatial_organization/rational.h"

#include <stdexcept>

#include "string_util.h"
#include "stl_util.h"

using std::shared_ptr;

namespace cx3d {
namespace spatial_organization {

#ifndef RATIONAL_NATIVE
Rational::Rational()
    : value_{0.0} {
}
#endif

Rational::Rational(int64_t numerator, int64_t denominator)
    :  value_{numerator / denominator} {
//  setBigIntTo(numerator_, numerator);
//  setBigIntTo(denominator_, denominator);
}

//Rational::Rational(const BigInteger& numerator, const BigInteger& denominator)
//    : numerator_(numerator),
//      denominator_(denominator) {
//  if (sgn(denominator) == -1) {
//    mpz_neg(numerator_.get_mpz_t(), numerator.get_mpz_t());
//    mpz_neg(denominator_.get_mpz_t(), denominator.get_mpz_t());
//  }
//}

Rational::Rational(double value)
    : value_{value} {
}

Rational::~Rational() {
}

bool Rational::isZero() const {
  return std::abs(value_) < 1e-10;
}

shared_ptr<Rational> Rational::negate() {
  value_ *= -1;
  return shared_from_this();
}

shared_ptr<Rational> Rational::add(const shared_ptr<Rational>& other) const {
//  BigInteger gcd;
//  mpz_gcd(gcd.get_mpz_t(), denominator_.get_mpz_t(), other->denominator_.get_mpz_t());
//
//  BigInteger other_non_div = other->denominator_ / gcd;
//  BigInteger new_numerator = (numerator_ * other_non_div) + (other->numerator_ * (denominator_ / gcd));
//  BigInteger new_denominator = denominator_ * other_non_div;
  double value = value_ + other->value_;
  return Rational::create(value);
}

shared_ptr<Rational> Rational::increaseBy(const shared_ptr<Rational>& other) {
//  BigInteger gcd;
//  mpz_gcd(gcd.get_mpz_t(), denominator_.get_mpz_t(), other->denominator_.get_mpz_t());
//
//  BigInteger other_non_div = other->denominator_ / gcd;
//  numerator_ = (numerator_ * other_non_div) + (other->numerator_ * (denominator_ / gcd));
//  denominator_ = denominator_ * other_non_div;
  value_ += other->value_;
  return shared_from_this();
}

shared_ptr<Rational> Rational::subtract(const shared_ptr<Rational>& other) const {
//  BigInteger gcd;
//  mpz_gcd(gcd.get_mpz_t(), denominator_.get_mpz_t(), other->denominator_.get_mpz_t());
//
//  BigInteger other_non_div = other->denominator_ / gcd;
//  BigInteger new_numerator = (numerator_ * other_non_div) - (other->numerator_ * (denominator_ / gcd));
//  BigInteger new_denominator = denominator_ * other_non_div;

//  return Rational::create(new_numerator, new_denominator);
  auto value = value_ + other->value_;
  return Rational::create(value);
}

shared_ptr<Rational> Rational::decreaseBy(const shared_ptr<Rational>& other) {
//  BigInteger gcd;
//  mpz_gcd(gcd.get_mpz_t(), denominator_.get_mpz_t(), other->denominator_.get_mpz_t());
//
//  BigInteger other_non_div = other->denominator_ / gcd;
//  numerator_ = (numerator_ * other_non_div) - (other->numerator_ * (denominator_ / gcd));
//  denominator_ = denominator_ * other_non_div;
//
//  return shared_from_this();
  value_ -= other->value_;
  return shared_from_this();
}

shared_ptr<Rational> Rational::multiply(const shared_ptr<Rational>& other) const {
//  BigInteger this_num_other_denom_gcd;
//  mpz_gcd(this_num_other_denom_gcd.get_mpz_t(), numerator_.get_mpz_t(), other->denominator_.get_mpz_t());
//  BigInteger other_num_this_denom_gcd;
//  mpz_gcd(other_num_this_denom_gcd.get_mpz_t(), other->numerator_.get_mpz_t(), denominator_.get_mpz_t());
//
//  BigInteger new_numerator = (numerator_ / this_num_other_denom_gcd) * (other->numerator_ / other_num_this_denom_gcd);
//  BigInteger new_denominator = (denominator_ / other_num_this_denom_gcd)
//      * (other->denominator_ / this_num_other_denom_gcd);
//
//  return Rational::create(new_numerator, new_denominator);
  return create(value_ * other->value_);
}

shared_ptr<Rational> Rational::multiplyBy(const shared_ptr<Rational>& other) {
//  BigInteger this_num_other_denom_gcd;
//  mpz_gcd(this_num_other_denom_gcd.get_mpz_t(), numerator_.get_mpz_t(), other->denominator_.get_mpz_t());
//  BigInteger other_num_this_denom_gcd;
//  mpz_gcd(other_num_this_denom_gcd.get_mpz_t(), other->numerator_.get_mpz_t(), denominator_.get_mpz_t());
//
//  numerator_ = (numerator_ / this_num_other_denom_gcd) * (other->numerator_ / other_num_this_denom_gcd);
//  denominator_ = (denominator_ / other_num_this_denom_gcd) * (other->denominator_ / this_num_other_denom_gcd);
//
  value_ *= other->value_;
  return shared_from_this();
}

shared_ptr<Rational> Rational::divide(const shared_ptr<Rational>& other) const {
//  if (other->numerator_ == 0) {
//    //fnoexceptionthrow std::invalid_argument("Divisor must not be zero!");
//  }
//  BigInteger this_num_other_denom_gcd;
//  mpz_gcd(this_num_other_denom_gcd.get_mpz_t(), numerator_.get_mpz_t(), other->denominator_.get_mpz_t());
//  BigInteger other_num_this_denom_gcd;
//  mpz_gcd(other_num_this_denom_gcd.get_mpz_t(), other->numerator_.get_mpz_t(), denominator_.get_mpz_t());
//
//  BigInteger new_numerator = (numerator_ / this_num_other_denom_gcd) * (other->denominator_ / other_num_this_denom_gcd);
//  BigInteger new_denominator = (denominator_ / other_num_this_denom_gcd)
//      * (other->numerator_ / this_num_other_denom_gcd);
//
//  return Rational::create(new_numerator, new_denominator);
  return create(value_ / other->value_);
}

shared_ptr<Rational> Rational::divideBy(const shared_ptr<Rational>& other) {
//  if (other->numerator_ == 0) {
//    //fnoexceptionthrow std::invalid_argument("Divisor must not be zero!");
//  }
//  BigInteger this_num_other_denom_gcd;
//  mpz_gcd(this_num_other_denom_gcd.get_mpz_t(), numerator_.get_mpz_t(), other->denominator_.get_mpz_t());
//  BigInteger other_num_this_denom_gcd;
//  mpz_gcd(other_num_this_denom_gcd.get_mpz_t(), other->numerator_.get_mpz_t(), denominator_.get_mpz_t());
//
//  numerator_ = (numerator_ / this_num_other_denom_gcd) * (other->denominator_ / other_num_this_denom_gcd);
//  denominator_ = (denominator_ / other_num_this_denom_gcd) * (other->numerator_ / this_num_other_denom_gcd);

  value_ /= other->value_;
  return shared_from_this();
}

double Rational::doubleValue() const {
  return value_;
}

std::string Rational::toString() {
  return StringUtil::toStr(doubleValue());
}

void Rational::cancel() {
//  BigInteger gcd;
//  mpz_gcd(gcd.get_mpz_t(), numerator_.get_mpz_t(), denominator_.get_mpz_t());
//
//  numerator_ = numerator_ / gcd;
//  denominator_ = denominator_ / gcd;
}

//const BigInteger Rational::pow2(int exp) const {
//  BigInteger result(1);  // = new BigInteger(1);
//  BigInteger temp(2);
//  while (exp > 0) {
//    if ((exp & 1) == 1) {
//      result = result * temp;
//    }
//    temp = temp * temp;
//    exp >>= 1;
//  }
//  return result;
//}

int Rational::compareTo(const shared_ptr<Rational>& other) const {
  return STLUtil::sgn(this->subtract(other)->value_);
}

//void Rational::setBigIntTo(BigInteger& big_int, int64_t value) {
//  auto representation = big_int.get_mpz_t();
//
//  mpz_set_si(representation, static_cast<int>(value >> 32));
//  mpz_mul_2exp(representation, representation, 32);
//  mpz_add_ui(representation, representation, static_cast<unsigned int>(value));
//}

bool Rational::equalTo(const std::shared_ptr<Rational>& other) {
  return compareTo(other) == 0;
}

}  // namespace spatial_organization
}  // namespace cx3d
