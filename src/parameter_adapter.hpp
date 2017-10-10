#pragma once

namespace elelel {
  namespace sqlite {
    template <typename Container, typename T>
    struct param_adapter {
      param_adapter(Container& cont, const int index);

      param_adapter<Container, T>& operator=(const T&);
    private:
      Container& cont_;
      int index_;
    };
  }
}
