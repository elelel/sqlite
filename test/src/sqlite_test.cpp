#include "../contrib/Catch/include/catch.hpp"

#include "../include/elelel/sqlite/sqlite"
#include "../include/elelel/sqlite/type_policy/native"

#include <unistd.h>

SCENARIO("TDD") {
  namespace sqlite = elelel::sqlite;
  unlink("test.sqlite");
  auto db = std::make_shared<sqlite::database>("test.sqlite");
  WHEN("Creating a table") {
    sqlite::query q(db, R"(
CREATE TABLE `table1` (
	`int_field`	INTEGER,
	`int_field_not_null`	INTEGER NOT NULL
);
)");
    q.execute();
    WHEN("Binding single argument") {
      sqlite::query<sqlite::params<int32_t>::type> q(db, "SELECT count(*) FROM `table1` WHERE `int_field` = ?");
      WHEN("Using bind_value with index") {
        q.params.bind_value(1, std::optional<int32_t>(1));
      }
      WHEN("Using bind_values variadic") { 
        q.params.bind_values(std::optional<int32_t>(1));
      }
      WHEN("Using bind_tuple") {
        q.params.bind_tuple(sqlite::row<int32_t>::type(std::optional<int32_t>(1)));
      }
      WHEN("Using make_params") {
        q.params.bind_params(sqlite::make_params<int32_t>(1));
        q.params.bind_params(sqlite::make_params(1));
        int i = 1;
        q.params.bind_params(sqlite::make_params(i));
      }
    }
    WHEN("Binding two arguments") {
      using params_type = sqlite::params<int32_t, int32_t>::type;
      sqlite::query<params_type> q(db, "SELECT count(*) FROM `table1` WHERE `int_field` = ? and `int_field_not_null` = ?");
      WHEN("Using bind_values variadic") {
        q.params.bind_values(std::optional<int32_t>(1), std::optional<int32_t>(2));
      }
      WHEN("Using bind_tuple") {
        q.params.bind_tuple(sqlite::row<int32_t, int32_t>::type(std::optional<int32_t>(1),
                                                                std::optional<int32_t>(2)));
      }
      WHEN("Using make_params") {
        q.params.bind_params(sqlite::make_params<int32_t, int32_t>(1, 2));
        int i1 = 1;
        int i2 = 2;
        q.params.bind_params(sqlite::make_params(i1, i2));
      }
    }
  }
}


