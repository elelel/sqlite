#include "../contrib/Catch/include/catch.hpp"

#include <iostream>

#include "../include/elelel/sqlite/sqlite"
#include "../include/elelel/sqlite/type_policy/native"

#include <unistd.h>

SCENARIO("Native type policy") {
  WHEN("Prepare the database") {
    unlink("native_tp_test.sqlite");
    namespace sqlite = elelel::sqlite;
    auto db = std::make_shared<sqlite::database>("native_tp_test.sqlite");
    sqlite::query create(db, R"(
CREATE TABLE `types_test` (
	`int`	INTEGER,
	`int64`	INTEGER,
	`real`	REAL,
	`blob`	BLOB,
	`text`	TEXT
);
)");
    create.execute();
    WHEN("Int32, Int64") {
      using params_type = sqlite::params<int32_t, int64_t>::type;
      THEN("Prepare") {
        auto ins = sqlite::query<params_type, sqlite::row<>::type>(db, "INSERT into `types_test` (`int`, `int64`) VALUES (?, ?)");
        THEN("Make params") {
          // auto correct_row = sqlite::make_params(int32_t{1}, int64_t{2});
          THEN("Bind") {
            REQUIRE_NOTHROW(ins.params.bind(sqlite::make_params(int32_t{1}, int64_t{2})));
            THEN("Execute") {
              REQUIRE_NOTHROW(ins.execute());
            }
          }
        }
      }
    }
  }
}


/*
      int sz = 128;
      void* test_buf = malloc(sz);
      memset(test_buf, sz, 1);
      auto blob = std::tuple<void*, int>{test_buf, sz};
      free(test_buf);
*/
