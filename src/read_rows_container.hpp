#pragma once

#include "statement.hpp"
#include "read_rows_iterator.hpp"

namespace elelel {
  namespace sqlite {
    template <typename RowTuple>
    struct read_rows_container {
      using type = read_rows_container<RowTuple>;
      using iterator = read_rows_iterator<type, RowTuple>;
      
      read_rows_container(std::shared_ptr<statement> stmt);
      
      iterator begin();
      iterator end();

      
    private:
      std::shared_ptr<statement> stmt_;
    };
  }
}
