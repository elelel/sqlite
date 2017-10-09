#include "../result.hpp"


inline std::error_code elelel::sqlite::make_error_code(elelel::sqlite::result_code e) {
  return {static_cast<int>(e), elelel::sqlite::result_code_category::instance()};
}

inline std::error_condition elelel::sqlite::make_error_condition(elelel::sqlite::result e) {
  return {static_cast<int>(e), elelel::sqlite::result_category::instance()};
}
