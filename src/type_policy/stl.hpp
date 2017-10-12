#include "../type_policy.hpp"

#include <optional>

#include "../result.hpp"

namespace elelel {
  namespace sqlite {
    template <>
    struct type_policy<std::vector<char>> {
      using bind_type = const std::optional<std::vector<char>>&;
      using return_type = std::optional<std::vector<char>>;
      static const auto fundamental = SQLITE_BLOB;

      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
        if (value) {
          return result_code{sqlite3_bind_blob(stmt, i, value->data(), value->size(), SQLITE_TRANSIENT)};
        } else {
          return result_code{sqlite3_bind_null(stmt, i)};
        }
      }

      static return_type get(sqlite3_stmt* stmt, const int i) {
        if (::sqlite3_column_type(stmt, i) != SQLITE_NULL) {
          const auto sz = ::sqlite3_column_bytes(stmt, i);
          const auto p = ::sqlite3_column_blob(stmt, i);
          return_type rslt{return_type::value_type()};
          rslt->reserve(sz);
          std::copy(p, (void*)((ptrdiff_t)p + sz), std::back_inserter(*rslt));
          return rslt;
        }
        return return_type{};
      }
    };

    template <>
    struct type_policy<std::string> {
      using bind_type = const std::optional<std::string>&;
      using return_type = std::optional<std::string>;
      static const auto fundamental = SQLITE_TEXT;

      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
        if (value) {
          return result_code{sqlite3_bind_text(stmt, i, value->data(), value->size(), SQLITE_TRANSIENT)};
        } else {
          return result_code{sqlite3_bind_null(stmt, i)};
        }
      }

      static return_type get(sqlite3_stmt* stmt, const int i) {
        if (::sqlite3_column_type(stmt, i) != SQLITE_NULL) {
          const auto sz = ::sqlite3_column_bytes(stmt, i);
          const auto p = ::sqlite3_column_text(stmt, i);
          return_type rslt{return_type::value_type()};
          rslt->reserve(sz);
          std::copy(p, (void*)((uintptr_t)p + sz), std::back_inserter(*rslt));
          return rslt;
        }
        return return_type{};
      }
    };
    
  }
}
