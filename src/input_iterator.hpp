#pragma once

#include "query.hpp"

namespace elelel {
  namespace sqlite {
    template <typename Query, typename T>
    struct input_iterator {
      using type = input_iterator<Query, T>;
      using value_type = T;
      using container_type = Query;
      using difference_type = std::ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      using iterator_category = std::forward_iterator_tag;
      
      input_iterator(Query& query);
      input_iterator(const input_iterator& other);
      input_iterator& operator=(const input_iterator& other);


    private:
      Query& query_;
    };
  }
}
