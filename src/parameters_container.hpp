#pragma once

#include "statement.hpp"
#include "type_policy.hpp"
#include "parameter_iterator.hpp"

namespace elelel {
  namespace sqlite {
    
    template <typename OutputTuple>
    struct parameters_container {
      using arg_tuple = OutputTuple;
      using type = parameters_container<arg_tuple>;
      using iterator = parameters_iterator<type, arg_tuple>;
      
      parameters_container(std::shared_ptr<statement> stmt);
      parameters_container(const type& other);
      void swap(type& other);
      type& operator=(const type& other);

      // Bind single param by index
      template <typename WrappedValue>
      void bind_value(std::error_code& ec, const int i, WrappedValue&& value) {
        typedef typename std::remove_reference<WrappedValue>::type::value_type value_type;
        ec = type_policy<value_type>::bind(**stmt_, i, std::forward<WrappedValue>(value));
      }

      template <typename WrappedValue>
      void bind_value(const int i, WrappedValue&& value) {
        std::error_code ec;
        bind_value(ec, i, std::forward<WrappedValue>(value));
        if (ec != result::success) throw std::system_error(ec);
      }

      // Bing multiple params, variadic
      template <size_t I = 1, typename WrappedValue>
      void bind_values(std::error_code& ec, WrappedValue&& value) {
        ec = type_policy<typename WrappedValue::value_type>::bind(**stmt_, I, std::forward<WrappedValue>(value));
      }
      
      template <size_t I = 1, typename WrappedValue, typename... WrappedValues>
      void bind_values(std::error_code& ec, WrappedValue&& value, WrappedValues&&... values) {
        ec = type_policy<typename WrappedValue::value_type>::bind(**stmt_, I, value);
        if (ec == result::success) {
          bind_values(ec, std::forward<WrappedValues>(values)...);
        }
      }

      template <typename... WrappedValues>
      void bind_values(WrappedValues&&... values) {
        std::error_code ec;
        bind_values(ec, std::forward<WrappedValues>(values)...);
        if (ec != result::success) throw std::system_error(ec);
      }

      // Bind params as tuple
      std::error_code bind_tuple(OutputTuple&& tuple) {
        return bind_tuple_(std::forward<OutputTuple>(tuple));
      }

      // Bind params synonym
      std::error_code bind(OutputTuple&& tuple) {
        return bind_tuple(std::forward<OutputTuple>(tuple));
      }

      // Clear bindings
      void clear(std::error_code& ec) const;
      void clear() const;

    private:
      std::shared_ptr<statement> stmt_;

      // Bind argments tuple implementation
      template <size_t I = 0, typename... TupleArgs>
      typename std::enable_if<I == sizeof...(TupleArgs), std::error_code>::type
      bind_tuple_(std::tuple<TupleArgs...>&&) {
        return result_code{0};
      }

      template <size_t I = 0, typename... TupleArgs>
      typename std::enable_if<I != sizeof...(TupleArgs), std::error_code>::type
      bind_tuple_(std::tuple<TupleArgs...>&& tuple) {
        std::error_code ec;
        bind_value(ec, I + 1, std::get<I>(tuple));
        if (ec != result::success) return ec;
        return bind_tuple_<I + 1, TupleArgs...>(std::forward<std::tuple<TupleArgs...>>(tuple));
      }
    };
  }
}
