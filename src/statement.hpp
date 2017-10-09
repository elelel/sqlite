#pragma once

#include <sqlite3.h>

namespace elelel {
  namespace sqlite {
    // Defined solely to workaround shady C patterns in sqlite3.h of dynamic version of Sqlite
    struct statement {
      statement(::sqlite3_stmt* stmt) :
        stmt_(stmt) {
      }

      ~statement() {
        sqlite3_finalize(stmt_);        
      }
      
      ::sqlite3_stmt* operator*() const {
        return stmt_;
      }
    private:
      ::sqlite3_stmt* stmt_{nullptr};
    };
  }
}
