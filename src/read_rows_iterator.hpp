#pragma once

namespace elelel {
  namespace sqlite {
    template <typename Container, typename T>
    struct read_rows_iterator {
      using type = read_rows_iterator<Container, T>;
      using value_type = T;  // Argument descriptor type
      using container_type = Container;
      using difference_type = std::ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      using iterator_category = std::input_iterator_tag;
      
      read_rows_iterator(Container& cont, const bool end);

      bool operator==(const type& other) const;
      bool operator!=(const type& other) const;
      type& operator++();
      type operator++(int);

      reference operator*() const;

    private:
      int pos_{0};
      bool end_{false};
    };
  }
}
