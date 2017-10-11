#include "../type_policy.hpp"

#include <optional>

#include "../result.hpp"

namespace elelel {
  namespace sqlite {
    template <>
    struct type_policy<int32_t> {
      using bind_type = const std::optional<int32_t>&;
      using return_type = std::optional<int32_t>;
      static const auto fundamental = SQLITE_INTEGER;

      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
        if (value) {
          return result_code{sqlite3_bind_int(stmt, i, *value)};
        } else {
          return result_code{sqlite3_bind_null(stmt, i)};
        }
      }

      static return_type get(sqlite3_stmt* stmt, const int i) {
        if (::sqlite3_column_type(stmt, i) != SQLITE_NULL) {
          return return_type{::sqlite3_column_int(stmt, i)};
        }
        return return_type{};
      }
    };

    template <>
    struct type_policy<int64_t> {
      using bind_type = const std::optional<int32_t>&;
      using return_type = std::optional<int32_t>;
      static const auto fundamental = SQLITE_INTEGER;

      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
        if (value) {
          return result_code{sqlite3_bind_int64(stmt, i, *value)};
        } else {
          return result_code{sqlite3_bind_null(stmt, i)};
        }
      }

      static return_type get(sqlite3_stmt* stmt, const int i) {
        if (::sqlite3_column_type(stmt, i) != SQLITE_NULL) {
          return return_type{::sqlite3_column_int64(stmt, i)};
        }
        return return_type{};
      }
    };
    
    template <>
    struct type_policy<double> {
      using bind_type = const std::optional<double>&;
      using return_type = std::optional<double>;
      static const auto fundamental = SQLITE_FLOAT;

      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
        if (value) {
          return result_code{sqlite3_bind_double(stmt, i, *value)};
        } else {
          return result_code{sqlite3_bind_null(stmt, i)};
        }
      }

      static return_type get(sqlite3_stmt* stmt, const int i) {
        if (::sqlite3_column_type(stmt, i) != SQLITE_NULL) {
          return return_type{::sqlite3_column_double(stmt, i)};
        }
        return return_type{};
      }
    };
  }
}
