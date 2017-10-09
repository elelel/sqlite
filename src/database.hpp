#pragma once

#include <memory>
#include <string>
#include <system_error>

#include <sqlite3.h>

#include "statement.hpp"

namespace elelel {
  namespace sqlite {
    struct database {
      database();
      database(const std::string& filename);
      database(const database& other);
      database(database&& other);
      void swap(database& other);
      database& operator=(const database& other);
      database& operator=(database&& other);
      operator bool() const;
      
      void open(const std::string& filename, std::error_code& ec);
      void open(const std::string& filename);
      std::shared_ptr<statement> prepare_statement(const std::string& query_str,
                                                   std::error_code& ec,
                                                   unsigned int flags = 0) const;
      std::shared_ptr<statement> prepare_statement(const std::string& query_str,
                                                   const unsigned int flags = 0) const;

    private:
      std::string filename_;
      std::shared_ptr<::sqlite3> db_;
    };

    using database_ptr = std::shared_ptr<database>;
  }
}
