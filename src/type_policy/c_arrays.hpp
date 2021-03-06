#include "../type_policy.hpp"

#include <optional>

#include "../result.hpp"

namespace elelel {
  namespace sqlite {
    template <>
    struct type_policy<std::tuple<void*, int>> {
      using bind_type = const std::optional<std::tuple<const void*, int>>&;
      using return_type = std::optional<std::tuple<const void*, int>>;
      static const auto fundamental = SQLITE_BLOB;

      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
        if (value && std::get<0>(*value)) {
          return result_code{sqlite3_bind_blob(stmt, i, std::get<0>(*value), std::get<1>(*value), SQLITE_TRANSIENT)};
        } else {
          return result_code{sqlite3_bind_null(stmt, i)};
        }
      }

      static return_type get(sqlite3_stmt* stmt, const int i) {
        if (::sqlite3_column_type(stmt, i) != SQLITE_NULL) {
          return return_type{{::sqlite3_column_blob(stmt, i), ::sqlite3_column_bytes(stmt, i)}};
        }
        return return_type{};
      }
    };
    
    template <>
    struct type_policy<std::tuple<char*, int>> {
      using bind_type = const std::optional<std::tuple<const char*, int>>&;
      using return_type = std::optional<std::tuple<char*, int>>;
      static const auto fundamental = SQLITE_TEXT;

      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
        if (value && std::get<0>(*value)) {
          return result_code{sqlite3_bind_text(stmt, i, std::get<0>(*value), std::get<1>(*value), SQLITE_TRANSIENT)};
        } else {
          return result_code{sqlite3_bind_null(stmt, i)};
        }
      }

      static return_type get(sqlite3_stmt* stmt, const int i) {
        if (::sqlite3_column_type(stmt, i) != SQLITE_NULL) {
          return return_type{{(char*)::sqlite3_column_blob(stmt, i), ::sqlite3_column_bytes(stmt, i)}};
        }
        return return_type{};
      }
      
    };
  }
}
