// License here.

#ifndef HASH_HPP_
#define HASH_HPP_

#include <string>

#include "stateless_operator.hpp"

namespace afs {

class Hash : public StatelessOperator {

private:
	uint64_t hash;
	uint64_t scale;
	uint64_t hardener;

    // the return value is in the range [0, max_value-1]
    const size_t MAX_VALUE;

public:
    Hash(size_t max_value);

    ~Hash();

    size_t Process(unsigned char* key, size_t len);
};

} // namespace afs

#endif // HASH_HPP_
