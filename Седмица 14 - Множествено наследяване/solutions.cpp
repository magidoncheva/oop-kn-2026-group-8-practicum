#include <chrono>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

class String {
public:
  String(const char* data = "")
    : data(new char[strlen(data) + 1]{}) {
    strcpy(this->data, data);
  }

  String(const String& other)
    : data(new char[strlen(other.data) + 1]{}) {
    strcpy(data, other.data);
  }

  String(String&& other)
    : data(std::exchange(other.data, nullptr)) {}

  ~String() {
    delete [] data;
  }

  String& operator=(const String& other) {
    String copy(other);
    swap(copy);

    return *this;
  }

  String& operator=(String&& other) {
    String copy(std::move(other));
    swap(copy);

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const String& string) {
    return os << string.data;
  }

private:
  char* data;

  void swap(String& other) {
    std::swap(data, other.data);
  }
};

class Dish {
public:
  Dish(const String ingredient_names[], std::size_t size, unsigned cook_time)
    : cook_time(cook_time), size(size) {
    for (std::size_t i = 0; i < size; ++i) {
      this->ingredient_names[i] = ingredient_names[i];
    }
  }

  virtual ~Dish() = default;

  void print() const noexcept {
    std::cout << "Cooking time: " << cook_time << "\nIngredients: \n";
    for (std::size_t i = 0; i < size; ++i) {
      std::cout << ingredient_names[i] << '\n';
    }

    details();
  }

private:
  static constexpr std::size_t MAX_SIZE = 100;

  String ingredient_names[MAX_SIZE];
  std::size_t size;
  unsigned cook_time;

  virtual void details() const = 0;
};

enum class Meat {
  Chicken,
  Beef,
  Pork,
  Duck
};

String meat_to_string(const Meat& meat) {
  switch (meat) {
  case Meat::Chicken:
    return "chicken";
  case Meat::Beef:
    return "beef";
  case Meat::Pork:
    return "pork";
  case Meat::Duck:
    return "duck";
  }
}

class MeatDish : virtual public Dish {
public:
  MeatDish(
    const String ingredient_names[],
    std::size_t size,
    unsigned cook_time,
    const Meat& meat
  ) : Dish(ingredient_names, size, cook_time), meat(meat) {}

private:
  Meat meat;

protected:
  void details() const {
    std::cout << "Type of meat: " << meat_to_string(meat) << '\n';
  }
};

class SeafoodDish : virtual public Dish {
public:
  SeafoodDish(
    const String ingredient_names[],
    std::size_t size,
    unsigned cook_time,
    const String& seafood
  ) : Dish(ingredient_names, size, cook_time), seafood(seafood) {}

private:
  String seafood;

protected:
  void details() const {
    std::cout << "Seafood: " << seafood << '\n';
  }
};

class SurfAndTurf : public MeatDish, public SeafoodDish {
public:
  SurfAndTurf(
    const String ingredient_names[],
    std::size_t size,
    unsigned cook_time,
    const Meat& meat,
    const String& seafood
  ) : Dish(ingredient_names, size, cook_time),
      MeatDish(ingredient_names, size, cook_time, meat),
      SeafoodDish(ingredient_names, size, cook_time, seafood) {}

private:
  void details() const {
    MeatDish::details();
    SeafoodDish::details();
  }
};

int main() {
  String ingredient_names[] = {"carrots", "onions"};

  Dish* m = new MeatDish(ingredient_names, 2, 30, Meat::Beef);
  m->print();

  std::cout << '\n';
  SeafoodDish s(ingredient_names, 2, 30, "Salmon");
  s.print();

  std::cout << '\n';
  SurfAndTurf st(ingredient_names, 2, 90, Meat::Chicken, "Tuna");
  st.print();

  delete m;
  return 0;
}