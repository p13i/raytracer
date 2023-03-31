#ifndef RT_TRACE_HPP
#define RT_TRACE_HPP

namespace rt {

template <class T>
struct Trace {
  T data;
  float luminance;

  bool isTerminal;
  struct Trace<T> *next;

  Trace(T data, float luminance, bool isTerminal = false,
        Trace *next = nullptr)
      : data(data),
        luminance(luminance),
        isTerminal(isTerminal),
        next(next) {}
};

}  // namespace rt

#endif  // RT_TRACE_HPP