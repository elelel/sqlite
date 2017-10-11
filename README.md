# Modern (C++17) Sqlite header-only library
* STL-like iterator interface
* NULLs and unit states are always safe (std::optional)
* No strange assumptions about your program's types (e.g. configure BLOB to be output to any type of your choice)
## Taste
```c++
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
  // Use the saved query to insert a second row with values 3 and 4
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
```
