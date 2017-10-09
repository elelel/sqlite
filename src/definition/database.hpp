#include "../database.hpp"

elelel::sqlite::database::database() {
}

elelel::sqlite::database::database(const std::string& filename) :
  database() {
  open(filename);
}

elelel::sqlite::database::database(const database& other) :
  filename_(other.filename_),
  db_(other.db_) {
}

elelel::sqlite::database::database(database&& other) :
  filename_(std::move(other.filename_)),
  db_(std::move(other.db_)) {
}

void elelel::sqlite::database::swap(database& other) {
  std::swap(filename_, other.filename_);
  std::swap(db_, other.db_);
}

auto elelel::sqlite::database::operator=(const database& other) -> database& {
  database tmp(other);
  swap(tmp);
  return *this;
}

auto elelel::sqlite::database::operator=(database&& other) -> database& {
  filename_ = std::move(other.filename_);
  db_ = std::move(other.db_);
  return *this;
}

elelel::sqlite::database::operator bool() const {
  return db_.operator bool();
}

void elelel::sqlite::database::open(const std::string& filename,
                                    std::error_code& ec) {
  ::sqlite3* db;
  ec = std::error_code(result_code{sqlite3_open(filename.c_str(), &db)});
  if (ec == result::success) {
    sqlite3_extended_result_codes(db, 1); // Enable extended result codes by default
    filename_ = filename;
    db_ = std::shared_ptr<::sqlite3>(db, [this] (sqlite3* p) {
        // Ignore rc and errors, we're in destructor
        try {
          sqlite3_close(p);
        } catch (...) {
          // Nothing can be thrown from C anyway...
        }
      });
  } 
}

void elelel::sqlite::database::open(const std::string& filename) {
  std::error_code ec;
  open(filename, ec);
  if (ec != result::success) { throw std::system_error(ec); }
}

auto elelel::sqlite::database::prepare_statement(const std::string& query_str, std::error_code& ec, const unsigned int flags) const -> std::shared_ptr<statement> {
  ::sqlite3_stmt* stmt;
  ec = std::error_code(result_code(sqlite3_prepare_v3(db_.get(), query_str.c_str(), query_str.length() + 1, flags, &stmt, nullptr)));
  if (ec == result::success) {
    return std::make_shared<statement>(stmt);
  }
  return nullptr;
}

auto elelel::sqlite::database::prepare_statement(const std::string& query_str, const unsigned int flags) const -> std::shared_ptr<statement> {
  std::error_code ec;
  auto rslt = prepare_statement(query_str, ec, flags);
  if (ec != result::success) { throw std::system_error(ec); }
  return rslt;
}
