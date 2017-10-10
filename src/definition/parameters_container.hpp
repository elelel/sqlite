#include "../parameters_container.hpp"

template <typename Query, typename OutputTuple>
elelel::sqlite::parameters_container<Query, OutputTuple>::parameters_container(Query& query) :
  query_(query) {
}

template <typename Query, typename OutputTuple>
elelel::sqlite::parameters_container<Query, OutputTuple>::parameters_container(const type& other) :
  query_(other.query_) {
}

template <typename Query, typename OutputTuple>
void elelel::sqlite::parameters_container<Query, OutputTuple>::clear(std::error_code& ec) const {
  ec = result_code{::sqlite3_clear_bindings(query_.stmt())};
}

template <typename Query, typename OutputTuple>
void elelel::sqlite::parameters_container<Query, OutputTuple>::clear() const {
  std::error_code ec;
  ec = result_code{::sqlite3_clear_bindings(query_.stmt())};
  if (ec != result::success) throw std::system_error(ec);
}
