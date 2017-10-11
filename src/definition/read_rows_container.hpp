#include "../read_rows_container.hpp"

template <typename Query, typename RowTuple>
elelel::sqlite::read_rows_container<Query, RowTuple>::read_rows_container(Query& q) :
  query_(q) {
}

template <typename Query, typename RowTuple>
elelel::sqlite::read_rows_container<Query, RowTuple>::read_rows_container(const type& other) {
}

template <typename Query, typename RowTuple>
void elelel::sqlite::read_rows_container<Query, RowTuple>::swap(type& other) {
}

template <typename Query, typename RowTuple>
auto elelel::sqlite::read_rows_container<Query, RowTuple>::operator=(const type& other) -> type& {
  return *this;
}


template <typename Query, typename RowTuple>
auto elelel::sqlite::read_rows_container<Query, RowTuple>::begin(std::error_code& ec) -> iterator {
  if (!query_.at_results_begin()) {
    query_.execute(ec);
    if (ec != result::success) throw std::system_error(ec);
  }
  return iterator(*this, false);
}

template <typename Query, typename RowTuple>
auto elelel::sqlite::read_rows_container<Query, RowTuple>::begin() -> iterator {
  std::error_code ec;
  auto rslt = begin(ec);
  if (ec == result::success) return rslt;
  else throw std::system_error(ec);
}

template <typename Query, typename RowTuple>
auto elelel::sqlite::read_rows_container<Query, RowTuple>::end(std::error_code& ec) -> iterator {
  ec = result::success;
  return end();
}

template <typename Query, typename RowTuple>
auto elelel::sqlite::read_rows_container<Query, RowTuple>::end() -> iterator {
  return iterator(*this, true);
}

template <typename Query, typename RowTuple>
template <typename UnwrappedValue>
void elelel::sqlite::read_rows_container<Query, RowTuple>::get(const int i, std::optional<UnwrappedValue>& value) const {
  value.emplace(type_policy<UnwrappedValue>::get(query_.stmt(), i));
}

template <typename Query, typename RowTuple>
template <typename UnwrappedValue>
std::optional<UnwrappedValue> elelel::sqlite::read_rows_container<Query, RowTuple>::get(const int i) const {
  return type_policy<UnwrappedValue>::get(query_.stmt(), i);
}

template <typename Query, typename RowTuple>
template <typename UnwrappedValue>
int elelel::sqlite::read_rows_container<Query, RowTuple>::size(const int i) const {
  return ::sqlite3_column_bytes(*query_.stmt(), i);
}

template <typename Query, typename RowTuple>
RowTuple elelel::sqlite::read_rows_container<Query, RowTuple>::row() const {
  RowTuple r;
  row_(r);
  return r;
}
