#include "../parameters_iterator.hpp"

template <typename Container, typename T>
T elelel::sqlite::parameters_iterator<Container, T>::operator*(const type& other) const {
  return value_type(cont_, pos_);
}

template <typename Container, typename T>
bool elelel::sqlite::parameters_iterator<Container, T>::operator==(const type& other) const {
  return pos_ == other.pos_;
}

template <typename Container, typename T>
bool elelel::sqlite::parameters_iterator<Container, T>::operator!=(const type& other) const {
  return !(*this == other);
}

template <typename Container, typename T>
bool elelel::sqlite::parameters_iterator<Container, T>::operator<(const type& other) const {
  return pos_ < other.pos_;
}
 
template <typename Container, typename T>
bool elelel::sqlite::parameters_iterator<Container, T>::operator<=(const type& other) const {
  return (*this < other) || (*this == other);
}

template <typename Container, typename T>
bool elelel::sqlite::parameters_iterator<Container, T>::operator>(const type& other) const {
  return !(*this <= other);
}

template <typename Container, typename T>
bool elelel::sqlite::parameters_iterator<Container, T>::operator>=(const type& other) const {
  return !(*this < other);
}


