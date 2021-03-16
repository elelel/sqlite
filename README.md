WARNING: This is a very old and incomplete project :)

# Modern (C++17) Sqlite header-only library
* STL-like iterator interface
* NULLs and unit states are always safe (std::optional)
* No strange assumptions about your program's types (e.g. configure BLOB to be output to any type of your choice)
### Taste
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
```

### Using in a project
This is a header-only library. Add the original Sqlite C library as usual, then copy this library to your project's include dir.
Include the header files:
```c++
#include <elelel/sqlite/sqlite>   // Core functions
#include <elelel/sqlite/type_policy/native>   // Type conversion policy of your choice
```
Later in your code create a shared pointer to the database object
```c++
using sqlite = elelel::sqlite;  // Convinience shortcut
auto db = std::make_shared<sqlite::database>("filename.sqlite");
```

### Handling error/result codes
The library wraps all native Sqlite errors into std::error_code (sqlite::result_code class). Stanard library's error code category facilities are used to group the errors and make them comparable to parent error category. The categories are listed in result enum (see result.hpp file). Each function that can return error codes has two overloads: one that uses std::error_code reference parameter to receive the error code, and another without it that will throw std::system_error(error_code) if error_code != result::success. For example:
```c++
 query.params.bind_value(1, std::optional<int32_t>(1));  // Binds value 1 to query parameters, throws std::system_error if fails
 std::error_code ec;
 query.params.bind_value(ec, 1, std::optional<int32_t>(1));  // Binds value 1 to query parameters, doesn't throw, but fills ec in case of error
```

### NULLs and unit values
SQL NULL value is represented as std::optional<VALUE_TYPE> in empty state. The same empty state is used to define unit states, when the value has not been received for some reason, etc. Because of that, every column's type is declared std::optional, even
if it can't be by your database schema definition.

### Type policies and conversions
This library does not make assumptions about your program's data types. The conversion between your program's types and sqlite types is handled by type_policy<TYPE> template specializations. A type policy specialization is defined like this:
```c++
namespace elelel {
  namespace sqlite {
    template <>
    struct type_policy<NAKED_CPP_TYPE> {   // Type to dispatch on
      using bind_type = WRAPPED_CPP_TYPE;  // Type to accept for bind, usually const std::otpional<NAKED_CPP_TYPE>&
      using return_type = WRAPPED_CPP_TYPE; // Type to return when gettin, usually std::optional<NAKED_CPP_TYPE>
      static const auto fundamental = SQLITE_INTEGER;  // Fundamental SQLITE constant

      // Binds value to column i of sqlite stmt
      static std::error_code bind(sqlite3_stmt* stmt, const int i, bind_type value) {
      // Logic to pass to C API
      }

      // Gets column i from sqlite stmt
      static return_type get(sqlite3_stmt* stmt, const int i) {
      // Logic to pass to C API
      }
    };
  
```  
Imagine, for example, if you store bitmaps as BLOBS in your sqlite database. A naive C++ library implementaion would return some predefined type, perhaps std::vector<char> when a SELECT query is issued. Here instead you should declare the template specialization within elelel::sqlite namespace that defines the types and two functions: the one reading the blob and returning it converted to your bitmap type, and it's converse.

### Default type policies
Since the library does not make any implicit assumptions what datatypes you store in Sqlite database, you must either
write your own type policy as explained above, or explicitly include one of default type policies.
There are three default type policies:
 - c_primitive - primitive C wrappers for int/double ```#include <elelel/sqlite/type_policy/c_primitive>```
 - c_array - array C wrappers for std::tuple<char*, size>/std::tuple<void*, size> to convert to BLOBs and TEXTs ```#include <elelel/sqlite/type_policy/c_array>```
 - stl - std::vector<char>/std::string to BLOBs and TEXTs ```#include <elelel/sqlite/type_policy/stl>```

### Rows and columns access in query results
The columns of a current query's state can be accessed in different ways:
```c++
// the rows can be accessed through results member and iterated as a forward_iterator
for (const auto r : results) { std::cout << "I've got a row!\n"; };

// request the whole row and perform a strict-typed access to it's column;
// it ensures more correctness, because the type for the column at index is always matched against query's result type declaration
auto row = *query.results.begin();
auto col = std::get<0>(row);

// request single column by index;
// offers less type safety, but is useful if you want to decide which column to read dynamically at runtime
int i = 0;
auto col = query.results.get<int32_t>(i);
```
