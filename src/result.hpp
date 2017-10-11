#pragma once

#include <string>
#include <system_error>

#include <sqlite3.h>

// Sqlite result codes

namespace elelel {
  namespace sqlite {
    enum class result_code {
      ERROR = SQLITE_ERROR,
      INTERNAL = SQLITE_INTERNAL,
      PERM = SQLITE_PERM,
      ABORT = SQLITE_ABORT,
      BUSY = SQLITE_BUSY,
      LOCKED = SQLITE_LOCKED,
      NOMEM = SQLITE_NOMEM,
      READONLY = SQLITE_READONLY,
      INTERRUPT = SQLITE_INTERRUPT,
      IOERR = SQLITE_IOERR,
      CORRUPT = SQLITE_CORRUPT,
      NOTFOUND = SQLITE_NOTFOUND,
      FULL = SQLITE_FULL,
      CANTOPEN = SQLITE_CANTOPEN,
      PROTOCOL = SQLITE_PROTOCOL,
      EMPTY = SQLITE_EMPTY,
      SCHEMA = SQLITE_SCHEMA,
      TOOBIG = SQLITE_TOOBIG,
      CONSTRAINT = SQLITE_CONSTRAINT,
      MISMATCH = SQLITE_MISMATCH,
      MISUSE = SQLITE_MISUSE,
      NOLFS = SQLITE_NOLFS,
      AUTH = SQLITE_AUTH,
      FORMAT = SQLITE_FORMAT,
      RANGE = SQLITE_RANGE,
      NOTADB = SQLITE_NOTADB,
      NOTICE = SQLITE_NOTICE,
      WARNING = SQLITE_WARNING,
      ROW = SQLITE_ROW,
      DONE = SQLITE_DONE,

      // Extended
      ABORT_ROLLBACK = SQLITE_ABORT_ROLLBACK,
      BUSY_RECOVERY = SQLITE_BUSY_RECOVERY,
      BUSY_SNAPSHOT = SQLITE_BUSY_SNAPSHOT,
      CANTOPEN_CONVPATH = SQLITE_CANTOPEN_CONVPATH,
      CANTOPEN_FULLPATH = SQLITE_CANTOPEN_FULLPATH,
      CANTOPEN_ISDIR = SQLITE_CANTOPEN_ISDIR,
      CANTOPEN_NOTEMPDIR = SQLITE_CANTOPEN_NOTEMPDIR,
      CONSTRAINT_CHECK = SQLITE_CONSTRAINT_CHECK,
      CONSTRAINT_COMMITHOOK = SQLITE_CONSTRAINT_COMMITHOOK,
      CONSTRAINT_FOREIGNKEY = SQLITE_CONSTRAINT_FOREIGNKEY,
      CONSTRAINT_FUNCTION = SQLITE_CONSTRAINT_FUNCTION,
      CONSTRAINT_NOTNULL = SQLITE_CONSTRAINT_NOTNULL,
      CONSTRAINT_PRIMARYKEY = SQLITE_CONSTRAINT_PRIMARYKEY,
      CONSTRAINT_ROWID = SQLITE_CONSTRAINT_ROWID,
      CONSTRAINT_TRIGGER = SQLITE_CONSTRAINT_TRIGGER,
      CONSTRAINT_UNIQUE = SQLITE_CONSTRAINT_UNIQUE,
      CONSTRAINT_VTAB = SQLITE_CONSTRAINT_VTAB,
      CORRUPT_VTAB = SQLITE_CORRUPT_VTAB,
      IOERR_ACCESS = SQLITE_IOERR_ACCESS,
      IOERR_BLOCKED = SQLITE_IOERR_BLOCKED,
      IOERR_CHECKRESERVEDLOCK = SQLITE_IOERR_CHECKRESERVEDLOCK,
      IOERR_CLOSE = SQLITE_IOERR_CLOSE,
      IOERR_CONVPATH = SQLITE_IOERR_CONVPATH,
      IOERR_DELETE = SQLITE_IOERR_DELETE,
      IOERR_DELETE_NOENT = SQLITE_IOERR_DELETE_NOENT,
      IOERR_DIR_CLOSE = SQLITE_IOERR_DIR_CLOSE,
      IOERR_DIR_FSYNC = SQLITE_IOERR_DIR_FSYNC,
      IOERR_FSTAT = SQLITE_IOERR_FSTAT,
      IOERR_FSYNC = SQLITE_IOERR_FSYNC,
      IOERR_GETTEMPPATH = SQLITE_IOERR_GETTEMPPATH,
      IOERR_LOCK = SQLITE_IOERR_LOCK,
      IOERR_MMAP = SQLITE_IOERR_MMAP,
      IOERR_NOMEM = SQLITE_IOERR_NOMEM,
      IOERR_RDLOCK = SQLITE_IOERR_RDLOCK,
      IOERR_READ = SQLITE_IOERR_READ,
      IOERR_SEEK = SQLITE_IOERR_SEEK,
      IOERR_SHMLOCK = SQLITE_IOERR_SHMLOCK,
      IOERR_SHMMAP = SQLITE_IOERR_SHMMAP,
      IOERR_SHMOPEN = SQLITE_IOERR_SHMOPEN,
      IOERR_SHMSIZE = SQLITE_IOERR_SHMSIZE,
      IOERR_SHORT_READ = SQLITE_IOERR_SHORT_READ,
      IOERR_TRUNCATE = SQLITE_IOERR_TRUNCATE,
      IOERR_UNLOCK = SQLITE_IOERR_UNLOCK,
      IOERR_WRITE = SQLITE_IOERR_WRITE,
      LOCKED_SHAREDCACHE = SQLITE_LOCKED_SHAREDCACHE,
      NOTICE_RECOVER_ROLLBACK = SQLITE_NOTICE_RECOVER_ROLLBACK,
      NOTICE_RECOVER_WAL = SQLITE_NOTICE_RECOVER_WAL,
      OK_LOAD_PERMANENTLY = SQLITE_OK_LOAD_PERMANENTLY,
      READONLY_CANTLOCK = SQLITE_READONLY_CANTLOCK,
      READONLY_DBMOVED = SQLITE_READONLY_DBMOVED,
      READONLY_RECOVERY = SQLITE_READONLY_RECOVERY,
      READONLY_ROLLBACK = SQLITE_READONLY_ROLLBACK,
      WARNING_AUTOINDEX = SQLITE_WARNING_AUTOINDEX      
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


