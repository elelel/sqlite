#pragma once

#include <memory>
#include <sqlite3.h>

#include "database.hpp"
#include "parameters_container.hpp"
#include "read_rows_container.hpp"

namespace elelel {
  namespace sqlite {
    template <typename Query, typename T>
    struct input_iterator;
    template <typename Query, typename T>
    struct args_iterator;
    
    template <typename ParamsTuple = std::tuple<>, typename ResultsTuple = std::tuple<>>
    struct query {
      
      template <typename Query, typename T>
      friend struct input_iterator;

      using type = query<ParamsTuple, ResultsTuple>;
      using params_tuple = ParamsTuple;
      using results_tuple = ResultsTuple;


      query(database_ptr db, const std::string& query_str);

      // Execute
      void step(std::error_code& ec); 
      void step();
      void execute(std::error_code& ec); 
      void execute();

      // Prepare to reexecute again
      void reset(std::error_code& ec);
      void reset();

      // Accessors
      ::sqlite3_stmt* stmt() const;
      bool at_results_begin() const;
      bool at_results_end() const;

    private:
      database_ptr db_;
      bool at_results_begin_;
      bool at_results_end_;
      std::shared_ptr<statement> stmt_;
    public:
      parameters_container<type, ParamsTuple> params;
      read_rows_container<type, ResultsTuple> results;
    };
  }
}
