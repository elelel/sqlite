#include "../read_rows_container.hpp"

template <typename Container, typename T>
elelel::read_rows_iterator<Container, T>::read_rows_iterator(Container& cont, const bool end) :
  cont_(cont),
  pos_(0),
  end_(end) {
  }

template <typename Container, typename T>
elelel::read_rows_iterator<Container, T>::read_rows_iterator(const type& other) :
  pos_(other.pos_)
  end_(other.end_) {
}

template <typename Container, typename T>
void elelel::read_rows_iterator<Container, T>::swap(type& other) {
  std::swap(pos_, other.pos_);
  std::swap(end_, other.end_);
}

template <typename Container, typename T>
auto elelel::read_rows_iterator<Container, T>::operator=(const type& other) -> type& {
  type tmp(other);
  swap(tmp);
  return *this;
}

template <typename Container, typename T>
bool elelel::read_rows_iterator<Container, T>::operator==(const type& other) const {
  return (pos_ == other.pos_) && (end_ == other.end_);
}

template <typename Container, typename T>
bool elelel::read_rows_iterator<Container, T>::operator!=(const type& other) const {
  return !(*this == other);
}

template <typename Container, typename T>
auto elelel::read_rows_iterator<Container, T>::operator++() -> type& {
  std::error_code ec;
  query_.step(ec);
  switch (ec.value()) {
  case result_code::DONE:
    ++pos_;
    end_ = true;
    break;
  case result_code::SQLITE_ROW:
    ++pos_;
    break;
  default:
    throw std::system_error(ec);
  }
  return *this;
}

template <typename Container, typename T>
auto elelel::read_rows_iterator<Container, T>::operator++(int) -> type {
  type tmp;
  this->operator++();
  return tmp;
}

template <typename Container, typename T>
auto elelel::read_rows_iterator<Container, T>::operator*() -> reference const {
  // Recurse tuple and get each column value
}
