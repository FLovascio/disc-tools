#pragma once
#include <type_traits>

template<typename T>
struct has_size_method{
private:
	typedef std::true_type yes;
	typedef std::false_type no;
	template<typename U> static auto test(int) -> decltype(std::declval<U>().size() == 1, yes());
	template<typename> static no test(...);
public:
	static constexpr bool value = std::is_same<decltype(test<T>(0)),yes>::value;
};

template<typename T>
struct is_indexable{
private:
	typedef std::true_type yes;
	typedef std::false_type no;
	template<typename U> static auto test(U tested) -> decltype(tested.operator[](0), yes());
	template<typename> static no test(...);
public:
	static constexpr bool value = std::is_same<decltype(test<T>(0)),yes>::value;
};
