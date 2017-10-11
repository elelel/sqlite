#pragma once

#include "query.hpp"
#include "read_rows_iterator.hpp"
#include "statement.hpp"

namespace elelel {
  namespace sqlite {
    template <typename Query, typename RowTuple>
    struct read_rows_container {
      template <typename Container, typename T>
      friend struct read_rows_iterator;

      using type = read_rows_container<Query, RowTuple>;
      using iterator = read_rows_iterator<type, RowTuple>;
      
      read_rows_container(Query& q);
      read_rows_container(const type& other);
      void swap(type& other);
      type& operator=(const type& other);
      
      iterator begin(std::error_code& ec);
      iterator begin();
      iterator end(std::error_code& ec);
      iterator end();

      // Return curret row's wrapped value of column i 
      template <typename UnwrappedValue>
      void get(const int i, std::optional<UnwrappedValue>& value) const;
      template <typename UnwrappedValue>
      std::optional<UnwrappedValue> get(const int i) const;

      // Return curret row's size of column i in bytes
      template <typename UnwrappedValue>
      int size(const int i) const;
      
      // Read current row's values and returned them in wrapped form
      RowTuple row() const;
      
    private:
      Query& query_;

      template <size_t I = 0>
      typename std::enable_if<I == std::tuple_size<RowTuple>::value, void>::type
      row_(RowTuple&) const {
      }

      template <size_t I = 0>
      typename std::enable_if<I != std::tuple_size<RowTuple>::value, void>::type
      row_(RowTuple& r) const {
        using wrapped_type = typename std::tuple_element<I, RowTuple>::type;
        using unwrapped_type = typename wrapped_type::value_type;
        std::get<I>(r) = wrapped_type{type_policy<unwrapped_type>::get(query_.stmt(), I)};
        row_<I + 1>(r);
      }
    };
  }
}
