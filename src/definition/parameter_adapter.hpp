#include "../param_adapter.hpp"

elelel::sqlite::param_adapter<Container, T>::param_adapter(Container& cont, const int index) :
  container_(cont),
  index_(index) {
  }

auto elelel::sqlite::param_adapter<Container, T>::operator=(const T& v) -> param_adapter<Container, T> {
  std::error_code ec = cont_.bind_value(index_, v);
  if (ec != result::success) throw std::system_error(ec);
  return *this;
}
