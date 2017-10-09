#include "../error_condition.hpp"

std::string elelel::sqlite::error::condition_category_success::message(int value) const {
  return "Operation completed successfuly";
}

bool elelel::sqlite::error::condition_category_success::equivalent(const std::error_code& code, int condition) const {
  switch (static_cast<error_condition>(condition)) {
    success
  }
  
}

bool elelel::sqlite::error::condition_category_success::equivalent(int condition, const std::error_code& code) const {
}
