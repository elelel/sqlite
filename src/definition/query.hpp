#include "../query.hpp"

template <typename ParamsTuple, typename ResultsTuple>
elelel::sqlite::query<ParamsTuple, ResultsTuple>::query(database_ptr db, const std::string& query_str) :
  db_(db),
  at_results_begin_{false},
  at_results_end_{false},
  stmt_{db_->prepare_statement(query_str)},
  params{*this},
  results{*this}
{
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::step(std::error_code& ec) {
  ec = result_code{::sqlite3_step(**stmt_)};
  at_results_begin_ = true;
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
  if (ec == result::success) at_results_begin_ = false;
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::reset() {
  std::error_code ec;
  reset(ec);
  if (ec != result::success) throw std::system_error(ec);
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::close(std::error_code& ec) {
  ec = result_code{::sqlite3_finalize(**stmt_)};
  if (ec == result::success) {
    at_results_begin_ = false;
    at_results_end_ = false;
  }
}

template <typename ParamsTuple, typename ResultsTuple>
void elelel::sqlite::query<ParamsTuple, ResultsTuple>::close() {
  std::error_code ec;
  close(ec);
  if (ec != result::success) throw std::system_error(ec);
}

template <typename ParamsTuple, typename ResultsTuple>
auto elelel::sqlite::query<ParamsTuple, ResultsTuple>::stmt() const -> ::sqlite3_stmt* {
  return **stmt_;
}

template <typename ParamsTuple, typename ResultsTuple>
auto elelel::sqlite::query<ParamsTuple, ResultsTuple>::at_results_begin() const -> bool {
  return at_results_begin_;
}

template <typename ParamsTuple, typename ResultsTuple>
auto elelel::sqlite::query<ParamsTuple, ResultsTuple>::at_results_end() const -> bool {
  return at_results_end_;
}
