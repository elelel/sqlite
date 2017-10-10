#pragma once

#include "statement.hpp"
#include "read_rows_iterator.hpp"
#include "query.hpp"

namespace elelel {
  namespace sqlite {
    template <typename Query, typename RowTuple>
    struct read_rows_container {
      using type = read_rows_container<Query, RowTuple>;
      using iterator = read_rows_iterator<type, RowTuple>;
      
      read_rows_container(Query& q);
      
      iterator begin(std::error_code& ec);
      iterator begin();
      iterator end(std::error_code& ec);
      iterator end();
      
    private:
      Query& query_;
    };
  }
}
