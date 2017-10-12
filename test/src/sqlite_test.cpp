#include "../contrib/Catch/include/catch.hpp"

#include <iostream>

#include "../include/elelel/sqlite/sqlite"
#include "../include/elelel/sqlite/type_policy/native"

#include <unistd.h>

SCENARIO("Taste") {
  unlink("test.sqlite");
  namespace sqlite = elelel::sqlite;
  auto db = std::make_shared<sqlite::database>("test.sqlite");
  sqlite::query create(db, "CREATE TABLE `table1` (`field1` INTEGER NOT NULL, `field2` INTEGER);");
  create.execute();
  // Declare query parameters
  using params_type = sqlite::params<int32_t, int32_t>::type;
  // Create a query to insert rows
  sqlite::query<params_type> ins(db, "INSERT INTO `table1` (`field1`, `field2`) VALUES (?, ?)");
  ins.params.bind(sqlite::make_params(1, 2));
  ins.execute();
  // Use the saved query to insert a second row with values 3 and NULL
  ins.params.clear();
  ins.reset();
  ins.params.bind(sqlite::make_params(3, sqlite::null<int32_t>()));
  ins.execute();
  // Select records
  using results_type = sqlite::row<int32_t, int32_t>::type;
  sqlite::query<std::tuple<>, results_type> sel(db, "SELECT `field1`, `field2` FROM `table1`");
  size_t count{0};
  auto value_or_null = [] (std::optional<int32_t> v) {
    if (v) return std::to_string(*v); else return std::string{"NULL"};
  };
  for (const auto r : sel.results) {
    std::cout << "Record " << count << "\n"
              << " field1 (by row tuple) = " << value_or_null(std::get<0>(r)) << ", "
              << "field2 (by row tuple) = " << value_or_null(std::get<1>(r)) << "\n"
      // The same without declaring row record
              << " field1 (by column index) = " << value_or_null(sel.results.get<int32_t>(0)) << ", "
              << "field2 (by column index) = " << value_or_null(sel.results.get<int32_t>(1)) << "\n";
  }
}

SCENARIO("TDD") {
  namespace sqlite = elelel::sqlite;
  unlink("test.sqlite");
  auto db = std::make_shared<sqlite::database>("test.sqlite");
  sqlite::query create(db, R"(
CREATE TABLE `table1` (
  `int` INTEGER,
  `int_not_null` INTEGER NOT NULL,
  `int64` INTEGER,
  `int64_not_null` INTEGER NOT NULL
);
)");
  create.execute();
  WHEN("Binding single argument") {
    sqlite::query<sqlite::params<int32_t>::type> q(db, "SELECT count(*) FROM `table1` WHERE `int` = ?");
    WHEN("Using bind_value with index") {
      q.params.bind_wrapped_value(1, std::optional<int32_t>(1));
    }
    WHEN("Using bind_values variadic") { 
      q.params.bind_wrapped_values(std::optional<int32_t>(1));
    }
    WHEN("Using bind_tuple") {
      q.params.bind_wrapped_tuple(sqlite::row<int32_t>::type(std::optional<int32_t>(1)));
    }
    WHEN("Using make_params") {
      q.params.bind(sqlite::make_params<int32_t>(1));
      q.params.bind(sqlite::make_params(1));
      int i = 1;
      q.params.bind(sqlite::make_params(i));
    }
  }
  WHEN("Binding two arguments") {
    using params_type = sqlite::params<int32_t, int32_t>::type;
    sqlite::query<params_type> q(db, "SELECT count(*) FROM `table1` WHERE `int` = ? and `int_not_null` = ?");
    WHEN("Using bind_values variadic") {
      q.params.bind_wrapped_values(std::optional<int32_t>(1), std::optional<int32_t>(2));
    }
    WHEN("Using bind_tuple") {
      q.params.bind_wrapped_tuple(sqlite::row<int32_t, int32_t>::type(std::optional<int32_t>(1),
                                                                      std::optional<int32_t>(2)));
    }
    WHEN("Using make_params") {
      q.params.bind(sqlite::make_params<int32_t, int32_t>(1, 2));
      int i1 = 1;
      int i2 = 2;
      q.params.bind(sqlite::make_params(i1, i2));
    }
  }
  WHEN("Create insert query") {
    using params_type = sqlite::params<int32_t, int32_t, int64_t, int64_t>::type;
    sqlite::query<params_type> ins(db, "INSERT INTO `table1` (`int`, `int_not_null`, `int64`, `int64_not_null`) VALUES (?, ?, ?, ?)");
    WHEN("Bind insert parameters") {
      WHEN("as named params tuple") {
        auto params = sqlite::make_params(201, 202, 203, 204);
        ins.params.bind(params);
      }
      WHEN("by index") {
        WHEN("One by one") {
          ins.params.bind_value(1, 1);
          ins.params.bind_value(1, 2);
        }
        WHEN("Variadic") {
          ins.params.bind_values(1, 2);
        }
      }
      WHEN("as params tuples; with execute()") {
        ins.params.bind(sqlite::make_params(0x101, 0x102, 0x103, 0x104));
        ins.execute();

        ins.params.clear();
        ins.reset();
        ins.params.bind(sqlite::make_params(0x201, 0x202, 0x203, 0x204));
        ins.execute();
        THEN("Select the rows") {
          using results_type = sqlite::row<int32_t, int32_t, int64_t, int64_t>::type;
          sqlite::query<std::tuple<>, results_type> sel(db, "SELECT `int`, `int_not_null`, `int64`, `int64_not_null` FROM `table1`");
          sel.execute();
          // Requesting single element
          REQUIRE((sel.results.get<int32_t>(0).value() & 0xf) == 1);
          REQUIRE((sel.results.get<int32_t>(1).value() & 0xf) == 2);
          REQUIRE((sel.results.get<int64_t>(2).value() & 0xf) == 3);
          REQUIRE((sel.results.get<int64_t>(3).value() & 0xf) == 4);
          // Requesting row
          results_type row = sel.results.row();
          REQUIRE((std::get<0>(row).value() & 0xf) == 1);
          REQUIRE((std::get<1>(row).value() & 0xf) == 2);
          REQUIRE((std::get<2>(row).value() & 0xf) == 3);
          REQUIRE((std::get<3>(row).value() & 0xf) == 4);
    
          size_t count{0};
          for (const auto r : sel.results) {
            REQUIRE((std::get<0>(r).value() & 0xf) == 1);
            REQUIRE((std::get<1>(r).value() & 0xf) == 2);
            REQUIRE((std::get<2>(r).value() & 0xf) == 3);
            REQUIRE((std::get<3>(r).value() & 0xf) == 4);
            ++count;
          }
          REQUIRE(count == 2);
        }
      }
    }
  }
}
