#pragma once

#include "statement.hpp"
#include "type_policy.hpp"
#include "parameter_iterator.hpp"

namespace elelel {
  namespace sqlite {
    
    template <typename Query, typename OutputTuple>
    struct parameters_container {
      using arg_tuple = OutputTuple;
      using type = parameters_container<Query, arg_tuple>;
      using iterator = parameters_iterator<type, arg_tuple>;
      
      parameters_container(Query& query);
      parameters_container(const type& other);

      // --- Bind single param by index
      // Wrapped
      template <typename WrappedValue>
      void bind_wrapped_value(std::error_code& ec, const int i, WrappedValue&& value) {
        typedef typename std::remove_reference<WrappedValue>::type::value_type value_type;
        ec = type_policy<value_type>::bind(query_.stmt(), i, std::forward<WrappedValue>(value));
      }

      template <typename WrappedValue>
      void bind_wrapped_value(const int i, WrappedValue&& value) {
        std::error_code ec;
        bind_wrapped_value(ec, i, std::forward<WrappedValue>(value));
        if (ec != result::success) throw std::system_error(ec);
      }

      // Unwrapped
      template <typename UnwrappedValue>
      void bind_value(std::error_code& ec, const int i, UnwrappedValue&& value) {
        bind_wrapped_value(ec, i, nullable<UnwrappedValue>::wrap(std::forward<UnwrappedValue>(value)));
      }

      template <typename UnwrappedValue>
      void bind_value(const int i, UnwrappedValue&& value) {
        bind_wrapped_value(i, nullable<UnwrappedValue>::wrap(std::forward<UnwrappedValue>(value)));
      }

      // --- Bing multiple params, variadic
      // Wrapped
      template <size_t I = 1, typename WrappedValue>
      void bind_wrapped_values(std::error_code& ec, WrappedValue&& value) {
        ec = type_policy<typename WrappedValue::value_type>::bind(query_.stmt(), I, std::forward<WrappedValue>(value));
      }
      
      template <size_t I = 1, typename WrappedValue, typename... WrappedValues>
      void bind_wrapped_values(std::error_code& ec, WrappedValue&& value, WrappedValues&&... values) {
        ec = type_policy<typename WrappedValue::value_type>::bind(query_.stmt(), I, value);
        if (ec == result::success) {
          bind_wrapped_values(ec, std::forward<WrappedValues>(values)...);
        }
      }

      template <typename... WrappedValues>
      void bind_wrapped_values(WrappedValues&&... values) {
        std::error_code ec;
        bind_wrapped_values(ec, std::forward<WrappedValues>(values)...);
        if (ec != result::success) throw std::system_error(ec);
      }

      // Unwrapped
      template <typename... UnwrappedValues>
      void bind_values(std::error_code& ec, UnwrappedValues&&... values) {
        ec = bind_wrapped_tuple_(sqlite::make_params(std::forward<UnwrappedValues>(values)...));
      }

      template <typename... UnwrappedValues>
      void bind_values(UnwrappedValues&&... values) {
        std::error_code ec;
        bind_values(ec, std::forward<UnwrappedValues>(values)...);
        if (ec != result::success) throw std::system_error(ec);
      }

      // --- Bind params as tuple
      template <typename ParamsTuple>
      void bind_wrapped_tuple(std::error_code& ec, ParamsTuple&& tuple) {
        ec = bind_wrapped_tuple_(std::forward<OutputTuple>(tuple));
      }

      template <typename ParamsTuple>
      void bind_wrapped_tuple(ParamsTuple&& tuple) {
        std::error_code ec;
        bind_wrapped_tuple(ec, std::forward<ParamsTuple>(tuple));
        if (ec != result::success) throw std::system_error(ec);
      }
      
      // Bind params synonym
      template <typename ParamsTuple>
      void bind(std::error_code& ec, ParamsTuple&& tuple) {
        using A = typename std::remove_reference<ParamsTuple>::type;
        using B = typename std::remove_reference<OutputTuple>::type;
        static_assert(std::is_same<A, B>::value, "parameters tuple to bind should be the same as in declaration of query");
        bind_wrapped_tuple(ec, std::forward<OutputTuple>(tuple));
      }

      template <typename ParamsTuple>
      void bind(ParamsTuple&& tuple) {
        using A = typename std::remove_reference<ParamsTuple>::type;
        using B = typename std::remove_reference<OutputTuple>::type;
        static_assert(std::is_same<A, B>::value, "parameters tuple to bind should be the same as in declaration of query");
        bind_wrapped_tuple(std::forward<OutputTuple>(tuple));
      }

      // Clear bindings
      void clear(std::error_code& ec) const;
      void clear() const;

    private:
      Query& query_;

      // Bind argments tuple implementation
      template <size_t I = 0, typename... TupleArgs>
      typename std::enable_if<I == sizeof...(TupleArgs), std::error_code>::type
      bind_wrapped_tuple_(std::tuple<TupleArgs...>&&) {
        return result_code{0};
      }

      template <size_t I = 0, typename... TupleArgs>
      typename std::enable_if<I != sizeof...(TupleArgs), std::error_code>::type
      bind_wrapped_tuple_(std::tuple<TupleArgs...>&& tuple) {
        std::error_code ec;
        bind_wrapped_value(ec, I + 1, std::get<I>(tuple));
        if (ec != result::success) return ec;
        return bind_wrapped_tuple_<I + 1, TupleArgs...>(std::forward<std::tuple<TupleArgs...>>(tuple));
      }
    };
  }
}
