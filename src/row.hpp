#pragma once

namespace elelel {
  namespace sqlite {
    // NULL template to create null sql values
    template <typename T>
    struct null {
      using sqlite_null_tag = int;
      using value_type = T;
      null() {};
    };

    // Nullable wrapper, wrap value into optional
    template <typename T>
    struct nullable {
      using type = std::optional<typename std::remove_reference<T>::type>;

      static type wrap(T&& value) {
        return type(std::forward<T>(value));
      }
    };

    // Nullable wrapper, constuct empty optional
    template <typename T>
    struct nullable<null<T>> {
      using type = std::optional<T>;

      static type wrap(null<T>&&) {
        return type{};
      }
    };

    // Sqlite tuple type
    template <typename... Elements>
    struct sqlite_tuple;

    template <>
    struct sqlite_tuple<> {
      using type = std::tuple<>;
    };

    template <typename Element, typename... Elements>
    struct sqlite_tuple<Element, Elements...> {
      using type =
        decltype(std::tuple_cat
                 (std::tuple<typename nullable<Element>::type>(),
                  typename sqlite_tuple<Elements...>::type()));
    };

    template <typename Element>
    typename sqlite_tuple<Element>::type make_sqlite_tuple(Element&& c) {
      return std::make_tuple(nullable<Element>::wrap(std::forward<Element>(c)));
    }

    template <typename Element, typename... Elements>
    typename sqlite_tuple<Element, Elements...>::type make_sqlite_tuple(Element&& c, Elements&&... cs) {
      return std::tuple_cat(std::make_tuple(nullable<Element>::wrap(std::forward<Element>(c))),
                            make_sqlite_tuple(std::forward<Elements>(cs)...));
    }

    // Synonyms
    template <typename... Columns>
    using row = sqlite_tuple<Columns...>;

    template <typename... Columns>
    typename row<Columns...>::type make_row(Columns&&... cs) {
      return make_sqlite_tuple(std::forward<Columns>(cs)...);
    }

    template <typename... Params>
    using params = sqlite_tuple<Params...>;

    template <typename... Params>
    typename params<Params...>::type make_params(Params&&... ps) {
      return make_sqlite_tuple(std::forward<Params>(ps)...);
    }
  }
}
