#pragma once

namespace elelel {
  namespace sqlite {
    template <typename Container, typename T>
    struct argument_adapter {
      argument_adapter(Container& cont, const int index);

      argument_adapter<Container, T>& operator=(const T&);
    private:
      Container& cont_;
      int index_;
    };
  }
}
