#pragma once

namespace elelel {
  namespace sqlite {
    template <typename Container, typename T>
    struct read_rows_iterator {
      using type = read_rows_iterator<Container, T>;
      using value_type = T;
      using container_type = Container;
      using difference_type = std::ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      using iterator_category = std::input_iterator_tag;
      
      read_rows_iterator(Container& cont, const bool end = false);
      read_rows_iterator(const type& other);
      void swap(type& other);
      type& operator=(const type& other);

      bool operator==(const type& other) const;
      bool operator!=(const type& other) const;
      type& operator++();
      type operator++(int);

      value_type operator*() const;

    private:
      Container& cont_;
      int pos_{0};
      bool end_{false};
    };
  }
}
