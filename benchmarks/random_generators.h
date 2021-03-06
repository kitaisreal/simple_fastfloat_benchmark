#ifndef RANDOM_GENERATORS_H
#define RANDOM_GENERATORS_H

#include "string_format.h"
#include <random>

struct string_number_generator {
    virtual std::string new_string(bool concise_string) { return ""; }
    virtual std::string describe() { return "abstract class"; }
};

struct uniform_generator : string_number_generator {
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_real_distribution<double> dis;
  explicit uniform_generator(double a = 0, double b = 1): rd(), gen(rd()),  dis(a, b) {}
  std::string describe() {
      return std::string("generate random numbers uniformly in the interval [") + accurate_to_string_concise((dis.min)()) + std::string(",") + accurate_to_string_concise((dis.max)()) + std::string("]");
  }
  std::string new_string(bool concise_string) {
    double x = dis(gen);
    return concise_string ? accurate_to_string_concise(x) : accurate_to_string(x);
  }
};

struct integer_uniform_generator : string_number_generator {
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<uint64_t> dis;
  explicit integer_uniform_generator(uint64_t a = 0, uint64_t b = 1): rd(), gen(rd()),  dis(a, b) {}
  std::string describe() {
      return std::string("generate random untegers numbers uniformly in the interval [") + integer_to_string((dis.min)()) + std::string(",") + integer_to_string((dis.max)()) + std::string("]");
  }
  std::string new_string(bool c = true) {
    uint64_t x = dis(gen);
    return integer_to_string(x);
  }
};

struct one_over_rand32 : string_number_generator {
  std::random_device rd;
  std::mt19937 gen;
  explicit one_over_rand32(): rd(), gen(rd()) {}
  std::string describe() { return "1 / rand()"; }
  std::string new_string(bool concise_string) {
    auto g = gen();
    while (g == 0) {
      g = gen();
    }
    double x = 1.0 / double(g);
    return concise_string ? accurate_to_string_concise(x) : accurate_to_string(x);

  }
};

struct simple_uniform32 : string_number_generator {
  std::random_device rd;
  std::mt19937 gen;
  explicit simple_uniform32(): rd(), gen(rd()) {}
  std::string describe() { return "rand() / 0xFFFFFFFF "; }
  std::string new_string(bool concise_string) {
    double x = double(gen()) / double((std::mt19937::max)());
    return concise_string ? accurate_to_string_concise(x) : accurate_to_string(x);
  }
};

struct simple_int32 : string_number_generator {
  std::random_device rd;
  std::mt19937 gen;
  explicit simple_int32(): rd(), gen(rd()) {}
  std::string describe() { return "rand()"; }
  std::string new_string(bool c = true) {
    return integer_to_string(gen());
  }
};

struct simple_int64 : string_number_generator {
  std::random_device rd;
  std::mt19937_64 gen;
  std::string describe() { return "rand64()"; }
  explicit simple_int64(): rd(), gen(rd()) {}
  std::string new_string(bool c = true) {
    return integer_to_string(gen());
  }
};


struct int_e_int : string_number_generator {
  simple_int32 g;
  integer_uniform_generator g2;
  std::string describe() { return "rand() e rand()"; }
  explicit int_e_int() :  g(), g2(0,99) {} 
  std::string new_string(bool) {
    return g.new_string() + "e" + g2.new_string();
  }
};

struct big_ints : string_number_generator {
  simple_int64 g;
  explicit big_ints() : g() {}
  std::string describe() { return "rand64() rand64() rand64()  --> may generate more than 19 digits!"; }
  std::string new_string(bool) {
    return g.new_string() + g.new_string() + g.new_string();
  }
};


struct bigint_int_dot_int : string_number_generator {
  simple_int32 g;
  std::string describe() { return "rand().rand()  --> may generate more than 19 digits!"; }
  explicit bigint_int_dot_int() : g() {}
  std::string new_string(bool) {
    return g.new_string() + "." + g.new_string();
  }
};

std::vector<std::string> model_names = {"uniform", "one_over_rand32", "simple_uniform32", "simple_int32",  "int_e_int",  "simple_int64",  "bigint_int_dot_int", "big_ints"};
string_number_generator* get_generator_by_name(std::string name) {
    std::cout << "available models :";
    for(std::string name : model_names) { std::cout << name << ",";  }
    std::cout << std::endl;
    // This is naive, but also not very important.
    if(name == "uniform") { return new uniform_generator(); }
    if(name == "one_over_rand32") { return new one_over_rand32(); }
    if(name == "simple_uniform32") { return new simple_uniform32(); }
    if(name == "simple_int32") { return new simple_int32(); }
    if(name == "simple_int64") { return new simple_int64(); }
    if(name == "bigint_int_dot_int") { return new bigint_int_dot_int(); }
    if(name == "int_e_int") { return new int_e_int(); }
    if(name == "big_ints") { return new big_ints(); }
    std::cerr << " I do not recognize " << name << std::endl;
    std::cerr << " Warning: falling back on uniform generator. " << std::endl;
    return new uniform_generator();
}


#endif