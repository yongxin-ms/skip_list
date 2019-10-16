#ifndef SKIP_LIST_RANDOM_H_
#define SKIP_LIST_RANDOM_H_
#include <random>

namespace skiplist {
	class Random {
	public:
		template <class T>
		static T RandomInt(T low, T high) {
			static std::random_device rd;
			static std::default_random_engine engine(rd());

			std::uniform_int_distribution<T> dis(0, high - low);
			T dice_roll = dis(engine) + low;
			return dice_roll;
		}
	};
}

#endif //SKIP_LIST_RANDOM_H_
