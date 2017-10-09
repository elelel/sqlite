#include "../query.hpp"

template <typename ParamsTuple, typename ResultsTuple>
elelel::sqlite::query<ParamsTuple, ResultsTuple>::query(database_ptr db, const std::string& query_str) :
  db_(db),
  begin_{false},
  end_{false},
  stmt_{db_->prepare_statement(query_str)},
  params{stmt_},
  results{stmt_} {
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::step(std::error_code& ec) {
  ec = result_code{::sqlite3_step(**stmt_)};
  begin_ = true;
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::step() {
  std::error_code ec;
  step(ec);
  if (ec != result::success) throw std::system_error(ec);
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::execute(std::error_code& ec) {
  step(ec);
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::execute() {
  step();
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::reset(std::error_code& ec) {
  ec = result_code{::sqlite3_reset(**stmt_)};
  if (ec == result::success) begin_ = false;
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::reset() {
  std::error_code ec;
  reset(ec);
  if (ec != result::success) throw std::system_error(ec);
}

template <typename ParamsTuple, typename ResultsTuple>
auto elelel::sqlite::query<ParamsTuple, ResultsTuple>::stmt() const -> ::sqlite3_stmt* {
  return *stmt_;
}
