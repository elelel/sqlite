#pragma once

#include <string>
#include <system_error>

#include <sqlite3.h>

// Sqlite result codes

namespace elelel {
  namespace sqlite {
    enum class result_code {
      ERROR = 1,
      INTERNAL = 2,
      PERM = 3,
      ABORT = 4,
      BUSY = 5,
      LOCKED = 6,
      NOMEM = 7,
      READONLY = 8,
      INTERRUPT = 9,
      IOERR = 10,
      CORRUPT = 11,
      NOTFOUND = 12,
      FULL = 13,
      CANTOPEN = 14,
      PROTOCOL = 15,
      EMPTY = 16,
      SCHEMA = 17,
      TOOBIG = 18,
      CONSTRAINT = 19,
      MISMATCH = 20,
      MISUSE = 21,
      NOLFS = 22,
      AUTH = 23,
      FORMAT = 24,
      RANGE = 25,
      NOTADB = 26,
      NOTICE = 27,
      WARNING = 28,
      ROW = 100,
      DONE = 101,
      // Extended

      ABORT_ROLLBACK = 516,
      BUSY_RECOVERY = 261,
      BUSY_SNAPSHOT = 517,
      CANTOPEN_CONVPATH = 1038,
      CANTOPEN_FULLPATH = 782,
      CANTOPEN_ISDIR = 526,
      CANTOPEN_NOTEMPDIR = 270,
      CONSTRAINT_CHECK = 275,
      CONSTRAINT_COMMITHOOK = 531,
      CONSTRAINT_FOREIGNKEY = 787,
      CONSTRAINT_FUNCTION = 1043,
      CONSTRAINT_NOTNULL = 1299,
      CONSTRAINT_PRIMARYKEY = 1555,
      CONSTRAINT_ROWID = 2579,
      CONSTRAINT_TRIGGER = 1811,
      CONSTRAINT_UNIQUE = 2067,
      CONSTRAINT_VTAB = 2323,
      CORRUPT_VTAB = 267,
      IOERR_ACCESS = 3338,
      IOERR_BLOCKED = 2826,
      IOERR_CHECKRESERVEDLOCK = 3594,
      IOERR_CLOSE = 4106,
      IOERR_CONVPATH = 6666,
      IOERR_DELETE = 2570,
      IOERR_DELETE_NOENT = 5898,
      IOERR_DIR_CLOSE = 4362,
      IOERR_DIR_FSYNC = 1290,
      IOERR_FSTAT = 1802,
      IOERR_FSYNC = 1034,
      IOERR_GETTEMPPATH = 6410,
      IOERR_LOCK = 3850,
      IOERR_MMAP = 6154,
      IOERR_NOMEM = 3082,
      IOERR_RDLOCK = 2314,
      IOERR_READ = 266,
      IOERR_SEEK = 5642,
      IOERR_SHMLOCK = 5130,
      IOERR_SHMMAP = 5386,
      IOERR_SHMOPEN = 4618,
      IOERR_SHMSIZE = 4874,
      IOERR_SHORT_READ = 522,
      IOERR_TRUNCATE = 1546,
      IOERR_UNLOCK = 2058,
      IOERR_WRITE = 778,
      LOCKED_SHAREDCACHE = 262,
      NOTICE_RECOVER_ROLLBACK = 539,
      NOTICE_RECOVER_WAL = 283,
      OK_LOAD_PERMANENTLY = 256,
      READONLY_CANTLOCK = 520,
      READONLY_DBMOVED = 1032,
      READONLY_RECOVERY = 264,
      READONLY_ROLLBACK = 776,
      WARNING_AUTOINDEX = 284,      
    };

    std::error_code make_error_code(result_code ev);
  }
}

namespace std {
  template <>
  struct is_error_code_enum<elelel::sqlite::result_code> : true_type {};
}

namespace elelel {
  namespace sqlite {
    struct result_code_category : std::error_category {
      virtual const char* name() const noexcept override { return "Sqlite result code"; }
      virtual std::string message(int v) const override {
        return std::string(sqlite3_errstr(v));
      }
      
      static result_code_category& instance() {
        static result_code_category c;
        return c;
      }
    };
  }
}

namespace elelel {
  namespace sqlite {
    enum class result {
      success = 1,
      error,
      internal,
      perm,
      abort,
      busy,
      locked,
      nomem,
      readonly,
      interrupt,
      ioerr,
      corrupt,
      notfound,
      full,
      cantopen,
      protocol,
      empty,
      schema,
      toobig,
      constraint,
      mismatch,
      misuse,
      nolfs,
      auth,
      format,
      range,
      notadb,
      notice,
      warning,
      row,
      done
    };

    std::error_condition make_error_condition(result r);
  }
}

namespace std {
  template <>
  struct is_error_condition_enum<elelel::sqlite::result> : true_type {};
}

namespace elelel {
  namespace sqlite {
    struct result_category : std::error_category {
      virtual const char* name() const noexcept override { return "Sqlite result"; }
          
      virtual std::string message(int v) const override {
        return std::string(sqlite3_errstr(v));
      }
          
      bool equivalent(const std::error_code& ec, int condition) const noexcept override {
        switch (static_cast<result>(condition)) {
        case result::success: return !ec
            || (ec == result_code::OK_LOAD_PERMANENTLY)
            || (ec == result_code::ROW)
            || (ec == result_code::DONE);
        case result::error: return (ec == result_code::ERROR);
        case result::internal: return (ec == result_code::INTERNAL);
        case result::perm: return (ec == result_code::PERM);
        case result::abort: return (ec == result_code::ABORT)
            || (ec == result_code::ABORT_ROLLBACK);
        case result::busy: return (ec == result_code::BUSY)
            || (ec == result_code::BUSY_RECOVERY)
            || (ec == result_code::BUSY_SNAPSHOT)
            ;
        case result::locked: return (ec == result_code::LOCKED)
            || (ec == result_code::LOCKED_SHAREDCACHE);
        case result::nomem: return (ec == result_code::NOMEM);
        case result::readonly: return (ec == result_code::READONLY)
            || (ec == result_code::READONLY_CANTLOCK)
            || (ec == result_code::READONLY_DBMOVED)
            || (ec == result_code::READONLY_RECOVERY)
            || (ec == result_code::READONLY_ROLLBACK);

        case result::interrupt: return (ec == result_code::INTERRUPT);
        case result::ioerr: return (ec == result_code::IOERR)
            || (ec == result_code::IOERR_ACCESS)
            || (ec == result_code::IOERR_BLOCKED)
            || (ec == result_code::IOERR_CHECKRESERVEDLOCK)
            || (ec == result_code::IOERR_CLOSE)
            || (ec == result_code::IOERR_CONVPATH)
            || (ec == result_code::IOERR_DELETE)
            || (ec == result_code::IOERR_DELETE_NOENT)
            || (ec == result_code::IOERR_DIR_CLOSE)
            || (ec == result_code::IOERR_DIR_FSYNC)
            || (ec == result_code::IOERR_FSTAT)
            || (ec == result_code::IOERR_FSYNC)
            || (ec == result_code::IOERR_GETTEMPPATH)
            || (ec == result_code::IOERR_LOCK)
            || (ec == result_code::IOERR_MMAP)
            || (ec == result_code::IOERR_NOMEM)
            || (ec == result_code::IOERR_RDLOCK)
            || (ec == result_code::IOERR_READ)
            || (ec == result_code::IOERR_SEEK)
            || (ec == result_code::IOERR_SHMLOCK)
            || (ec == result_code::IOERR_SHMMAP)
            || (ec == result_code::IOERR_SHMOPEN)
            || (ec == result_code::IOERR_SHMSIZE)
            || (ec == result_code::IOERR_SHORT_READ)
            || (ec == result_code::IOERR_TRUNCATE)
            || (ec == result_code::IOERR_UNLOCK)
            || (ec == result_code::IOERR_WRITE);
            
        case result::corrupt: return (ec == result_code::CORRUPT)
            || (ec == result_code::CORRUPT_VTAB);
        case result::notfound: return (ec == result_code::NOTFOUND);
        case result::full: return (ec == result_code::FULL);
        case result::cantopen: return (ec == result_code::CANTOPEN)
            || (ec == result_code::CANTOPEN_CONVPATH)
            || (ec == result_code::CANTOPEN_FULLPATH)
            || (ec == result_code::CANTOPEN_ISDIR)
            || (ec == result_code::CANTOPEN_NOTEMPDIR);

        case result::protocol: return (ec == result_code::PROTOCOL);
        case result::empty: return (ec == result_code::EMPTY);
        case result::schema: return (ec == result_code::SCHEMA);
        case result::toobig: return (ec == result_code::TOOBIG);
        case result::constraint: return (ec == result_code::CONSTRAINT)
            || (ec == result_code::CONSTRAINT_CHECK)
            || (ec == result_code::CONSTRAINT_COMMITHOOK)
            || (ec == result_code::CONSTRAINT_FOREIGNKEY)
            || (ec == result_code::CONSTRAINT_FUNCTION)
            || (ec == result_code::CONSTRAINT_NOTNULL)
            || (ec == result_code::CONSTRAINT_PRIMARYKEY)
            || (ec == result_code::CONSTRAINT_ROWID)
            || (ec == result_code::CONSTRAINT_TRIGGER)
            || (ec == result_code::CONSTRAINT_UNIQUE)
            || (ec == result_code::CONSTRAINT_VTAB);

        case result::mismatch: return (ec == result_code::MISMATCH);
        case result::misuse: return (ec == result_code::MISUSE);
        case result::nolfs: return (ec == result_code::NOLFS);
        case result::auth: return (ec == result_code::AUTH);
        case result::format: return (ec == result_code::FORMAT);
        case result::range: return (ec == result_code::RANGE);
        case result::notadb: return (ec == result_code::NOTADB);
        case result::notice: return (ec == result_code::NOTICE)
            || (ec == result_code::NOTICE_RECOVER_ROLLBACK)
            || (ec == result_code::NOTICE_RECOVER_WAL);
            
        case result::warning: return (ec == result_code::WARNING)
            || (ec == result_code::WARNING_AUTOINDEX);
        case result::row: return (ec == result_code::ROW);
        case result::done: return (ec == result_code::DONE);
        default: return false;
        }
      }
          
      static result_category& instance() {
        static result_category c;
        return c;
      }
    };
  }
}


