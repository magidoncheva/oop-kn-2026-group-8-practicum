#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <vector>

template <typename T>
bool is_fixed_point(const std::function<T(T)>& f, const T& x) {
  return x == f(x);
}

template <typename T, typename U, typename S>
std::function<S(T)> composition(
  const std::function<U(T)>& f,
  const std::function<S(U)>& g
) {
  return [&f, &g](T x){
    return f(g(x));
  };
}

std::function<double(double)> derivative(const std::function<double(double)>& f) {
  constexpr double h = 1e-7;
  
  return [&f, &h](double x){
    return (f(x + h) - f(x)) / h;
  };
}

template <typename T, typename U>
std::vector<U> map(
  const std::vector<T>& arr, 
  const std::function<U(T)>& f
) {
  std::vector<U> result;

  for (std::size_t i = 0; i < arr.size(); ++i) {
    result.push_back(f(arr[i]));
  }

  return result;
}

template <typename T>
std::vector<T> filter(
  const std::vector<T>& arr,
  const std::function<bool(const T&)>& predicate
) {
  std::vector<T> result;

  for (const T& x : arr) {
    if (predicate(x)) {
      result.push_back(x);
    }
  }

  return result;
}

template <typename T>
bool all(
  const std::vector<T>& arr,
  const std::function<bool(T)>& predicate
) {
  for (std::size_t i = 0; i < arr.size(); ++i) {
    if (!predicate(arr[i])) {
      return false;
    }
  }

  return true;
}

int main() {
  std::vector<int> arr = {1, 2, 3, 4, 5};
  std::vector<int> result = filter<int>(arr, [](int x){
    return x % 2 == 0;
  });

  for (std::size_t i = 0; i < result.size(); ++i) {
    std::cout << result[i] << ' ';
  }
  std::cout << '\n';
  return 0;
}