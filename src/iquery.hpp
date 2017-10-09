#pragma once

#include "basic_query.hpp"

namespace elelel {
  namespace sqlite {
    template <typename Tuple>
    struct iquery : basic_query {
      iquery(database_ptr db);
      
    };
  }
}
