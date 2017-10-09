#include "../parameters_container.hpp"

template <typename OutputTuple>
elelel::sqlite::parameters_container<OutputTuple>::parameters_container(std::shared_ptr<statement> stmt) :
  stmt_(stmt) {
}

template <typename OutputTuple>
elelel::sqlite::parameters_container<OutputTuple>::parameters_container(const type& other) :
  stmt_(other.stmt_) {
}

template <typename OutputTuple>
void elelel::sqlite::parameters_container<OutputTuple>::swap(type& other) {
  std::swap(stmt_, other.stmt_);
}

template <typename OutputTuple>
auto elelel::sqlite::parameters_container<OutputTuple>::operator=(const type& other) -> type& {
  type tmp(other);
  swap(other);
  return *this;
}


