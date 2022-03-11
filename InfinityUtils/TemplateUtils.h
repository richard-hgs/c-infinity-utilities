#pragma once

/*
 * Usage Exapmle
 *
 * int a = 100;
 * auto b = [&](void*) {return ++a;};
 * Converting lambda with captures to a C pointer
 * 
 * void (*f1)(void*) = Lambda::ptr(b);
 * f1(nullptr);
 * printf("%d\n", a);  // 101 
 * Can be used this way as well
 * 
 * auto f2 = Lambda::ptr(b);
 * f2(nullptr);
 * printf("%d\n", a); // 102
 * In case return value should be used
 * 
 * int (*f3)(void*) = Lambda::ptr<int>(b);
 * printf("%d\n", f3(nullptr)); // 103
 * And in case data is used
 * 
 * auto b2 = [&](void* data) {return *(int*)(data) + a;};
 * int (*f4)(void*) = Lambda::ptr<int>(b2);
 * int data = 5;
 * printf("%d\n", f4(&data)); // 108
 */

struct Lambda {
	template<typename Tret, typename T>
	static Tret lambda_ptr_exec(void* data) {
		return (Tret)(*(T*)fn<T>())(data);
	}

	template<typename Tret = void, typename Tfp = Tret(*)(void*), typename T>
	static Tfp ptr(T& t) {
		fn<T>(&t);
		return (Tfp)lambda_ptr_exec<Tret, T>;
	}

	template<typename T>
	static void* fn(void* new_fn = nullptr) {
		static void* fn;
		if (new_fn != nullptr)
			fn = new_fn;
		return fn;
	}
};

//template <typename CT, typename ... A> struct function
//	: public function<decltype(&CT::operator())(A...)> {};
//
//template <typename C> struct function<C> {
//private:
//	C mObject;
//
//public:
//	function(const C& obj)
//		: mObject(obj) {}
//
//	template<typename... Args> typename
//		std::result_of<C(Args...)>::type operator()(Args... a) {
//		return this->mObject.operator()(a...);
//	}
//
//	template<typename... Args> typename
//		std::result_of<const C(Args...)>::type operator()(Args... a) const {
//		return this->mObject.operator()(a...);
//	}
//};
//
//namespace infinityUtils {
//	namespace make {
//		template<typename C> auto function(const C& obj) {
//			return ::function<C>(obj);
//		}
//	}
//}


//template<typename OT, typename RT, typename ... A>
//struct lambda_expression {
//	OT _object;
//	RT(OT::* _function)(A...)const;
//
//	lambda_expression(const OT& object)
//		: _object(object), _function(&decltype(_object)::operator()) {}
//
//	RT operator() (A ... args) const {
//		return (_object.*_function)(args...);
//	}
//};