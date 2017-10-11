#include "../read_rows_container.hpp"

template <typename Container, typename T>
elelel::sqlite::read_rows_iterator<Container, T>::read_rows_iterator(Container& cont, const bool end) :
  cont_(cont),
  pos_(0),
  end_(end) {
  }

template <typename Container, typename T>
elelel::sqlite::read_rows_iterator<Container, T>::read_rows_iterator(const type& other) :
  cont_(other.cont_),
  pos_(other.pos_),
  end_(other.end_) {
}

template <typename Container, typename T>
void elelel::sqlite::read_rows_iterator<Container, T>::swap(type& other) {
  std::swap(pos_, other.pos_);
  std::swap(end_, other.end_);
}

template <typename Container, typename T>
auto elelel::sqlite::read_rows_iterator<Container, T>::operator=(const type& other) -> type& {
  type tmp(other);
  swap(tmp);
  return *this;
}

template <typename Container, typename T>
bool elelel::sqlite::read_rows_iterator<Container, T>::operator==(const type& other) const {
  return (end_ && other.end_) || ((end_ == other.end_) && (pos_ == other.pos_));
}

template <typename Container, typename T>
bool elelel::sqlite::read_rows_iterator<Container, T>::operator!=(const type& other) const {
  return !(*this == other);
}

template <typename Container, typename T>
auto elelel::sqlite::read_rows_iterator<Container, T>::operator++() -> type& {
  std::error_code ec;
  cont_.query_.step(ec);
  switch (ec.value()) {
  case static_cast<int>(result_code::DONE):
    ++pos_;
    end_ = true;
    break;
  case static_cast<int>(result_code::ROW):
    ++pos_;
    break;
  default:
    throw std::system_error(ec);
  }
  return *this;
}

template <typename Container, typename T>
auto elelel::sqlite::read_rows_iterator<Container, T>::operator++(int) -> type {
  type tmp;
  this->operator++();
  return tmp;
}

template <typename Container, typename T>
auto elelel::sqlite::read_rows_iterator<Container, T>::operator*() const -> value_type {
  return value_type{cont_.row()};
}
