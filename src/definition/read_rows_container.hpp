#include "../read_rows_container.hpp"

template <typename RowTuple>
elelel::sqlite::read_rows_container<RowTuple>::read_rows_container(std::shared_ptr<statement> stmt) :
  stmt_(stmt) {
}
