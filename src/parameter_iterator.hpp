#pragma once

namespace elelel {
  namespace sqlite {
    template <typename Container, typename T>
    struct parameters_iterator {
      using type = parameters_iterator<Container, T>;
      using value_type = T;  // Argument descriptor type
      using container_type = Container;
      using difference_type = std::ptrdiff_t;
      using pointer = T*;
      using reference = T&;
      using iterator_category = std::random_access_iterator_tag;

      parameters_iterator(Container cont, const int pos = 1);
      parameters_iterator(const type& other);
      void swap(type& other);
      type& operator=(const type& other);

      value_type operator*();

      bool operator==(const type& other) const;
      bool operator!=(const type& other) const;
      bool operator<(const type& other) const;
      bool operator<=(const type& other) const;
      bool operator>(const type& other) const;
      bool operator>=(const type& other) const;
      
      
    private:
      Container& container_;
      int pos_;
    };
  }
}
